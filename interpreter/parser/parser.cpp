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
unique_ptr<Expression> Parser::parsePrimary() {
     if(check(TokenType::NUMBER) || check(TokenType::STRING)){
        Token t = advance();
        auto literal = std::make_unique<LiteralExpression>();
        literal->op = t;
        return literal;
     }
     else if(check(TokenType::IDENTIFIER)){
        Token t = advance();
        auto identifier = std::make_unique<IdentifierExpression>();
        identifier->name = t;
        return identifier;
     }
     else if (match(TokenType::LEFT_PAREN)){
        auto expr = parseExpression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
        return expr;
     }
     else{
        throw std::runtime_error("Invalid Token");
     }
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
