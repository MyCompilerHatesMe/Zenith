#include "lexer.h"

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

bool Lexer::isDigit(char c) const{
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) const{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const{
    return isAlpha(c) || isDigit(c);
}
