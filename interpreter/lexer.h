#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "token.h"


class Lexer {
    public: 
        Lexer(const std::string& source);
        Token scanToken();

    private: 
        std::string source;
        int start = 0;
        int current = 0;
        int line = 1;


        // navigation methods
        char advance();
        bool match(char expected);
        bool isAtEnd() const;
        char peek() const;
        char peekNext() const;

        // classification methods
        bool isDigit(char c) const;
        bool isAlpha(char c) const;
        bool isAlphaNumeric(char c) const;

        // token methods
        Token string();
        Token number();
        Token identifier();

        Token makeToken(TokenType type) const;
};

#endif