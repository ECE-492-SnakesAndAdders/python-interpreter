/*********************************************************************************
* Description: Helper functions implementation for the main Python interpreter file
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#include "utility.h"


// the maximum number of characters in a literal
#ifndef MAX_LIT_LEN
#define MAX_LIT_LEN 32
#endif


/**
 * \brief Determines if a particular character is a numerical digit or not, like the standard isdigit().
 * \param [in] character The character to test.
 * \return True if the character is a digit; false otherwise.
 */
bool isdigit(char character) {
    return ((character >= '0') && (character <= '9'));
}


/**
 * \brief Determines if a particular character is a letter or not, like the standard isalpha().
 * \param [in] character The character to test.
 * \return True if the character is a letter or underscore; false otherwise.
 */
bool isalpha(char character) {
    return (((character >= 'a') && (character <= 'z')) ||
            ((character >= 'A') && (character <= 'Z')) ||
            (character == '_'));
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
 * \brief Determines if a given string is made of only letters and numbers
 * \param [in] str The string to validate
 * \return True if the string only contains letters and/or numbers, False otherwise
 */
bool isalphanumeric(char * str) {
    // check if each character is either a character or number
    while (*str) {
        if (!isalphanumeric(*str)){
            return false;
        }
        (str)++;
    }
    return true;
}


/**
 * \brief Converts a string of numbers to an integer, like the standard stoi() or atoi().
 * \param [in] num_str Pointer to the string to be converted.
 * \param [in] str_len The length of the string to convert.
 * \return The numerical integer representation of the string.
 */
int stoi(char ** num_str, int str_len) {
    // iterate backwards until the number's length is found
    int i = str_len - 1;
    while (*(*num_str + i) == '\0') {
        i--;
    }
    // cumulative value of the integer
    int value = 0;
    // add up contribution from each digit in the number
    for (int j = 0; (j < i + 1) && (j < str_len); j++) {
        // multiply current number by 10, which is a decimal point shift right
        value *= 10;
        // add latest value into new ones column
        // 48 is the ASCII value of '0', so offset numbers by that for real value
        value += (*(*num_str + j) - 48);
    }
    return value;
}


/**
 * \brief Converts an integer to a string; the opposite of stoi() above.
 * \param [in] num_str Pointer to where to store the produced string.
 * \param [in] num_value The integer to convert.
 * \return The string representation of the integer.
 */
void itos(char ** num_str, int num_value) {
    // maximum number of 65535, so we need to account for 5 characters

    // the number of characters stored so far
    int length = 0;
    // 2's complement negate any number with a negative sign bit
    if (num_value >= 32768) {
        // insert negative sign and increment because there is another character
        **num_str = '-';
        length++;
        num_value = ~num_value + 1;
    }

    bool is_significant = false;
    int counter = 10000;
    int digit;
    // iteratively insert any non-zero digits into the string as their respective characters
    for (int i = 0; i < 5; i++) {
        digit = (num_value / counter) % 10;
        counter /= 10;
        if (digit || is_significant) {
            // insert digit's character and increment because there is another character
            // 48 is the ASCII value of '0', so offset numbers by that for real value
            *(*num_str + length) = digit + 48;
            length++;
            // all digits after first non-zero one are significant
            is_significant = true;
        }
    }

    // make sure that a zero value is still displayed
    if (!(**num_str)) {
        **num_str = '0';
    }
}
