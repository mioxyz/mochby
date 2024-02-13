#!/bin/bash
g++ -std=c++20 mochby.cpp config.cpp -o ./bin/mod_chord_bypass && ./bin/mod_chord_bypass /dev/input/by-path/platform-i8042-serio-0-event-kbd; xinput reattach 9 3;
# call `xset r rate 152 44` once mochby is running.
