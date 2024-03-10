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
void Environment::write_variable(char name[], literal_value value) {
    // check if that variable is already in use
    for (uint16_t i = 0; i < num_used; i++) {
        // if so, overwrite it with the new value
        if (strcmp(names[i], name)) {
            values[i] = value;
            return;
        }
    }

    // if variable does not yet exist, make a new one
    for (uint16_t i = 0; i < MAX_IDENTIFIER_LEN; i++) {
        names[num_used][i] = name[i];
    }
    values[num_used] = value;
    // one more variable exists to keep track of
    num_used++;
}


/**
 *
 */
uint16_t Environment::read_variable(char name[], literal_value * value) {
    for (uint16_t i = 0; i < num_used; i++) {
        if (strcmp(names[i], name)) {
            *value = values[i];
            return 0;
        }
    }
    return 1;
}
