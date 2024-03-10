/*********************************************************************************
* Description: Defines the environment structure used to implement variable scope
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


#include <XPD.h>
#include "expr.h"


// the maximum number of variables that may exist in a given scope
#ifndef MAX_NUM_VAR
#define MAX_NUM_VAR 64
#endif

// the maximum number of characters in any identifier
#ifndef MAX_IDENTIFIER_LEN
#define MAX_IDENTIFIER_LEN 32
#endif


/** 
 * \brief The internal representation of the set of variables in existence.
 */
struct environment {
    // stores the names of each variable
    char names[MAX_NUM_VAR][MAX_IDENTIFIER_LEN];
    // stores the actual corresponding values of these variables
    literal_value values[MAX_NUM_VAR];
    // tracks the number of variables being used (where to insert a new one)
    uint16_t num_used = 0;
    // points to the parent scope for nested variable scopes
    environment * parent = NULL;
};


// assigns a value to a variable (either existing or new)
void write_variable(environment * env, char name[], literal_value value);
// fetches the associated value from a variable
uint16_t read_variable(environment * env, char name[], literal_value * value);


#endif
