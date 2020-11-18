// name: Shaokang Ma
// id: a1790846

// convert Hack VM Code into Hack Assembler

#include "iobuffer.h"
#include "tokeniser.h"
#include "assembler-internal.h"

// to make out programs a bit neater
using namespace std ;

using namespace CS_IO_Buffers ;
using namespace Hack_Virtual_Machine ;

// store the function name
static string label_name ;

// update the lable number
static int label_num ;

// store the class name
static string class_name ;

// predefined variable
static string R13 = "R13" ;
static string R14 = "R14" ;

// pop the first value - not to be used in loop
static void translate_vm_pop()
{
    output_assembler("@SP") ;
    output_assembler("AM=M-1") ;
    output_assembler("D=M") ;
    output_assembler("A=A-1") ;
}

// push a value to the stack - using a D register
static void translate_vm_push()
{
    output_assembler("@SP") ;
    output_assembler("AM=M+1") ;
    output_assembler("A=A-1") ;
    output_assembler("M=D") ;
}

// get the first value but not pop it
static void last_value()
{
    output_assembler("@SP") ;
    output_assembler("A=M-1") ;
}

// translate vm operator command into assembly language
// * vm_operator ::= 'add' | 'and' | 'eq' | 'gt' | 'lt' | 'neg' | 'not' | 'or' | 'sub' | 'return'
static void translate_vm_operator(TokenKind the_op)
{
    start_of_vm_operator_command(the_op) ;

    switch (the_op)
    {
    case tk_add:
        translate_vm_pop() ;
        output_assembler("M=D+M") ;
        break ;
    
    case tk_and:
        translate_vm_pop() ;
        output_assembler("M=D&M") ;
        break ;

    case tk_eq:
        // update the label number
        label_num ++ ;

        // use JEQ to check the subtraction
        translate_vm_pop() ;
        output_assembler("D=M-D") ;
        output_assembler("@" + label_name + "$" + "EQ" + to_string(label_num)) ;
        output_assembler("D;JEQ") ;  

        // body of procedure if not eq           
        last_value() ;
        output_assembler("M=0") ;
        output_assembler("@" + label_name + "$" + "NOTEQ" + to_string(label_num)) ;
        output_assembler("0;JMP") ;

        // body of procedure if eq             
        output_assembler("(" + label_name + "$" + "EQ" + to_string(label_num) + ")") ;
        last_value() ;
        output_assembler("M=-1") ;

        output_assembler("(" + label_name + "$" + "NOTEQ" + to_string(label_num) + ")") ;
        break ;

    case tk_gt:
    
        // update the label number
        label_num ++ ;

        // denote the first the value on stack B and the second A
        output_assembler("@SP") ;
        output_assembler("AM=M-1") ;
        output_assembler("D=M") ;

        // store the value of B
        output_assembler("@" + R13) ;
        output_assembler("M=D") ;

        // check if B is smaller than 0
        output_assembler("@" + label_name + "$" + "BLT" + to_string(label_num)) ;
        output_assembler("D;JLT") ;

        // if B is equal or bigger than 0, check if A is smaller than 0
        last_value() ;
        output_assembler("D=M") ;

        // if A < 0 and B >= 0 -> A < B  -> false
        output_assembler("@" + label_name + "$" + "NOTGT" + to_string(label_num)) ;
        output_assembler("D;JLT") ;

        // body of procedure if A >= 0 and B >= 0 or A < 0 and B < 0
        // check if A - B > 0
        output_assembler("(" + label_name + "$" + "ASUBB" + to_string(label_num) + ")") ;
        last_value() ;
        output_assembler("D=M") ;
        output_assembler("@" + R13) ;
        output_assembler("D=D-M") ;

        // if A > B
        output_assembler("@" + label_name + "$" + "GT" + to_string(label_num)) ;
        output_assembler("D;JGT") ;

        // if A <= B
        output_assembler("@" + label_name + "$" + "NOTGT" + to_string(label_num)) ;
        output_assembler("0;JMP") ;

        // if B is smaller than 0, check if A is equal or bigger than 0
        output_assembler("(" + label_name + "$" + "BLT" + to_string(label_num) + ")") ;
        last_value() ;
        output_assembler("D=M") ;

        // if A >= 0 and B < 0 -> A > B -> true
        output_assembler("@" + label_name + "$" + "GT" + to_string(label_num)) ;
        output_assembler("D;JGE") ;

        // else check the subtraction
        output_assembler("@" + label_name + "$" + "ASUBB" + to_string(label_num)) ;
        output_assembler("0;JMP") ;

        // body of procedure if not gt
        output_assembler("(" + label_name + "$" + "NOTGT" + to_string(label_num) + ")") ;
        last_value() ;
        output_assembler("M=0") ;
        output_assembler("@" + label_name + "$" + "ENDOFGT" + to_string(label_num)) ;
        output_assembler("0;JMP") ;

        // body of procedure if gt
        output_assembler("(" + label_name + "$" + "GT" + to_string(label_num) + ")") ;
        last_value() ;
        output_assembler("M=-1") ;

        output_assembler("(" + label_name + "$" + "ENDOFGT" + to_string(label_num) + ")") ;
        break ;
    
    case tk_lt:

        // update the label number
        label_num ++ ;

        // denote the first the value on stack B and the second A
        output_assembler("@SP") ;
        output_assembler("AM=M-1") ;
        output_assembler("D=M") ;

        // store the value of B
        output_assembler("@" + R13) ;
        output_assembler("M=D") ;

        // check if B is smaller than 0
        output_assembler("@" + label_name + "$" + "BLT" + to_string(label_num)) ;
        output_assembler("D;JLT") ;

        // if B is equal or bigger than 0, check if A is smaller than 0
        last_value() ;
        output_assembler("D=M") ;

        // if A < 0 and B >= 0 -> A < B  -> true
        output_assembler("@" + label_name + "$" + "LT" + to_string(label_num)) ;
        output_assembler("D;JLT") ;

        // body of procedure if A >= 0 and B >= 0 or A < 0 and B < 0
        // check if A - B < 0
        output_assembler("(" + label_name + "$" + "ASUBB" + to_string(label_num) + ")") ;
        last_value() ;
        output_assembler("D=M") ;
        output_assembler("@" + R13) ;
        output_assembler("D=D-M") ;

        // if A < B
        output_assembler("@" + label_name + "$" + "LT" + to_string(label_num)) ;
        output_assembler("D;JLT") ;

        // if A >= B
        output_assembler("@" + label_name + "$" + "NOTLT" + to_string(label_num)) ;
        output_assembler("0;JMP") ;

        // if B is smaller than 0, check if A is equal or bigger than 0
        output_assembler("(" + label_name + "$" + "BLT" + to_string(label_num) + ")") ;
        last_value() ;
        output_assembler("D=M") ;

        // if A >= 0 and B < 0 -> A > B -> false
        output_assembler("@" + label_name + "$" + "NOTLT" + to_string(label_num)) ;
        output_assembler("D;JGE") ;

        // else check the subtraction
        output_assembler("@" + label_name + "$" + "ASUBB" + to_string(label_num)) ;
        output_assembler("0;JMP") ;

        // body of procedure if not lt
        output_assembler("(" + label_name + "$" + "NOTLT" + to_string(label_num) + ")") ;
        last_value() ;
        output_assembler("M=0") ;
        output_assembler("@" + label_name + "$" + "ENDOFLT" + to_string(label_num)) ;
        output_assembler("0;JMP") ;

        // body of procedure if lt
        output_assembler("(" + label_name + "$" + "LT" + to_string(label_num) + ")") ;
        last_value() ;
        output_assembler("M=-1") ;

        output_assembler("(" + label_name + "$" + "ENDOFLT" + to_string(label_num) + ")") ;
        break ;

    case tk_neg:
        last_value() ;
        output_assembler("M=-M") ;
        break ;

    case tk_not:
        last_value() ;
        output_assembler("M=!M") ;
        break ;
    
    case tk_or:
        translate_vm_pop() ;
        output_assembler("M=D|M") ;
        break ;
    
    case tk_sub:
        translate_vm_pop() ;
        output_assembler("M=M-D") ;
        break ;

    case tk_return:

        // store the frame
        output_assembler("@LCL") ;
        output_assembler("D=M") ;
        output_assembler("@" + R13) ;
        output_assembler("M=D") ;

        // store the return address
        output_assembler("@5") ;
        output_assembler("A=D-A") ;
        output_assembler("D=M") ;
        output_assembler("@" + R14) ;
        output_assembler("M=D") ;

        // reposition the return value for caller
        translate_vm_pop() ;
        output_assembler("@ARG") ;
        output_assembler("A=M") ;
        output_assembler("M=D") ;

        // restores the caller's SP
        output_assembler("@ARG") ;
        output_assembler("D=M+1") ;
        output_assembler("@SP") ;
        output_assembler("M=D") ;

        // restores the caller's THAT
        output_assembler("@R13") ;
        output_assembler("AM=M-1") ;
        output_assembler("D=M") ;
        output_assembler("@THAT") ;
        output_assembler("M=D") ;

        // restores the caller's THIS
        output_assembler("@R13") ;
        output_assembler("AM=M-1") ;
        output_assembler("D=M") ;
        output_assembler("@THIS") ;
        output_assembler("M=D") ;

        // restores the caller's ARG
        output_assembler("@R13") ;
        output_assembler("AM=M-1") ;
        output_assembler("D=M") ;
        output_assembler("@ARG") ;
        output_assembler("M=D") ;

        // restores the caller's LCL
        output_assembler("@R13") ;
        output_assembler("AM=M-1") ;
        output_assembler("D=M") ;
        output_assembler("@LCL") ;
        output_assembler("M=D") ;

        // goto return address
        output_assembler("@R14") ;
        output_assembler("A=M") ;
        output_assembler("0;JMP") ;
        break ;
    
    default:
        break ;
    }

    end_of_vm_command() ;
}

