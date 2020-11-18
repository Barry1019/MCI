// Author name: Shaokang Ma
// Author id: a1790846

#include "iobuffer.h"
#include "symbols.h"
#include "abstract-syntax-tree.h"

// to shorten our code:
using namespace std ;
using namespace CS_IO_Buffers ;
using namespace Jack_Compiler ;

// ***** WHAT TO DO *****
//
// MODIFY the skeleton code below to walk an abstract syntax tree, ast, of a Jack class
//        and generate the equivalent Hack Virtual Machine Code.
//
// NOTE: the correct segment and offset is recorded with every variable in the ast
//       so the code generator does not need to use any symbol tables
//
// The skeleton code has one function per node in the abstract tree
//  - they each extract all fields into local variables
//  - they each call the appropriate walk_* function to walk any sub-trees
//
// The structure of the skeleton code is just a suggestion
//  - you may want to change the parameters / results to suit your own logic
//  - you can change it as much as you like

// forward declarations of one function per node in the abstract syntax tree
void walk_class(ast t) ;
void walk_class_var_decs(ast t) ;
void walk_var_dec(ast t) ;
void walk_subr_decs(ast t) ;
void walk_subr(ast t) ;
void walk_constructor(ast t) ;
void walk_function(ast t) ;
void walk_method(ast t) ;
void walk_param_list(ast t) ;
void walk_subr_body(ast t) ;
void walk_var_decs(ast t) ;
void walk_statements(ast t) ;
void walk_statement(ast t) ;
void walk_let(ast t) ;
void walk_let_array(ast t) ;
void walk_if(ast t) ;
void walk_if_else(ast t) ;
void walk_while(ast t) ;
void walk_do(ast t) ;
void walk_return(ast t) ;
void walk_return_expr(ast t) ;
void walk_expr(ast t) ;
void walk_term(ast t) ;
void walk_int(ast t) ;
void walk_string(ast t) ;
void walk_bool(ast t) ;
void walk_null(ast t) ;
void walk_this(ast t) ;
void walk_unary_op(ast t) ;
void walk_var(ast t) ;
void walk_array_index(ast t) ;
void walk_call_as_function(ast t) ;
void walk_call_as_method(ast t) ;
void walk_subr_call(ast t) ;
void walk_expr_list(ast t) ;
string walk_infix_op(ast t) ;

// to store the class name
static string myclassname ;

// keep global counters so we can create unique labels in while and if statements
static int while_counter = 0 ;
static int if_counter = 0 ;

// get the field variable number
static int field_var_num  = 0 ;

// keep the call name
static string call_name ;

// to tell if it is a function or a method
static bool is_method = false ;

// keep the unary_op
static string unary_op ;

// op - a string - one of "+", "-", "*", "/", "&", "|", "<", ">", "="
static string translate_op(string op)
{
    switch(op[0])
    {
    case '+': return "add" ;
    case '-': return "sub" ;
    case '*': return "call Math.multiply 2" ;
    case '/': return "call Math.divide 2" ;
    case '&': return "and" ;
    case '|': return "or" ;
    case '<': return "lt" ;
    case '>': return "gt" ;
    case '=': return "eq" ;
    default:;
    }

    fatal_error(-1,"translate_op passed unknown op: " + op + "\n") ;
    return op ;
}

// walk an ast class node with fields:
// class_name - a string
// var_decs   - ast vector of variable declarations
// subr_decs  - ast vector of subroutine declarations
//
void walk_class(ast t)
{
    myclassname = get_class_class_name(t) ;
    ast var_decs = get_class_var_decs(t) ;
    ast subr_decs = get_class_subr_decs(t) ;

    walk_class_var_decs(var_decs) ;
    walk_subr_decs(subr_decs) ;
}

// walk an ast class var decs node
// it is an ast vector of variable declarations
//
void walk_class_var_decs(ast t)
{
    int ndecs = size_of_class_var_decs(t) ;
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        walk_var_dec(get_class_var_decs(t,i)) ;
    }
}

// walk an ast variable declaration with fields
// name - a string
// type - a string - "int", "char", "boolean" or a class name
// segment - a string
// offset - an int
// this is used for statics, fields, parameters and local variables
//
void walk_var_dec(ast t)
{
    string name = get_var_dec_name(t) ;
    string type = get_var_dec_type(t) ;
    string segment = get_var_dec_segment(t) ;

    // increment the field variable number
    if ( segment == "this" ) field_var_num ++ ;
}

