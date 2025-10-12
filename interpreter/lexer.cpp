#include "lexer.h"
#include <iostream>
#include <cstdlib>

using std::cerr;

Token Lexer::scanToken() {

    // loop through everything
    // consume and delete whitespace \t, \r
    // if \n increment line

    //if // is found, skip to end of line

    // after all meaningless tokens are consumed
    // check if we're at EOF return makeToken(END_OF_FILE)

    // start -> current index
    // consume a character advance()
    
    // switch (char) 
    // for single character tokens return makeToken(TokenType)
    // for mutlichar tokens, check next char with match()

    // if " return string()
    // if digit return number()
    // if alpha return identifier()
    // else error  
}

/// Navigation functions

char Lexer::advance(){
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


/// Classification functions

bool Lexer::isDigit(char c) const{
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) const{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const{
    return isAlpha(c) || isDigit(c);
}

Token Lexer::string(){
    // just realised start = current should be in scanToken not here
    while(peek() != '"' && !isAtEnd()){
        if(peek() == '\n') line++;
        advance();
    }

    if(isAtEnd()){
        cerr << "Unterminated string at line " << line << "\n";
        std::exit(1);  // error means fuck you get out
    }

    // consume the " from the stream
    advance();
    // token will figure everything else
    return makeToken(STRING);
}

Token Lexer::number(){
    while(isDigit(peek())){
        advance();
    }
    return makeToken(NUMBER);
}

Token Lexer::identifier(){
    start = current;
    while(isAlphaNumeric(peek())){
        advance();
    }

    // find the identifiers contents.
    std::string text = source.substr(start, current - start);

    auto it = keywords.find(text);
    
    //if its in the map return a keyword type
    if(it != keywords.end()) return makeToken(it->second);
    //if its not in the map, return an identifier
    return makeToken(IDENTIFIER, text);
}

Token Lexer::makeToken(TokenType type) const{
    return {
        type,
        source.substr(start, current - start),
        line,
    };
}

// one less method call. yes. i care.
Token Lexer::makeToken(TokenType type, std::string lexeme) const{
    return{
        type,
        lexeme,
        line,
    };
}