/*********************************************************************************
* Description: Some helper functions for the main Python interpreter file
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef UTILITY_H
#define UTILITY_H


// for checking if a character is a number
bool isdigit(char character);
// for checking if a character is a letter
bool isalpha(char character);
// for checking if a character is a number or letter
bool isalphanumeric(char character);
// for checking if a string is made of only letters and/or numbers
bool isalphanumeric(char * str);
// for converting between numerical strings and integer values
int stoi(char ** num_str, int str_len);
void itos(char ** num_str, int num_value);


#endif
