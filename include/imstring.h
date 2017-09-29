#ifndef IMSTR_H
#define IMSTR_H

#include <iostream>

/**
 * Removes all leading whitespace from the input string.
 */
std::string trim_leading_whitespace(std::string str);

/**
 * Removes all characters following the comment 'delim'eter.
 */
std::string remove_comments(std::string str, char delim);

/**
 * Returns true if the string has either 0 length, or only
 * contains white space characters.
 */
bool is_string_empty(std::string str);

#endif
