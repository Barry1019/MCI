// Author name: Shaokang Ma
// Author id: a1790846

#include <string>
#include <vector>
#include "iobuffer.h"
#include "symbols.h"
#include "abstract-syntax-tree.h"

// to shorten our code:
using namespace std ;
using namespace CS_IO_Buffers ;
using namespace CS_Symbol_Tables ;
using namespace Jack_Compiler ;

// ***** WHAT TO DO *****
//
// MODIFY the skeleton code below to parse a Jack class and construct the equivalent abstract syntax tree, ast.
//        The main function will print the ast formatted as XML
//
// NOTE: the correct segment and offset is recorded with every variable in the ast
//       so the code generator does not need to use any symbol tables
//
// NOTE: use the fatal_token_context() function to print fatal error messages with the current
//       input file position marked.
//
// The skeleton code has one function per non-terminal in the Jack grammar
//  - comments describing how to create relevant ast nodes prefix most functions
//  - ast nodes are immutable - you must create all the sub-trees before creating a new node
//
// The structure of the skeleton code is just a suggestion
//  - it matches the grammar but does not quite match the ast structure
//  - you will need to change the parameters / results / functions to suit your own logic
//  - you can change it as much as you like
//
// NOTE: the best place to start is to write code to parse the input without attempting to create
//       any ast nodes. If correct this parser will report errors when it finds syntax errors but
//       correct programs will fail because the tree printer will not be provided with a valid ast.
//       Once the parsing works modify your code to create the ast nodes.
//
// NOTE: the return create_empty() ; statements are only present so that this skeleton code will compile.
//       these statements need to be replaced by code that creates the correct ast nodes.
//

// The Jack grammar to be recognised:
// program          ::= One or more classes, each class in a separate file named <class_name>'.Jack'
// class            ::= 'class' identifier '{' class_var_decs subr_decs '}'
// class_var_decs   ::= (static_var_dec | field_var_dec)*
// static_var_dec   ::= 'static' type identifier (',' identifier)* ';'
// field_var_dec    ::= 'field' type identifier (',' identifier)* ';'
// type             ::= 'int' | 'char' | 'boolean' | identifier
// vtype            ::= 'void' | type
// subr_decs        ::= (constructor | function | method)*
// constructor      ::= 'constructor' identifier identifier '(' param_list ')' subr_body
// function         ::= 'function' vtype identifier '(' param_list ')' subr_body
// method           ::= 'method' vtype identifier '(' param_list ')' subr_body
// param_list       ::= ((type identifier) (',' type identifier)*)?
// subr_body        ::= '{' var_decs statements '}'
// var_decs         ::= var_dec*
// var_dec          ::= 'var' type identifier (',' identifier)* ';'

// statements       ::= statement*
// statement        ::= let | if | while | do | return
// let              ::= 'let' identifier index? '=' expr ';'
// if               ::= 'if' '(' expr ')' '{' statements '}' ('else' '{' statements '}')?
// while            ::= 'while' '(' expr ')' '{' statements '}'
// do               ::= 'do' identifier (id_call | call) ';'
// return           ::= 'return' expr? ';'

// expr             ::= term (infix_op term)*
// term             ::= integer_constant | string_constant | 'true' | 'false' | 'null' | 'this' | '(' expr ')' | unary_op term | var_term
// var_term         ::= identifier (index | id_call | call)?
// id_call          ::= '.' identifier call
// call             ::= '(' expr_list ')'
// expr_list        ::= (expr (',' expr)*)?
// infix_op         ::= '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
// unary_op         ::= '-' | '~'


// forward declarations of one function per non-terminal rule in the Jack grammar - except for program
// NOTE: parse_type(), parse_vtype(), parse_unary_op() all return a Token not an ast
//
ast parse_class() ;
ast parse_class_var_decs() ;
ast parse_static_var_dec() ;
ast parse_field_var_dec() ;
Token parse_type() ;
Token parse_vtype() ;
ast parse_subr_decs() ;
ast parse_constructor() ;
ast parse_function() ;
ast parse_method() ;
ast parse_param_list() ;
ast parse_subr_body() ;
ast parse_var_decs() ;
ast parse_var_dec() ;

ast parse_statements() ;
ast parse_statement() ;
ast parse_let() ;
ast parse_if() ;
ast parse_while() ;
ast parse_do() ;
ast parse_return() ;

ast parse_expr() ;
ast parse_term() ;
ast parse_var_term() ;
ast parse_index() ;
ast parse_id_call() ;
ast parse_call() ;
ast parse_expr_list() ;
ast parse_infix_op() ;
Token parse_unary_op() ;

// a global variable to store the local offset
static int local_offset = -1 ;

// a global variable to store the static offset
static int static_offset = -1 ;

// a global variable to store the argument offset
static int argument_offset = -1 ;

// a global variable to store the this offset
static int this_offset = -1 ;

