#ifndef LIB_SYMBOLS_H
#define LIB_SYMBOLS_H

#include <string>

namespace CS_Symbol_Tables
{
    // to shorten the code
    typedef std::string string ;

    // *********** Lookup Tables ***********
    //
    // A symbol table is a way of recording a mapping from a string key to a value
    // Three kinds of table are provided to allow mappings to strings, integers or variable descriptions
    //
    // Each table has the following functions:
    // create_* this creates a new empty table
    // delete_* this destroys an existing table
    // insert_* this inserts a new key to value mapping into the table or returns false if the key is already in the table
    // update_* this inserts a new key to value mapping into the table or updates and existing mapping if the key is already in the table
    // lookup_* this returns the value that the given key maps to or returns a specified value if the key is not in the table
    //
    // If an error occurs fatal_error will be called with an exist status of -1 and the associated error message cannot be suppressed
    // Errors include
    // - insufficient memory to create a new table
    // - passing the wrong kind of table to a delete_*, insert_*, update_* or lookup_* function
    // - passing an invalid table to a delete_*, insert_*, update_* or lookup_* function
    //

    // type definition for a symbol table (implementation is hidden)
    typedef int symbols ;

    // ***************  Symbol table mapping strings to int values  *********************

    // a symbol table from string to int, failed lookups return -1
    extern symbols create_ints() ;
    extern void    delete_ints(symbols table) ;
    extern bool    insert_ints(symbols table,string symbol,int value) ;
    extern void    update_ints(symbols table,string symbol,int value) ;
    extern int     lookup_ints(symbols table,string symbol) ;

    // ***************  Symbol table mapping strings to string values  *********************

    // a symbol table from string to string, failed lookups return ""
    extern symbols create_strings() ;
    extern void    delete_strings(symbols table) ;
    extern bool    insert_strings(symbols table,string symbol,string value) ;
    extern void    update_strings(symbols table,string symbol,string value) ;
    extern string  lookup_strings(symbols table,string symbol) ;

    // ***************  Symbol table mapping strings to variable values  *********************

    // a symbol table from string to variable, failed lookups return a variable with a name, segment and type of "" and offset of -1
    class st_variable ;
    extern symbols     create_variables() ;
    extern void        delete_variables(symbols table) ;
    extern bool        insert_variables(symbols table,string symbol,st_variable value) ;
    extern void        update_variables(symbols table,string symbol,st_variable value) ;
    extern st_variable lookup_variables(symbols table,string symbol) ;

    // class st_variable - this records the information required by a Jack Compiler
    class st_variable
    {
    public:
        // to create an empty variable:
        st_variable()
        : name(""), type(""), segment(""), offset(-1)
        {
        }

        // to create an initialised variable:
        st_variable(string name,string type,string segment,int offset)
        : name(name), type(type), segment(segment), offset(offset)
        {
        }

        string name ;
        string type ;
        string segment ;
        int offset ;
    };


    // ***************   EXAMPLES OF HOW TO USE THE SYMBOL TABLES   *********************

    // ***************  Symbol table mapping strings to int values  *********************

    // Example of how to use an int symbol table
    //
    // Create a new empty table
    //     symbols mytable = create_ints() ;
    //
    // Now add a mapping from "bob" to 73, this will not report an error because "bob" is not in mytable
    //     if ( !insert_ints(mytable,"bob",73) ) /* report an error somehow? */
    //
    // Now lookup "bob"'s value, this will return 73
    //     x = lookup_ints(mytable,"bob") ;
    //
    // Now change "bob"'s 73 to 17 
    //     update_ints(mytable,"bob",17) ;
    //
    // Now lookup "bob"'s value, this will return 17
    //     x = lookup_ints(mytable,"bob") ;
    //
    // Now lookup "bill"'s value, this will return -1 because "bill" is not in mytable
    //     x = lookup_ints(mytable,"bill") ;
    //
    // Now delete the table
    //     delete_ints(mytable) ;
    //

    // ***************  Symbol table mapping strings to string values  *********************

    // Example of how to use a string symbol table
    //
    // Create a new empty table
    //     symbols mytable = create_strings() ;
    //
    // Now add a mapping from "bob" to "hello mum", this will not report an error because "bob" is not in mytable
    //     if ( !insert_strings(mytable,"bob","hello mum") ) /* report an error somehow? */
    //
    // Now change "bob"'s "hello mum" to "bye bye mum"
    //     update_strings(mytable,"bob","bye bye mum") ;
    //
    // Now lookup "bob"'s value, this will return "bye bye mum"
    //     y = lookup_strings(mytable,"bob") ;
    //
    // Now lookup "bill"'s value, this will return "" because "bill" is not in mytable
    //     y = lookup_strings(mytable,"bill") ;
    //
    // Now delete the table
    //     delete_strings(mytable) ;
    //

    // ***********  Symbol table mapping strings to variable objects  *************

    // Example of how to use a variable symbol table
    //
    // Create a variable object myvar for the variable named "bob" of type "int" in segment "local" at offset 2
    //     st_variable myvar("bob","int","local",2) ;
    //
    // Create a new table
    //     symbols mytable = create_variables() ;
    //
    // Now add a mapping from "bob" to our variable object myvar, this will not report an error because "bob" is not in mytable
    //     if ( !insert_variables(mytable,"bob",myvar) ) /* report an error somehow? */
    //
    // Now change "bob"'s offset to 5
    //     myvar.offset = 5 ;
    //     update_strings(mytable,"bob",myvar) ;
    //
    // Now lookup "bob"'s offset, this will return 5
    //     st_variable temp = lookup_variables(mytable,"bob") ;
    //     z = temp.offset ;
    //
    // Now lookup "bill"'s offset, this will result in -1 because "bill" is not in mytable
    //     st_variable temp = lookup_variables(mytable,"bill") ;
    //     z = temp.offset ;
    //
    // Now delete the table
    //     delete_variables(mytable) ;
    //

}
#endif //LIB_SYMBOLS_H

