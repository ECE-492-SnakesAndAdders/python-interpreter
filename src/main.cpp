#include <SystemClock.h>
#include <XPD.h>
#include <GPIO.h>
#include <Thread.h>
#include "main.h"


#ifndef MAX_INPUT_LEN
#define MAX_INPUT_LEN 64
#endif

#ifndef MAX_OUTPUT_LEN
#define MAX_OUTPUT_LEN 256
#endif


/**
 * \brief Reads in a line (until a '\n' character) of input.
 * \param [in] input_text Pointer to where to store the input received.
 * \return 0 on success, an error number on failure.
 */
uint16_t read(uint16_t * input_text) {
    return 0;
}


/**
 * \brief Parses, analyzes, and executes the Python code to produce output.
 * \param [in] input_text Pointer to where the input received is stored.
 * \param [in] output_text Pointer to where to store the text to be printed.
 * \return 0 on success, an error number on failure.
 */
uint16_t eval(uint16_t * input_text, uint16_t * output_text) {
    return 0;
}


/**
 * \brief Prints some code output.
 * \param [in] output_text Pointer to where the text to be printed is stored.
 * \return 0 on success, an error number on failure.
 */
uint16_t print(uint16_t * output_text) {
    return 0;
}


/**
 * \brief Produces the main REPL behavior of the interpreter.
 * \return 0 on success, another integer on failure.
 */
int main() {
    xpd_puts("Hello, World!");

    uint16_t input[MAX_INPUT_LEN] = "";
    uint16_t output[MAX_OUTPUT_LEN] = "";
    uint16_t return_code = 0;
    // REPL: Read-Eval-Print-Loop
    while (true) {
        // memset(input, NULL, sizeof(input));
        // memset(output, NULL, sizeof(output));

        // read in user input (command / code)
        if (return_code = read(&input)) {
            xpd_puts("FATAL: error ");
            xpd_echo_int(return_code, XPD_Flag_UnsignedDecimal);
            xpd_puts(" in read()\n");
            break;
        }

        // evaluate and execute input received
        if (return_code = eval(&input, &output);) {
            xpd_puts("FATAL: error ");
            xpd_echo_int(return_code, XPD_Flag_UnsignedDecimal);
            xpd_puts(" in eval()\n");
            break;
        }
        
        // display the correct output from this input
        if (return_code = print(&output);) {
            xpd_puts("FATAL: error ");
            xpd_echo_int(return_code, XPD_Flag_UnsignedDecimal);
            xpd_puts(" in print()\n");
            break;
        }

        // continue looping in this cycle
    }
    return 0;
}
