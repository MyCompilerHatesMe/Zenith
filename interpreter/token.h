#ifndef TOKEN_H
#define TOKEN_H


#include <string>

enum TokenType {
    // single characters
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR,

    // one or two character tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // literals
    IDENTIFIER, STRING, NUMBER,

    // keywords
    AND, CLASS, ELSE, FALSE,
    FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE,
    VAR, WHILE,

    // end of file
    END_OF_FILE,
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};


#endif