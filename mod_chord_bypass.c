#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// #define _DEBUG

enum key_names {
   kn_capslock = 58,
   kn_special = 86, // right above left strg!,
   kn_function = 143, // function key on this thinkpad
   kn_world = 172,
   kn_calc = 140,
   kn_backspace = 14,
   kn_tab = 15,
   kn_left_shift = 42,
   kn_tick = 1,
   kn_1 = 2,
   kn_2 = 3,
   kn_3 = 4,
   kn_4 = 5,
   kn_5 = 6,
   kn_6 = 7,
   kn_7 = 8,
   kn_8 = 9,
   kn_9 = 10,
   kn_0 = 11,
   kn_minus = 12,
   kn_equals = 13,
   kn_Q = 16,
   kn_W = 17,
   kn_E = 18,
   kn_R = 19,
   kn_T = 20,
   kn_Y = 21,
   kn_U = 22,
   kn_I = 23,
   kn_O = 24,
   kn_P = 25,
   kn_bracket_open = 26,
   kn_bracket_close = 27,
   kn_A = 30,
   kn_S = 31,
   kn_D = 32,
   kn_F = 33,
   kn_G = 34,
   kn_H = 35,
   kn_J = 36,
   kn_K = 37,
   kn_L = 38,
   kn_semi_colon = 39,
   kn_quote_single = 40,
   kn_down_slash = 41,
   kn_Z = 44,
   kn_X = 45,
   kn_C = 46,
   kn_V = 47,
   kn_B = 48,
   kn_N = 49,
   kn_M = 50,
   kn_comma = 51,
   kn_dot = 52,
   kn_up_slash = 53
};

struct ChordMap
{
   char* const system_command;
   unsigned short mod_key[256]; // which mod keys need to be pressed
   unsigned mod_key_count;      // the amount of mod keys pressed, which should be at least 1 (i.e. the size of mod_key
   unsigned short terminal_key; // the key which triggers the command
   bool on_release;
   //std::string system_command;
};

#ifdef _DEBUG
static const char *const evval[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"
};
#endif

// --- Config ------------------------------------------------------------- >>>

// the place where the typey thing lives
const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";

// a list of all mod keys you plan on using (within mochby)
const unsigned short mod_keys[1] = { kn_capslock }; // list of all mod keys used in any Keymap struct.
//bool mod_key_state[1]; // state of each mod_key,

static const unsigned chord_map_count = 3;
const struct ChordMap cm[chord_map_count] = {
   {
      "i3-msg 'workspace prev'", // command
      { kn_capslock },           // mod keys
      1,                         // mod key count
      kn_Q,                      // terminal key q
      false                      // on release (does nothing in the moment)
   },
   {
      "i3-msg 'workspace next'", // command
      { kn_capslock },           // mod keys
      1,                         // mod key count
      kn_D,                      // terminal key
      false                      // on release (does nothing in the moment)
   },
   {
      "xdotool key 'Escape'",    // command. Holy shit, that actually works fine
      { kn_capslock },           // mod keys
      1,                         // mod key count
      kn_F,                      // terminal key
      false                      // on release (does nothing in the moment)
   }
};

// <<< ------------------------------------------------------------- Config ---



int main(void)
{
   int mod_key_count = sizeof(mod_keys);
   bool mod_key_state[255];

   const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
   struct input_event inputEvent;
   ssize_t n;
   int raw;
   raw = open( dev, O_RDONLY );
   if( raw == -1 )
   {
#ifdef _STDIO
      fprintf( stderr, "Cannot open %s: %s.\n", dev, strerror(errno) );
#endif
      return EXIT_FAILURE;
   }

   LOOP: do
   {
      n = read( raw, &inputEvent, sizeof inputEvent );

      if( n == (ssize_t) - 1 )
      {
         if (errno == EINTR) continue;
         else break;
      }
      else if( n != sizeof inputEvent )
      {
         errno = EIO;
      }

      if( EV_KEY == inputEvent.type && (0 < inputEvent.value) && (2 > inputEvent.value) )
      {
#ifdef _DEBUG
         printf("%s 0x%04x (%d), ev.value:%d\n", evval[ev.value], (int)ev.code, (int)ev.code, ev.value);
#endif
         for( int k = 0; k < mod_key_count; ++k )
         {
            if( inputEvent.code == mod_keys[k] )
            {
               
               switch(inputEvent.value)
               {
                  case 0: mod_key_state[inputEvent.code] = false; break;
                  case 1: mod_key_state[inputEvent.code] = true;
                  // case 2:  break;
               }
               goto LOOP;
            }
         }
         // printf("chord_map_count: %d", chord_map_count);
         for( int k = 0; k < chord_map_count; ++k )
         {
            if( inputEvent.code == cm[k].terminal_key )
            {
               //check if mandatory mod keys are supressed, and no other known mod keys are suppressed.
               if( inputEvent.value == 1 )
               {
                  for( int j = 0; j < cm[k].mod_key_count; ++j )
                     if( !mod_key_state[cm[k].mod_key[j]] )
                        goto JOMAMA;
                  system( cm[k].system_command );
               }
               JOMAMA: continue;
            }
         }
      }
   }while(1);

#ifdef _STDIO
   fflush(stdout);
   fprintf(stderr, "%s.\n", strerror(errno));
#endif

   return EXIT_FAILURE;
}
