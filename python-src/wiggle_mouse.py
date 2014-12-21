#!/usr/bin/env python
'''
# wiggle_mouse

Wiggles the mouse every set number of seconds.

Configureable via `wiggle_mouse_config.txt`.
'''
from __future__ import print_function
import win32api
import win32con
import time
import os.path

CONFIG_FILENAME = 'wiggle_mouse_config.txt'

DEFAULT_CONFIG_FILE = '''# Wiggle mouse config
# Any text following the hash mark is a comment and will be ignored.
# Blank lines are also ignored.
# If you make any changes to this config file, restart the program.

# All times are in seconds
time_between_mouse_movement = 5.0
time_mouse_spends_moving = 1.0
time_between_user_movement_check = 0.5  # Checks every half-second to see if 
                                        # the mouse has moved.

# All distances are in pixels
distance_mouse_moves = 40
'''


class ParseException(Exception):
    '''An exception thrown whenever an error is encountered with the
    config file.'''
    pass


def ensure_file_exists(filename):
    '''If `filename` does not exists, creates a new config file at that
    location.'''
    if not os.path.isfile(filename):
        with open(filename, 'w') as stream:
            stream.write(DEFAULT_CONFIG_FILE)
            print('Error! Unable to find config file. '
                  'Creating new one at `{0}`'.format(filename))


def parse_config_file(filename):
    '''Parses the config file, return a dictionary mapping keys to values.'''
    output = {}
    with open(filename) as stream:
        for raw_line in stream:
            line, _, _ = raw_line.partition('#')
            line = line.strip()
            if len(line) == 0:
                continue
            key, _, value = line.partition('=')
            if key.strip() == '' or value.strip() == '':
                raise ParseException(
                    'The line `{0}` is malformed.\n'.format(raw_line) +
                    'It should take the form `KEY = VALUE`')
            output[key.strip()] = value.strip()
    return output


def attempt_conversion(raw_config_data, key, func):
    '''Attempts to retrieved the desired key from the raw config file dict
    and convert it using the provided func. Will raise a ParseException
    upon error.'''
    try:
        return func(raw_config_data[key])
    except KeyError as ex:
        raise ParseException(
            'Unable to find `{0}`.\n'.format(key) + 
            'Consider deleting the config file -- a new one with correct\n' +
            'default values will be auto-generated.')
    except ValueError as ex:
        raise ParseException(
            '`{0}` is not a valid number.'.format(raw_config_data[key]))
    except Exception as ex:
        raise ParseException(
            'Encountered unknown error with `{0}`'.format(key))


def get_config(raw_config_data):
    '''Takes in the raw config data and returns a sanitized version. Will
    raise a ParseException upon error.'''
    def attempt(key, func):
        output = attempt_conversion(raw_config_data, key, func)
        if output <= 0:
            raise ParseException('Value for `{0}` must be greater than zero.')
        return output;
        
    time_between_swipes = attempt('time_between_mouse_movement', float)
    movement_recheck_delay = attempt('time_between_user_movement_check', float)
    swipe_pixel_distance = attempt('distance_mouse_moves', int)
    swipe_time = attempt('time_mouse_spends_moving', float)

    if time_between_swipes <= movement_recheck_delay:
        raise ParseException('Value for `{0}`\n must be less then `{1}`.'.format(
            'time_between_mouse_movement',
            'time_between_user_movement_check'))
    return {
        'swipe_time_delay': swipe_time / swipe_pixel_distance,
        'swipe_pixel_distance': swipe_pixel_distance,
        'user_movement_recheck_delay': movement_recheck_delay,
        'time_between_swipes': time_between_swipes
    }


def set_mouse_coords(x_coord, y_coord):
    '''Sets the mouse to the given x/y coordinate pair.'''
    # Using `win32api.SetCursorPos(...)` didn't trigger the mouse event
    # needed to prevent the screensaver from starting.
    win32api.mouse_event(
        win32con.MOUSEEVENTF_MOVE | win32con.MOUSEEVENTF_ABSOLUTE,
        x_coord * 65535 / win32api.GetSystemMetrics(0),
        y_coord * 65535 / win32api.GetSystemMetrics(1)
    )


def swipe_mouse(distance, delay):
    '''Swipes the mouse the given distance in pixel with `delay` seconds
    between each single-pixel movement.'''
    x_coord, y_coord = original_pos = win32api.GetCursorPos()
    for i in range(distance):
        set_mouse_coords(x_coord + i, y_coord)
        time.sleep(delay)

    win32api.SetCursorPos(original_pos)


def wait_to_swipe(delta, delay):
    '''Waits for `delta` seconds with no user movement. Checks for user input
    every `delay` seconds.'''
    reawake_time = time.time() + delta
    original_pos = win32api.GetCursorPos()

    while time.time() < reawake_time:
        current_pos = win32api.GetCursorPos()
        if current_pos != original_pos:
            reawake_time = time.time() + delta
            original_pos = current_pos
            print('Moved!')
        time.sleep(delay)


def main():
    '''The main entry point of the program.'''
    print('')
    try:
        ensure_file_exists(CONFIG_FILENAME)
        config = get_config(parse_config_file(CONFIG_FILENAME))

        print('Starting mouse wiggle.')
        print('Hit Ctrl+C or close this window to stop.')
        print('')

        while True:
            print('Starting swipe.')
            swipe_mouse(
                config['swipe_pixel_distance'],
                config['swipe_time_delay'])

            print('Waiting...')
            wait_to_swipe(
                config['time_between_swipes'],
                config['user_movement_recheck_delay'])

            print('')
    except KeyboardInterrupt:
        pass
    except ParseException as ex:
        print('Encountered error in config file!')
        print(ex)
        print('')
        raw_input('Press [Enter] to exit ')


if __name__ == '__main__':
    main()