// a static symbol table
static symbols static_table ;

// a field symbol table
static symbols field_table ;

// a local symbol table
static symbols local_table ;

// an argument symbol table
static symbols argument_table ;

// to store the class name
static string class_name ;

// class ::= 'class' identifier '{' class_var_decs subr_decs '}'
// create_class(myclassname,class_var_decs,class_subrs)
ast parse_class()
{
    push_error_context("parse_class()") ;

    // reset segments
    static_offset = -1 ;
    local_offset = -1 ;
    argument_offset = -1 ;
    this_offset = -1 ;

    // reset symbol tables for static and field variables
    static_table = create_variables() ;
    field_table = create_variables() ;

    // a class variable and a subroutine variable declaration nodes
    ast class_var_decs = nullptr ;
    ast subr_decs = nullptr ;

    // parse a class
    mustbe(tk_class) ;
    class_name = token_spelling(mustbe(tk_identifier)) ;
    mustbe(tk_lcb) ;
    class_var_decs = parse_class_var_decs() ;
    subr_decs = parse_subr_decs() ;
    mustbe(tk_rcb) ;

    // return a class node
    ast ret = create_class(class_name, class_var_decs, subr_decs) ;

    // reset the tables
    delete_variables(static_table) ;
    delete_variables(field_table) ;

    pop_error_context() ;
    return ret ;
}

// class_var_decs ::= (static_var_dec | field_var_dec)*
// returns ast_class_var_decs: create_class_var_decs(vector<ast> decs)
// create_class_var_decs must be passed a vector of ast_var_decs
//
ast parse_class_var_decs()
{
    push_error_context("parse_class_var_decs()") ;
    
    // a vector of class variable declarations
    vector<ast> class_var_decs ;

    // parse class variable declarations
    while ( have(tg_starts_class_var) )
    {
        if ( have(tk_static) ) class_var_decs.push_back(parse_static_var_dec()) ; else
        class_var_decs.push_back(parse_field_var_dec()) ;
    }

    // return a class variable declarations node
    ast ret = create_class_var_decs(class_var_decs) ;
    pop_error_context() ;
    return ret ;
}

// static_var_dec ::= 'static' type identifier (',' identifier)* ';'
// returns ast_var_decs: create_var_decs(vector<ast> decs)
// create_var_decs must be passed a vector of ast_var_dec
//
// ast_var_dec: create_var_dec(string name,string segment,int offset,string type)
// create_var_dec must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
ast parse_static_var_dec()
{
    push_error_context("parse_class()") ;

    // update the offset
    static_offset ++ ;

    // to store type and id
    string tp, id ;

    // segment is static
    string segment = "static" ;

    // a vector of static variable declarations
    vector<ast> static_var_decs ;

    // a static variable declaration node
    ast static_var_dec = nullptr ;

    // parse static variable declareations
    mustbe(tk_static) ;
    tp = token_spelling(parse_type()) ;
    id = token_spelling(mustbe(tk_identifier)) ;
    static_var_dec = create_var_dec(id, segment, static_offset, tp) ;
    static_var_decs.push_back(static_var_dec) ;

    // insert the variable to the table
    st_variable myvar(id, tp, segment, static_offset) ;
    if ( !insert_variables(static_table, id, myvar) )
    {
        fatal_token_context("Duplicate variable names") ;
    }

    while ( have_next(tk_comma) )
    {
        // update the offset
        static_offset ++ ;

        // store the id
        id = token_spelling(mustbe(tk_identifier)) ;

        // add new nodes
        static_var_dec = create_var_dec(id, segment, static_offset, tp) ;
        static_var_decs.push_back(static_var_dec) ;

        // insert the variable to the table
        st_variable myvar(id, tp, segment, static_offset) ;
        if ( !insert_variables(static_table, id, myvar) )
        {
            fatal_token_context("Duplicate variable names") ;
        }
    }

    // check the semi
    mustbe(tk_semi) ;

    // return a static variable declarations node
    ast ret = create_var_decs(static_var_decs) ;
    pop_error_context() ;
    return ret ;
}

