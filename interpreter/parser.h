#ifndef PARSER_H
#define PARSER_H

#include <token.h>
#include <memory>
#include <string>
#include <vector>
struct Expression {
    virtual ~Expression() = default;
};
struct BinaryExpression : Expression {
    std::unique_ptr<Expression> left;
    Token op;
    std::unique_ptr<Expression> right;
};
struct UnaryExpression : Expression {
    Token op;
    std::unique_ptr<Expression> expr;
};
struct LiteralExpression : Expression {
    Token op;
};

class Parser {
    private:
    std::vector<Token> tokens;
    int current;
    public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}
    // Parses the tokens and returns the root of the AST
    std::unique_ptr<Expression> parseExpression();
    private:
        Token peek() const;
        Token advance();
        bool match(TokenType type);
        Token consume(TokenType type, const std::string& message);
        bool check(TokenType type) const;
        bool isAtEnd();

        //Levels of Precedence
        std::unique_ptr<Expression> parseAssignment();
        std::unique_ptr<Expression> parseLogicalOr();
        std::unique_ptr<Expression> parseLogicalAnd();
        std::unique_ptr<Expression> parseEquality();
        std::unique_ptr<Expression> parseComparison();
        std::unique_ptr<Expression> parseAddition();
        std::unique_ptr<Expression> parseMultiplication();
        std::unique_ptr<Expression> parseUnary();
        std::unique_ptr<Expression> parseCall();
        std::unique_ptr<Expression> parsePrimary();
};

#endif 
