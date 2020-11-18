//  name: Shaokang Ma
// id: a1790846

#include "tokeniser-extras.h"
#include <deque>

// to shorten the code
using namespace std ;

/////////////////////////////////////////////////////////////////////////

namespace Assignment_Tokeniser
{

    // the current input character
    // the eof marker can be confused with a legal character but not one accepted by our tokeniser
    char ch ;

    //  store the current spelling of a new token
    static string spelling ;

    //  to store the number of current line and column
    static int line = 1 ;
    static int column = 0 ;
    static int column_number = 1 ;
    static int line_number = 1 ;

    //  to deal with the special characters
    static int spaces = 0 ;
    static bool last_is_return = false ;

    //  to store the whole line
    static string current_line = "" ;
    static deque<int> lines ;
    static int count = 0 ;

    // create a new token using characters remembered since the last token was created
    // NOTE: the first character in the input is in column 1 of line 1
    Token new_token(TokenKind kind)
    {
        // create a new token object and reset spelling
        Token token = new_token(kind,spelling,line_number,column_number) ;
        //  indicate the next char is the beginning of a token
        line_number = line ;
        column_number = column ;

        // reset the spelling
        spelling = "" ;

        return token ;
    }

    // generate a context string for the given token
    // all lines are shown after the preprocssing of special characters
    // it shows the line before the token, with a '$' marking the end of line,
    // the line containing the token up to the end of the token, and
    // a line of spaces ending with a ^ under the token's first character to mark its position
    // the lines are prefixed by line numbers, right justified in four spaces, followed by ": ", eg "  45: "
    // NOTE: if a token includes newline character(s), only display the token before the first newline followed by a '$'
    string token_context(Token token)
    {
        string context = "" ;
        int curline = token_line(token) ;
        int curcolumn = token_column(token) ;
        int token_length = token_original(token).length() ;
        int prevline = curline - 1 ;

        for ( int i = 0; i < current_line.length(); i++)
        {
            if ( current_line[i] == '\n' ) current_line[i] = '$' ;
        }
        
        //  produce one previous line if it exists
        //  the way to substring depends on whether this is the first line
        if ( prevline > 0 )
        {
            int length = 4 - std::to_string(prevline).length() ;
            for ( int i = 0; i < length; i++ )
            {
                context += " " ;
            }
            context += std::to_string(prevline) ;
            context += ": " ;
            if ( prevline > 1 )
            {
                context += current_line.substr(lines[prevline - 2], lines[prevline - 1] - lines[prevline - 2]) ;
            } else
            {
                context += current_line.substr(0, lines[prevline - 1]) ;
            }
            context += "\n" ;
        }
        
        int length = 4 - std::to_string(curline).length() ;
        for ( int i = 0; i < length; i++ )
        {
            context += " " ;
        }
        context += std::to_string(curline) ;
        context += ": " ;

        string temp ;
        if ( curline == 1 )
        {
            temp = current_line.substr(0, curcolumn + token_length - 1) ;
        } else
        {
            temp = current_line.substr(lines[prevline - 1], curcolumn + token_length - 1) ;
        }
        
        //  see if there is a '\n' before the end of the token
        if ( temp.find_first_of("$") != string::npos ) temp = temp.substr(0, temp.find_first_of("$") + 1) ;
        context += temp ;
        context += "\n" ;
        
        for ( int i = 0; i < curcolumn + 5; i++ )
        {
            context += " " ;
        }
        context += "^" ;
        context += "\n" ;
        
        return context ;
    }

    // read next character if not at the end of input and update the line and column numbers
    // additional code will be required here to handle preprocessing of '\t' and '\r'
    // in some cases you may wish to remember a character to use next time instead of calling read_char()
    void nextch()
    {
        // stop reading once we have read EOF 
        if ( ch == EOF ) return ;           

        // remember the old ch, it is part of the current token being parsed
        spelling += ch ;                    

        //  check if this is a new line
        if ( ch == '\n' )
        {
            column = 0 ;
            line ++ ;
        }

        //  complete the replacement of tab
        if ( spaces > 0 )
        {
            ch = ' ' ;
            spaces -= 1 ;                       
        } else
        {
            ch = read_char() ;     
        }

        //  handle '\r' and '\n'
        if ( ch == '\n' && last_is_return ) ch = read_char() ;
        
        last_is_return = false ;

        if ( ch == '\r' )
        {
            ch = '\n' ;
            last_is_return = true ;
        }

        column += 1 ;
        
        //  handle '\t'
        //  update the number of spaces
        if ( ch == '\t' )
        {
            ch = ' ' ;
            spaces = 4 - column % 4 ;     
        }

        //  store the current line
        current_line += ch ;
        count ++ ;
        if ( ch == '\n' ) lines.push_back(count) ;
    }

    // initialise the tokeniser
    void initialise_tokeniser()
    {
        ch = '\n' ;                       
        nextch() ;                       
        spelling = "" ;                   
        line = 1 ;
        column = 1 ;
        line_number = 1 ;
        column_number = 1 ;
    }
}
