#ifndef WIGGLE_MOUSE_MAIN_H_
#define WIGGLE_MOUSE_MAIN_H_

#include <Windows.h>
#include <WinBase.h>
#include <iostream>
#include <ctime>

#include "utils.h"
#include "config.h"

// Sets the mouse position to the provided x and y coordinates.
void set_mouse_coords(int x_coord, int y_coord);

// Swipes the mouse to the right by the provided distance with 
// the given delay (in seconds) between each pixel movement.
void swipe_mouse(int distance, double delay);

// Waits for delta seconds with no user mouse movement before 
// terminating. Checks to see if the user has moved the mouse 
// every delta seconds.
void wait_to_swipe(double delta, double delay);

// The main entry point of the program.
int main();

#endif