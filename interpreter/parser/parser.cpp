#include <string>
#include <vector>
#include <memory>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "interpreter/token.h"
#include "parser.h"

// only use this to access token stream
Token Parser::peek() const {
    return tokens.at(current);
}

// this works
bool Parser::isAtEnd() const {
    return peek().type == END_OF_FILE;
}

Token Parser::previous() const {
    return tokens.at(current - 1);
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();

    std::cerr << "[line " << peek().line << "] Error: " << message << std::endl;
    std::exit(1);
}

bool Parser::isTypeKeyword() const {
    TokenType t = peek().type;
    return t == TYPE_INT || t == TYPE_STRING || t == TYPE_BOOL || t == TYPE_CHAR;
}


// top level
std::vector<unique_ptr<Statement>> Parser::parse() {
    std::vector<unique_ptr<Statement>> statements;
    while (!isAtEnd()){
        statements.push_back(parseStatement());
    }
    return statements;
}

// statement parsing
unique_ptr<Statement> Parser::parseStatement() {
    if (isTypeKeyword())        return parseVarDecl();
    if (match(PRINT))           return parsePrintStatement();
    if (match(IF))              return parseIfStatement();
    if (match(WHILE))           return parseWhileStatement();
    if (match(FOR))             return parseForStatement();
    if (check(LEFT_BRACE))      return parseBlock();
    return parseExpressionStatement();
}

// int x = expr;
unique_ptr<Statement> Parser::parseVarDecl() {
    TokenType typeKeyword = advance().type;  // consume type
    Token name = consume(IDENTIFIER, "Expected variable name after type.");
    consume(EQUAL, "Expected '=' after variable name.");
    auto initialiser = parseExpression();
    consume(SEMICOLON, "Expected ';' after variable declaration.");

    auto decl = std::make_unique<VarDeclStatement>();
    decl->typeKeyword = typeKeyword;
    decl->name = name;
    decl->initialiser = std::move(initialiser);
    return decl;
}

// display(expr);
unique_ptr<Statement> Parser::parsePrintStatement() {
    consume(LEFT_PAREN, "Expected '(' after 'display'.");
    auto expr = parseExpression();
    consume(RIGHT_PAREN, "Expected ')' after expression.");
    consume(SEMICOLON, "Expected ';' after display statement.");

    auto stmt = std::make_unique<PrintStatement>();
    stmt->expr = std::move(expr);
    return stmt;
}

// if (expr) block (else block)?
unique_ptr<Statement> Parser::parseIfStatement() {
    consume(LEFT_PAREN, "Expected '(' after 'if'.");
    auto condition = parseExpression();
    consume(RIGHT_PAREN, "Expected ')' after if condition.");

    auto thenBranch = parseBlock();

    unique_ptr<Statement> elseBranch = nullptr;
    if (match(ELSE)) {
        // else if is just an else whose body is another if statement
        if (check(IF)) {
            advance();  // consume 'if'
            elseBranch = parseIfStatement();
        } else {
            elseBranch = parseBlock();
        }
    }

    auto stmt = std::make_unique<IfStatement>();
    stmt->condition = std::move(condition);
    stmt->thenBranch = std::move(thenBranch);
    stmt->elseBranch = std::move(elseBranch);
    return stmt;
}

// while (expr) block
unique_ptr<Statement> Parser::parseWhileStatement() {
    consume(LEFT_PAREN, "Expected '(' after 'while'.");
    auto condition = parseExpression();
    consume(RIGHT_PAREN, "Expected ')' after while condition.");
    auto body = parseBlock();

    auto stmt = std::make_unique<WhileStatement>();
    stmt->condition = std::move(condition);
    stmt->body = std::move(body);
    return stmt;
}

// for (expr; expr; expr) block
unique_ptr<Statement> Parser::parseForStatement() {
    consume(LEFT_PAREN, "Expected '(' after 'for'.");
    auto init = parseExpression();
    consume(SEMICOLON, "Expected ';' after for initializer.");
    auto condition = parseExpression();
    consume(SEMICOLON, "Expected ';' after for condition.");
    auto increment = parseExpression();
    consume(RIGHT_PAREN, "Expected ')' after for clauses.");
    auto body = parseBlock();

    auto stmt = std::make_unique<ForStatement>();
    stmt->init = std::move(init);
    stmt->condition = std::move(condition);
    stmt->increment = std::move(increment);
    stmt->body = std::move(body);
    return stmt;
}

// { stmt* }
unique_ptr<Statement> Parser::parseBlock() {
    consume(LEFT_BRACE, "Expected '{'.");
    auto block = std::make_unique<BlockStatement>();
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        block->statements.push_back(parseStatement());
    }
    consume(RIGHT_BRACE, "Expected '}' after block.");
    return block;
}