// field_var_dec ::= 'field' type identifier (',' identifier)* ';'
// returns ast_var_decs: create_var_decs(vector<ast> decs)
// create_var_decs must be passed a vector of ast_var_dec
//
// ast_var_dec: create_var_dec(string name,string segment,int offset,string type)
// create_var_dec must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
ast parse_field_var_dec()
{
    push_error_context("parse_class()") ;

    // update the offset
    this_offset ++ ;

    // to store type and id
    string tp, id ;

    // segment is this
    string segment = "this" ;

    // a vector of field variable declarations
    vector<ast> field_var_decs ;

    // a field variable declaration node
    ast field_var_dec = nullptr ;

    // parse field variable declareations
    mustbe(tk_field) ;
    tp = token_spelling(parse_type()) ;
    id = token_spelling(mustbe(tk_identifier)) ;
    field_var_dec = create_var_dec(id, segment, this_offset, tp) ;
    field_var_decs.push_back(field_var_dec) ;

    // insert the variable to the table
    st_variable myvar(id, tp, segment, this_offset) ;
    if ( !insert_variables(field_table, id, myvar) )
    {
        fatal_token_context("Duplicate variable names") ;
    }

    while ( have_next(tk_comma) )
    {
        this_offset ++ ;

        // store the id
        id = token_spelling(mustbe(tk_identifier)) ;

        // add new nodes
        field_var_dec = create_var_dec(id, segment, this_offset, tp) ;
        field_var_decs.push_back(field_var_dec) ;

        // insert the variable to the table
        st_variable myvar(id, tp, segment, this_offset) ;
        if ( !insert_variables(field_table, id, myvar) )
        {
            fatal_token_context("Duplicate variable names") ;
        }
    }

    // check the semi
    mustbe(tk_semi) ;

    // return a field variable declarations node
    ast ret = create_var_decs(field_var_decs) ;
    pop_error_context() ;
    return ret ;
}

// type ::= 'int' | 'char' | 'boolean' | identifier
// returns the Token for the type
Token parse_type()
{
    push_error_context("parse_type()") ;

    // a type token
    Token type = mustbe(tg_starts_type) ;

    pop_error_context() ;
    return type ;
}

// vtype ::= 'void' | type
// returns the Token for the type
Token parse_vtype()
{
    push_error_context("parse_vtype()") ;

    // a vtype token
    Token vtype = mustbe(tg_starts_vtype) ;

    pop_error_context() ;
    return vtype ;
}

// subr_decs ::= (constructor | function | method)*
// returns ast_subr_decs: create_subr_decs(vector<ast> subrs)
// create_subr_decs must be passed an vector of ast_subr
//
// ast_subr: create_subr(ast subr)
// create_subr must be passed one of: ast_constructor, ast_function or ast_method
//
ast parse_subr_decs()
{
    push_error_context("parse_subr_decs()") ;

    // a vector of subroutine declarations nodes
    vector<ast> subrs ;

    // a subroutine node
    ast subr = nullptr ;

    // one of: ast_constructor, ast_function or ast_method
    ast ast_subr = nullptr ;

    while ( have(tg_starts_subroutine) )
    {
        switch (token_kind())
        {
        case tk_constructor:
            subr = parse_constructor() ;
            break ;
        
        case tk_function:
            subr = parse_function() ;
            break ;
        
        case tk_method:
            subr = parse_method() ;
            break ;

        default:
            did_not_find(tg_starts_subroutine) ;
            break ;
        }

        ast_subr = create_subr(subr) ;
        subrs.push_back(ast_subr) ;
    }

    // return a subroutine node
    ast ret = create_subr_decs(subrs) ;
    pop_error_context() ;
    return ret ;
}

// constructor ::= 'constructor' identifier identifier '(' param_list ')' subr_body
// returns ast_constructor: create_constructor(string vtype,string name,ast params,ast body)
// . vtype: the constructor's return type, this must be it's class name
// . name: the constructor's name within its class
// . params: ast_param_list - the constructor's parameters
// . body: ast_subr_body - the constructor's body
//
ast parse_constructor()
{
    push_error_context("parse_constructor()") ;

    // to store the type and id
    string tp, id ;

    // reset the tables
    local_table = create_variables() ;
    argument_table = create_variables() ;

    // reset the offsets
    local_offset = -1 ;
    argument_offset = -1 ;

    // a parameter list node and a sub body node
    ast params = nullptr ;
    ast body = nullptr ;

    // parse constructor
    mustbe(tk_constructor) ;
    tp = token_spelling(parse_vtype()) ;
    id = token_spelling(mustbe(tk_identifier)) ;
    if ( tp != class_name )
    {
        fatal_token_context("Type should be the same as the class") ;
    }
    mustbe(tk_lrb) ;
    params = parse_param_list() ;
    mustbe(tk_rrb) ;
    body = parse_subr_body() ;

    // return a constructor node
    ast ret = create_constructor(tp, id, params, body) ;

    // reset the tables
    delete_variables(local_table) ;
    delete_variables(argument_table) ;

    pop_error_context() ;
    return ret ;
}