// walk an ast class var decs node
// it is an ast vector of subroutine declarations
//
void walk_subr_decs(ast t)
{
    int size = size_of_subr_decs(t) ;
    for ( int i = 0 ; i < size ; i++ )
    {
        walk_subr(get_subr_decs(t,i)) ;
    }
}

// walk an ast subroutine node with a single field
// subr - an ast constructor, ast function or ast method node
//
void walk_subr(ast t)
{
    ast subr = get_subr_subr(t) ;

    switch(ast_node_kind(subr))
    {
    case ast_constructor:
        walk_constructor(subr) ;
        break ;
    case ast_function:
        walk_function(subr) ;
        break ;
    case ast_method:
        walk_method(subr) ;
        break ;
    default:
        fatal_error(0,"Unexpected subroutine kind") ;
        break ;
    }
}

// walk an ast constructor node with fields
// vtype - a string - the name of the class
// name - a string
// param list - an ast vector of variable declarations
// subr body - an ast subr body node
//
void walk_constructor(ast t)
{
    //reset counters
    if_counter = 0;
    while_counter = 0;

    string vtype = get_constructor_vtype(t) ;
    string name = get_constructor_name(t) ;
    ast param_list = get_constructor_param_list(t) ;
    ast subr_body = get_constructor_subr_body(t) ;

    // to know the number of local variables
    ast decs = get_subr_body_decs(subr_body) ;
    int size = size_of_var_decs(decs) ;

    // walk a constructor
    write_to_output("function " + myclassname + "." + name + " " + to_string(size) + "\n") ;
    write_to_output("push constant " + to_string(field_var_num) + "\n") ;
    write_to_output("call Memory.alloc 1\n") ;
    write_to_output("pop pointer 0\n") ;

    walk_param_list(param_list) ;
    walk_subr_body(subr_body) ;
}

// walk an ast function node with fields
// vtype - a string - one of "void", "int", "char", "boolean" or the name of a class
// name - a string
// param list - an ast vector of variable declarations
// subr body - an ast subr body node
//
void walk_function(ast t)
{
    //reset counters
    if_counter = 0;
    while_counter = 0;

    string vtype = get_function_vtype(t) ;
    string name = get_function_name(t) ;
    ast param_list = get_function_param_list(t) ;
    ast subr_body = get_function_subr_body(t) ;

    // to know the number of local variables
    ast decs = get_subr_body_decs(subr_body) ;
    int size = size_of_var_decs(decs) ;

    // walk a function
    write_to_output("function " + myclassname + "." + name + " " + to_string(size) + "\n") ;

    walk_param_list(param_list) ;
    walk_subr_body(subr_body) ;
}

// walk an ast method node with fields
// vtype - a string - one of "void", "int", "char", "boolean" or the name of a class
// name - a string
// param list - an ast vector of variable declarations
// subr body - an ast subr body node
//
void walk_method(ast t)
{
    //reset counters
    if_counter = 0;
    while_counter = 0;

    string vtype = get_method_vtype(t) ;
    string name = get_method_name(t) ;
    ast param_list = get_method_param_list(t) ;
    ast subr_body = get_method_subr_body(t) ;

    // to know the number of local variables
    ast decs = get_subr_body_decs(subr_body) ;
    int size = size_of_var_decs(decs) ;

    // walk a method
    write_to_output("function " + myclassname + "." + name + " " + to_string(size) + "\n") ;
    write_to_output("push argument 0\n") ;
    write_to_output("pop pointer 0\n") ;

    walk_param_list(param_list) ;
    walk_subr_body(subr_body) ;
}

// walk an ast param list node
// it is an ast vector of variable declarations
//
void walk_param_list(ast t)
{
    int ndecs = size_of_param_list(t) ;
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        walk_var_dec(get_param_list(t,i)) ;
    }
}

// walk an ast subr body node with fields
// decs - an ast vector of variable declarations
// body - an ast vector of statement nodes
//
void walk_subr_body(ast t)
{
    ast decs = get_subr_body_decs(t) ;
    ast body = get_subr_body_body(t) ;

    walk_var_decs(decs) ;
    walk_statements(body) ;
}

