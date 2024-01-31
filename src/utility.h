/*********************************************************************************
* Description: Some helper functions for the main Python interpreter file
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


/**
 * \brief Zeroes out a particular region of memory for a variable.
 * \param [in] array Pointer to the array fo memory to be erased.
 * \param [in] length Integer number of elements in the array to zero out.
 */
void memclear(char * array, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        array[i] = 0;
    }
}
