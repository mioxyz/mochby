#ifndef _MOCHBY_CONFIG_
#define _MOCHBY_CONFIG_
#include <array>
#include <linux/input-event-codes.h>

static const char* transpileTime = /*%__TRANSPILE_TIME__%*/"";
static const char* configVersion = /*%__CONFIG_VERSION__%*/"";

// a list of all mod keys you plan on using (within mochby)
static const unsigned short mod_keys[] = {
   /*%__MOD_KEYS_BODY__%*/
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
