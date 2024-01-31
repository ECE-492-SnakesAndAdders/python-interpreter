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


#ifndef MAX_INPUT_LEN
#define MAX_INPUT_LEN 64
#endif

#ifndef MAX_OUTPUT_LEN
#define MAX_OUTPUT_LEN 256
#endif


/**
 * \brief Reads in a line (until a '\n' character) of input.
 * \param [in] input_ptr Pointer to where to store the input received.
 * \return 0 on success, an error number on failure.
 */
uint16_t read(char ** input_ptr) {
    char* input_text = *input_ptr;
    xpd_puts(">>> ");
    xpd_puts("READ\n");
    // read characters until 'enter' key is hit
    for (uint16_t i = 0; i < MAX_INPUT_LEN; i++) {
        input_text[i] = (char) xpd_getchar();
        xpd_putc(input_text[i]);
        xpd_putc('\n');
        if ((input_text[i] == 10) || (i == MAX_INPUT_LEN - 1)) {
            // reassign 'enter' key or final slot to NULL terminator character
            input_text[i] = '\0';
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
    xpd_puts("EVAL\n");
    char* input_text = *input_ptr;
    char* output_text = *output_ptr;
    // temporary implementation; pass input to output for printing to test pipeline
    uint16_t LEN = MAX_INPUT_LEN;
    if (MAX_INPUT_LEN > MAX_OUTPUT_LEN) {
        LEN = MAX_OUTPUT_LEN;
    }
    for (uint16_t i = 0; i < LEN; i++) {
        output_text[i] = input_text[i];
    }
    return 0;
}


/**
 * \brief Prints some code output.
 * \param [in] output_ptr Pointer to where the text to be printed is stored.
 * \return 0 on success, an error number on failure.
 */
uint16_t print(char ** output_ptr) {
    xpd_puts("PRINT\n");
    char* output_text = *output_ptr;
    xpd_puts(output_text);
    return 0;
}


/**
 * \brief Produces the main REPL behavior of the interpreter.
 * \return 0 on success, another integer on failure.
 */
int main() {
    xpd_puts("Welcome to Python on the C3 board.\n");

    // stores the input command received
    char input[MAX_INPUT_LEN] = "";
    char ** input_ptr = (char **) &input;
    // stores the output to be printed
    char output[MAX_OUTPUT_LEN] = "";
    char ** output_ptr = (char **) &output;

    uint16_t return_code = 0;
    // REPL: Read-Eval-Print-Loop
    while (true) {
        memclear(input, MAX_INPUT_LEN);
        memclear(output, MAX_OUTPUT_LEN);

        // read in user input (command / code)
        if ((return_code = read(input_ptr))) {
            xpd_puts("FATAL: error ");
            xpd_echo_int(return_code, XPD_Flag_UnsignedDecimal);
            xpd_puts(" in read()\n");
            break;
        }

        // evaluate and execute input received
        if ((return_code = eval(input_ptr, output_ptr))) {
            xpd_puts("FATAL: error ");
            xpd_echo_int(return_code, XPD_Flag_UnsignedDecimal);
            xpd_puts(" in eval()\n");
            break;
        }
        
        // display the correct output from this input
        if ((return_code = print(output_ptr))) {
            xpd_puts("FATAL: error ");
            xpd_echo_int(return_code, XPD_Flag_UnsignedDecimal);
            xpd_puts(" in print()\n");
            break;
        }

        // continue looping in this cycle
    }
    return 0;
}