// translate vm jump command into assembly language
// * vm_jump ::= 'goto' | 'if-goto' | 'label'
static void translate_vm_jump(TokenKind jump, string label)
{
    start_of_vm_jump_command(jump,label) ;

    switch (jump)
    {
    case tk_goto:
        output_assembler("@" + label_name + "$" + label) ;
        output_assembler("0;JMP") ;
        break ;

    case tk_if_goto:
        translate_vm_pop() ;
        output_assembler("@" + label_name + "$" + label) ;
        output_assembler("D;JGT") ;
        output_assembler("D;JLT") ;
        break ;

    case tk_label:
        output_assembler("(" + label_name + "$" + label +")") ;
        break ;
    
    default:
        break ;
    }

    end_of_vm_command() ;
}

// translate vm call or function command into assembly language
// * vm_func ::= 'call' | 'function'
static void translate_vm_function(TokenKind func, string label, int n)
{
    // update the class name
    class_name = label.substr(0, label.find_first_of('.')) ;

    // update the label name if function is used
    if ( func == tk_function ) label_name = label ;

    start_of_vm_func_command(func,label,n) ;

    switch (func)
    {
    case tk_call:

        // update the label number
        label_num ++ ;

        // saves the return address
        output_assembler("@" + label_name + "$" + "retAddr" + to_string(label_num)) ;
        output_assembler("D=A") ;
        translate_vm_push() ;

        // push LCL of caller
        output_assembler("@LCL") ;
        output_assembler("D=M") ;
        translate_vm_push() ;

        // push ARG of caller
        output_assembler("@ARG") ;
        output_assembler("D=M") ;
        translate_vm_push() ;

        // push THIS of caller
        output_assembler("@THIS") ;
        output_assembler("D=M") ;
        translate_vm_push() ;

        // push THAT of caller
        output_assembler("@THAT") ;
        output_assembler("D=M") ;
        translate_vm_push() ;

        //reposition ARG for callee
        output_assembler("@" + to_string(n)) ;
        output_assembler("D=A") ;
        output_assembler("@5") ;
        output_assembler("D=D+A") ;
        output_assembler("@SP") ;
        output_assembler("D=M-D") ;
        output_assembler("@ARG") ;
        output_assembler("M=D") ;

        // reposition LCL for callee
        output_assembler("@SP") ;
        output_assembler("D=M") ;
        output_assembler("@LCL") ;
        output_assembler("M=D") ;

        // transfer control to callee
        output_assembler("@" + label) ;
        output_assembler("0;JMP") ;

        // generate a label for return address
        output_assembler("(" + label_name + "$" + "retAddr" + to_string(label_num)) ;
        break ;

    case tk_function:
        output_assembler("(" + label + ")") ;

        // push n 0s onto the stack
        for ( int i = 0 ; i < n ; i++ )
        {
            output_assembler("@SP") ;
            output_assembler("AM=M+1") ;
            output_assembler("A=A-1") ;
            output_assembler("M=0") ;
        }
        
        break ;
    
    default:
        break ;
    }

    end_of_vm_command() ;
}