// function ::= 'function' vtype identifier '(' param_list ')' subr_body
// returns ast_function: create_function(string vtype,string name,ast params,ast body)
// . vtype: the function's return type
// . name: the function's name within its class
// . params: ast_param_list - the function's parameters
// . body: ast_subr_body - the function's body
//
ast parse_function()
{
    push_error_context("parse_function()") ;

    // reset the segments
    local_offset = -1 ;
    argument_offset = -1 ;

    // reset the local and argument tables
    local_table = create_variables() ;
    argument_table = create_variables() ;

    // to store the vtype and id
    string tp, id ;

    // a parameter node
    ast params = nullptr ;

    // a body node
    ast body = nullptr ;

    // parse a function
    mustbe(tk_function) ;
    tp = token_spelling(parse_vtype()) ;
    id = token_spelling(mustbe(tk_identifier)) ;
    mustbe(tk_lrb) ;
    params = parse_param_list() ;
    mustbe(tk_rrb) ;
    body = parse_subr_body() ;

    // return a function node    
    ast ret = create_function(tp, id, params, body) ;

    // reset the tables
    delete_variables(local_table) ;
    delete_variables(argument_table) ;

    pop_error_context() ;
    return ret ;
}

// method ::= 'method' vtype identifier '(' param_list ')' subr_body
// returns ast_method: create_method(string vtype,string name,ast params,ast body)
// . vtype: the method's return type
// . name: the method;s name within its class
// . params: ast_param_list - the method's explicit parameters
// . body: ast_subr_body - the method's body
//
ast parse_method()
{
    push_error_context("parse_method()") ;

    // to store the id the type
    string id, tp ;

    // a parameter list node and a sub body node
    ast params = nullptr ;
    ast body = nullptr ;

    // reset segments
    argument_offset = 0 ;
    local_offset = -1 ;

    // reset local and argument tables
    local_table = create_variables() ;
    argument_table = create_variables() ;

    // parse method
    mustbe(tk_method) ;
    tp = token_spelling(parse_vtype()) ;
    id = token_spelling(mustbe(tk_identifier)) ;
    mustbe(tk_lrb) ;
    params = parse_param_list() ;
    mustbe(tk_rrb) ;
    body = parse_subr_body() ;

    // return a method node
    ast ret = create_method(tp, id, params, body) ;

    // reset the tables
    delete_variables(local_table) ;
    delete_variables(argument_table) ;
    
    pop_error_context() ;
    return ret ;
}

// param_list ::= ((type identifier) (',' type identifier)*)?
// returns ast_param_list: create_param_list(vector<ast> params)
// create_param_list must be passed a vector of ast_var_dec
//
// ast_var_dec: create_var_dec(string name,string segment,int offset,string type)
// create_var_dec must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
ast parse_param_list()
{
    push_error_context("parse_param_list()") ;

    // update the offset
    argument_offset ++ ;

    // a vector of parameter nodes
    vector<ast> params ;

    // to store type and id
    string tp, id ;

    // segment is argument
    string segment = "argument" ;

    // a parameter node
    ast param = nullptr ;

    // parse a parameter
    if ( have(tg_starts_type) )
    {
        // store the type
        tp = token_spelling(parse_type()) ;

        // store the id
        id = token_spelling(mustbe(tk_identifier)) ;

        // create the first parameter node
        param = create_var_dec(id, segment, argument_offset, tp) ;
        params.push_back(param) ;

        // insert the variable to the table
        st_variable myvar(id, tp, segment, argument_offset) ;
        if ( !insert_variables(argument_table, id, myvar) )
        {
            fatal_token_context("Duplicate variable names") ;
        }

        // keep parsing
        while ( have_next(tk_comma) )
        {
            // update the offset
            argument_offset ++ ;

            // store the type
            tp = token_spelling(parse_type()) ;
            
            // store the id
            id = token_spelling(mustbe(tk_identifier)) ;

            // create parameter nodes
            param = create_var_dec(id, segment, argument_offset, tp) ;
            params.push_back(param) ;

            // insert the variable to the table
            st_variable myvar(id, tp, segment, argument_offset) ;
            if ( !insert_variables(argument_table, id, myvar) )
            {
                fatal_token_context("Duplicate variable names") ;
            }
        }
    }

    // return a parameter list node
    ast ret = create_param_list(params) ;
    pop_error_context() ;
    return ret ;
}

// subr_body ::= '{' var_decs statements '}'
// returns ast_subr_body: create_subr_body(ast decs,ast body)
// create_subr_body must be passed:
// . decs: ast_var_decs - the subroutine's local variable declarations
// . body: ast_statements - the statements within the body of the subroutinue
//
ast parse_subr_body()
{
    push_error_context("parse_subr_body()") ;

    // a local variable node
    ast var_decs = nullptr ;

    // a statements node
    ast statements = nullptr ;

    // parse a subroutine body
    mustbe(tk_lcb) ;
    var_decs = parse_var_decs() ;
    statements = parse_statements() ;
    mustbe(tk_rcb) ;   

    // return a subroutine body node
    ast ret = create_subr_body(var_decs, statements) ;
    pop_error_context() ;
    return ret ;
}

