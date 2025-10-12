#define PARSER_h
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "token.h"

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};

