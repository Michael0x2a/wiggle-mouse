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

// A raw key-value mapping of entries in the config file.
using RawConfig = std::map<std::string, std::string>;

// A struct containing all relevant values from the config file.
struct CleanConfig {
	double swipe_time_delay;
	int swipe_pixel_distance;
	double user_movement_recheck_delay;
	double time_between_swipes;
};

// Default name of the config file.
const std::string CONFIG_FILENAME = "wiggle_mouse_config.txt";

// Default text to insert into the config file.
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

// A varient of `std::runtime_error` which is raised whenever an error is encountered
// while parsing the config file.
class ParseException : public std::runtime_error
{
public:
	ParseException(std::string const &s) : std::runtime_error(s) { }
};

// If the config file does not exist, creates one at the given filename.
void ensure_file_exists(const std::string &filename);

// Opens the given file (which is assumed to exist) and extracts the data from it.
RawConfig parse_config_file(const std::string &filename) throw (ParseException);

// Attempts to extract the provided key from the raw config object, and throws a 
// ParseException if it does not exist.
double attempt_conversion(RawConfig raw_data, std::string key) throw (ParseException);

// Converts a RawConfig object into a CleanConfig object after running sanity checks
// and cleaning the data.
CleanConfig get_config(RawConfig raw_data) throw (ParseException);

#endif