// var_decs ::= var_dec*
// returns ast_var_decs: create_var_decs(vector<ast> decs)
// create_var_decs must be passed a vector of ast_var_decs
//
ast parse_var_decs()
{
    push_error_context("parse_var_decs()") ;

    // a vector of variable declarations node
    vector<ast> decs ;

    // a variable declarations node
    ast var_dec = nullptr ;

    // parse variable declarations
    while ( have(tk_var) )
    {
        var_dec = parse_var_dec() ;
        decs.push_back(var_dec) ;
    }

    // return a variable declarations node
    ast ret = create_var_decs(decs) ;
    pop_error_context() ;
    return ret ;
}

// var_dec ::= 'var' type identifier (',' identifier)* ';'
// returns ast_var_decs: create_var_decs(vector<ast> decs)
// create_var_decs must be passed a vector of ast_var_dec
//
// ast_var_dec: create_var_dec(string name,string segment,int offset,string type)
// create_var_dec must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
ast parse_var_dec()
{
    push_error_context("parse_var_dec()") ;

    // update the offset
    local_offset ++ ;

    // to store the type and id
    string tp, id ;

    // the segment is local
    string segment = "local" ;

    // a local variable node
    ast var_dec = nullptr ;

    // a vector of variable declarations
    vector<ast> decs ;

    // parse a local variable
    mustbe(tk_var) ;

    // store the type
    tp = token_spelling(parse_type()) ;

    // store the id
    id = token_spelling(mustbe(tk_identifier)) ;

    var_dec = create_var_dec(id, segment, local_offset, tp) ;
    decs.push_back(var_dec) ;

    // insert the variable to the table
    st_variable myvar(id, tp, segment, local_offset) ;
    if ( !insert_variables(local_table, id, myvar) )
    {
        fatal_token_context("Duplicate variable names") ;
    }

    while ( have_next(tk_comma) )
    {
        // update the offset
        local_offset ++ ;

        // store the id
        id = token_spelling(mustbe(tk_identifier)) ;

        var_dec = create_var_dec(id, segment, local_offset, tp) ;
        decs.push_back(var_dec) ;

        // insert the variable to the table
        st_variable myvar(id, tp, segment, local_offset) ;
        if ( !insert_variables(local_table, id, myvar) )
        {
            fatal_token_context("Duplicate variable names") ;
        }
    }

    // check the semi
    mustbe(tk_semi) ;

    // return a local variable declarations node
    ast ret = create_var_decs(decs) ;
    pop_error_context() ;
    return ret ;
}

// statements ::= statement*
// create_statements(vector<ast> statements)
// create_statements must be passed a vector of ast_statement
//
ast parse_statements()
{
    push_error_context("parse_statements()") ;

    // a vector of statements node
    vector<ast> statements ;

    // a statements node
    ast statement = nullptr ;

    // parse statements
    while ( have(tg_starts_statement) )
    {
        statement = parse_statement() ;
        statements.push_back(statement) ;
    }

    // return a statements node
    ast ret = create_statements(statements) ;
    pop_error_context() ;
    return ret ;
}

// statement ::= let | if | while | do | return
// create_statement(ast statement)
// create_statement initialiser must be one of: ast_let, ast_let_array, ast_if, ast_if_else, ast_while, ast_do, ast_return or ast_return_expr
//
ast parse_statement()
{
    push_error_context("parse_statement()") ;

    // a statement node
    ast statement = nullptr ;

    // parse a statement
    switch (token_kind())
    {
    case tk_let:
        statement = parse_let() ;
        break ;

    case tk_if:
        statement = parse_if() ;
        break ;
    
    case tk_while:
        statement = parse_while() ;
        break ;
    
    case tk_do:
        statement = parse_do() ;
        break ;
    
    case tk_return:
        statement = parse_return() ;
        break ;
    
    default:
        did_not_find(tg_starts_statement) ;
        break ;
    }

    // return a statement node
    ast ret = create_statement(statement) ;
    pop_error_context() ;
    return ret ;
}

// let ::= 'let' identifier index? '=' expr ';'
// return one of: ast_let or ast_let_array
//
// create_let(ast var,ast expr)
// . var: ast_var - the variable being assigned to
// . expr: ast_expr - the variable's new value
//
// create_let_array(ast var,ast index,ast expr)
// . var: ast_var - the variable for the array being assigned to
// . index: ast_expr - the array index
// . expr: ast_expr - the array element's new value
//
ast parse_let()
{
    push_error_context("parse_let()") ;

    // parse let
    mustbe(tk_let) ;
    
    // get information
    string id = token_spelling(mustbe(tk_identifier)) ;
    st_variable temp = lookup_variables(static_table, id) ;
    if ( temp.offset == -1 ) temp = lookup_variables(field_table, id) ;
    if ( temp.offset == -1 ) temp = lookup_variables(local_table, id) ;
    if ( temp.offset == -1 ) temp = lookup_variables(argument_table, id) ;
    if ( temp.offset == -1 ) fatal_token_context("undeclared variable") ;

    // a variable node
    ast var = create_var(temp.name, temp.segment, temp.offset, temp.type) ;

    // if it is an array
    if ( have(tk_lsb) )
    {
        ast index = parse_index() ;
        mustbe(tk_eq) ;
        ast expr = parse_expr() ;
        mustbe(tk_semi) ;
        ast ret = create_let_array(var, index, expr) ;
        pop_error_context() ;
        return ret ;
    }

    // if it is not an array
    mustbe(tk_eq) ;
    ast expr = parse_expr() ;
    mustbe(tk_semi) ;
    ast ret = create_let(var, expr) ;
    pop_error_context() ;
    return ret ;
}

