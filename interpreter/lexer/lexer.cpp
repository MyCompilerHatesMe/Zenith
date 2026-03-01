#include "lexer.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

using std::cerr;
using std::vector;
using std::exit;


Lexer::Lexer(const std::string& source) : source(source) {
    // the member initializer list ": source(source)" does all the work.
    // it directly constructs the class's 'source' member with the
    // 'source' string that was passed into the constructor.
}


vector<Token> Lexer::scanTokens() {
    vector<Token> tokens;

    while(true) {
        Token token = scanToken();

        tokens.push_back(token);

        if(token.type == END_OF_FILE)
            break; 
    }

    return tokens;
}


Token Lexer::scanToken() {

    // skip whitespace woah, who would've thought
    skipWhitespace();
    
    // check if we're at EOF return makeToken(END_OF_FILE)
    if (isAtEnd()) return makeToken(END_OF_FILE);
    
    // start -> current index
    start = current;

    // consume a character advance()
    char c = advance();

    if(isAlpha(c)) return identifier();
    if(isDigit(c)) return number();


    // switch (char) 
    // for single character tokens return makeToken(TokenType)
    // for mutlichar tokens, check next char with match()
    switch (c) {
        // single char tokens
        case '(': return makeToken(LEFT_PAREN);
        case ')': return makeToken(RIGHT_PAREN);
        case '{': return makeToken(LEFT_BRACE);
        case '}': return makeToken(RIGHT_BRACE);
        case ',': return makeToken(COMMA);
        case '.': return makeToken(DOT);
        case '-': return makeToken(MINUS);
        case '+': return makeToken(PLUS);
        case ';': return makeToken(SEMICOLON);
        case '*': return makeToken(STAR);
        // skip whitespace handles comments
        case '/': return makeToken(SLASH);

        // one or two char tokens
        case '!': return makeToken(match('=') ? BANG_EQUAL : BANG);
        case '=': return makeToken(match('=') ? EQUAL_EQUAL : EQUAL);
        case '<': return makeToken(match('=') ? LESS_EQUAL : LESS);
        case '>': return makeToken(match('=') ? GREATER_EQUAL : GREATER);

        // string
        case '"': return string();
    }

    // uhhhh what? how did you get here
    cerr << "Unexpected characater '" << c << "' at line " << line << "\n";
    exit(1);
}

/// Navigation functions

char Lexer::advance() {
    return source[current++];
}

bool Lexer::match(char expected){
    if(peek() == expected){
        advance();
        return true;
    }
    return false;
}

bool Lexer::isAtEnd() const {
    return current >= source.length();
}

char Lexer::peek() const{
    if(isAtEnd()) return '\0';
    return source[current];
}
char Lexer::peekNext() const{
    if(current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

void Lexer::skipWhitespace() {
    while (true) {
        char c = peek();
        switch (c) {
            // actual empty space
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line++;
                advance();
                break;

            // comment case
            case '/':
                if (peekNext() == '/') {
                    // comment so skip to EOL
                    while (peek() != '\n' && !isAtEnd()) advance();
                } 
                // division operator
                else return;
                break;

            // not a whitespace character. give it back to scanToken();
            default:
                return;
        }
    }
}


/// Classification functions

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) const{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

Token Lexer::string() {
    // just realised start = current should be in scanToken not here
    while(peek() != '"' && !isAtEnd()){
        if(peek() == '\n') line++;
        advance();
    }

    if(isAtEnd()){
        cerr << "Unterminated string at line " << line << "\n";
        exit(1);  // error means fuck you get out
    }

    // consume the " from the stream
    advance();
    // token will figure everything else
    return makeToken(STRING);
}

Token Lexer::number() {
    while(isDigit(peek())){
        advance();
    }
    return makeToken(NUMBER);
}

Token Lexer::identifier() {
    while(isAlphaNumeric(peek())){
        advance();
    }

    // find the identifiers contents.
    // this has to be std::string cuz Lexer::string exists alr
    std::string_view text(source.data() + start, current - start);

    auto it = keywords.find(std::string(text));
    
    //if its in the map return a keyword type
    if(it != keywords.end()) return makeToken(it->second);
    //if its not in the map, return an identifier
    return makeToken(IDENTIFIER);
}

Token Lexer::makeToken(TokenType type) const {
    if(type != END_OF_FILE){
        return {
            type,
            std::string_view(source.data() + start, current - start),
            line,
        };
    }else{
        return {
            type,
            "EOF",
            line,
        };
    }
}