// translate vm push or pop command into assembly language
// * vm_stack ::= 'pop' | 'push'
// * vm_segment ::= 'argument' | 'constant' | 'local' | 'pointer' | 'static' | 'temp' | 'that' | 'this'
static void translate_vm_stack(TokenKind stack,TokenKind segment,int offset)
{
    start_of_vm_stack_command(stack,segment,offset) ;

    switch (segment)
    {
    
    // the 4 segments below are the same
    // 2 situations - push and pop
    case tk_argument:
    case tk_local:
    case tk_that:
    case tk_this:
        if ( stack == tk_push )
        {
            output_assembler("@" + to_string(offset)) ;
            output_assembler("D=A") ;

            // use A-instruction according to the segment
            switch (segment)
            {
            case tk_argument:
                output_assembler("@ARG") ;
                break ;

            case tk_local:
                output_assembler("@LCL") ;
                break ;
            
            case tk_that:
                output_assembler("@THAT") ;
                break ;
            
            case tk_this:
                output_assembler("@THIS") ;
                break ;
            
            default:
                break ;
            }

            // store the value
            output_assembler("A=D+M") ;
            output_assembler("D=M") ;

            // push the value onto the stack
            translate_vm_push() ;
        }
        else
        {
            output_assembler("@" + to_string(offset)) ;
            output_assembler("D=A") ;

            // use A-instruction according to the segment
            switch (segment)
            {
            case tk_argument:
                output_assembler("@ARG") ;
                break ;

            case tk_local:
                output_assembler("@LCL") ;
                break ;
            
            case tk_that:
                output_assembler("@THAT") ;
                break ;
            
            case tk_this:
                output_assembler("@THIS") ;
                break ;
            
            default:
                break ;
            }
            // store the address
            output_assembler("D=D+M") ;
            output_assembler("@" + R13) ;
            output_assembler("M=D") ;

            // update the RAM with the value popped
            translate_vm_pop() ;
            output_assembler("@" + R13) ;
            output_assembler("A=M") ;
            output_assembler("M=D") ;
        }
        break ;

    // constant can only be pushed
    case tk_constant:
        output_assembler("@" + to_string(offset)) ;
        output_assembler("D=A") ;
        translate_vm_push() ;
        break ;

    // pointer is used to modify the start addresses of this and that segments
    case tk_pointer:
        if ( stack == tk_push )
        {
            if ( offset == 0 ) output_assembler("@THIS") ; else
            output_assembler("@THAT") ;
            output_assembler("D=M") ;
            translate_vm_push() ;
        }
        else
        {
            translate_vm_pop() ;
            if ( offset == 0 ) output_assembler("@THIS") ; else
            output_assembler("@THAT") ;
            output_assembler("M=D") ;
        }
        break ;

    case tk_static:
        if ( stack == tk_push )
        {
            output_assembler("@" + class_name + "." + to_string(offset)) ;
            output_assembler("D=M") ;
            translate_vm_push() ;
        }
        else
        {
            translate_vm_pop() ;
            output_assembler("@" + class_name + "." + to_string(offset)) ;
            output_assembler("M=D") ;
        }
        break ;

    case tk_temp:
        if ( stack == tk_push )
        {
            // temp segment starts at 5
            output_assembler("@" + to_string(offset + 5)) ;
            output_assembler("D=M") ;
            translate_vm_push() ;
        }
        else
        {
            translate_vm_pop() ;
            output_assembler("@" + to_string(offset + 5)) ;
            output_assembler("M=D") ;
        }
        break ;
    
    default:
        break ;
    }

    end_of_vm_command() ;
}