// walk an ast param list node
// it is an ast vector of variable declarations
//
void walk_var_decs(ast t)
{
    int ndecs = size_of_var_decs(t) ;
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        walk_var_dec(get_var_decs(t,i)) ;
    }
}

// walk an ast statements node
// it is an ast vector of statement nodes
//
void walk_statements(ast t)
{
    int nstatements = size_of_statements(t) ;
    for ( int i = 0 ; i < nstatements ; i++ )
    {
        walk_statement(get_statements(t,i)) ;
    }
}

// walk an ast statement node with a single field
// statement - one of the following ast nodes, let, let array, if, if else, while, do, return, return expr or statements
//
void walk_statement(ast t)
{
    ast statement = get_statement_statement(t) ;

    switch(ast_node_kind(statement))
    {
    case ast_let:
        walk_let(statement) ;
        break ;
    case ast_let_array:
        walk_let_array(statement) ;
        break ;
    case ast_if:
        walk_if(statement) ;
        break ;
    case ast_if_else:
        walk_if_else(statement) ;
        break ;
    case ast_while:
        walk_while(statement) ;
        break ;
    case ast_do:
        walk_do(statement) ;
        break ;
    case ast_return:
        walk_return(statement) ;
        break ;
    case ast_return_expr:
        walk_return_expr(statement) ;
        break ;
    case ast_statements:
        walk_statements(statement) ;
        break ;
    default:
        fatal_error(0,"Unexpected statement kind") ;
        break ;
    }
}

// walk an ast let node with fields
// var  - an ast variable
// expr - an ast expr node
//
void walk_let(ast t)
{
    ast var = get_let_var(t) ;
    ast expr = get_let_expr(t) ;

    // walk a let
    walk_expr(expr) ;
    string segment = get_var_segment(var) ;
    int offset = get_var_offset(var) ;
    write_to_output("pop " + segment + " " + to_string(offset) + "\n") ;
}

// walk an ast let array node with fields
// var    - an ast variable
// index  - an ast expr node
// expr   - an ast expr node
//
void walk_let_array(ast t)
{
    ast var = get_let_array_var(t) ;
    ast index = get_let_array_index(t) ;
    ast expr = get_let_array_expr(t) ;

    walk_expr(index) ;
    walk_var(var) ;
    write_to_output("add\n") ;
    walk_expr(expr) ;
    write_to_output("pop temp 0\n") ;
    write_to_output("pop pointer 1\n") ;
    write_to_output("push temp 0\n") ;
    write_to_output("pop that 0\n") ;
}

// walk an ast if node with fields
// condition - an ast expr node
// if true   - an ast statements node
//
void walk_if(ast t)
{
    ast condition = get_if_condition(t) ;
    ast if_true = get_if_if_true(t) ;

    string label = to_string(if_counter++) ;

    // walk an if
    walk_expr(condition) ;
    write_to_output("if-goto IF_TRUE" + label + "\n") ;
    write_to_output("goto IF_FALSE" + label + "\n") ;
    write_to_output("label IF_TRUE" + label + "\n") ;
    walk_statements(if_true) ;
    write_to_output("label IF_FALSE" + label + "\n") ;
}

// walk an ast if else node with fields
// condition - an ast expr node
// if true   - an ast statements node
// if else   - an ast statements node
//
void walk_if_else(ast t)
{
    ast condition = get_if_else_condition(t) ;
    ast if_true = get_if_else_if_true(t) ;
    ast if_false = get_if_else_if_false(t) ;

    // walk an if-else
    string label = to_string(if_counter++) ;

    walk_expr(condition) ;
    write_to_output("if-goto IF_TRUE" + label + "\n") ;
    write_to_output("goto IF_FALSE" + label + "\n") ;
    write_to_output("label IF_TRUE" + label + "\n") ;
    walk_statements(if_true) ;
    write_to_output("goto IF_END" + label + "\n") ;
    write_to_output("label IF_FALSE" + label + "\n") ;
    walk_statements(if_false) ;
    write_to_output("label IF_END" + label + "\n") ;
}

