#ifndef _MOCHBY_CONFIG_
#define _MOCHBY_CONFIG_
#include <array>
#include <linux/input-event-codes.h>

static const char* transpileTime = "2022-05-14 03:44:51";
static const char* configVersion = "1.54.220514_0344.cb1142f";

// a list of all mod keys you plan on using (within mochby)
static const unsigned short mod_keys[] = {
   KEY_CAPSLOCK,
   KEY_A,
   KEY_S,
   KEY_LEFTSHIFT
};

enum KeyPressEventTypes {
   keyReleased = 0,
   keyPressed = 1,
   keyHeld = 2
};

struct ChordMap
{
   char* const system_command;
   unsigned short mod_key[256] = {0}; // which mod keys need to be pressed
   unsigned mod_key_count;            // the amount of mod keys pressed, which should be at least 1 (i.e. the size of mod_key)
   unsigned short terminal_key;       // the key which triggers the command
   signed int trigger = keyPressed;
   bool exclusive = false;
   unsigned short virtual_keys[256] = {0}; // which virtual keys should be pressed
   unsigned virtual_key_count = 0;            // the amount of mod keys pressed, which should be at least 1 (i.e. the size of mod_key)
};

static const char* const evval[3] = {
   "RELEASED",
   "PRESSED ",
   "REPEATED"
};

static const std::array<int,  10> virtualKeys = {
   KEY_LEFTCTRL,
   KEY_DELETE,
   KEY_LEFTSHIFT,
   KEY_HOME,
   KEY_END,
   KEY_LEFT,
   KEY_RIGHT,
   KEY_ESC,
   KEY_PAGEUP,
   KEY_PAGEDOWN
};

// TODO change to vector
static const std::array<ChordMap, 25> cm = {{
   { "su -P -c \"export DISPLAY=:0;rofi -show window\" ottoma" , {KEY_CAPSLOCK}, 1, KEY_G, keyPressed, true },
   { nullptr, {KEY_CAPSLOCK, KEY_A}, 2, KEY_O, keyPressed, true, {KEY_LEFTCTRL, KEY_DELETE}, 2 },
   { nullptr, {KEY_CAPSLOCK, KEY_A}, 2, KEY_X, keyPressed, true, {KEY_LEFTSHIFT, KEY_HOME}, 2 },
   { nullptr, {KEY_CAPSLOCK, KEY_A}, 2, KEY_C, keyPressed, true, {KEY_LEFTSHIFT, KEY_END}, 2 },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_X, keyPressed, true, {KEY_HOME}, 1 },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_C, keyPressed, true, {KEY_END}, 1 },
   { "su -P -c \"export DISPLAY=:0;xdotool type 'Ö'\" ottoma" , {KEY_CAPSLOCK, KEY_S}, 2, KEY_9, keyPressed, true },
   { "su -P -c \"export DISPLAY=:0;xdotool type 'ö'\" ottoma" , {KEY_CAPSLOCK}, 1, KEY_9, keyPressed, true },
   { "su -P -c \"export DISPLAY=:0;xdotool type 'Ü'\" ottoma" , {KEY_CAPSLOCK, KEY_S}, 2, KEY_7, keyPressed, true },
   { "su -P -c \"export DISPLAY=:0;xdotool type 'ü'\" ottoma" , {KEY_CAPSLOCK}, 1, KEY_7, keyPressed, true },
   { "su -P -c \"export DISPLAY=:0;xdotool type 'Ä'\" ottoma" , {KEY_CAPSLOCK, KEY_S}, 2, KEY_8, keyPressed, true },
   { "su -P -c \"export DISPLAY=:0;xdotool type 'ä'\" ottoma" , {KEY_CAPSLOCK}, 1, KEY_8, keyPressed, true },
   { nullptr, {KEY_CAPSLOCK, KEY_LEFTSHIFT, KEY_A}, 3, KEY_J, keyPressed, false, {KEY_LEFTCTRL, KEY_LEFTSHIFT, KEY_LEFT}, 3 },
   { nullptr, {KEY_CAPSLOCK, KEY_LEFTSHIFT, KEY_A}, 3, KEY_L, keyPressed, false, {KEY_LEFTCTRL, KEY_LEFTSHIFT, KEY_RIGHT}, 3 },
   { "su -P -c \"export DISPLAY=:0;xdotool key 'BackSpace'; xdotool type '_'\" ottoma" , {KEY_CAPSLOCK}, 1, KEY_SPACE, keyPressed, false },
   { nullptr, {KEY_CAPSLOCK, KEY_A}, 2, KEY_J, keyPressed, false, {KEY_LEFTCTRL, KEY_LEFT}, 2 },
   { nullptr, {KEY_CAPSLOCK, KEY_A}, 2, KEY_L, keyPressed, false, {KEY_LEFTCTRL, KEY_RIGHT}, 2 },
   { "su -P -c \"export DISPLAY=:0;i3-msg 'workspace prev'\" ottoma" , {KEY_CAPSLOCK}, 1, KEY_Q, keyPressed, false },
   { "su -P -c \"export DISPLAY=:0;i3-msg 'workspace next'\" ottoma" , {KEY_CAPSLOCK}, 1, KEY_D, keyPressed, false },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_F, keyPressed, false, {KEY_ESC}, 1 },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_U, keyPressed, false, {KEY_PAGEUP}, 1 },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_H, keyPressed, false, {KEY_PAGEDOWN}, 1 },
   { "su -P -c \"export DISPLAY=:0;i3-msg focus down\" ottoma" , {KEY_CAPSLOCK}, 1, KEY_H, keyPressed, false },
   { "su -P -c \"export DISPLAY=:0;/home/ottoma/code/projects/i3-scripts/i3_cycle_windows.rb\" ottoma" , {KEY_CAPSLOCK}, 1, KEY_N, keyPressed, false },
   { "su -P -c \"export DISPLAY=:0;/home/ottoma/code/projects/i3-scripts/i3_open_term_in_empty_workspace.rb\" ottoma" , {KEY_CAPSLOCK, KEY_A}, 2, KEY_H, keyPressed, false }
}};

constexpr std::size_t chord_map_count = sizeof(cm) / sizeof(ChordMap);
constexpr std::size_t mod_key_count = sizeof(mod_keys) / sizeof(unsigned short);

#endif
