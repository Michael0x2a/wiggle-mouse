#include "config.h"

void ensure_file_exists(const std::string &filename) {
	std::ifstream fin(filename);
	if (fin.fail()) {
		std::ofstream config_file;
		config_file.open(filename);
		config_file << DEFAULT_CONFIG_FILE;
		config_file.close();

		std::cout << "Error! Unable to find config file. "
			<< "Creating new one at `" + filename + "`." << std::endl;
	}
}

RawConfig parse_config_file(const std::string &filename) throw (ParseException) {
	RawConfig output;

	std::string raw_line;
	std::ifstream stream(filename);
	if (stream.is_open()) {
		while (std::getline(stream, raw_line)) {
			std::vector<std::string> components = split(raw_line, '#');
			if (components.size() == 0) {
				continue;
			}
			std::string line = trim(components[0]);
			if (line.empty()) {
				continue;
			}
			std::vector<std::string> items = split(line, '=');
			if (items.size() != 2) {
				throw ParseException(
					"The line `" + raw_line + "` is malformed.\n"
					"It should take the form `KEY = VALUE`.");
			}

			std::string key = items[0];
			std::string value = items[1];
			output[trim_right(key)] = trim_left(value);
		}
		stream.close();
	}

	return output;
}

double attempt_conversion(RawConfig raw_data, std::string key) throw (ParseException) {
	if (raw_data.count(key) == 0) {
		throw ParseException(
			"Unable to find `" + key + "`.\n"
			"Consider deleting the config file -- a new one with correct\n"
			"default values will be auto-generated.");
	}

	std::string value = raw_data[key];
	std::istringstream stream(value);
	double num;
	stream >> std::noskipws >> num;
	if (!stream.eof() || stream.fail()) {
		throw ParseException("`" + value + "` is not a valid number.");
	}

	if (num <= 0) {
		throw ParseException("Value for `" + key + "` must be greater than zero.");
	}

	return num;
}

CleanConfig get_config(RawConfig raw_data) throw (ParseException) {
	double time_between_swipes = attempt_conversion(
		raw_data,
		"time_between_mouse_movement");
	double movement_recheck_delay = attempt_conversion(
		raw_data,
		"time_between_user_movement_check");
	double swipe_pixel_distance = attempt_conversion(
		raw_data,
		"distance_mouse_moves");
	double swipe_time = attempt_conversion(
		raw_data,
		"time_mouse_spends_moving");

	if (time_between_swipes <= movement_recheck_delay) {
		throw ParseException(
			"Value for `time_between_mouse_movement`\n"
			"must be less than `time_between_user_movement_check`.");
	}

	CleanConfig output = {};
	output.swipe_time_delay = swipe_time / swipe_pixel_distance;
	output.swipe_pixel_distance = (int) swipe_pixel_distance;
	output.user_movement_recheck_delay = movement_recheck_delay;
	output.time_between_swipes = time_between_swipes;

	return output;
}