// grammar to be parsed:
// vm_class ::= command* eoi
// command ::= vm_operator | vm_jump | vm_func | vm_stack
// vm_operator ::= 'add' | 'and' | 'eq' | 'gt' | 'lt' | 'neg' | 'not' | 'or' | 'sub' | 'return'
// vm_jump ::= jump label
// vm_func ::= func label number
// vm_stack ::= stack segment number

// forward declare parsing functions - one per rule
static void parse_class() ;
static void parse_command() ;
static void parse_op() ;
static void parse_jump() ;
static void parse_func() ;
static void parse_stack() ;

// Note: always read one token after the one recognised

// vm_class ::= command* eoi
static void parse_class()
{
    // tell the output system we are starting to translate VM commands for a Jack class
    start_of_vm_class() ;

    while ( have(tk_vm_command) ) parse_command() ;
    mustbe(tk_eoi) ;

    // tell the output system we have just finished translating VM commands for a Jack class
    end_of_vm_class() ;
}

// command ::= vm_operator | vm_jump | vm_func | vm_stack
static void parse_command()
{
    if ( have(tk_vm_operator) ) parse_op() ; else
    if ( have(tk_vm_jump) )     parse_jump() ; else
    if ( have(tk_vm_func) )     parse_func() ; else
    if ( have(tk_vm_stack) )    parse_stack() ; else
    mustbe(tk_vm_command) ;
}

