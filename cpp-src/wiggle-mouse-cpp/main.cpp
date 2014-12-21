#include "main.h"

void set_mouse_coords(int x_coord, int y_coord) {
	mouse_event(
		MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE,
		x_coord * 65535 / GetSystemMetrics(0),
		y_coord * 65535 / GetSystemMetrics(1),
		0,
		0);
}

void swipe_mouse(int distance, double delay) {
	POINT original_pos;
	GetCursorPos(&original_pos);
	for (int i = 0; i < distance; i++) {
		set_mouse_coords(original_pos.x + i, original_pos.y);
		Sleep((int)(delay * 1000));
	}

	SetCursorPos(original_pos.x, original_pos.y);
}

void wait_to_swipe(double delta, double delay) {
	int reawakeTime = (int)(std::time(nullptr) + delta);
	POINT original_pos;
	GetCursorPos(&original_pos);

	while (std::time(nullptr) < reawakeTime) {
		POINT current_pos;
		GetCursorPos(&current_pos);
		if (current_pos.x != original_pos.x || current_pos.y != original_pos.y) {
			reawakeTime = (int)(std::time(nullptr) + delta);
			original_pos = current_pos;
			std::cout << "Moved!" << std::endl;
		}
		Sleep((int)(delay * 1000));
	}
}

int main() {
	try {
		ensure_file_exists(CONFIG_FILENAME);
		CleanConfig config = get_config(parse_config_file(CONFIG_FILENAME));

		std::cout << "Starting mouse wiggle." << std::endl
			<< "Hit Ctrl+C or close this window to stop." << std::endl
			<< std::endl;

		while (true) {
			std::cout << "Starting swipe." << std::endl;
			swipe_mouse(
				config.swipe_pixel_distance,
				config.swipe_time_delay);

			std::cout << "Waiting..." << std::endl;
			wait_to_swipe(
				config.time_between_swipes,
				config.user_movement_recheck_delay);

			std::cout << std::endl;
		}
	}
	catch (ParseException ex) {
		std::cout << "Encountered error in config file!" << std::endl
			<< ex.what() << std::endl
			<< std::endl
			<< "Press [Ctrl-C] to exit" << std::endl;
		int temp;
		std::cin >> temp;
	}

	return 0;
}