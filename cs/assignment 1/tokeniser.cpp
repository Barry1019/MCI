//  name: Shaokang Ma
// id: a1790846

#include "tokeniser-extras.h"

// to shorten the code
using namespace std ;

/////////////////////////////////////////////////////////////////////////

namespace Assignment_Tokeniser
{

    static TokenKind new_token_kind ;

    // parse one extra character so that ch is the start of the next token
    // we have already parsed all the characters that make up the current token
    // we just need to remember the token kind
    static void parse_extra(TokenKind kind)
    {
        new_token_kind = kind ;
        nextch() ;
    }

    // parse a single character of white space
    // wspace ::= '\n' | ' '
    // ch is the white space character
    static void parse_wspace(TokenKind kind)
    {
        parse_extra(kind) ;
    }

    // parse an identifier - always read one extra character
    // * identifier ::= (letter|'$') (letter|digit|'-'|'$'|'.')*
    //   - letter ::= 'a'-'z'|'A'-'Z'
    // ch is the first character of the identifier
    static void parse_identifier()
    {
        new_token_kind = tk_identifier ;
        
        do nextch() ; while ( c_have(cg_extends_identifier) ) ;
    }

    // parse an integer - always read one extra character
    // * integer ::= zero | digits
    //   - zero ::= '0'
    //   - digits ::= digit19 digit*
    //   - digit19  ::= '1'-'9'
    //   - digit  ::= '0'-'9'
    static void parse_integer()
    {
        // we are parsing an integer
        new_token_kind = tk_integer ;

        //  if ch is digits
        if ( c_have(cg_digit19) )
        {
            do nextch() ; while ( c_have( cg_digit) ) ;
        } else
        {
            nextch() ;       //  if ch is zero
        }
    }

    // * scientific ::= integer fraction? exponent
    //   - fraction ::= '.' digit*
    //   - exponent ::= ('e'|'E') ('+'|'-')? digits0123
    //   - digits0123 ::= zero | digits123
    //   - digits123 ::= digit19 digit? digit?
    static void parse_scientific()
    {
        //  if ch is integer
        parse_integer() ;

        //  if ch is fraction
        if ( c_have(cg_start_of_fraction) )
        {
            //  we are parsing a scientific
            new_token_kind = tk_scientific ;
            do nextch() ; while ( c_have(cg_digit) ) ;
            if ( !c_have(cg_start_of_exponent) ) c_did_not_find(cg_start_of_exponent) ;
        }

        //  if ch is start of exponent
        if ( c_have(cg_start_of_exponent) )
        {
            //  we are parsing a scientific
            new_token_kind = tk_scientific ;
            nextch();
            //  if ch is a sign
            if ( c_have(cg_sign) ) nextch() ;
            if ( c_have('0') ) nextch(); else
            if ( c_have(cg_digit19) )
            {
                nextch() ;
                if ( c_have(cg_digit) ) nextch() ;
                if ( c_have(cg_digit) ) nextch() ;
            } else
            {
                c_did_not_find(cg_digit) ;
            }    
        }
    }

    // * number ::= integer | scientific
    static void parse_number()
    {
        new_token_kind = tg_number ;

        parse_scientific() ;
    }

    // * string ::= '"' instring* '"'
    //   - instring ::= ' '|'!'|'#'-'~'
    static void parse_string()
    {
        new_token_kind = tk_string ;

        do nextch() ; while ( c_have(cg_instring) ) ;
        c_mustbe('"') ;
        nextch() ;
    }

    // parse << or <<< - always read one extra character
    static void parse_lshift_l()
    {
        nextch() ;
        if (c_have('<') )
        {
            new_token_kind = tk_lshift ;
            nextch() ;
            if ( c_have('<') )
            {
                new_token_kind = tk_lshift_l ;
                nextch() ;
            }
        } else
        {
            c_did_not_find('<') ;
        }
    }

    // parse >> or >>> - always read one extra character
    static void parse_rshift_l()
    {
        nextch() ;
        if (c_have('>') )
        {
            new_token_kind = tk_rshift ;
            nextch() ;
            if ( c_have('>') )
            {
                new_token_kind = tk_rshift_l ;
                nextch() ;
            }
        } else
        {
            c_did_not_find('>') ;
        }
    }

