#ifndef WIGGLE_MOUSE_UTILS_H_
#define WIGGLE_MOUSE_UTILS_H_

#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>

std::vector<std::string> &split(const std::string &s, char delim,
	std::vector<std::string> &elems);

std::vector<std::string> split(const std::string &s, char delim);

std::string &trim_left(std::string &s);

std::string &trim_right(std::string &s);

std::string &trim(std::string &s);

#endif