// expr;
unique_ptr<Statement> Parser::parseExpressionStatement() {
    auto expr = parseExpression();
    consume(SEMICOLON, "Expected ';' after expression.");
    auto stmt = std::make_unique<ExpressionStatement>();
    stmt->expr = std::move(expr);
    return stmt;
}



// expression parsing is below this, i think
unique_ptr<Expression> Parser::parsePrimary() {
     if(check(TokenType::NUMBER) || check(TokenType::STRING) 
        || check(TRUE) || check(FALSE) || check(NIL)){
        auto literal = std::make_unique<LiteralExpression>();
        literal->op = advance();
        return literal;
     }
     else if(check(TokenType::IDENTIFIER)){
        auto identifier = std::make_unique<IdentifierExpression>();
        identifier->name = advance();
        return identifier;
     }
     else if (match(TokenType::LEFT_PAREN)){
        auto expr = parseExpression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
        return expr;
     }
     throw std::runtime_error(
        std::string("[line " + std::to_string(peek().line) + "] Unexpected token '" +
        std::string(peek().lexeme) + "'.")
     );
}
unique_ptr<Expression> Parser::parseUnary(){
    if(match(TokenType::MINUS) || match(TokenType::BANG)){
        Token unaryT = previous();
        auto right  = parseUnary();
        auto UnaryExpr = std::make_unique<UnaryExpression>();
        UnaryExpr->op = unaryT;
        UnaryExpr->expr = std::move(right);
        return UnaryExpr;
    } 
    return parsePrimary();
}
unique_ptr<Expression> Parser::parseMultiplication(){
    auto expr = parseUnary();
    while(match(TokenType::SLASH) || match(TokenType::STAR)){
        Token op = previous();
        auto right = parseUnary();

        auto bin = std::make_unique<BinaryExpression>();
        bin->left = std::move(expr);
        bin->op = op;
        bin->right = std::move(right);
        expr = std::move(bin);
    }
    return expr;
}
unique_ptr<Expression> Parser::parseAddition(){
    auto expr = parseMultiplication();
    while(match(TokenType::MINUS) || match(TokenType::PLUS)){
        Token op = previous();
        auto right = parseMultiplication();

        auto bin = std::make_unique<BinaryExpression>();
        bin->left = std::move(expr);
        bin->op = op;
        bin->right = std::move(right);
        expr = std::move(bin);
    }
    return expr;
}
unique_ptr<Expression> Parser::parseComparision(){
    auto expr = parseAddition();
    while(match(TokenType::GREATER) || match(TokenType::GREATER_EQUAL) 
          || match(TokenType::LESS) || match(TokenType::LESS_EQUAL)){
          Token op = previous();
          auto right = parseAddition();

          auto bin = std::make_unique<BinaryExpression>();
          bin->left = std::move(expr);
          bin->op = op;
          bin->right = std::move(right);
          expr = std::move(bin);
    }  
    return expr;
}
unique_ptr<Expression> Parser::parseEquality(){
    auto expr = parseComparision();
    while(match(TokenType::EQUAL_EQUAL) || match(TokenType::BANG_EQUAL)){
        Token op = previous();
        auto right = parseComparision();

        auto bin = std::make_unique<BinaryExpression>();
        bin->left = std::move(expr);
        bin->op = op;
        bin->right = std::move(right);
        expr = std::move(bin);
    }
    return expr;
}
unique_ptr<Expression> Parser::parseLogicalAnd(){
    auto expr = parseEquality();

    while(match(TokenType::AND)){
        Token op = previous();
        auto right = parseEquality();

        auto bin = std::make_unique<BinaryExpression>();
        bin->left = std::move(expr);
        bin->op = op;
        bin->right = std::move(right);
        expr = std::move(bin);
    }
    return expr;
}
unique_ptr<Expression> Parser::parseLogicalOr(){
    auto expr = parseLogicalAnd();
    
    while(match(TokenType::OR)){
        Token op = previous();
        auto right = parseLogicalAnd();

        auto bin  = std::make_unique<BinaryExpression>();
        bin->left = std::move(expr);
        bin->op = op;
        bin->right = std::move(right);
        expr = std::move(bin);
    }
    return expr;
}
unique_ptr<Expression> Parser::parseExpression(){
    return parseAssignment();
}
unique_ptr<Expression> Parser:: parseAssignment(){
    auto expr = parseLogicalOr();

    if(match(TokenType::EQUAL)){
        Token eq = previous();
        auto value = parseAssignment();
        
        if(auto* identifier = dynamic_cast<IdentifierExpression*>(expr.get())) {
        auto assignment  = std::make_unique<AssignmentExpression>();
        assignment->name =identifier->name;
        assignment->value = std::move(value);
        return assignment;
        }
    throw std::runtime_error("Invalid Assignment");
       }
   return expr;
}
