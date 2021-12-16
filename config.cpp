#ifndef _MOCHBY_CONFIG_
#define _MOCHBY_CONFIG_
#include <array>
#include "key_names.h"

// a list of all mod keys you plan on using (within mochby)
static const unsigned short mod_keys[] = {
    kn_capslock, // primary modifier
    kn_A,
    kn_S
};

enum KeyPressEventTypes {
    keyPressed = 1,
    keyReleased = 0,
    keyHeld = 2
};

struct ChordMap
{
   char* const system_command;
   unsigned short mod_key[256] = {0}; // which mod keys need to be pressed
   unsigned mod_key_count;            // the amount of mod keys pressed, which should be at least 1 (i.e. the size of mod_key
   unsigned short terminal_key;       // the key which triggers the command
   signed int trigger = keyPressed;
   bool exclusive = false;
};

static const char* const evval[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"
};

static const std::array<ChordMap, 23> cm = {{
    { "su -P -c \"export DISPLAY=:0; xdotool type 'Ö'\" mio" , {kn_capslock, kn_S}, 1, kn_Y, keyPressed, true },
    { "su -P -c \"export DISPLAY=:0; xdotool type 'Ü'\" mio" , {kn_capslock, kn_S}, 1, kn_U, keyPressed, true },
    { "su -P -c \"export DISPLAY=:0; xdotool type 'Ä'\" mio" , {kn_capslock, kn_S}, 1, kn_H, keyPressed, true },
    { "su -P -c \"export DISPLAY=:0; xdotool type 'ü'\" mio" , {kn_capslock, kn_A}, 1, kn_U, keyPressed, true },
    { "su -P -c \"export DISPLAY=:0; xdotool type 'ö'\" mio" , {kn_capslock, kn_A}, 1, kn_Y, keyPressed, true },
    { "su -P -c \"export DISPLAY=:0; xdotool type 'ä'\" mio" , {kn_capslock, kn_A}, 1, kn_H, keyPressed, true },
    { "su -P -c \"export DISPLAY=:0; xdotool key '0xffbe'\" mio" , {kn_capslock, kn_A}, 1, kn_J, keyPressed, true },
    { "su -P -c \"export DISPLAY=:0; xdotool key '0xffc0'\" mio" , {kn_capslock, kn_A}, 1, kn_L, keyPressed, true },
    { "su -P -c \"export DISPLAY=:0; xdotool key 'Shift+Ctrl+Up'\" mio"  , {kn_capslock, kn_S}, 1, kn_I, keyPressed, true },
    { "su -P -c \"export DISPLAY=:0; xdotool key 'Shift+Ctrl+Down'\" mio", {kn_capslock, kn_S}, 1, kn_K, keyPressed, true },
    { "su -P -c \"export DISPLAY=:0; xdotool key '0xffbe'\" mio"          , {kn_capslock, kn_A}, 1, kn_J, keyHeld, true }, // this does work, it just feels like driving a bloody boat. The 50-60ms delay isn't that apparent when performing single keystrokes, but doing  keystrokes every 44 milliseconds does change things. Another reason to get the virtual UHID keyboard thing working
    { "su -P -c \"export DISPLAY=:0; xdotool key '0xffc0'\" mio"          , {kn_capslock, kn_A}, 1,        kn_L,    keyHeld, true },
    { "su -P -c \"export DISPLAY=:0; xdotool key 'ctrl+0xffc2'\" mio"     , {kn_capslock, kn_S}, 1, kn_up_slash, keyPressed, true }, //for findJump VSC-plugin
    { "su -P -c \"export DISPLAY=:0; xdotool key 'ctrl+0xffc1'\" mio"     , {kn_capslock}, 0, kn_up_slash, keyPressed, false }, 
    { "su -P -c \"export DISPLAY=:0; xdotool key 'Page_Down'\" mio"       , {kn_capslock}, 0, kn_H },
    { "su -P -c \"export DISPLAY=:0; i3-msg 'workspace prev'\" mio"       , {kn_capslock}, 0, kn_Q },
    { "su -P -c \"export DISPLAY=:0; i3-msg 'workspace next'\" mio"       , {kn_capslock}, 0, kn_D },
    { "su -P -c \"export DISPLAY=:0; xdotool key 'Escape'\" mio"          , {kn_capslock}, 0, kn_F },
    { "su -P -c \"export DISPLAY=:0; xdotool key 'Page_Up'\" mio"         , {kn_capslock}, 0, kn_U },
    { "su -P -c \"export DISPLAY=:0; xdotool key 'Page_Down'\" mio"       , {kn_capslock}, 0, kn_H },
    { "su -P -c \"export DISPLAY=:0; i3-msg focus down\" mio"             , {kn_capslock, kn_A}, 1, kn_N, keyPressed,  true },
    { "su -P -c \"export DISPLAY=:0; i3-msg focus left\" mio"             , {kn_capslock      }, 0, kn_N, keyPressed, false },
    { "su -P -c \"export DISPLAY=:0; /home/mio/code/ruby/i3_open_term_in_empty_workspace.rb\" mio", {kn_capslock}, 0, kn_T },
}};

constexpr std::size_t chord_map_count = sizeof(cm) / sizeof(ChordMap);
constexpr std::size_t mod_key_count = sizeof(mod_keys) / sizeof(unsigned short);

#endif


/*  xdotool keycodes:
    web: gitlab.com/cunidev/gestures/-/wikis/xdotool-list-of-key-codes
     F1: »0xffbe«
     F2: »0xffbf«
     F3: »0xffc0«
     F4: »0xffc1«
     F5: »0xffc2«
     F6: »0xffc3«
     F7: »0xffc4«
     F8: »0xffc5«
     F9: »0xffc6«
    F10: »0xffc7«
    F11: »0xffc8«

    slash »/«: »0x002f«
 */