// walk an ast while node with fields
// condition - an ast expr node
// body      - an ast statements node
//
void walk_while(ast t)
{
    ast condition = get_while_condition(t) ;
    ast body = get_while_body(t) ;

    // update while counter
    string label = to_string(while_counter++) ;

    // walk a while
    write_to_output("label WHILE_EXP" + label + "\n") ;
    walk_expr(condition) ;
    write_to_output("not\nif-goto WHILE_END" + label + "\n") ;
    walk_statements(body) ;
    write_to_output("goto WHILE_EXP" + label + "\n") ;
    write_to_output("label WHILE_END" + label + "\n") ;
}

// walk an ast do node with a single field
// call - an ast call as function node or an ast call as method node
//
void walk_do(ast t)
{
    ast call = get_do_call(t) ;

    switch(ast_node_kind(call))
    {
    case ast_call_as_function:
        walk_call_as_function(call) ;
        write_to_output("pop temp 0\n") ;
        break ;
    case ast_call_as_method:
        walk_call_as_method(call) ;
        write_to_output("pop temp 0\n") ;
        break ;
    default:
        fatal_error(0,"Unexpected call kind") ;
        break ;
    }
}

// walk an ast return node, it has not fields
//
void walk_return(ast t)
{
    write_to_output("push constant 0\n") ;
    write_to_output("return\n") ;
}

// walk an ast return expr node with a single field
// expr - an ast expr node
//
void walk_return_expr(ast t)
{
    ast expr = get_return_expr(t) ;

    // walk return expr
    walk_expr(expr) ;
    write_to_output("return\n") ;
}

// walk an ast param list node
// it is an ast vector of ast term and infix op nodes
// it must be of odd length > 0, ie 1, 3, 5, 7, ...
// all elements at even indices are an ast term node
// all elements at odd indices are an ast infix op
//
void walk_expr(ast t)
{
    int term_ops = size_of_expr(t) ;

    //a string to store the infix
    string infix ;

    for ( int i = 0 ; i < term_ops ; i++ )
    {
        ast term_op = get_expr(t,i) ;
        if ( i % 2 == 0 )
        {
            walk_term(term_op) ;
            write_to_output(infix) ;
        }
        else
        {
            infix = walk_infix_op(term_op);
        }
    }
}

// walk an ast term node with a single field
// term - one of the following ast nodes:
//        int, string, bool, null, this, expr, unary op,
//        variable, array index, call as function or call as method
//
void walk_term(ast t)
{
    ast term = get_term_term(t) ;

    switch(ast_node_kind(term))
    {
    case ast_int:
        walk_int(term) ;
        break ;
    case ast_string:
        walk_string(term) ;
        break ;
    case ast_bool:
        walk_bool(term) ;
        break ;
    case ast_null:
        walk_null(term) ;
        break ;
    case ast_this:
        walk_this(term) ;
        break ;
    case ast_expr:
        walk_expr(term) ;
        break ;
    case ast_unary_op:
        walk_unary_op(term) ;
        break ;
    case ast_var:
        walk_var(term) ;
        break ;
    case ast_array_index:
        walk_array_index(term) ;
        break ;
    case ast_call_as_function:
        walk_call_as_function(term) ;
        break ;
    case ast_call_as_method:
        walk_call_as_method(term) ;
        break ;
    default:
        fatal_error(0,"Unexpected term kind") ;
        break ;
    }
}

// walk an ast int node with a single field
// constant - an integer in the range -32,768 to 32,767
//
void walk_int(ast t)
{
    int _constant = get_int_constant(t) ;
    write_to_output("push constant " + to_string(_constant) + "\n") ;
}

// walk an ast string node with a single field
// constant - a string
//
void walk_string(ast t)
{
    string _constant = get_string_constant(t) ;

    // create the string
    int length = _constant.length() ;
    write_to_output("push constant " + to_string(length) + "\n") ;
    write_to_output("call String.new 1\n") ;

    // append chars
    for ( int i = 0 ; i < length ; i++ )
    {
        int char_value = (int)_constant[i] ;
        write_to_output("push constant " + to_string(char_value) + "\n") ;
        write_to_output("call String.appendChar 2\n") ;
    }
}

