/*********************************************************************************
* Description: Main executable file for the Python interpreter
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#include <SystemClock.h>
#include <XPD.h>
#include <GPIO.h>
#include <Thread.h>
#include "main.h"
#include "utility.h"
#include "lexer.h"
#include "parser.h"
#include "expr.h"
#include "evaluator.h"
#include "error.h"


#ifndef MAX_INPUT_LEN
#define MAX_INPUT_LEN 64
#endif


#ifndef MAX_INPUT_TOKENS
#define MAX_INPUT_TOKENS 64
#endif


#ifndef MAX_OUTPUT_LEN
#define MAX_OUTPUT_LEN 256
#endif


/** This project is a tree-walk interpreter that uses a recursive descent algorithm.
    Much of the code is based on Crafting Interpreters by Robert Nystrom (https://craftinginterpreters.com/).
*/


/**
 * \brief Reads in a line (until a '\n' character) of input.
 * \param [in] input_ptr Pointer to where to store the input received.
 * \return 0 on success, an error number on failure.
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
 * \return 0 on success, an error number on failure.
 */
uint16_t eval(char ** input_ptr, char ** output_ptr) {
    uint16_t return_code = 0;

    lexed_command token_sequence;
    Lexer lexer(input_ptr, &token_sequence);
    if ((return_code = lexer.scan_input())) {
        return 1;
    }
    // ------------------------------------------------------------------------
    // FOR DEBUGGING; print each token to see that lexer works
    xpd_puts("LEXED INFO:\n");
    xpd_puts("Tokens: ");
    for (uint16_t i = 0; i < token_sequence.token_count; i++) {
        xpd_echo_int(token_sequence.tokens[i], XPD_Flag_UnsignedDecimal);
        xpd_putc(' ');
        xpd_puts(token_names[token_sequence.tokens[i]]);
        xpd_putc(',');
        xpd_putc(' ');
    }
    xpd_putc('\n');
    xpd_puts("Strings: ");
    for (uint16_t i = 0; i < token_sequence.str_lit_count; i++) {
        xpd_puts(token_sequence.str_lits[i]);
        xpd_putc(',');
        xpd_putc(' ');
    }
    xpd_putc('\n');
    xpd_puts("Numbers: ");
    for (uint16_t i = 0; i < token_sequence.num_lit_count; i++) {
        xpd_echo_int(token_sequence.num_lits[i], XPD_Flag_UnsignedDecimal);
        xpd_putc(',');
        xpd_putc(' ');
    }
    xpd_putc('\n');
    xpd_puts("Identifiers: ");
    for (uint16_t i = 0; i < token_sequence.identifier_count; i++) {
        xpd_puts(token_sequence.identifiers[i]);
        xpd_putc(',');
        xpd_putc(' ');
    }
    xpd_putc('\n');
    // ------------------------------------------------------------------------

    Parser parser(token_sequence);
    node * tree = parser.parse_input();
    // ------------------------------------------------------------------------
    // FOR DEBUGGING; print tree to see that parser works
    xpd_puts("PARSED INFO:\n");
    xpd_echo_int(tree -> type, XPD_Flag_UnsignedDecimal);
    xpd_putc('\n');
    xpd_echo_int(tree -> entry.unary_val.opcode, XPD_Flag_UnsignedDecimal);
    xpd_putc('\n');
    xpd_echo_int(tree -> entry.unary_val.right -> type, XPD_Flag_UnsignedDecimal);
    xpd_putc('\n');
    xpd_echo_int(tree -> entry.unary_val.right -> entry.literal_val.type, XPD_Flag_UnsignedDecimal);
    xpd_putc('\n');
    xpd_echo_int(tree -> entry.unary_val.right -> entry.literal_val.data.number, XPD_Flag_UnsignedDecimal);
    xpd_putc('\n');
    // ------------------------------------------------------------------------

    Evaluator evaluator(0);
    literal_value x = evaluator.evaluate(*tree);
    // ------------------------------------------------------------------------
    // FOR DEBUGGING; print answer to see that evaluator works
    xpd_puts("EVALUATED INFO:\n");
    xpd_echo_int(x.type, XPD_Flag_UnsignedDecimal);
    xpd_putc('\n');
    xpd_echo_int(x.data.number, XPD_Flag_UnsignedDecimal);
    xpd_putc('\n');
    // ------------------------------------------------------------------------

    // temporary implementation; pass input to output for printing to test pipeline
    uint16_t LEN = MAX_INPUT_LEN;
    if (MAX_INPUT_LEN > MAX_OUTPUT_LEN) {
        LEN = MAX_OUTPUT_LEN;
    }
    for (uint16_t i = 0; i < LEN; i++) {
        *(*output_ptr + i) = *(*input_ptr + i);
    }
    return 0;
}


/**
 * \brief Prints some code output.
 * \param [in] output_ptr Pointer to where the text to be printed is stored.
 * \return 0 on success, an error number on failure.
 */
uint16_t print(char ** output_ptr) {
    // print the output string received
    xpd_puts(*output_ptr);
    return 0;
}


/**
 * \brief Produces the main REPL behavior of the interpreter.
 * \return 0 on success, another integer on failure.
 */
int main() {
    xpd_puts("\nWelcome to Python on the C3 board.\n");

    // stores the input command received
    char input[MAX_INPUT_LEN] = "";
    char * input_ptr = (char *) input;
    // stores the output to be printed
    char output[MAX_OUTPUT_LEN] = "";
    char * output_ptr = (char *) output;

    uint16_t return_code = 0;
    // REPL: Read-Eval-Print-Loop
    while (true) {
        memclear(input_ptr, MAX_INPUT_LEN);
        memclear(output_ptr, MAX_OUTPUT_LEN);

        // read in user input (command / code), handle sytem error
        if ((return_code = read(&input_ptr))) {
            xpd_puts("FATAL: error ");
            xpd_echo_int(return_code, XPD_Flag_UnsignedDecimal);
            xpd_puts(" in read()\n");
            break;
        }

        // evaluate and execute input received
        if ((return_code = eval(&input_ptr, &output_ptr))) {
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
