#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <iostream>
#include <unistd.h>
#include "key_names.h"
#include "config.cpp"

#define DEBUG_PRINT

int main(int argc, char** argv )
{
   char* dev = argv[1];

   bool mod_key_state[255] = {false};

   struct input_event inputEvent;
   ssize_t n;
   int raw;
   raw = open(dev, O_RDONLY);

   if (raw == -1)
   {
      fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
      return EXIT_FAILURE;
   }

LOOP:
   do {
      n = read(raw, &inputEvent, sizeof inputEvent);
      if (n == (ssize_t)-1)
      {
         if (errno == EINTR)
            continue;
         else
            break;
      }
      else if (n != sizeof inputEvent)
      {
         errno = EIO;
      }

      if((EV_KEY == inputEvent.type) && (0 <= inputEvent.value) && (inputEvent.value <= 2)) {
         for(int k = 0; k < mod_key_count; ++k)
         {
            if(mod_keys[k] == inputEvent.code)
            {
               if(2 == inputEvent.value) goto LOOP;
               mod_key_state[mod_keys[k]] = (1 == inputEvent.value) ? true : false;
               //print modkeys
#ifdef DEBUG_PRINT
               fprintf(stderr, "=====================\n");
               for(auto& key : mod_keys)
                  if(2 != inputEvent.value)
                     fprintf(stderr, "key: %d, state: %d\n", key, mod_key_state[key]);
               fprintf(stderr, "=====================\n");
#endif
               goto LOOP; // this is to shortciruit the rest of the code. continuing here
               // will only break the inner for-loop, meaning it would still traverse cm completely.
            }
         }
         for (int k = 0; k < chord_map_count; ++k) {
            if (inputEvent.code == cm[k].terminal_key)
            {
               //check if mandatory mod keys are supressed, and no other known mod keys are suppressed.
               if (inputEvent.value == cm[k].trigger)
               {
                  for (int j = 0; j <= cm[k].mod_key_count; ++j)
                  {
                     if (!mod_key_state[cm[k].mod_key[j]])
                     {
                        goto CHORD_MAP_CONTINUE;
                     }
                  }
#ifdef DEBUG_PRINT
                  fprintf(stderr, "chord matches, executing: »");
                  fprintf(stderr, cm[k].system_command);
                  fprintf(stderr, "«\n");
#endif
                  system(cm[k].system_command);
                  if(cm[k].exclusive) {
                     goto LOOP;
                  }
               }
            CHORD_MAP_CONTINUE:
               continue;
            }
         }

      }
   } while (1);

   fflush(stdout);
   fprintf(stderr, "%s.\n", strerror(errno));

   return EXIT_FAILURE;
}

