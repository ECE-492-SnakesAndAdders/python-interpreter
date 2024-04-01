/*********************************************************************************
* Description: Main executable file for the Python interpreter
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#include <GPIO.h>
#include <SystemClock.h>
#include <Thread.h>
#include <XPD.h>
#include "error.h"
#include "evaluator.h"
#include "expr.h"
#include "lexer.h"
#include "main.h"
#include "parser.h"
#include "utility.h"


// the maximum number of characters in a line of input
#ifndef MAX_INPUT_LEN
#define MAX_INPUT_LEN 64
#endif

// the maximum number of tokens in a line of input
#ifndef MAX_INPUT_TOKENS
#define MAX_INPUT_TOKENS 64
#endif

// the maximum number of characters in a line of output
#ifndef MAX_OUTPUT_LEN
#define MAX_OUTPUT_LEN 256
#endif


/** This project is a rough implementation of the Python language on Eleven Engineering's Credit Card Computer (C3).
    This repository contains a collection of C and C++ code that forms tree-walk interpreter using a recursive descent algorithm.
    Much of the code is based on the book Crafting Interpreters by Robert Nystrom (https://craftinginterpreters.com/).
*/


// forward declarations
void setup_LCD();
void setup_UART();
char get_char_now();


/**
 * \brief Reads in a line (until a '\n' character) of input.
 * \param [in] input_ptr Pointer to where to store the input received.
 * \return 0 on success; a non-zero error number on failure.
 */
uint16_t read(char ** input_ptr) {
    // prompt user for command
    print_string(">>> ");
    // read characters until 'enter' key is hit or buffer is full
    for (uint16_t i = 0; i < MAX_INPUT_LEN - 1; i++) {
        *(*input_ptr + i) = get_char_now();
        if ((*(*input_ptr + i) == '\r') || (i == MAX_INPUT_LEN - 2)) {
            *(*input_ptr + i + 1) = '\0';
            break;
        }
    }
    return 0;
}


/**
 * \brief Parses, analyzes, and executes the Python code to produce output.
 * \param [in] input_ptr Pointer to where the input received is stored.
 * \param [in] output_ptr Pointer to where to store the text to be printed.
 * \return 0 on success; a non-zero error number on failure.
 */
uint16_t eval(char ** input_ptr, char ** output_ptr) {
    uint16_t return_code = 0;

    // lex command, convert raw string into a sequence of tokens
    lexed_command token_sequence;
    Lexer lexer(input_ptr, &token_sequence);
    if ((return_code = lexer.scan_input())) {
        return 1;
    }
    // ------------------------------------------------------------------------
    // // FOR DEBUGGING; print each token to see that lexer works
    // print_string("LEXED INFO:\n");
    // print_string("Tokens: ");
    // for (uint16_t i = 0; i < token_sequence.token_count; i++) {
    //     // xpd_echo_int(token_sequence.tokens[i], XPD_Flag_UnsignedDecimal);
    //     print_string(" ");
    //     print_string(token_names[token_sequence.tokens[i]]);
    //     print_string(",");
    //     print_string(" ");
    // }
    // print_string("\n");
    // print_string("Strings: ");
    // for (uint16_t i = 0; i < token_sequence.str_lit_count; i++) {
    //     print_string(token_sequence.str_lits[i]);
    //     print_string(",");
    //     print_string(" ");
    // }
    // print_string("\n");
    // print_string("Numbers: ");
    // for (uint16_t i = 0; i < token_sequence.num_lit_count; i++) {
    //     // xpd_echo_int(token_sequence.num_lits[i], XPD_Flag_UnsignedDecimal);
    //     print_string(",");
    //     print_string(" ");
    // }
    // print_string("\n");
    // print_string("Identifiers: ");
    // for (uint16_t i = 0; i < token_sequence.identifier_count; i++) {
    //     print_string(token_sequence.identifiers[i]);
    //     print_string(",");
    //     print_string(" ");
    // }
    // print_string("\n");
    // ------------------------------------------------------------------------

    // parse command, convert sequence of tokens into a syntax tree
    node * tree;
    Parser parser(token_sequence, &tree);
    if ((return_code = parser.parse_input())) {
        return 1;
    }
    // ------------------------------------------------------------------------
    // // FOR DEBUGGING; print tree to see that parser works
    // xpd_puts("PARSED INFO:\n");
    // print_tree(*tree);
    // print_string("\n");
    // ------------------------------------------------------------------------

    // evaluate command, convert syntax tree into a result
    literal_value result;
    Evaluator evaluator(0);
    if ((return_code = evaluator.evaluate_input(tree, &result))) {
        return 1;
    }

    stringify_value(result, output_ptr);
    return 0;
}


/**
 * \brief Prints some code output.
 * \param [in] output_ptr Pointer to where the text to be printed is stored.
 * \return 0 on success; a non-zero error number on failure.
 */
uint16_t print(char ** output_ptr) {
    // print the output string received
    if (**output_ptr) {
        print_string(output_ptr);
        print_string("\n");
    }
    return 0;
}


/**
 * \brief Produces the main REPL behavior of the interpreter.
 * \return 0 on success; a non-zero integer on failure.
 */
int main() {
    // initialize the peripheral drivers
    setup_LCD();
    setup_UART();
    
    print_string("Welcome to Python on the C3 board.\r\n");

    uint16_t return_code = 0;
    // REPL: Read-Eval-Print-Loop
    while (true) {
        // stores the input command received
        char input[MAX_INPUT_LEN] = "";
        char * input_ptr = (char *) input;
        memclear(input, MAX_INPUT_LEN);
        // stores the output to be printed
        char output[MAX_OUTPUT_LEN] = "";
        char * output_ptr = (char *) output;
        memclear(output, MAX_OUTPUT_LEN);

        // read in user input (command / code), handle sytem error
        if ((return_code = read(&input_ptr))) {
            // print_string(&input_ptr);
            report_failure("error in read()");
            break;
        }

        // print_string("HELLO WORLD:\r\n");
        // print_string(&input_ptr);
        // print_string("HELLO WORLD DONE\r\n");

        // print_string("HELLO WORLD 2");

        // evaluate and execute input received
        if ((return_code = eval(&input_ptr, &output_ptr))) {
            // if an error occurred, stop this command and prompt for a new one
            continue;
        }

        // print_string("HELLO WORLD 3");
        
        // display the correct output from this input, handle sytem error
        if ((return_code = print(&output_ptr))) {
            report_failure("error in print()");
            break;
        }

        // continue looping in this cycle
    }
    return 0;
}
