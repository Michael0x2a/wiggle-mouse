#ifndef WIGGLE_MOUSE_MAIN_H_
#define WIGGLE_MOUSE_MAIN_H_

#include <Windows.h>
#include <WinBase.h>
#include <iostream>
#include <ctime>

#include "utils.h"
#include "config.h"

void set_mouse_coords(int x_coord, int y_coord);

void swipe_mouse(int distance, double delay);

void wait_to_swipe(double delta, double delay);

int main();

#endif