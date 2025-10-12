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

