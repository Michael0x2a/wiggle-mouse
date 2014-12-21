#ifndef WIGGLE_MOUSE_CONFIG_H_
#define WIGGLE_MOUSE_CONFIG_H_

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <locale>
#include <exception>

#include "utils.h"

using RawConfig = std::map<std::string, std::string>;

struct CleanConfig {
	double swipe_time_delay;
	int swipe_pixel_distance;
	double user_movement_recheck_delay;
	double time_between_swipes;
};

const std::string CONFIG_FILENAME = "wiggle_mouse_config.txt";

const std::string DEFAULT_CONFIG_FILE = "# Wiggle mouse config\n"
"# Any text following the hash mark is a comment and will be ignored.\n"
"# Blank lines are also ignored.\n"
"# If you make any changes to this config file, restart the program.\n"
"\n"
"# All times are in seconds\n"
"time_between_mouse_movement = 5.0\n"
"time_mouse_spends_moving = 1.0\n"
"time_between_user_movement_check = 0.5  # Checks every half - second to see if\n"
"                                        # the mouse has moved.\n"
"\n"
"# All distances are in pixels\n"
"distance_mouse_moves = 40\n";

class ParseException : public std::runtime_error
{
public:
	ParseException(std::string const &s) : std::runtime_error(s) { }
};

void ensure_file_exists(const std::string &filename);

RawConfig parse_config_file(const std::string &filename) throw (ParseException);

double attempt_conversion(RawConfig raw_data, std::string key) throw (ParseException);

CleanConfig get_config(RawConfig raw_data) throw (ParseException);

#endif