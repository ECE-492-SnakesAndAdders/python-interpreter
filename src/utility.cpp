/*********************************************************************************
* Description: Helper functions implementation for the main Python interpreter file
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>
#include "utility.h"


/**
 * \brief Zeroes out a particular region of memory for a variable.
 * \param [in] array Pointer to the array fo memory to be erased.
 * \param [in] length Integer number of elements in the array to zero out.
 */
void memclear(void * array, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        *(((char *) array) + i) = 0;
    }
}


/**
 * \brief Determines if a particular character is a numerical digit or not, like the standard isdigit().
 * \param [in] character The character to test.
 * \return True if the character is a digit; false otherwise.
 */
bool isdigit(char character) {
    return (character >= '0') && (character <= '9');
}


/**
 * \brief Determines if a particular character is a letter or not, like the standard isalpha().
 * \param [in] character The character to test.
 * \return True if the character is a letter or underscore; false otherwise.
 */
bool isalpha(char character) {
    return ((character >= 'a') && (character <= 'z')) ||
           ((character >= 'A') && (character <= 'Z')) ||
           (character == '_');
}


/**
 * \brief Determines if a particular character is a letter or digit not, like the standard isalphanumeric().
 * \param [in] character The character to test.
 * \return True if the character is a letter or digit; false otherwise.
 */
bool isalphanumeric(char character) {
    return (isalpha(character) || isdigit(character));
}


/**
 * \brief Determines if two strings are equal (look the same).
 * \param [in] s1 The first string to be compared for equality.
 * \param [in] s2 The second string to be compared for equality.
 * \return True if the two strings are equal; false otehrwise.
 * \deprecated
 */
bool strcmp(const char * s1, const char * s2) {
    // traverse down strng, ensuring it exists and checking for equality
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return !((*(char *) s1) - (*(char *) s2));
}


/**
 * \brief Converts a string of numbers to an integer, like the standard stoi() or atoi().
 * \param [in] num_str Pointer to the string to be converted.
 * \return The numerical integer representation of the string.
 */
uint16_t stoi(char ** num_str, uint16_t str_len) {
    // iterate backwards until the number's length is found
    uint16_t i = str_len - 1;
    while (*(*num_str + i) == '\0') {
        i--;
    }
    // cumulative value of the integer
    uint16_t value = 0;
    // add up contribution from each digit in the number
    for (uint16_t j = 0; (j < i + 1) && (j < str_len); j++) {
        // multiply current number by 10, which is a decimal point shift right
        value *= 10;
        // add latest value into new ones column
        // 48 is the ASCII value of '0', so offset numbers by that for real value
        value += (*(*num_str + j) - 48);
    }
    return value;
}
