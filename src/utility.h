/*********************************************************************************
* Description: Some helper functions for the main Python interpreter file
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef UTILITY_H
#define UTILITY_H

// for emptying arrays between runs
void memclear(void * array, uint16_t length);
// for checking if a character is a number
bool isdigit(char character);
// for checking if a character is a letter
bool isalpha(char character);
// for checking if a character is a number or letter
bool isalphanumeric(char character);
// for chceking if two strings matchs
bool strcmp(const char * s1, const char * s2);
// for converting a numerical string to an integer value
uint16_t stoi(char ** num_str, uint16_t str_len);

#endif
