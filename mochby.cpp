// #include "key_names.h"
#include "config.cpp"
#include <cstring> /* memset? */
#include <errno.h>
#include <fcntl.h> /* O_WRONLY, O_NONBLOCK */
#include <iostream>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* sleep, write */

//#include "virtual_keyboard.h"

#define DEBUG_PRINT

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

int main(int argc, char** argv )
{
   int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
   struct uinput_setup usetup;
   sleep(1);
   auto errCode = ioctl(fd, UI_SET_EVBIT, EV_KEY); fprintf(stdout, "\nioctl(UI_SET_EVBIT) code: »%d«", errCode);
   ioctl(fd, UI_SET_KEYBIT, KEY_SPACE);            fprintf(stdout, "\nioctl(UI_SET_KEYBIT) code: »%d«", errCode);
   ioctl(fd, UI_SET_KEYBIT, KEY_A);                fprintf(stdout, "\nioctl(UI_SET_KEYBIT) code: »%d«", errCode);
   for(auto& key : virtualKeys) {
      ioctl(fd, UI_SET_KEYBIT, key);               fprintf(stdout, "\nioctl(UI_SET_KEYBIT) code: »%d«", errCode);
   }
   memset(&usetup, 0, sizeof(usetup));
   strcpy(usetup.name, "Mochby Keyboard");
   usetup.id.bustype = BUS_USB;
   usetup.id.vendor  = 0x0777;  // vendor
   usetup.id.product = 0x0777; // product
   ioctl(fd, UI_DEV_SETUP, &usetup);               fprintf(stdout, "\nioctl(UI_DEV_SETUP) code: »%d«", errCode);
   ioctl(fd, UI_DEV_CREATE);                       fprintf(stdout, "\nioctl(UI_DEV_CREATE) code: »%d«", errCode);

   char* dev = argv[1];
   bool mod_key_state[255] = {false};

   struct input_event inputEvent;
   ssize_t n;
   int raw;
   raw = open(dev, O_RDONLY);

   if(raw == -1)
   {
      fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
      return EXIT_FAILURE;
   }

LOOP:
   do {
      n = read(raw, &inputEvent, sizeof inputEvent);
      if(n == (ssize_t)-1)
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
// #ifdef DEBUG_PRINT
//          if(mod_key_state[KEY_CAPSLOCK] && mod_key_state[KEY_A]) fprintf(stderr,"\ncode: %d, val: %d, type: %d", inputEvent.code, inputEvent.value, inputEvent.type);
// #endif
         for(int k = 0; k < mod_key_count; ++k)
         {
            if(mod_keys[k] == inputEvent.code)
            {
               if(KeyPressEventTypes::keyHeld == inputEvent.value)
                  goto LOOP;

               mod_key_state[mod_keys[k]] = (KeyPressEventTypes::keyPressed == inputEvent.value) ? true : false;
               //print modkeys
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
               if((KeyPressEventTypes::keyReleased == inputEvent.value) && (0 < cm[k].virtual_key_count)) {
                  // ...release all virtual keys, this might cause issues, but it is unlikely
                  fprintf(stdout, "\nvirtual_key_count: %d, keys: ", cm[k].virtual_key_count);
                  for(int i = 0; i < cm[k].virtual_key_count; ++i) fprintf(stdout, " %d", cm[k].virtual_keys[i]);
                  
                  for(int i = 0; i < cm[k].virtual_key_count; ++i)
                  {
                     // ..we might want to only send keyUp events to a subset of the keys
                     fprintf(stdout, "\npressing VIRT key: %d", cm[k].virtual_keys[i]);
                     emit(fd, EV_KEY, cm[k].virtual_keys[i], 0);
                     emit(fd, EV_SYN, SYN_REPORT, 0);
                  }
               }

               //check if mandatory mod keys are supressed, and no other known mod keys are suppressed.
               if(inputEvent.value == cm[k].trigger)
               {
                  for(int j = 0; j <= cm[k].mod_key_count; ++j)
                  {
                     if(!mod_key_state[cm[k].mod_key[j]])
                     {
                        goto CHORD_MAP_CONTINUE;
                     }
                  }
#ifdef DEBUG_PRINT
                  if(nullptr != cm[k].system_command) {
                     fprintf(stderr, "\nchord matches, executing: »");
                     fprintf(stderr, "%s", cm[k].system_command);
                     fprintf(stderr, "«");
                  }
#endif

                  if(0 < cm[k].virtual_key_count) {
                     fprintf(stdout, "\ngot here vkk: %d", cm[k].virtual_key_count);
                     for(int i = 0; i < cm[k].virtual_key_count; ++i) {
                        // ..we might want to only send keyUp events to a subset of the keys
                        fprintf(stdout, "\nreleasing key: %d", cm[k].virtual_keys[i]); 
                        emit(fd, EV_KEY, cm[k].virtual_keys[i], 1);
                        emit(fd, EV_SYN, SYN_REPORT, 0);
                     }
                  }else{
                     system(cm[k].system_command);
                  }
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
