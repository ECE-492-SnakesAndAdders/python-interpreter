#include <SystemClock.h>
#include <XPD.h>
#include <GPIO.h>
#include <Thread.h>
#include "main.h"


// TODO: choose how data is stored and transferred, affecting all function signatures


/**
 * \brief Produces the main REPL behavior of the interpreter.
 * \return 0 on success, another integer on failure.
 */
int main() {
    xpd_puts("Hello, World!");
    // REPL: Read-Eval-Print-Loop
    while (true) {
        // read in user input (command / code)
        read();
        // evaluate and execute input received
        eval();
        // display the correct output from this input
        print();
        // continue looping in this cycle
    }
    return 0;
}
