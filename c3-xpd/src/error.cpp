/*********************************************************************************
* Description: Error reporting and handling functions for the interpreter files
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#include <cstdio>
#include "error.h"
#include "utility.h"


/** The main error types are the following:
    https://docs.python.org/3/library/exceptions.html
        AssertionError (raised when assert fails)
        IndexError (raised when index is out of range)
        NameError (raised when variable name is not found)
        SyntaxError (raised when lexer/parser finds a syntax problem)
        IndentationError (raised when some problem occurs with improper indentation)
        TypeError (raised when an operand of an invalid type is used)
        RuntimeError (raised when some other error occurs)
        ZeroDivisionError (raised when there is a division by 0)
*/


/**
 * \brief Prints error information for the user.
 * \param [in] type The type of error that occurred.
 * \param [in] message The specific error information to share.
 */
void report_error(error_types type, const char * message) {
    // standard Python error reporting template, support details later
    if (type != SYNTAX) {
        printf("Traceback (most recent call last):\n");
    }
    printf("  File \"<stdin>\", line 1, in <module>\n");
    // display error info shared from interpreter in useful format
    printf(error_names[type]);
    printf(": ");
    printf(message);
    printf("\n");
}


/**
 * \brief Prints error information for the developer.
 * \param [in] message The specific information to share.
 */
void report_failure(const char * message) {
    printf("FATAL: unexpected error internal to interpreter\n  ");
    printf(message);
    printf("\n  disregard outputs and reboot the interpreter for safety\n");
}
