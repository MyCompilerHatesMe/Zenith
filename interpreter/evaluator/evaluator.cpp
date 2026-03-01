#include "evaluator.h"
#include <iostream>
#include <cstdlib>
#include <stdexcept>

// helpers 

std::string valueToString(const Value& v) {
    return std::visit([](auto&& val) -> std::string {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, int>)         return std::to_string(val);
        if constexpr (std::is_same_v<T, bool>)        return val ? "true" : "false";
        if constexpr (std::is_same_v<T, std::string>) return val;
        if constexpr (std::is_same_v<T, char>)        return std::string(1, val);
        if constexpr (std::is_same_v<T, std::monostate>) return "null";
    }, v);
}

void printValue(const Value& v) {
    std::cout << valueToString(v) << "\n";
}

// env
// all errors rage quit for now, i don't know if i'll rework that

void Environment::define(const std::string& name, Value value) {
    if (values.count(name)) {
        std::cerr << "[ERROR] Redeclaration of variable '" << name << "'.\n";
        std::exit(1);
    }
    values[name] = std::move(value);
}

Value Environment::get(const std::string& name, int line) const {
    auto it = values.find(name);
    if (it != values.end()) return it->second;
    if (enclosing) return enclosing->get(name, line);

    std::cerr << "[line " << line << "] ERROR: Undefined variable '" << name << "'.\n";
    std::exit(1);
}

void Environment::assign(const std::string& name, Value value, int line) {
    auto it = values.find(name);
    if (it != values.end()) {
        it->second = std::move(value);
        return;
    }
    if (enclosing) {
        enclosing->assign(name, std::move(value), line);
        return;
    }
    std::cerr << "[line " << line << "] ERROR: Undefined variable '" << name << "'.\n";
    std::exit(1);
}

// eval

void Evaluator::typeError(const std::string& msg, int line) {
    std::cerr << "[line " << line << "] TYPE ERROR: " << msg << "\n";
    std::exit(1);
}

bool Evaluator::isTruthy(const Value& v) {
    if (std::holds_alternative<bool>(v))        return std::get<bool>(v);
    if (std::holds_alternative<std::monostate>(v)) return false;
    return true;  // everything else is truthy
}

bool Evaluator::isEqual(const Value& a, const Value& b) {
    return a == b;
}

void Evaluator::checkTypeMatch(TokenType declared, const Value& val, int line) {
    bool valid = false;
    switch (declared) {
        case TYPE_INT:    valid = std::holds_alternative<int>(val);         break;
        case TYPE_STRING: valid = std::holds_alternative<std::string>(val); break;
        case TYPE_BOOL:   valid = std::holds_alternative<bool>(val);        break;
        case TYPE_CHAR:   valid = std::holds_alternative<char>(val);        break;
        default: break;
    }
    if (!valid) {
        typeError("Type mismatch in variable declaration.", line);
    }
}

// run.

void Evaluator::run(const std::vector<std::unique_ptr<Statement>>& statements) {
    for (const auto& stmt : statements) {
        execute(*stmt);
    }
}

// actual execution stuff

void Evaluator::execute(const Statement& stmt) {

    if (const auto* s = dynamic_cast<const PrintStatement*>(&stmt)) {
        Value val = evaluate(*s->expr);
        printValue(val);
        return;
    }

    if (const auto* s = dynamic_cast<const VarDeclStatement*>(&stmt)) {
        Value val = evaluate(*s->initialiser);
        checkTypeMatch(s->typeKeyword, val, s->name.line);
        env->define(std::string(s->name.lexeme), std::move(val));
        return;
    }

    if (const auto* s = dynamic_cast<const BlockStatement*>(&stmt)) {
        Environment blockEnv(env);
        executeBlock(*s, &blockEnv);
        return;
    }

    if (const auto* s = dynamic_cast<const IfStatement*>(&stmt)) {
        Value cond = evaluate(*s->condition);
        if (isTruthy(cond)) {
            execute(*s->thenBranch);
        } else if (s->elseBranch) {
            execute(*s->elseBranch);
        }
        return;
    }

    if (const auto* s = dynamic_cast<const WhileStatement*>(&stmt)) {
        while (isTruthy(evaluate(*s->condition))) {
            execute(*s->body);
        }
        return;
    }

    if (const auto* s = dynamic_cast<const ForStatement*>(&stmt)) {
        // init runs once
        evaluate(*s->init);
        while (isTruthy(evaluate(*s->condition))) {
            execute(*s->body);
            evaluate(*s->increment);
        }
        return;
    }

    if (const auto* s = dynamic_cast<const ExpressionStatement*>(&stmt)) {
        evaluate(*s->expr);
        return;
    }

    // how did we get here? 
    std::cerr << "[ERROR] Unknown statement type.\n";
    std::exit(1);
}