// if ::= 'if' '(' expr ')' '{' statements '}' ('else' '{' statements '}')?
// return one of: ast_if or ast_if_else
//
// create_if(ast condition,ast if_true)
// . condition: ast_expr - the if condition
// . if_true: ast_statements - the if true branch
//
// create_if_else(ast condition,ast if_true,ast if_false)
// . condition: ast_expr - the if condition
// . if_true: ast_statements - the if true branch
// . if_false: ast_statements - the if false branch
//
ast parse_if()
{
    push_error_context("parse_if()") ;

    // an expression node and statement nodes
    ast condition = nullptr ;
    ast if_true = nullptr ;
    ast if_false = nullptr ;

    // parse if
    mustbe(tk_if) ;
    mustbe(tk_lrb) ;
    condition = parse_expr() ;
    mustbe(tk_rrb) ;
    mustbe(tk_lcb) ;
    if_true = parse_statements() ;
    mustbe(tk_rcb) ;

    // if have else
    if ( have_next(tk_else) )
    {
        mustbe(tk_lcb) ;
        if_false = parse_statements() ;
        mustbe(tk_rcb) ;
        ast ret = create_if_else(condition, if_true, if_false) ;
        pop_error_context() ;
        return ret ;
    }

    // if only if
    ast ret = create_if(condition, if_true) ;
    pop_error_context() ;
    return ret ;
}

// while ::= 'while' '(' expr ')' '{' statements '}'
// returns ast_while: create_while(ast condition,ast body)
// . condition: ast_expr - the loop condition
// . body: ast_statements - the loop body
//
ast parse_while()
{
    push_error_context("parse_while()") ;

    // an expression node
    ast condition = nullptr ;

    // a body node
    ast body = nullptr ;

    // parse while
    mustbe(tk_while) ;
    mustbe(tk_lrb) ;
    condition = parse_expr() ;
    mustbe(tk_rrb) ;
    mustbe(tk_lcb) ;
    body = parse_statements() ;
    mustbe(tk_rcb) ;

    // return a while node
    ast ret = create_while(condition, body) ;
    pop_error_context() ;
    return ret ;
}

// do ::= 'do' identifier (call | id_call) ';'
// returns ast_do: create_do(ast call)
// create_do must be passed one of: ast_call_as_function or ast_call_as_method
//
// ast_call_as_function: create_call_as_function(string class_name,ast subr_call)
// create_ast_call_as_function must be passed:
// . class_name: name of the function's class
// . subr_call: ast_subr_call - the function's name within it's class and it's explicit arguments
//
// ast_call_as_method: create_call_as_method(string class_name,ast object,ast subr_call)
// create_ast_call_as_method must be passed:
// . class_name: name of the method's class
// . object: ast_expr - the object the method is applied to
// . subr_call: ast_subr_call - the method's name within it's class and it's explicit arguments
//
ast parse_do()
{
    push_error_context("parse_do()") ;

    // parse do
    mustbe(tk_do) ;

    // method or funtion
    bool is_function = false ;

    // get information
    string id = token_spelling(mustbe(tk_identifier)) ;
    st_variable temp = lookup_variables(static_table, id) ;
    if ( temp.offset == -1 ) temp = lookup_variables(field_table, id) ;
    if ( temp.offset == -1 ) temp = lookup_variables(local_table, id) ;
    if ( temp.offset == -1 ) temp = lookup_variables(argument_table, id) ;
    if ( temp.offset == -1 ) is_function = true ;

    // a variable node
    ast var = create_var(temp.name, temp.segment, temp.offset, temp.type) ;

    // a call node
    ast call = nullptr ;

    // if it is a direct function call
    if ( have(tk_lrb) )
    {
        ast object = create_this() ;
        ast ast_call = parse_call() ;
        ast subr_call = create_subr_call(id, ast_call) ;
        call = create_call_as_method(class_name, object, subr_call) ;
    }

    // if it is an indirect call - funtion or method
    if ( have(tk_stop) )
    {
        ast subr_call = parse_id_call() ;

        // if the type is not the current class name then it is a method
        if ( !is_function )
        {
            call = create_call_as_method(temp.type, var, subr_call) ;
        }
        else
        {
            call = create_call_as_function(id, subr_call) ;
        }   
    }

    // check the semi
    mustbe(tk_semi) ;

    // return a do node
    ast ret = create_do(call) ;
    pop_error_context() ;
    return ret ;
}

