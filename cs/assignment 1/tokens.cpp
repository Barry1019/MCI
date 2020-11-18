// convert Text into Tokens 
#include "tokeniser.h"
#include "iobuffer.h"

using namespace std ;
using namespace CS_IO_Buffers ;
using namespace Assignment_Tokeniser ;

// this main program tokenises standard input and
// prints the tokens as they are found
int main(int argc,char **argv)
{
    Token token ;
    int count = 0 ;

    // configure io buffers to output immediately
    config_output(iob_immediate) ;
    config_errors(iob_immediate) ;

    // wrap this all in a try catch - just in case
    try
    {
        // remember and display each token as it is read
        initialise_tokeniser() ;
        token = next_token() ;
        while ( token_kind(token) != tk_eoi )
        {
            write_to_output(to_string(token)+"\n") ;
            count++ ;
            token = next_token() ;
        }
    }
    catch ( std::string e )
    {
        // silently ignore hoping it was thrown by c_did_not_find()
    }
    write_to_output("Read " + to_string(count) + " tokens\n") ;

    return 0 ;
}
