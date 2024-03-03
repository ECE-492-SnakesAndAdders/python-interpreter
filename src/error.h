/*********************************************************************************
* Description: Error reporting and handling functions for the interpreter files
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef ERROR_H
#define ERROR_H


/**
 * \brief The list of all possible error classes in Python.
 */
enum error_types {
    ASSERTION, INDEX, NAME, NOTIMPLEMENTED, SYNTAX, INDENTATION, TYPE, RUNTIME
};


// for ease of printing
const char * const error_names[] = {
    "AssertionError", "IndexError", "NameError", "NotImplementedError", "SyntaxError", "IndentationError", "TypeError", "RuntimeError"
};


// prints error information for the user
void report_error(error_types type, const char * message);
// prints information about an internal unexpected interpreter failure
void report_failure(const char * message);


#endif