// walk an ast bool node with a single field
// constant - either true or false
//
void walk_bool(ast t)
{
    bool _constant = get_bool_t_or_f(t) ;
    if ( _constant ) write_to_output("push constant 0\nnot\n") ; else
    write_to_output("push constant 0\n") ;
}

// walk an ast null node, it has not fields
//
void walk_null(ast t)
{
    write_to_output("push constant 0\n") ;
}

// walk an ast this node, it has not fields
//
void walk_this(ast t)
{
    write_to_output("push pointer 0\n") ;
}

// walk an ast unary op node with fields
// op   - a string, either "-" or "~"
// term - an ast term node
//        int, string, bool, null, this, expr, unary op,
//        variable, array index, call as function or call as method
//
void walk_unary_op(ast t)
{
    string uop = get_unary_op_op(t);
    ast term = get_unary_op_term(t) ;
    if ( uop[0] == '-' ) unary_op += "neg\n" ; else
    unary_op += "not\n" ;

    walk_term(term) ;
    write_to_output(unary_op) ;

    // reset the unary_op
    unary_op = "" ;
}

// walk an ast variable node with fields
// name - a string
// type - a string - "int", "char", "boolean" or a class name
// segment - a string
// offset - an int
//
void walk_var(ast t)
{
    string name = get_var_name(t) ;
    string type = get_var_type(t) ;
    string segment = get_var_segment(t) ;
    int offset = get_var_offset(t) ;

    // walk a var
    write_to_output("push " + segment + " " + to_string(offset) + "\n") ;
}

// walk an ast array index node with fields
// var   - an ast variable node
// index - an ast expr node
//
void walk_array_index(ast t)
{
    ast var = get_array_index_var(t) ;
    ast index = get_array_index_index(t) ;

    walk_expr(index) ;
    walk_var(var) ;
    write_to_output("add\n") ;
    write_to_output("pop pointer 1\n") ;
    write_to_output("push that 0\n") ;
}

// walk an ast subr call as method with fields
// class name - a string
// call       - an ast subr call node
//
void walk_call_as_function(ast t)
{
    string class_name = get_call_as_function_class_name(t) ;
    ast subr_call = get_call_as_function_subr_call(t) ;

    call_name = "call " + class_name ;
    walk_subr_call(subr_call) ;
}

// walk an ast subr call as method with fields
// class name - a string
// var        - an ast variable or ast this node, hidden first parameter of the call
// call       - an ast subr call node
//
void walk_call_as_method(ast t)
{
    string class_name = get_call_as_method_class_name(t) ;
    ast var = get_call_as_method_var(t) ;
    ast subr_call = get_call_as_method_subr_call(t) ;

    switch(ast_node_kind(var))
    {
    case ast_this:
        walk_this(var) ;
        break ;
    case ast_var:
        walk_var(var) ;
        break ;
    default:
        fatal_error(0,"Expected var or this") ;
        break ;
    }
    
    is_method = true ;
    call_name = "call " + class_name ;
    walk_subr_call(subr_call) ;
}

// walk an ast subr call node with fields
// name      - a string
// expr list - a vector of ast expr nodes
//
void walk_subr_call(ast t)
{
    string subr_name = get_subr_call_subr_name(t) ;
    ast expr_list = get_subr_call_expr_list(t) ;

    call_name = call_name + "." + subr_name + " ";
    walk_expr_list(expr_list) ;
}

// walk an ast expr list node
// it is an ast vector of ast expr nodes
//
void walk_expr_list(ast t)
{
    int nexpressions = size_of_expr_list(t) ;

    // to deal with different parameter numbers of functions and methods
    int param = nexpressions ;
    
    for ( int i = 0 ; i < nexpressions ; i++ )
    {
        walk_expr(get_expr_list(t,i)) ;
    }

    // finish the call
    if ( is_method ) param ++ ; is_method = false ;
    write_to_output(call_name + to_string(param) + "\n") ;
}

// walk an ast infix op node with a single field
// op - a string - one of "+", "-", "*", "/", "&", "|", "<", ">", "="
//
string walk_infix_op(ast t)
{
    string op = get_infix_op_op(t) ;
    return translate_op(op) + "\n" ;
}

// main program
int main(int argc,char **argv)
{
    // walk an AST parsed from XML and print VM code
    walk_class(ast_parse_xml()) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

