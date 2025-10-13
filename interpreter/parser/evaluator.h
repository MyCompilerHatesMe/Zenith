#ifndef PARSER_H
#define PARSER_H

#include "interpreter/token.h"
#include <memory>
#include <string>
#include <vector>

using std::unique_ptr;

struct Expression {
    virtual ~Expression() = default;
};
struct BinaryExpression : Expression {
    unique_ptr<Expression> left;
    Token op;
    unique_ptr<Expression> right;
};
struct UnaryExpression : Expression {
    Token op;
    unique_ptr<Expression> expr;
};
struct LiteralExpression : Expression {
    Token op;
};

class Parser {
    public:
        Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}
        // Parses the tokens and returns the root of the AST
        unique_ptr<Expression> parseExpression();
    
    private:
        std::vector<Token> tokens;
        int current;
        
        // navigation
        Token peek() const;
        Token advance();
        Token previous() const;
        bool isAtEnd() const;

        // checking and consuming
        bool match(TokenType type);
        bool check(TokenType type) const;
        Token consume(TokenType type, const std::string& message);

        //Levels of Precedence
        unique_ptr<Expression> parseAddition();
        unique_ptr<Expression> parseMultiplication();
        unique_ptr<Expression> parseUnary();
        unique_ptr<Expression> parsePrimary();
};

#endif 
