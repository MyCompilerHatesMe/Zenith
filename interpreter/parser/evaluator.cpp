#define PARSER_h
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "interpreter/token.h"
#include "evaluator.h"



// navigation
Token Parser::peek() const {
    return tokens[current];
}

Token Parser::advance() {
    if(!isAtEnd()) return tokens[current++];
}

bool Parser::isAtEnd() const {
    return peek().type == END_OF_FILE;
}

Token Parser::previous() const {
    //make sure current >= 1 still using at
    return tokens.at(current - 1);
}

// check and consume
bool Parser::match(TokenType type) {
    if(check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) const {
    if(!isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if(check(type)) return advance();
    std::cerr << "[line " << peek().line << "] Error: " << message << std::endl;
    std::exit(1);
}