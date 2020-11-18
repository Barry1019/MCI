#ifndef WORKSHOP_TOKENISER_EXTRAS_H
#define WORKSHOP_TOKENISER_EXTRAS_H


#include "tokeniser.h"

// This include file provides definitions that are not required to use the tokeniser
// It extends the interface for the workshop tokeniser
//
// Some functions are provided by tokeniser-basics.cpp
// tokeniser-basics.cpp may be replaced during testing
//
namespace Assignment_Tokeniser
{
    // character groupings so we can easily check if a character belongs to a group
    // for use with c_have(), c_have_next(), c_mustbe() and c_did_not_find()
    // groups start after 128 so they do not clash with ASCII chars or EOF(-1 => 255)
    // a char can be passed to a function expecting a Char
    enum Char : int
    {
        cg_min = 128,           // make all groups start after 128

        cg_start_of_token,      // '\n' | ' ' | '$' | ('a'-'z') | ('A'-'Z') | ('0'-'9') | '"' | '/' |
                                // '@' | '-' | '+' | '*' | '!' | '=' | '<' | '>' | '{' | '}' | '(' | ')' | '[' | ']'

                                // any wspace character
        cg_wspace,              // '\n' | ' '

                                // internal parts of a number
        cg_digit,               // '0'-'9'
        cg_digit19,             // '1'-'9'
        cg_start_of_fraction,   // '.'
        cg_start_of_exponent,   // 'e' | 'E'
        cg_sign,                // '-' | '+'

                                // internal part of an identifier
        cg_extends_identifier,  // '$' | '-' | '.' | ('a'-'z') | ('A'-'Z') | ('0'-'9')

                                // any op
        cg_instring,            //  ' ' | '!' | '#'-'~'

                                // comments
        cg_after_slash,         // '/' | '*' | '='
        cg_eol_comment_char,    // ' '-'~'
        cg_adhoc_comment_char,  // ' '-'~' | '\n'

        cg_oops                 // for error handling
    } ;

    // ***** the following are implemented in the pre-compiled lib/lib.a object file *****

    // ***** Parsing functions *****
 
    // report an error and exit because ch was not expected?
    // if ch is expected, this call is assumed to be the result of a logic error!
    extern void c_did_not_find(int expected) ;

    // is ch the given character or does it belong to the given group of characters?
    // eg if ch is '0' c_have(cg_integer) returns true
    // eg if ch is '=' c_have('=') returns true
    extern bool c_have(int expected) ;

    // is ch expected, if it is also call nextch()?
    extern bool c_have_next(int expected) ;

    // ch must be expected, if it is call next(),
    // otherwise call c_did_not_find(expected)?
    extern void c_mustbe(int expected) ;

    // ***** Token functions *****
 
    // if s is a keyword it returns its token kind otherwise it returns tk_identifier
    // eg keyword_or_identifier("if") returns tk_if, keyword_or_identifier("bob") returns tk_identifier
    extern TokenKind keyword_or_identifier(string s) ;

    // create a new Token object and return its ID
    extern Token new_token(TokenKind kind,string spelling,int line,int column) ;

    // set the TokenKind for the given token
    // it is a fatal error if the token was not returned by a call to new_token()
    extern void set_token_kind(Token token,TokenKind kind) ;

    // change the spelling for the given token
    // it is a fatal error if the token was not returned by a call to new_token()
    extern void set_token_spelling(Token token,string spelling) ;

    // return the original spelling used to create the token
    // it is a fatal error if the token was not returned by a call to new_token()
    extern string token_original(Token token) ;

    // is kind the given kind or does it belong to the given group of token kinds?
    // eg token_is(tk_colon,tk_colon) returns true
    // eg token_is(tk_sub,tk_op) returns true
    extern bool kind_is(TokenKind kind,TokenKind kind_or_grouping) ;

    // ***** Input functions *****
 
    // read the next character from the input, this may or may not be standard input during testing
    extern char read_char() ;

    // ***** the workshop involves implementing the following functions by completing tokeniser-basics.cpp *****

    // ***** Input functions *****
 
    // read next character if not at the end of input
    // no tab (\t) or carriage return ('\r') characters are returned
    // \t will have been replaced by sufficient spaces to reach the next 4 column boundary
    // \r\n will have been replaced by a \n, a single \r will have been replaced by \n
    // all input must be read using this function
    // this must be called once to initialise the tokeniser
    extern void nextch() ;

    // the last character read by nextch()
    extern char ch ;

    // ***** Token functions *****
 
    // create a new Token object of the given kind and return its ID
    // the token's spelling is all but the last character read by nextch() since the last call to this function
    // the token's length is the length of the spelling
    // the token's line, column correspond to its first character
    extern Token new_token(TokenKind kind) ;
}

#endif //WORKSHOP_TOKENISER_EXTRAS_H
