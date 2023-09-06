#ifndef _MOCHBY_CONFIG_
#define _MOCHBY_CONFIG_
#include <array>
#include <linux/input-event-codes.h>

static const char* transpileTime = "2023-09-02 16:58:36";
static const char* configVersion = "1.120.230902_1658.c775a31";

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

#define PASSTHROUGH_MODE 1

#ifdef PASSTHROUGH_MODE
static const std::array<int,  112> virtualKeys = {
   KEY_LEFTMETA,
   KEY_LEFTCTRL,
   KEY_DELETE,
   KEY_ESC,
   KEY_1,
   KEY_2,
   KEY_3,
   KEY_4,
   KEY_5,
   KEY_6,
   KEY_7,
   KEY_8,
   KEY_9,
   KEY_0,
   KEY_MINUS,
   KEY_EQUAL,
   KEY_BACKSPACE,
   KEY_TAB,
   KEY_Q,
   KEY_W,
   KEY_E,
   KEY_R,
   KEY_T,
   KEY_Y,
   KEY_U,
   KEY_I,
   KEY_O,
   KEY_P,
   KEY_LEFTBRACE,
   KEY_RIGHTBRACE,
   KEY_ENTER,
   KEY_LEFTCTRL,
   KEY_A,
   KEY_S,
   KEY_D,
   KEY_F,
   KEY_G,
   KEY_H,
   KEY_J,
   KEY_K,
   KEY_L,
   KEY_SEMICOLON,
   KEY_APOSTROPHE,
   KEY_GRAVE,
   KEY_LEFTSHIFT,
   KEY_BACKSLASH,
   KEY_Z,
   KEY_X,
   KEY_C,
   KEY_V,
   KEY_B,
   KEY_N,
   KEY_M,
   KEY_COMMA,
   KEY_DOT,
   KEY_SLASH,
   KEY_RIGHTSHIFT,
   KEY_KPASTERISK,
   KEY_LEFTALT,
   KEY_SPACE,
   KEY_F1,
   KEY_F2,
   KEY_F3,
   KEY_F4,
   KEY_F5,
   KEY_F6,
   KEY_F7,
   KEY_F8,
   KEY_F9,
   KEY_F10,
   KEY_NUMLOCK,
   KEY_SCROLLLOCK,
   KEY_KP7,
   KEY_KP8,
   KEY_KP9,
   KEY_KPMINUS,
   KEY_KP4,
   KEY_KP5,
   KEY_KP6,
   KEY_KPPLUS,
   KEY_KP1,
   KEY_KP2,
   KEY_KP3,
   KEY_KP0,
   KEY_KPDOT,
   KEY_ZENKAKUHANKAKU,
   KEY_102ND,
   KEY_F11,
   KEY_F12,
   KEY_RO,
   KEY_KPENTER,
   KEY_RIGHTCTRL,
   KEY_KPSLASH,
   KEY_SYSRQ,
   KEY_RIGHTALT,
   KEY_LINEFEED,
   KEY_HOME,
   KEY_UP,
   KEY_PAGEUP,
   KEY_LEFT,
   KEY_RIGHT,
   KEY_END,
   KEY_DOWN,
   KEY_PAGEDOWN,
   KEY_INSERT,
   KEY_DELETE,
   KEY_MUTE,
   KEY_VOLUMEDOWN,
   KEY_VOLUMEUP,
   KEY_KPEQUAL,
   KEY_KPPLUSMINUS,
   KEY_PAUSE
};
#elif
static const std::array<int,  12> virtualKeys = {
   KEY_LEFTMETA,
   KEY_J,
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
#endif

// TODO change to vector
static const std::array<ChordMap, 
#ifdef PASSTHROUGH_MODE
33
#elif
28
#endif
> cm = {{
   { "su -c \"export DISPLAY=:0;sleep 0.1; xdotool key 'Super+F1'\" mio" , {KEY_CAPSLOCK}, 1, KEY_F1, keyPressed, true },
   { "su -c \"export DISPLAY=:0;rofi -show window\" mio" , {KEY_CAPSLOCK}, 1, KEY_G, keyPressed, true },
   { "su -c \"export DISPLAY=:0;/home/mio/.config/i3/scripts/i3-scripts/unicode_inserter.rb\" mio" , {KEY_CAPSLOCK}, 1, KEY_P, keyPressed, false },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_TAB, keyPressed, true, {KEY_LEFTMETA, KEY_J}, 2 },
   { nullptr, {KEY_CAPSLOCK, KEY_A}, 2, KEY_O, keyPressed, true, {KEY_LEFTCTRL, KEY_DELETE}, 2 },
   { nullptr, {KEY_CAPSLOCK, KEY_A}, 2, KEY_X, keyPressed, true, {KEY_LEFTSHIFT, KEY_HOME}, 2 },
   { nullptr, {KEY_CAPSLOCK, KEY_A}, 2, KEY_C, keyPressed, true, {KEY_LEFTSHIFT, KEY_END}, 2 },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_X, keyPressed, true, {KEY_HOME}, 1 },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_C, keyPressed, true, {KEY_END}, 1 },
   { "su -c \"export DISPLAY=:0;xdotool type 'Ö'\" mio" , {KEY_CAPSLOCK, KEY_S}, 2, KEY_9, keyPressed, true },
   { "su -c \"export DISPLAY=:0;xdotool type 'ö'\" mio" , {KEY_CAPSLOCK}, 1, KEY_9, keyPressed, true },
   { "su -c \"export DISPLAY=:0;xdotool type 'Ü'\" mio" , {KEY_CAPSLOCK, KEY_S}, 2, KEY_7, keyPressed, true },
   { "su -c \"export DISPLAY=:0;xdotool type 'ü'\" mio" , {KEY_CAPSLOCK}, 1, KEY_7, keyPressed, true },
   { "su -c \"export DISPLAY=:0;xdotool type 'Ä'\" mio" , {KEY_CAPSLOCK, KEY_S}, 2, KEY_8, keyPressed, true },
   { "su -c \"export DISPLAY=:0;xdotool type 'ä'\" mio" , {KEY_CAPSLOCK}, 1, KEY_8, keyPressed, true },
   { nullptr, {KEY_CAPSLOCK, KEY_LEFTSHIFT, KEY_A}, 3, KEY_J, keyPressed, false, {KEY_LEFTCTRL, KEY_LEFTSHIFT, KEY_LEFT}, 3 },
   { nullptr, {KEY_CAPSLOCK, KEY_LEFTSHIFT, KEY_A}, 3, KEY_L, keyPressed, false, {KEY_LEFTCTRL, KEY_LEFTSHIFT, KEY_RIGHT}, 3 },
   { "su -c \"export DISPLAY=:0;xdotool key 'BackSpace'; xdotool type '_'\" mio" , {KEY_CAPSLOCK}, 1, KEY_SPACE, keyPressed, false },
   { nullptr, {KEY_CAPSLOCK, KEY_A}, 2, KEY_J, keyPressed, false, {KEY_LEFTCTRL, KEY_LEFT}, 2 },
   { nullptr, {KEY_CAPSLOCK, KEY_A}, 2, KEY_L, keyPressed, false, {KEY_LEFTCTRL, KEY_RIGHT}, 2 },
   { "su -c \"export DISPLAY=:0;i3-msg 'workspace prev'\" mio" , {KEY_CAPSLOCK}, 1, KEY_Q, keyPressed, false },
   { "su -c \"export DISPLAY=:0;i3-msg 'workspace next'\" mio" , {KEY_CAPSLOCK}, 1, KEY_D, keyPressed, false },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_F, keyPressed, false, {KEY_ESC}, 1 },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_U, keyPressed, false, {KEY_PAGEUP}, 1 },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_H, keyPressed, false, {KEY_PAGEDOWN}, 1 },
   { "su -c \"export DISPLAY=:0;i3-msg focus down\" mio" , {KEY_CAPSLOCK}, 1, KEY_H, keyPressed, false },
   { "su -c \"export DISPLAY=:0;/home/ottoma/code/projects/i3-scripts/i3_cycle_windows.rb\" mio" , {KEY_CAPSLOCK}, 1, KEY_N, keyPressed, false },
   { "su -c \"export DISPLAY=:0;/home/ottoma/code/projects/i3-scripts/i3_open_term_in_empty_workspace.rb\" mio" , {KEY_CAPSLOCK, KEY_A}, 2, KEY_H, keyPressed, false },
#ifdef PASSTHROUGH_MODE
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_J, keyPressed, true, {KEY_LEFT}, 1 },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_K, keyPressed, true, {KEY_DOWN}, 1 },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_I, keyPressed, true, {KEY_UP}, 1 },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_L, keyPressed, true, {KEY_RIGHT}, 1 },
   { nullptr, {KEY_CAPSLOCK}, 1, KEY_SEMICOLON, keyPressed, true, {KEY_ENTER}, 1 }
#endif
}};

constexpr std::size_t chord_map_count = sizeof(cm) / sizeof(ChordMap);
constexpr std::size_t mod_key_count = sizeof(mod_keys) / sizeof(unsigned short);

#endif
