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


/**
 * \brief Reads in a line (until a '\n' character) of input.
 * \param [in] input_ptr Pointer to where to store the input received.
 * \return 0 on success; a non-zero error number on failure.
 */
uint16_t read(char ** input_ptr) {
    // prompt user for command
    xpd_puts(">>> ");
    // read characters until 'enter' key is hit or buffer is full
    for (uint16_t i = 0; i < MAX_INPUT_LEN - 1; i++) {
        *(*input_ptr + i) = (char) xpd_getchar();
        if ((*(*input_ptr + i) == '\n') || (i == MAX_INPUT_LEN - 2)) {
            // add NULL terminator character after 'enter' key or final slot
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
    // xpd_puts("LEXED INFO:\n");
    // xpd_puts("Tokens: ");
    // for (uint16_t i = 0; i < token_sequence.token_count; i++) {
    //     xpd_echo_int(token_sequence.tokens[i], XPD_Flag_UnsignedDecimal);
    //     xpd_putc(' ');
    //     xpd_puts(token_names[token_sequence.tokens[i]]);
    //     xpd_putc(',');
    //     xpd_putc(' ');
    // }
    // xpd_putc('\n');
    // xpd_puts("Strings: ");
    // for (uint16_t i = 0; i < token_sequence.str_lit_count; i++) {
    //     xpd_puts(token_sequence.str_lits[i]);
    //     xpd_putc(',');
    //     xpd_putc(' ');
    // }
    // xpd_putc('\n');
    // xpd_puts("Numbers: ");
    // for (uint16_t i = 0; i < token_sequence.num_lit_count; i++) {
    //     xpd_echo_int(token_sequence.num_lits[i], XPD_Flag_UnsignedDecimal);
    //     xpd_putc(',');
    //     xpd_putc(' ');
    // }
    // xpd_putc('\n');
    // xpd_puts("Identifiers: ");
    // for (uint16_t i = 0; i < token_sequence.identifier_count; i++) {
    //     xpd_puts(token_sequence.identifiers[i]);
    //     xpd_putc(',');
    //     xpd_putc(' ');
    // }
    // xpd_putc('\n');
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
    // xpd_putc('\n');
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
        xpd_puts(*output_ptr);
        xpd_putc('\n');
    }
    return 0;
}


/**
 * \brief Produces the main REPL behavior of the interpreter.
 * \return 0 on success; a non-zero integer on failure.
 */
int main() {
    xpd_puts("\nWelcome to Python on the C3 board.\n");

    uint16_t return_code = 0;
    // REPL: Read-Eval-Print-Loop
    while (true) {
        // stores the input command received
        char input[MAX_INPUT_LEN] = "";
        char * input_ptr = (char *) input;
        // stores the output to be printed
        char output[MAX_OUTPUT_LEN] = "";
        char * output_ptr = (char *) output;

        // read in user input (command / code), handle sytem error
        if ((return_code = read(&input_ptr))) {
            xpd_puts("FATAL: error ");
            xpd_echo_int(return_code, XPD_Flag_UnsignedDecimal);
            xpd_puts(" in read()\n");
            break;
        }

        // evaluate and execute input received
        if ((return_code = eval(&input_ptr, &output_ptr))) {
            // if an error occurred, stop this command and prompt for a new one
            continue;
        }
        
        // display the correct output from this input, handle sytem error
        if ((return_code = print(&output_ptr))) {
            xpd_puts("FATAL: error ");
            xpd_echo_int(return_code, XPD_Flag_UnsignedDecimal);
            xpd_puts(" in print()\n");
            break;
        }

        // continue looping in this cycle
    }
    return 0;
}
