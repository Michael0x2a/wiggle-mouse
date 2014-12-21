#ifndef WIGGLE_MOUSE_UTILS_H_
#define WIGGLE_MOUSE_UTILS_H_

#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>

// Splits the string by the provided delimiter, and places 
// each piece into the provided vector.
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

// Splits the string by the provided delimiter, returning a 
// new vector containing each piece.
std::vector<std::string> split(const std::string &s, char delim);

// Returns a new version of the string with all whitespace trimmed
// from the left.
std::string &trim_left(std::string &s);

// Returns a new version of the string with all whitespace trimmed
// from the right.
std::string &trim_right(std::string &s);

// Returns a new version of the string with all whitespace trimmed
// from the left and the right.
std::string &trim(std::string &s);

#endif