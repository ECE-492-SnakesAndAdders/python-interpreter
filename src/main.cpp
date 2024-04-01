/*********************************************************************************
* Description: Main executable file for the Python interpreter
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#include <cstdio>
#include <cstring>
#include "error.h"
#include "evaluator.h"
#include "interpreter.h"
#include "lexer.h"
#include "parser.h"
#include "tree.h"
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
int read(char ** input_ptr) {
    // prompt user for command
    printf(">>> ");
    // read characters until 'enter' key is hit or buffer is full
    gets(*input_ptr);
    strcat(*input_ptr, "\n");
    // exit if user wants to
    if (strcmp(*input_ptr, "exit()") == 0) {
        return 1;
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
int eval(Interpreter interpreter, char ** input_ptr, char ** output_ptr) {
    return interpreter.interpret_command(input_ptr, output_ptr);
}


/**
 * \brief Prints some code output.
 * \param [in] output_ptr Pointer to where the text to be printed is stored.
 * \return 0 on success; a non-zero error number on failure.
 */
int print(char ** output_ptr) {
    // print the output string received
    if (**output_ptr) {
        printf(*output_ptr);
    }
    return 0;
}


/**
 * \brief Produces the main REPL behavior of the interpreter.
 * \return 0 on success; a non-zero integer on failure.
 */
int main() {
    printf("\nWelcome to Python on the C3 board.\n");

    int return_code = 0;
    // REPL: Read-Eval-Print-Loop
    environment env;
    Interpreter interpreter(&env);
    while (true) {
        // stores the input command received
        char input[MAX_INPUT_LEN] = "";
        char * input_ptr = (char *) input;
        memset(input, 0, MAX_INPUT_LEN);
        // stores the output to be printed
        char output[MAX_OUTPUT_LEN] = "";
        char * output_ptr = (char *) output;
        memset(output, 0, MAX_OUTPUT_LEN);

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
