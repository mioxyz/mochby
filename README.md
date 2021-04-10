# mochby
**Warning!** This application is a hobbist's (me) half-baked experiment. You shouldn't think of this as a full release. With this said, I think it might prove as a helpful proof-of-concept more apt C programmers could take take inspiration from. The application originated from one of those »'but what if I did *this*?« kinda ideas.

Gets raw keypresses from a keyboard device and executes commands if specified key-chords have been pressed. I started hacking this together last weekend as an answer to a years-old problem of mine, having to do with my window manager of choice, i3: It is not possible, as far as I know, to map »`capslock`+`any_letter`« to commands. This absolute botch of an application allows us to bypass the window manager completely, by reading directly from `/dev/path-to-keyboard-device` and proceeding to do whatever we want, which may or mayn't be a very good idea. Despite it's scariness, I do like this thing for it's zero-flips-given-naturedness.

## Setup & Install
You can configure everything in the c file. The struct array of type `ChordMap` defines keychord-to-commands. For example:
```c
const struct ChordMap cm[chord_map_count] = {
   {
      "i3-msg 'workspace prev'", // command
      { kn_capslock },           // mod keys
      1,                         // mod key count
      kn_Q,                      // terminal key q
      false                      // on release (useless)
   },
      ⋮
}
```
 Depending on how many elements you add to `ChordMap cm[⋯]`, you will need to set `chord_map_count` appropriately, namely to the amount of elements in the array.
Furthermore you might need to modify the enum `key_names` to fit your keyboard, depending if the keycodes of your hardware doesn't match up with that of mine. You can check this by running the application in debug mode by uncommenting the `#define _DEBUG` macro (and etc.)

## Things that you can do with this
1. map backspace to »`mod4`+`u`«. X11's xkb extension for one reason or another doesn't allow remapping backspace (though `Delete` is remappable), the normal `Enter` key (though the keypad enter key can be remapped to »`mod4`+`;`« for example).
2. map »`mod4`+`a`« to ctrl. This might seem a bit confusing if you haven't tried this before, but it becomes quickly unconfusing if tried in practice. Pressing mod4+a acts as if you would be pressing `mod4` and `ctrl` (and not the A-key) allowing for additional `ctrl`+`mod4`'ed keypresses to be performed. If you have mod4+l mapped to the right arrow key, then pressing »`mod4`+`a`+`l`« (in that sequence without releasing the pressed keys) will be as if pressing ctrl+→.

## Open
I would like to add a virtual keyboard output device to the mix, as to bypass xdotool (and not have to do the ugly `system("jomama");` calls) and to guarantee that the keystrokes arriving in the X11 window do not mix with the causal mod-chord. For example, if I would like to map »`mod4`+`d`« to »`Shift`+`t`« for whatever reason, I would prefer if the Window Manager and X11 would not mangle the keystrokes together. I'm not entirely sure if this makes sense, I'll have to try it out. In practice, it can't help but make sense.

## Credits
Thank goodness for [this stackoverflow post](https://stackoverflow.com/questions/20943322/accessing-keys-from-linux-input-device), it answers all of the hard (to me at least) bits and left relatively little to do. I'm a relative new-ish developer and my area of work is completely disjoint from the linux kernel land and I only recently acquired the necessary knowledge to type the right question into my favorite search engine to produce the aforementioned stackoverflow post.



