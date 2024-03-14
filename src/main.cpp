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
#include "interpreter.h"
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
    xpd_putc('>');
    xpd_putc('>');
    xpd_putc('>');
    xpd_putc(' ');
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
 * \param [in] interpreter The interpreter to analyze and execute the command.
 * \param [in] input_ptr Pointer to where the input received is stored.
 * \param [in] output_ptr Pointer to where to store the text to be printed.
 * \return 0 on success; a non-zero error number on failure.
 */
uint16_t eval(Interpreter interpreter, char ** input_ptr, char ** output_ptr) {
    return interpreter.interpret_command(input_ptr, output_ptr);
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
        xpd_putc('\n');
    }
    return 0;
}


/**
 * \brief Produces the main REPL behavior of the interpreter.
 * \return 0 on success; a non-zero integer on failure.
 */
int main() {
    print_string("\nWelcome to Python on the C3 board.\n");

    uint16_t return_code = 0;
    // REPL: Read-Eval-Print-Loop
    // Interpreter interpreter(0);
    // xpd_echo_int(-1, XPD_Flag_UnsignedDecimal);
    // xpd_putc('\n');
    // environment env;
    // env.num_used = 0;
    // xpd_echo_int(env.num_used, XPD_Flag_UnsignedDecimal);
    // xpd_putc('\n');
    // literal_value test1;
    // test1.type = TRUE_VALUE;
    // write_variable(&env, "name", test1);
    // literal_value test2;
    // test2.type = NUMBER_VALUE;
    // test2.data.number = 23;
    // write_variable(&env, "number", test2);
    // literal_value ans;
    // read_variable(&env, "name", &ans);
    // xpd_echo_int(ans.type, XPD_Flag_UnsignedDecimal);
    // xpd_putc('\n');
    // read_variable(&env, "number", &ans);
    // xpd_echo_int(ans.type, XPD_Flag_UnsignedDecimal);
    // xpd_putc('\n');
    // xpd_echo_int(ans.data.number, XPD_Flag_UnsignedDecimal);
    // xpd_putc('\n');
    environment env;
    // env.num_used = 0;
    Interpreter interpreter(&env);
    xpd_echo_int(-1, XPD_Flag_UnsignedDecimal);
    xpd_putc('\n');

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
            report_failure("error in read()");
            break;
        }

        // evaluate and execute input received
        if ((return_code = eval(interpreter, &input_ptr, &output_ptr))) {
            // if an error occurred, stop this command and prompt for a new one
            continue;
        }
        
        // display the correct output from this input, handle sytem error
        if ((return_code = print(&output_ptr))) {
            report_failure("error in print()");
            break;
        }

        // continue looping in this cycle
    }
    return 0;
}