    //  we are parsing a symbol - except '/'
    static void parse_symbol()
    {
        new_token_kind = tg_symbol ;

        switch (ch)
        {
        case '@':
            parse_extra(tk_at) ;
            break ;
        case '.':
            parse_extra(tk_stop) ;
            break ;
        case '{':
            parse_extra(tk_lcb) ;
            break ;
        case '}':
            parse_extra(tk_rcb) ;
            break ;
        case '(':
            parse_extra(tk_lrb) ;
            break ;
        case ')':
            parse_extra(tk_rrb) ;
            break ;
        case '[':
            parse_extra(tk_lsb) ;
            break ;
        case ']':
            parse_extra(tk_rsb) ;
            break ;
        case '<':
            parse_lshift_l() ;
            break ;
        case '>':
            parse_rshift_l() ;
            break;
        case '-':
            new_token_kind = tk_sub_assign ;
            nextch() ;
            if ( c_have('=') ) nextch() ; else
            c_did_not_find('=') ;
            break ;
        case '+':
            new_token_kind = tk_add_assign ;
            nextch() ;
            if ( c_have('=') ) nextch() ; else
            c_did_not_find('=') ;
            break ;
        case '*':
            new_token_kind = tk_mult_assign ;
            nextch() ;
            if ( c_have('=') ) nextch() ; else
            c_did_not_find('=') ;
            break ;
        case '!':
            new_token_kind = tk_not_eq ;
            nextch() ;
            if ( c_have('=') ) nextch() ; else
            c_did_not_find('=') ;
            break ;
        case '=':
            new_token_kind = tk_eq ;
            nextch() ;
            if ( c_have('=') ) nextch() ; else
            c_did_not_find('=') ;
            break ;
        }
    }

    //  we are parsing comments - eol
    // * eol_comment ::= '//' eol_comment_char* '\n'
    //   - eol_comment_char ::= ' '-'~'
    static void parse_eol_comment()
    {
        new_token_kind = tk_eol_comment ;
        do nextch() ; while ( c_have(cg_eol_comment_char) ) ;
        if ( c_have('\n') ) nextch() ; else c_did_not_find('\n') ;
    }
    
    //  we are parsing comments - adhoc_comment
    // * adhoc_comment ::= '/*' adhoc_comment_char* '*/'
    //   - adhoc_comment_char ::= eol_comment_char|'\n'
    static void parse_adhoc_comment()
    {
        new_token_kind = tk_adhoc_comment ;
        nextch() ;
        while ( c_have(cg_adhoc_comment_char) )
        {
            if ( c_have('*') )
            {
                nextch() ;
                if ( c_have('/') )
                {
                    nextch() ;
                    break ;
                }
            } else
            {
                nextch() ;
            }
        }
    }

    //  we are parsing div_assign
    static void parse_div_assgin()
    {
        parse_extra(tk_div_assign) ;
    }

    //  we are parsing tokens after a slash
    static void parse_after_slash()
    {
        new_token_kind = tk_eol_comment ;
        nextch() ;
        if ( c_have('/') ) parse_eol_comment() ; else
        if ( c_have('*') ) parse_adhoc_comment() ; else
        if ( c_have('=') ) parse_div_assgin() ; else
        c_did_not_find(cg_after_slash) ;
    }

