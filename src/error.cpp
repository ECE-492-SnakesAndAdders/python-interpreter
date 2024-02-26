/*********************************************************************************
* Description: Error reporting and handling functions for the interpreter files
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>


/** The main error types are the following:
    https://docs.python.org/3/library/exceptions.html
        AssertionError (raised when assert fails)
        IndexError (raised when index is out of range)
        NameError (raised when variable name is not found)
        NotImplementedError (raised when non-existent feature is used)
        SyntaxError (raised when lexer/parser finds a syntax problem)
        IndentationError (raised when some problem occurs with improper indentation)
        TypeError (raised when an operand of an invalid type is used)
        RuntimeError (raised when some other error occurs)
*/


/**
 * \brief Prints error information for the user.
 * \param [in] type The type of error that occurred.
 * \param [in] message The specific error information to share.
 */
void report_error(const char * type, const char * message) {
    // standard Python error reporting template, support details later
    xpd_puts("Traceback (most recent call last):\n");
    xpd_puts("  File \"<stdin>\", line 1, in <module>\n");
    // display error info shared from interpreter in useful format
    xpd_puts(type);
    xpd_puts(": ");
    xpd_puts(message);
    xpd_putc('\n');
}