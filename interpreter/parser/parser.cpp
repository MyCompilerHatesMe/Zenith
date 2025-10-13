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