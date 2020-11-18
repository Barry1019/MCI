#ifndef ASSIGNMENT_TOKENISER_H
#define ASSIGNMENT_TOKENISER_H

#include <string>

// Interface to the assignment tokeniser
namespace Assignment_Tokeniser
{
    // a shorthand for string
    typedef std::string string ;

    // the ID used to reference a Token - the implementation is hidden
    class _Token ;
    typedef _Token *Token ;

    // ***** BNF *****
    //
    // *** SYNTAX
    // * ASCII literals inside ' '
    // * grouping ( )
    // * ASCII ranges -
    // * alternatives |
    // * 0 or 1 ?
    // * 0 or more *
    // * 1 or more +
    //
    // *** TOKEN RULES
    // * Grammar rule ::= Definition
    //
    // * input ::= (wspace | token)* eoi
    //
    // * wspace ::= '\n' | ' '
    //
    // * token ::= identifier | number | string | keyword | symbol | eol_comment | adhoc_comment
    //
    // * identifier ::= (letter|'$') (letter|digit|'-'|'$'|'.')*
    //   - letter ::= 'a'-'z'|'A'-'Z'
    //
    // * number ::= integer | scientific
    //
    // * integer ::= zero | digits
    //   - zero ::= '0'
    //   - digits ::= digit19 digit*
    //   - digit19  ::= '1'-'9'
    //   - digit  ::= '0'-'9'
    //
    // * scientific ::= integer fraction? exponent
    //   - fraction ::= '.' digit*
    //   - exponent ::= ('e'|'E') ('+'|'-')? digits0123
    //   - digits0123 ::= zero | digits123
    //   - digits123 ::= digit19 digit? digit?
    //
    // * string ::= '"' instring* '"'
    //   - instring ::= ' '|'!'|'#'-'~'
    //
    // * keyword ::= 'done'|'while'|'procedure'|'if-goto'|'this' 
    //   - keywords each have their own TokenKind
    //
    // * symbol ::= '@'|'-='|'+='|'*='|'/='|'!='|'=='|'<<<'|'<<'|'>>>'|'>>'|'{'|'}'|'('|')'|'['|']'
    //   - symbols each have their own TokenKind
    //
    // * eol_comment ::= '//' eol_comment_char* '\n'
    //   - eol_comment_char ::= ' '-'~'
    //
    // * adhoc_comment ::= '/*' adhoc_comment_char* '*/'
    //   - adhoc_comment_char ::= eol_comment_char|'\n'
    //
    // ** ERROR HANDLING
    // * when next_token() is called
    //   * if the first character cannot start a token, call c_did_not_find() to report an error
    // * after the first character of a token has been found
    //   * if the next character cannot extend the current token,
    //     * if the characters read so far can form a token, that token is returned
    //     * if the characters read so far cannot form a token, call c_did_not_find() to report an error
    // * once the tk_eoi token is returned,
    //   * all future calls to nextch() will not read any more input
    //   * all future calls to next_token() will return the tk_eoi token
    //
    // ** INPUT - tokeniser-basics.h
    // * all input must be read using the nextch() function provided by tokeniser-basics.cpp
    // * all tokens must be created using the new_token() function provided by tokeniser-basics.cpp
    // * nextch() must store the character it reads in ch
    //
    // ** INPUT PREPROCESSING - not required for MILESTONE SUBMISSIONS
    // * nextch() replaces tab characters ('\t') with one or more spaces so that the current line length becomes a multiple of four characters
    // *          if a tab is read nextch() returns a space,
    // *          if the tab must be replaced by 3 spaces then the next 2 calls of nextch() also return a space
    // * nextch() never sets ch to be a tab character
    // * nextch() discards carriage return characters ('\r') that are immediately followed by a newline character ('\n')
    // * nextch() replaces carriage return characters that are not immediately followed by a newline character with a newline character
    // * nextch() never sets ch to be a carriage return character
    // * only characters assigned to ch can appear in the results of calls to token_context()
    //
    // ** EXTRA FUNCTIONS - tokeniser-extras.h
    // * nextch() must read all input using the read_char() function
    // * c_did_not_find() must be called when an expected character cannot be found
    // * c_have(), c_have_next(), c_mustbe() are expected to be used appropriately to parse tokens
    //
    // ** CHANGES TO TOKEN SPELLING - not required for MILESTONE SUBMISSIONS
    // * after parsing and creating a token, modify the token's spelling as follows:
    // * the '"' and '"' enclosing a string must be removed from the token's spelling
    // * the '//' and '\n' enclosing an eol_comment must be removed from the token's spelling
    // * the '/*' and '*/' enclosing an adhoc_comment must be removed from the token's spelling
    // * the token's spelling for a scientific must be modified as follows:
    //   * the value of the number must be expressed in scientific notation, eg 712.34e4 -> 7.1234e+6
    //   * if the value is 0, the spelling is "0.0e+0"
    //   * there must be a single non-zero digit before the '.'
    //   * there must be at least one digit after the '.'
    //   * there must be no trailing '0's after the first digit after the '.'
    //   * the exponent must be signed
    // * call the set_spelling() function provided by tokeniser-basics.cpp to change the spelling
    // * the original spelling is displayed in the results of calls to token_context()
    //
    // ** KEYWORDS - not required for MILESTONE SUBMISSIONS
    // * all the keywords satisfy the grammar rule for an identifier
    //   * keywords must be identified by
    //     1. parsing an identifier token
    //     2. calling token_spelling() to the identifier's spelling
    //     3. calling keyword_or_identifier() to get the TokenKind for the spelling
    //     4. if it is a keyword, call set_kind() to correct the token's kind