// return ::= 'return' expr? ';'
// returns one of: ast_return or ast_return_expr
//
// ast_return: create_return()
//
// ast_return_expr: create_return_expr(ast expr)
// create_return_expr must be passed an ast_expr
//
ast parse_return()
{
    push_error_context("parse_return()") ;

    // a expression node
    ast expr = nullptr ;

    // a return node
    ast ret = nullptr ;

    // parse return
    mustbe(tk_return) ;

    ret = create_return() ;

    // return ast_return_expr node
    if ( have(tg_starts_term) )
    {
        expr = parse_expr() ;
        ret = create_return_expr(expr) ;
    }   

    // check the semi
    mustbe(tk_semi) ;

    // return a return node
    pop_error_context() ;
    return ret ;
}

// expr ::= term (infix_op term)*
// returns ast_expr: create_expr(vector<ast> expr)
// the vector passed to create_expr:
// . must be an odd length of at least 1, ie 1, 3, 5, ...
// . must be  a vector alternating between ast_term and ast_infix_op nodes, ending with an ast_term
//
ast parse_expr()
{
    push_error_context("parse_expr()") ;
    
    // a term node
    ast term = nullptr ;

    // an infix_op node
    ast infix_op = nullptr ;

    // a vector of expression nodes
    vector<ast> expr ;

    // parse expressions
    term = parse_term() ;
    expr.push_back(term) ;

    while ( have(tg_infix_op) )
    {
        infix_op = parse_infix_op() ;
        expr.push_back(infix_op) ;
        term = parse_term() ;
        expr.push_back(term) ;
    }

    // return an expression node
    ast ret = create_expr(expr) ;
    pop_error_context() ;
    return ret ;
}

// term ::= integer_constant | string_constant | 'true' | 'false' | 'null' | 'this' | '(' expr ')' | unary_op term | var_term
// returns ast_term: create_term(ast term)
// create_term must be passed one of: ast_int, ast_string, ast_bool, ast_null, ast_this, ast_expr,
//                                    ast_unary_op, ast_var, ast_array_index, ast_call_as_function, ast_call_as_method
//
// ast_int: create_int(int _constant)
// create_int must be passed an integer value in the range 0 to 32767
//
// ast_string: create_string(string _constant)
// create_string must be passed any legal Jack string literal
//
// ast_bool: create_bool(bool t_or_f)
// create_bool must be passed true or false
//
// ast_null: create_null()
//
// ast_this: create_this()
//
// ast_unary_op: create_unary_op(string op,ast term)
// create_unary_op must be passed:
// . op: the unary op
// . term: ast_term
//
ast parse_term()
{
    push_error_context("parse_term()") ;

    // a term node
    ast term = nullptr ;

    // parse term
    if ( have(tg_unary_op) )
    {
        string op = token_spelling(parse_unary_op()) ;
        term = parse_term() ;
        ast ret = create_term(create_unary_op(op, term)) ;
        pop_error_context() ;
        return ret ;
    }

    switch (token_kind())
    {
    case tk_integerConstant:
        term = create_int(token_ivalue()) ;
        next_token() ;
        break ;
    
    case tk_stringConstant:
        term = create_string(token_spelling()) ;
        next_token() ;
        break ;

    case tk_true:
        term = create_bool(true) ;
        next_token() ;
        break ;

    case tk_false:
        term = create_bool(false) ;
        next_token() ;
        break ;

    case tk_null:
        term = create_null() ;
        next_token() ;
        break ;
    
    case tk_this:
        term = create_this() ;
        next_token() ;
        break ;

    case tk_lrb:
        mustbe(tk_lrb) ;
        term = parse_expr() ;
        mustbe(tk_rrb) ;
        break ;

    case tk_identifier:
        term = parse_var_term() ;
        break ;

    default:
        did_not_find(tg_starts_term) ;
        break ;
    }

    // return a term node
    ast ret = create_term(term) ;
    pop_error_context() ;
    return ret ;
}

