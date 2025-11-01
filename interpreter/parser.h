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
        std::unique_ptr<Expression> parseAssignment();      // Handles: =
        std::unique_ptr<Expression> parseLogicalOr();       // Handles: ||
        std::unique_ptr<Expression> parseLogicalAnd();      // Handles: &&
        std::unique_ptr<Expression> parseEquality();        // Handles: ==, !=
        std::unique_ptr<Expression> parseComparison();      // Handles: <, >, <=, >=
        std::unique_ptr<Expression> parseAddition();        // Handles: +, -
        std::unique_ptr<Expression> parseMultiplication();  // Handles: *, /, %
        std::unique_ptr<Expression> parseUnary();           // Handles: -, !
        std::unique_ptr<Expression> parseCall();            // Handles: (), ., []
        std::unique_ptr<Expression> parsePrimary();         // Handles: literals, identifiers, ()
};

#endif 
