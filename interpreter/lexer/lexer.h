#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "interpreter/token.h"

using keywordMap_t = std::unordered_map<std::string, TokenType>;

class Lexer {
    public: 
        Lexer(const std::string& source);
        std::vector<Token> scanTokens();
        
    private: 
        std::string source;
        int start = 0; // starting index of any given token
        int current = 0; // current index of array
        int line = 1; // line number

        inline static const keywordMap_t keywords = {
            {"and", AND},
            {"class", CLASS},
            {"else", ELSE},
            {"false", FALSE},
            {"true", TRUE},
            {"for", FOR},
            {"fun", FUN},
            {"if", IF},
            {"or", OR},
            {"display", PRINT},
            {"return", RETURN},
            {"super", SUPER},
            {"this", THIS},
            {"var", VAR},
            {"while", WHILE},
            {"null", NIL},
            {"int", TYPE_INT},
            {"string", TYPE_STRING},
            {"bool", TYPE_BOOL},
            {"char", TYPE_CHAR},
        };
        
        
        // navigation methods
        char advance();
        bool match(char expected);
        bool isAtEnd() const;
        char peek() const;
        char peekNext() const;
        void skipWhitespace();

        // classification methods
        bool isDigit(char c) const;
        bool isAlpha(char c) const;
        bool isAlphaNumeric(char c) const;
        
        // token methods
        Token string();
        Token number();
        Token identifier();
        
        Token makeToken(TokenType type) const;
        Token makeToken(TokenType type, std::string lexeme) const;
        
        Token scanToken();

};

#endif