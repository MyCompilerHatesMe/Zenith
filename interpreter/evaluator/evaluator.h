#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "interpreter/parser/parser.h"
#include "interpreter/token.h"
#include <variant>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <iostream>

using Value = std::variant<int, bool, std::string, char, std::monostate>;

std::string valueToString(const Value& v);
void printValue(const Value& v);

class Environment {
    public:
        Environment() : enclosing(nullptr) {}
        Environment(Environment* enclosing) : enclosing(enclosing) {}

        void define(const std::string& name, int line) const;

        Value get(const std::string& name, Value value, int line);

        void assign(const std::string& name, Value value, int line);
    
    private:
        std::unordered_map<std::string, Value> values;
        Environment* enclosing;
};

class Evaluator {
    public:
        Evaluator() : env(new Environment()) {}

        void run(const std::vector<std::unique_ptr<Statement>>& statements);
    
    private:
        Environment* env;
        
        void execute(const Statement& stmt);
        void executeBlock(const BlockStatement& stmt, Environment* blockEnv);

        Value evaluate(const Expression& expr);

        void typeError(const std::string& msg, int line);
        void isTruthy(const Value& v);
        void isEqual(const Value& a, const Value& b);
        void checkTypeMaatch(TokenType declared, const Value& val, int line);
};


#endif