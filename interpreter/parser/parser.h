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
struct IdentifierExpression : Expression {
    Token name;
};
struct AssignmentExpression : Expression {
    Token name;
    unique_ptr<Expression> value;
};

// statements
struct Statement {
    virtual ~Statement() = default;
};

// display(expr)
struct PrintStatement : Statement {
    unique_ptr<Expression> expr;
};

// type x = expr;
struct VarDeclStatement : Statement {
    TokenType typeKeyword;
    Token name;
    unique_ptr<Expression> initialiser;
};

// {statement*}
struct BlockStatement : Statement {
    std::vector<unique_ptr<Statement>> statements;
};

// if (expr) block (else)?
struct IfStatement : Statement {
    unique_ptr<Expression> condition;
    unique_ptr<Statement> thenBranch;
    unique_ptr<Statement> elseBranch; // null if no else
};

// while (expr) block
struct WhileStatement : Statement {
    unique_ptr<Expression> condition;
    unique_ptr<Statement> body;
};

// for (expr; expr; expr;) block
struct ForStatement : Statement {
    unique_ptr<Expression> init;
    unique_ptr<Expression> condition;
    unique_ptr<Expression> increment;    
    unique_ptr<Statement> body;
};

// expr;
struct ExpressionStatement : Statement {
    unique_ptr<Expression> expr;
};


class Parser {
    public:
        Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}
        // Parses the tokens and returns the root of the AST
        std::vector<unique_ptr<Statement>> parse();
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
        bool isTypeKeyword() const;

        // statement parsing
        unique_ptr<Statement> parseStatement();
        unique_ptr<Statement> parseVarDecl();
        unique_ptr<Statement> parsePrintStatement();
        unique_ptr<Statement> parseIfStatement();
        unique_ptr<Statement> parseWhileStatement();
        unique_ptr<Statement> parseForStatement();
        unique_ptr<Statement> parseBlock();
        unique_ptr<Statement> parseExpressionStatement();

        //Levels of Precedence` 
        /*
        Right now it is only on the basic arithmetic that includes +, -[Addition], *, /[Multiplication] 
        and then unary operator
        */
        unique_ptr<Expression> parseAssignment();
        unique_ptr<Expression> parseLogicalOr();
        unique_ptr<Expression> parseLogicalAnd();
        unique_ptr<Expression> parseEquality();
        unique_ptr<Expression> parseComparision();
        unique_ptr<Expression> parseAddition();
        unique_ptr<Expression> parseMultiplication();
        unique_ptr<Expression> parseUnary();
        unique_ptr<Expression> parsePrimary();
};

#endif