    //
    // ** FINAL SUBMISSION Testing
    // * the final submission tests use input files that may contain tab ('\t') and carriage return ('\r') characters.
    // * do not expect to pass most final submission tests until you have properly implemented the input preprocessing.
    //
    // ** EXPECTED TEST OUTPUTS
    // * you should consult the expected test outputs if you need help interpreting the token definitions
    //


    // The kinds of token that are recognised
    enum TokenKind
    {
        // MILESTONE SUBMISSION Tokens to be recognised:

        tk_space,           // ' '
        tk_newline,         // '\n'

        tk_identifier,      // identifier
        tk_integer,         // integer

        tk_at,              // '@'
        tk_stop,            // '.'
        tk_lcb,             // '{'
        tk_rcb,             // '}'
        tk_lrb,             // '('
        tk_rrb,             // ')'
        tk_lsb,             // '['
        tk_rsb,             // ']'

        ////////////////////

        // FINAL SUBMISSION Tokens to be recognised: all of the above and

        tk_eol_comment,     // '//' eol_comment_char* '\n', '//' and '\n' are not part of the spelling
        tk_adhoc_comment,   // '/*' adhoc_comment_char* '*/', '/*' and '*/' are not part of the spelling

        tk_scientific,      // scientific
        tk_string,          // '"' instring* '"', '"' and '"' are not part of the spelling

        tk_done,            // 'done'
        tk_while,           // 'while'
        tk_procedure,       // 'procedure'
        tk_if_goto,         // 'if-goto'
        tk_this,            // 'this'

        tk_sub_assign,      // '-='
        tk_add_assign,      // '+='
        tk_mult_assign,     // '*='
        tk_div_assign,      // '/='
        tk_not_eq,          // '!='
        tk_eq,              // '=='
        tk_lshift_l,        // '<<<
        tk_lshift,          // '<<
        tk_rshift_l,        // '>>>'
        tk_rshift,          // '>>'

        ////////////////////

        tk_eoi,             // end of input reached

        tk_oops,            // for error handling

                            // the remaining tokens are not produced by the tokeniser
                            // these are provided to describe groups of tokens, hence named tg_* not tk_*

        tg_wspace,          // one of tk_space or tk_newline
        tg_number,          // one of tk_integer or tk_scientific
        tg_keyword,         // one of tk_done, tk_while, tk_procedure, tk_if_goto or tk_this
        tg_symbol           // one of tk_at, tk_sub_assign, tk_add_assign, tk_mult_assign, tk_div_assign, tk_not_eq, tk_eq,
                            //        tk_lshift_l, tk_lshift, tk_rshift_l, tk_rshift, tk_lcb, tk_rcb, tk_lrb, tk_rrb, tk_lsb or tk_rsb
    } ;

    // ***** the following are implemented in the pre-compiled lib/.../lib.a object file *****

    // return a string representation of the given token kind
    // returns "" if the kind is not recognised
    extern string to_string(TokenKind kind) ;

    // return the TokenKind for the given token
    // it is a fatal error if the token was not returned by a call to next_token()
    extern TokenKind token_kind(Token token) ;

    // return the characters that make up the given token
    // it is a fatal error if the token was not returned by a call to next_token()
    extern string token_spelling(Token token) ;

    // return the line number where the given token was found, the first line is line number 1
    // it is a fatal error if the token was not returned by a call to next_token()
    extern int token_line(Token token) ;

    // return the column position where the given token starts on its line, the first character is in column number 1
    // it is a fatal error if the token was not returned by a call to next_token()
    extern int token_column(Token token) ;

    // return a string representation of the given token
    // it is a fatal error if the token was not returned by a call to next_token()
    extern string to_string(Token token) ;

    // return a verbose string representation of the given token
    // it is a fatal error if the token was not returned by a call to next_token()
    extern string to_string_verbose(Token token) ;

    // is the token of the given kind or does it belong to the given group of tokens?
    // eg token_is(t,tk_colon) returns true if token_kind(t) is tk_colon
    // eg token_is(t,tk_op) returns true if token_kind(t) is tk_add, tk_sub, tk_times, tk_divide or tk_op
    extern bool token_is(Token token,TokenKind kind_or_grouping) ;

    // ***** the workshop involves implementing the following functions by completing tokeniser.cpp *****

    // read the next token from standard input
    extern Token next_token() ;

    // ***** the workshop involves implementing the following functions by completing tokeniser-basics.cpp *****

    // initialiase the tokeniser
    extern void initialise_tokeniser() ;

    // generate a context string for the given token
    // all lines are shown after the preprocssing of special characters
    // it shows the line before the token, with a '$' marking the end of line,
    // the line containing the token up to the end of the token, and
    // a line of spaces ending with a ^ under the token's first character to mark its position
    // the lines are prefixed by line numbers, right justified in four spaces, followed by ": ", eg "  45: "
    // NOTE: if a token includes newline character(s), only display the token before the first newline followed by a '$'
    // NOTE: this function is not required for the milestone submission
    extern string token_context(Token token) ;
}
#endif //ASSIGNMENT_TOKENISER_H
