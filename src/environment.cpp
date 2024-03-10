/*********************************************************************************
* Description: Defines the environment structure used to implement variable scope
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include "environment.h"
#include "utility.h"


/**
 *
 */
void write_variable(environment * env, char name[], literal_value value) {
    // check if that variable is already in use
    for (uint16_t i = 0; i < env -> num_used; i++) {
        // if so, overwrite it with the new value
        if (strcmp(env -> names[i], name)) {
            env -> values[i] = value;
            return;
        }
    }

    // if variable does not yet exist, make a new one
    for (uint16_t i = 0; i < MAX_IDENTIFIER_LEN; i++) {
        env -> names[env -> num_used][i] = name[i];
    }
    env -> values[env -> num_used] = value;
    // one more variable exists to keep track of
    env -> num_used++;
}


/**
 *
 */
uint16_t read_variable(environment * env, char name[], literal_value * value) {
    for (uint16_t i = 0; i < env -> num_used; i++) {
        if (strcmp(env -> names[i], name)) {
            *value = env -> values[i];
            return 0;
        }
    }
    return 1;
}