    //  modify the spelling of a scientific
    //  the value of the number must be expressed in scientific notation, eg 712.34e4 -> 7.1234e+6
    static string modify_scientific(string spelling)
    {
        string left ;
        string right ;
        bool is_right;
        size_t dot = spelling.find_first_of('.') ;
        int exponent = spelling.find_first_of("eE") ;
        
        // split the spelling into integer, fraction and exponent parts
        int length = atoi(spelling.substr(exponent + 1, spelling.length()).c_str()) ;
        if ( dot != string::npos )
        {
            left = spelling.substr(0, dot) ;
            right = spelling.substr(dot + 1, exponent - dot - 1) ;
            is_right = true ;
        } else
        {
            left = spelling.substr(0, exponent) ;
            is_right = false ;
        }

        //  if there is a fraction part
        if ( is_right )
        {
            if ( atoi(left.c_str()) == 0 )
            {
                if ( atoi(right.c_str()) == 0 ) return "0.0e+0" ; else
                {
                    length -= right.find_first_of("123456789") ;
                    left = right[right.find_first_of("123456789")] ;
                    if ( right.find_first_of("123456789") + 1 == right.length() )
                    {
                        right = "0" ;
                    } else
                    {
                        right = right.substr(right.find_first_of("123456789") + 1, right.length()) ;
                    }
                }  
            } else                  //  if the integer part is not zero
            {
                if ( left.length() != 1 )
                {
                    length += left.length() - 1;
                    right = left.substr(1, left.length()) + right ;
                    left = left[0] ;
                }
            }  
        } else                  //  if there is no fraction
        {
            if ( atoi(left.c_str()) == 0 ) return "0.0e+0" ; else
            if ( left.length() == 1 ) right = "0" ; else
            {
                length += left.length() - 1;
                right = left.substr(1, left.length()) ;
                left = left[0] ;
            }
        }

        //  dealing with the tailing zeroes
        int i = right.length() - 1 ;
        int zeroes = 0 ;
        while ( i > 0 )                 //  keep the first 0
        {
            if ( right[i] == '0' ) zeroes += 1 ; else break ;
        }
        right = right.substr(0, right.length() - zeroes) ;
        
        //  concantenate the string
        if ( length >= 0) spelling = left + "." + right + "e+" + std::to_string(length) ; else
        spelling = left + "." + right + "e" + std::to_string(length) ;

        return spelling ;
    }

    Token next_token()
    {
        switch(ch)      // ch is always the next char to read
        {
        case ' ':               // white space tokens
            parse_wspace(tk_space) ;
            break ;
        case '\n':
            parse_wspace(tk_newline) ;
            break ;

        case 'a'...'z':         // identifier tokens
        case 'A'...'Z':
        case '$':
            parse_identifier() ;
            break ;

        case '0'...'9':         // number tokens
            parse_number() ;
            break ;

        case '@':               // the start of a symbol token
        case '.':
        case '{':
        case '}':
        case '(':
        case ')':
        case '[':
        case ']':
        case '<':
        case '>':
        case '-':
        case '+':
        case '*':
        case '!':
        case '=':
            parse_symbol() ;
            break ;

        case '/':               //  comment or symbol tokens
            parse_after_slash() ;
            break ;

        case '"':               // string tokens
            parse_string() ;
            break ;

        case EOF:
            new_token_kind = tk_eoi ;
            break ;

        default:
            new_token_kind = tk_oops ;
            c_did_not_find(cg_start_of_token) ;
        }

        Token token = new_token(new_token_kind) ;

        // before returning a token check if the kind or spelling needs updating
        // update the kind of identifier to see if it is actually a keyword
        if ( new_token_kind == tk_identifier )                
        {
            string spelling = token_spelling(token) ;
            TokenKind if_keyword = keyword_or_identifier(spelling) ;
            if ( if_keyword != tk_identifier ) set_token_kind(token, if_keyword ) ;
        }

        //  update the spelling of scientific
        if ( new_token_kind == tk_scientific )
        {
            string spelling = token_spelling(token) ;
            spelling = modify_scientific(spelling) ;
            set_token_spelling(token, spelling) ;
        }

        //  update the spelling of strings
        if ( new_token_kind == tk_string )
        {
            string spelling = token_spelling(token) ;
            int length = spelling.length() ;
            spelling = spelling.substr(1, length - 1) ;
            set_token_spelling(token, spelling) ;
        }
        
        //update the spelling of comments
        if ( new_token_kind == tk_eol_comment )
        {
            string spelling = token_spelling(token) ;
            int length = spelling.length() ;
            spelling = spelling.substr(2, length - 3) ;
            set_token_spelling(token, spelling) ;
        }
        if ( new_token_kind == tk_adhoc_comment )
        {
            string spelling = token_spelling(token) ;
            int length = spelling.length() ;
            spelling = spelling.substr(2, length - 4) ;
            set_token_spelling(token, spelling) ;
        }

        return token ;
    }
}
