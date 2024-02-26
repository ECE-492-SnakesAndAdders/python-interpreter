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


#ifndef MAX_INPUT_LEN
#define MAX_INPUT_LEN 64
#endif

#ifndef MAX_INPUT_TOKENS
#define MAX_INPUT_TOKENS 64
#endif

#ifndef MAX_OUTPUT_LEN
#define MAX_OUTPUT_LEN 256
#endif


/** This project is a tree-walk interpreter that uses a recursive descent algorithm
    Much of the code is based on Crafting Interpreters by Robert Nystrom (https://craftinginterpreters.com/)
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
    Lexer lexer(input_ptr);
    lexer.scan_input();
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

        // read in user input (command / code)
        if ((return_code = read(&input_ptr))) {
            xpd_puts("FATAL: error ");
            xpd_echo_int(return_code, XPD_Flag_UnsignedDecimal);
            xpd_puts(" in read()\n");
            break;
        }

        // evaluate and execute input received
        if ((return_code = eval(&input_ptr, &output_ptr))) {
            xpd_puts("FATAL: error ");
            xpd_echo_int(return_code, XPD_Flag_UnsignedDecimal);
            xpd_puts(" in eval()\n");
            break;
        }
        
        // display the correct output from this input
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