// var_term ::= identifier (index | id_call | call)?
// returns one of: ast_var, ast_array_index, ast_call_as_function or ast_call_as_method
//
// ast_var: create_var(string name,string segment,int offset,string type)
// create_ast_var must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
// ast_array_index: create_array_index(ast var,ast index)
// create_ast_array_index must be passed:
// . var: ast_var - the array variable
// . index: ast_expr - the index into the array
//
// ast_call_as_function: create_call_as_function(string class_name,ast subr_call)
// create_ast_call_as_function must be passed:
// . class_name: name of the function's class
// . subr_call: ast_subr_call - the function's name within it's class and it's explicit arguments
//
// ast_call_as_method: create_call_as_method(string class_name,ast object,ast subr_call)
// create_ast_call_as_method must be passed:
// . class_name: name of the method's class
// . object: ast_expr - the object the method is applied to
// . subr_call: ast_subr_call - the method's name within it's class and it's explicit arguments
//
ast parse_var_term()
{
    push_error_context("parse_var_term()") ;

    // method of function
    bool is_function = false ;

    // get information
    string id = token_spelling(mustbe(tk_identifier)) ;
    st_variable temp = lookup_variables(static_table, id) ;
    if ( temp.offset == -1 ) temp = lookup_variables(field_table, id) ;
    if ( temp.offset == -1 ) temp = lookup_variables(local_table, id) ;
    if ( temp.offset == -1 ) temp = lookup_variables(argument_table, id) ;
    if ( temp.offset == -1 ) is_function = true ;

    // a variable node
    ast var = create_var(temp.name, temp.segment, temp.offset, temp.type) ;

    // the return node
    // initialise it with a var node
    ast ret = var ;

    // if it is an array node
    if ( have(tk_lsb) )
    {
        ast ast_index = parse_index() ;
        ret = create_array_index(var, ast_index) ;
    }

    // if it is a direct function call
    if ( have(tk_lrb) )
    {
        ast object = create_this() ;
        ast ast_call = parse_call() ;
        ast subr_call = create_subr_call(id, ast_call) ;
        ret = create_call_as_method(class_name, object, subr_call) ;
    }

    // if it is an indirect call - funtion or method
    if ( have(tk_stop) )
    {
        ast subr_call = parse_id_call() ;

        // if the type is not the current class name then it is a method
        if ( !is_function )
        {
            ret = create_call_as_method(temp.type, var, subr_call) ;
        }
        else
        {
            ret = create_call_as_function(id, subr_call) ;
        }   
    }

    // return a node
    pop_error_context() ;
    return ret ;
}

// index ::= '[' expr ']'
// returns ast_expr
ast parse_index()
{
    push_error_context("parse_index()") ;

    // parse index
    mustbe(tk_lsb) ;
    ast expr = parse_expr() ;
    mustbe(tk_rsb) ;

    pop_error_context() ;
    return expr ;
}

// id_call ::= '.' identifier call
// returns ast_subr_call: create_subr_call(string subr_name,ast expr_list)
// create_subr_call must be passed:
// . subr_name: the constructor, function or method's name within its class
// . expr_list: ast_expr_list - the explicit arguments to the call
//
ast parse_id_call()
{
    push_error_context("parse_id_call()") ;
    
    // parse id call
    mustbe(tk_stop) ;
    string subr_name = token_spelling(mustbe(tk_identifier)) ;
    ast expr_list = parse_call() ;
    ast ret = create_subr_call(subr_name, expr_list) ;

    // return an id call node
    pop_error_context() ;
    return ret ;
}

// call ::= '(' expr_list ')'
// returns ast_expr_list
//
ast parse_call()
{
    push_error_context("parse_call()") ;

    // parse call
    mustbe(tk_lrb) ;
    ast expr_list = parse_expr_list() ;
    mustbe(tk_rrb) ;

    // return a call node
    pop_error_context() ;
    return expr_list ;
}

// expr_list ::= (expr (',' expr)*)?
// returns ast_expr_list: create_expr_list(vector<ast> exprs)
// create_expr_list must be passed: a vector of ast_expr
//
ast parse_expr_list()
{
    push_error_context("parse_expr_list()") ;

    // a vector of expr nodes
    vector<ast> exprs ;

    // an expr node
    ast expr = nullptr ;

    if ( have(tg_starts_term) )
    {
        expr = parse_expr() ;
        exprs.push_back(expr) ;

        while ( have_next(tk_comma) )
        {
            expr = parse_expr() ;
            exprs.push_back(expr) ;
        }
    }

    // return an expr list node
    ast ret = create_expr_list(exprs) ;
    pop_error_context() ;
    return ret ;
}

// infix_op ::= '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
// returns ast_op: create_infix_op(string infix_op)
// create_infix_op must be passed:
// infix_op: the infix op
//
ast parse_infix_op()
{
    push_error_context("parse_infix_op()") ;

    string infix_op = token_spelling(mustbe(tg_infix_op)) ;

    // an infix_op node
    ast ret = create_infix_op(infix_op) ;

    pop_error_context() ;
    return ret ;
}

// unary_op ::= '-' | '~'
// returns Token for the unary_op
//
Token parse_unary_op()
{
    push_error_context("parse_unary_op()") ;

    Token ret = mustbe(tg_unary_op) ;

    pop_error_context() ;
    return ret ;
}

ast jack_parser()
{
    // read the first token to get the tokeniser initialised
    next_token() ;

    // construct tree and return as result
    return parse_class() ;
}

// main program
int main(int argc,char **argv)
{
    // parse a Jack class and print the abstract syntax tree as XML
    ast_print_as_xml(jack_parser(),4) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}