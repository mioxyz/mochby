// #include "key_names.h"
#include "config.cpp"
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// #define DEBUG_PRINT

#define die(str, args...) do { \
    perror(str); \
    exit(EXIT_FAILURE); \
} while(0)

void emit(int fd, int type, int code, int val)
{
   struct input_event ie;

   ie.type         = type;
   ie.code         = code;
   ie.value        = val;
   ie.time.tv_sec  = 0;
   ie.time.tv_usec = 0;

   write(fd, &ie, sizeof(ie));
}


int initializeUinput() {
   struct uinput_setup usetup;
   int fileHandle = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

   if(fileHandle < 0) 
      die("error: open");

   ioctl(fileHandle, UI_SET_EVBIT, EV_KEY);

   for(auto& key : virtualKeys)
      ioctl(fileHandle, UI_SET_KEYBIT, key);
   
   memset(&usetup, 0, sizeof(usetup));
   usetup.id.bustype = BUS_USB;
   usetup.id.vendor = 0x1911;
   usetup.id.product = 0x1911;
   strcpy(usetup.name, "Mod Chord Bypass");

   if(ioctl(fileHandle, UI_DEV_SETUP, &usetup) < 0) die("error: ioctl");

   int uiDevCreateReturnCode = ioctl(fileHandle, UI_DEV_CREATE);

   if(uiDevCreateReturnCode < 0)
      die("error: ioctl");

   return fileHandle;
}

int main(int argc, char** argv) {
   fprintf(stdout, "transpile time: %s,\nconfig version: %s\nmod-keys: [", transpileTime, configVersion);

   for(int k = 0; k < mod_key_count; ++k)
      fprintf(stdout, "%d, ", mod_keys[k]);

   fprintf(stdout, "] mod-key-count: »%d«", mod_key_count);

   int fileHandle_uinput = initializeUinput();

   sleep(1);

   char* devicePath = argv[1];
   bool mod_key_state[255] = {false};

   struct input_event inputEvent;
   ssize_t charactersRead;

   int fileHandle_keyboardInput = open(devicePath, O_RDONLY);

   if(fileHandle_keyboardInput == -1) {
      fprintf(stderr, "Cannot open %s: %s.\n", devicePath, strerror(errno));
      return EXIT_FAILURE;
   }

LOOP:
   do {
      charactersRead = read(fileHandle_keyboardInput, &inputEvent, sizeof inputEvent);

      if(charactersRead == (ssize_t)-1) {
         if(errno == EINTR)
            continue; else break;
      } else if (charactersRead != sizeof inputEvent) {
         errno = EIO;
      }

      if((EV_KEY == inputEvent.type) && (0 <= inputEvent.value) && (inputEvent.value <= 2)) {
         for(int k = 0; k < mod_key_count; ++k)
         {
            if(mod_keys[k] == inputEvent.code)
            {
               if(KeyPressEventTypes::keyHeld == inputEvent.value)
                  goto LOOP;

               mod_key_state[mod_keys[k]] = (KeyPressEventTypes::keyPressed == inputEvent.value) ? true : false;
#ifdef DEBUG_PRINT
               fprintf(stderr, "=====================\n");
               for(auto& key : mod_keys)
                  if(KeyPressEventTypes::keyHeld != inputEvent.value)
                     fprintf(stderr, "key: %d, state: %d\n", key, mod_key_state[key]);
               fprintf(stderr, "=====================\n");
#endif
               goto LOOP; // this is to shortciruit the rest of the code. continuing here
               // will only break the inner for-loop, meaning it would still traverse cm completely.
            }
         }
         for(int k = 0; k < chord_map_count; ++k) {
            if(inputEvent.code == cm[k].terminal_key)
            {
               if((KeyPressEventTypes::keyReleased == inputEvent.value) && (0 < cm[k].virtual_key_count))
               {
                  // ...release all virtual keys, this might cause issues, but it is unlikely
#ifdef DEBUG_PRINT
                  fprintf(stdout, "\nvirtual_key_count: %d, keys: ", cm[k].virtual_key_count);
                  for(int i = 0; i < cm[k].virtual_key_count; ++i) fprintf(stdout, " %d", cm[k].virtual_keys[i]);
#endif
                  for(int i = 0; i < cm[k].virtual_key_count; ++i)
                  {
                     // ..we might want to only send keyUp events to a subset of the keys
#ifdef DEBUG_PRINT
                     fprintf(stdout, "\n\npressing VIRT key: %d", cm[k].virtual_keys[i]);
#endif
                     emit(fileHandle_uinput, EV_KEY, cm[k].virtual_keys[i], 0);
                     emit(fileHandle_uinput, EV_SYN, SYN_REPORT, 0);
                  }
               }

               //check if mandatory mod keys are supressed, and no other known mod keys are suppressed.
               if(inputEvent.value == cm[k].trigger)
               {
                  for(int j = 0; j < cm[k].mod_key_count; ++j)
                  {
                     if(!mod_key_state[cm[k].mod_key[j]])
                     {
                        goto CHORD_MAP_CONTINUE;
                     }
                  }
                  if(0 < cm[k].virtual_key_count)
                  {
#ifdef DEBUG_PRINT
                     fprintf(stdout, "\n git into VirtKeyChord: %d", cm[k].virtual_key_count);
#endif
                     for(int i = 0; i < cm[k].virtual_key_count; ++i) {
                        // ..we might want to only send keyUp events to a subset of the keys
#ifdef DEBUG_PRINT
                        fprintf(stdout, "\nreleasing key: %d", cm[k].virtual_keys[i]);
#endif
                        emit(fileHandle_uinput, EV_KEY, cm[k].virtual_keys[i], 1);
                        emit(fileHandle_uinput, EV_SYN, SYN_REPORT, 0);
                     }
                  } else {
#ifdef DEBUG_PRINT
                        if(nullptr != cm[k].system_command) fprintf(stderr, "\nchord matches, exec SYSTEM CMD: »%s«", cm[k].system_command);
#endif
                     system(cm[k].system_command);
                  }
                  if(cm[k].exclusive)
                  {
#ifdef DEBUG_PRINT
                     fprintf(stdout, "exclusive chord, returning to LOOP");
#endif
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