// vm_operator ::= 'add' | 'and' | 'eq' | 'gt' | 'lt' | 'neg' | 'not' | 'or' | 'sub' | 'return'
static void parse_op()
{
    TokenKind the_op = token_kind(mustbe(tk_vm_operator)) ;

    translate_vm_operator(the_op) ;
}

// vm_jump ::= jump label
// jump command - label, goto label or if-goto label
static void parse_jump()
{
    TokenKind command = token_kind(mustbe(tk_vm_jump)) ;
    string label = token_spelling(mustbe(tk_a_label)) ;

    translate_vm_jump(command,label) ;
}

// vm_func ::= func label number
// call function-name #args or function function-name #locals
static void parse_func()
{
    TokenKind command = token_kind(mustbe(tk_vm_func)) ;
    string label = token_spelling(mustbe(tk_a_label)) ;
    int number = token_ivalue(mustbe(tk_a_number)) ;

    translate_vm_function(command,label,number) ;
}

// vm_stack ::= stack segment number
// push segment offset or pop segment offset
static void parse_stack()
{
    TokenKind stack = token_kind(mustbe(tk_vm_stack)) ;
    TokenKind segment = token_kind(mustbe(tk_vm_segment)) ;
    int number = token_ivalue(mustbe(tk_a_number)) ;

    translate_vm_stack(stack,segment,number) ;
}

// main program
int main(int argc,char **argv)
{
    // initialise the tokeniser by reading the first token
    next_token() ;

    // parse a class and output VM code
    parse_class() ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}