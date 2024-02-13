#ifndef _MOCHBY_CONFIG_
#define _MOCHBY_CONFIG_
#include <array>
#include <linux/input-event-codes.h>

static const char* transpileTime = /*%__TRANSPILE_TIME__%*/"";
static const char* configVersion = /*%__CONFIG_VERSION__%*/"";

// a list of all mod keys you plan on using (within mochby)

#define PASSTHROUGH_MODE
#define WINDOWS_MODE

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
#else
static const unsigned short mod_keys[] = {
   /*%__MOD_KEYS_BODY__%*/
};
#endif


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

static const std::array<int,  /*%__VIRTUAL_KEYS_COUNT__%*/0/*%_KEEP_ZERO_%*/> virtualKeys = {
   /*%__VIRTUAL_KEYS_BODY__%*/
};

// TODO change to vector
static const std::array<ChordMap, /*%__CHORD_MAP_COUNT__%*/0/*_KEEP_ZERO_*/> cm = {{
   /*%__CHORD_MAP_BODY__%*/
}};

constexpr std::size_t chord_map_count = sizeof(cm) / sizeof(ChordMap);
constexpr std::size_t mod_key_count = sizeof(mod_keys) / sizeof(unsigned short);

#endif