void Evaluator::executeBlock(const BlockStatement& block, Environment* blockEnv) {
    Environment* previous = env;
    env = blockEnv;
    for (const auto& stmt : block.statements) {
        execute(*stmt);
    }
    env = previous;
}

// expressions

Value Evaluator::evaluate(const Expression& expr) {

    if (const auto* e = dynamic_cast<const LiteralExpression*>(&expr)) {
        switch (e->op.type) {
            case NUMBER: return std::stoi(std::string(e->op.lexeme));
            case STRING: {
                std::string s = std::string(e->op.lexeme);
                return s.substr(1, s.size() - 2); // remove quotes
            }
            case TRUE:  return true;
            case FALSE: return false;
            case NIL:   return std::monostate{};
            default: break;
        }
    }

    if (const auto* e = dynamic_cast<const IdentifierExpression*>(&expr)) {
        return env->get(std::string(e->name.lexeme), e->name.line);
    }

    if (const auto* e = dynamic_cast<const AssignmentExpression*>(&expr)) {
        Value val = evaluate(*e->value);
        env->assign(std::string(e->name.lexeme), val, e->name.line);
        return val;
    }

    if (const auto* e = dynamic_cast<const UnaryExpression*>(&expr)) {
        Value right = evaluate(*e->expr);
        switch (e->op.type) {
            case MINUS:
                if (!std::holds_alternative<int>(right))
                    typeError("Operand of '-' must be an int.", e->op.line);
                return -std::get<int>(right);
            case BANG:
                return !isTruthy(right);
            default: break;
        }
    }

    if (const auto* e = dynamic_cast<const BinaryExpression*>(&expr)) {
        // short circuit logical operators before evaluating right
        if (e->op.type == AND) {
            Value left = evaluate(*e->left);
            if (!isTruthy(left)) return left;
            return evaluate(*e->right);
        }
        if (e->op.type == OR) {
            Value left = evaluate(*e->left);
            if (isTruthy(left)) return left;
            return evaluate(*e->right);
        }

        Value left  = evaluate(*e->left);
        Value right = evaluate(*e->right);
        int line = e->op.line;

        switch (e->op.type) {
            case PLUS:
                if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right))
                    return std::get<int>(left) + std::get<int>(right);
                if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
                    return std::get<std::string>(left) + std::get<std::string>(right);
                typeError("Operands of '+' must both be int or both be string.", line);
                break;

            case MINUS:
                if (!std::holds_alternative<int>(left) || !std::holds_alternative<int>(right))
                    typeError("Operands of '-' must be int.", line);
                return std::get<int>(left) - std::get<int>(right);

            case STAR:
                if (!std::holds_alternative<int>(left) || !std::holds_alternative<int>(right))
                    typeError("Operands of '*' must be int.", line);
                return std::get<int>(left) * std::get<int>(right);

            case SLASH:
                if (!std::holds_alternative<int>(left) || !std::holds_alternative<int>(right))
                    typeError("Operands of '/' must be int.", line);
                if (std::get<int>(right) == 0)
                    typeError("Division by zero.", line);
                return std::get<int>(left) / std::get<int>(right);

            case GREATER:
                if (!std::holds_alternative<int>(left) || !std::holds_alternative<int>(right))
                    typeError("Operands of '>' must be int.", line);
                return std::get<int>(left) > std::get<int>(right);

            case GREATER_EQUAL:
                if (!std::holds_alternative<int>(left) || !std::holds_alternative<int>(right))
                    typeError("Operands of '>=' must be int.", line);
                return std::get<int>(left) >= std::get<int>(right);

            case LESS:
                if (!std::holds_alternative<int>(left) || !std::holds_alternative<int>(right))
                    typeError("Operands of '<' must be int.", line);
                return std::get<int>(left) < std::get<int>(right);

            case LESS_EQUAL:
                if (!std::holds_alternative<int>(left) || !std::holds_alternative<int>(right))
                    typeError("Operands of '<=' must be int.", line);
                return std::get<int>(left) <= std::get<int>(right);

            case EQUAL_EQUAL: return isEqual(left, right);
            case BANG_EQUAL:  return !isEqual(left, right);

            default: break;
        }
    }

    // how did we get here?
    std::cerr << "[ERROR] Unknown expression type.\n";
    std::exit(1);
}