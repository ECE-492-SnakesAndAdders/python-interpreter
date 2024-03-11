/*********************************************************************************
* Description: Defines the environment structure used to implement variable scope
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include "environment.h"
#include "utility.h"


/**
 * \brief Looks up the desired variable in the stored table.
 * \param [in] env Pointer to the environment to find the variable in.
 * \param [in] name The name of the variable to find.
 * \return The index of the variable in the table if found; the next empty slot otherwise.
 */
uint16_t find_variable(environment * env, char name[]) {
    // TODO: support nesated environments with recursive search
    // check each variable being stored for a name match
    for (uint16_t i = 0; i < (env -> num_used); i++) {
        // check for a match of the current variable to a stored one
        if (strcmp(env -> names[i], name)) {
            // return the index of the variable with this name
            return i;
        }
    }
    // this return value means the variable not found
    return (env -> num_used);
}


/**
 * \brief Assigns a particular value to a variable (old or new) in the environment.
 * \param [in] env Pointer to the environment to store the variable in.
 * \param [in] name The name of the variable to save into.
 * \param [in] value The value to store in this variable.
 */
void write_variable(environment * env, char name[], literal_value value) {
    // determine if the variable exists and where to store it
    uint16_t index = find_variable(env, name);

    // if variable does not yet exist, make a new one
    if (index == (env -> num_used)) {
        // if variable does not yet exist, make a new one
        for (uint16_t i = 0; i < MAX_IDENTIFIER_LEN; i++) {
            env -> names[index][i] = name[i];
        }
        env -> values[index] = value;
        (env -> num_used)++;
    // if the variable already exists, just update its value
    } else {
        env -> values[index] = value;
    }
}


/**
 * \brief Fetches the value of a particular variable in an environment.
 * \param [in] env Pointer to the environment to read the variable from.
 * \param [in] name The name of the variable to read from.
 * \param [in] value Pointer to where to store the value held by this variable.
 * \return 0 if the operation was successful (the variable existed); 1 otherwise.
 */
uint16_t read_variable(environment * env, char name[], literal_value * value) {
    // determine if the variable exists and where it is stored
    uint16_t index = find_variable(env, name);

    // if variable does not yet exist, return sentinel error value
    if (index == (env -> num_used)) {
        return 1;
    // if the variable already exists, just fetch its value and return sentinel success value
    } else {
        *value = env -> values[index];
        return 0;
    }
}
