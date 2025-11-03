#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "interpreter/lexer/lexer.h"
#include "interpreter/token.h"

using std::string;
using std::ifstream;

string readFile(const string& path);
std::string tokenTypeToString(TokenType type);

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cerr << "Usage: zenith <filename>\n";
        return 1;
    }
    string sourceCode = readFile(argv[1]);

    if (sourceCode.empty())
        return 1;
        //readFile will print error

    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.scanTokens();
    for(const Token& token : tokens)
        std::cout << tokenTypeToString(token.type) << " " << token.lexeme << "\n";

}

string readFile(const string& path){
    
    if (path.length() <= 4 || path.substr(path.length() - 4) != ".zen") {
        std::cerr << "[ERROR] File must have a .zen extension: '" << path << "'\n";
        return "";
    }
    ifstream inputFile(path);

    if(!inputFile.is_open()){
        std::cerr << "[ERROR] Could not open file '" << path << "'\n";
        return "";
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();

    return buffer.str();
}

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case LEFT_PAREN:    return "LEFT_PAREN";
        case RIGHT_PAREN:   return "RIGHT_PAREN";
        case LEFT_BRACE:    return "LEFT_BRACE";
        case RIGHT_BRACE:   return "RIGHT_BRACE";
        case COMMA:         return "COMMA";
        case DOT:           return "DOT";
        case MINUS:         return "MINUS";
        case PLUS:          return "PLUS";
        case SEMICOLON:     return "SEMICOLON";
        case SLASH:         return "SLASH";
        case STAR:          return "STAR";
        case BANG:          return "BANG";
        case BANG_EQUAL:    return "BANG_EQUAL";
        case EQUAL:         return "EQUAL";
        case EQUAL_EQUAL:   return "EQUAL_EQUAL";
        case GREATER:       return "GREATER";
        case GREATER_EQUAL: return "GREATER_EQUAL";
        case LESS:          return "LESS";
        case LESS_EQUAL:    return "LESS_EQUAL";
        case IDENTIFIER:    return "IDENTIFIER";
        case STRING:        return "STRING";
        case NUMBER:        return "NUMBER";
        case AND:           return "AND";
        case CLASS:         return "CLASS";
        case ELSE:          return "ELSE";
        case FALSE:         return "FALSE";
        case FUN:           return "FUN";
        case FOR:           return "FOR";
        case IF:            return "IF";
        case NIL:           return "NIL";
        case OR:            return "OR";
        case PRINT:         return "PRINT";
        case RETURN:        return "RETURN";
        case SUPER:         return "SUPER";
        case THIS:          return "THIS";
        case TRUE:          return "TRUE";
        case VAR:           return "VAR";
        case WHILE:         return "WHILE";
        case END_OF_FILE:   return "END_OF_FILE";
        case TYPE_INT:      return "TYPE_INT";
        case TYPE_STRING:   return "TYPE_STRING";
        case TYPE_BOOL:     return "TYPE_BOOL";
        case TYPE_CHAR:     return "TYPE_CHAR";
        default:            return "UNKNOWN";
    }
}