# mochby
Gets raw keypresses from a keyboard device and executes commands if specified key-chords have been pressed. I started hacking this together last weekend as an answer to a years-old problem of mine, having to do with my window manager of choice, i3: It is not possible, as far as I know, to map »`capslock`+`any_letter`« to commands. This absolute botch of an application allows us to bypass the window manager completely, by reading directly from `/dev/path-to-keyboard-device` and proceeding to do whatever we want, which may or mayn't be a very good idea. Despite it's scariness, I do like this thing for it's zero-flips-given-naturedness.

   **Warning**: last commit is dirty.

## Setup & Install
Run the `./auto/transpile_config.rb` script to generate `config.cpp`, then run `./auto/full_install.rb`. `config.json` is meant to be edited. It contains a key `chord_maps` containing chord objects:
```
{
   "chord_maps": [
      {
         "shell_command": "rofi -show window",
         "mod_keys": [ "KEY_CAPSLOCK" ],
         "terminal_key": "KEY_G",
         "exclusive": true
      }, {
         "virtual_keys": [ "KEY_LEFTCTRL", "KEY_DELETE" ],
         "mod_keys": [ "KEY_CAPSLOCK", "KEY_A" ],
         "terminal_key": "KEY_O",
         "exclusive": true
      },
    ⋮
   ]
}
```
Above are two example `chord_map`s. A `chord_map` can either have a `shell_command` or `virtual_keys` but not both. When all `mod_keys` and the `terminal_key` are depressed simultaneously, the `shell_command` is executed or the `virtual_keys` are pressed, respectively. A `chord_map` can have an `exclusive` key, which is by default `false`. When `exclusive` is set to `true`for a `chord_map`, when this particular `chord_map` is triggered, all subsequent `chord_map`s are skipped. This means that any preceding `chord_map`s that can be triggered, will trigger.

A `chord_map` can also have a `trigger` key, which is by default `keyPressed` and can alternatively be set to `keyReleased` or `keyHeld`, which will cause the `chord_map` to trigger when the *terminal key* key is released or held (the mod keys are presumed to be all pressed, if you want more complicated behaviour, I'd suggest modifying the C code directly).

`virtual_keys` and `mod_keys` are based on the `linux/input-event-codes.h` key macros.

Editing the generated `config.cpp` file directly is no longer necessary.

### Requirements
You'll need the following ruby gems: OpenSturct (`ostruct`), `colorize` and `json`. A C compiler is also necessary.

 Depending on how many elements you add to `ChordMap cm[⋯]`, you will need to set `chord_map_count` appropriately, namely to the amount of elements in the array.
Furthermore you might need to modify the enum `key_names` to fit your keyboard, depending if the keycodes of your hardware doesn't match up with that of mine. You can check this by running the application in debug mode by uncommenting the `#define _DEBUG` macro (and etc.)

## Gotchas
My keyboard allows for multiple keys to be pressed simultaneously, but certain keys become mutually exclusive, due to the hardware, for example of the three keys [capslock, A, S] if you press any of the two in this set it will register, but pressing the third key will always not register. I'm not 100% sure, though.

## Why?
1. map backspace to »`mod4`+`u`«. X11's xkb extension for one reason or another doesn't allow remapping backspace (though `Delete` is remappable) to the normal `Enter` key (though the keypad enter key can be remapped to »`mod4`+`;`« for example).

2. map »`mod4`+`a`« to ctrl. This might seem a bit confusing if you haven't tried this before, but it becomes quickly unconfusing if tried in practice. Pressing mod4+a acts as if you would be pressing `mod4` and `ctrl` (and not the A-key) allowing for additional `ctrl`+`mod4`'ed keypresses to be performed. If you have mod4+l mapped to the right arrow key, then pressing »`mod4`+`a`+`l`« (in that sequence without releasing the pressed keys) will be as if pressing ctrl+→.

## Open
- [+] I would like to add a virtual keyboard output device to the mix, as to bypass xdotool (and not have to do the ugly `system("stuff");` calls) and to guarantee that the keystrokes arriving in the X11 window do not mix with the causal mod-chord. For example, if I would like to map »`mod4`+`d`« to »`Shift`+`t`« for whatever reason, I would prefer if the Window Manager and X11 would not mangle the keystrokes together. I'm not entirely sure if this makes sense, I'll have to try it out.~~
- [+] update the config.json for stuff like mapping `Capslock+A+O` onto `Ctrl+Shift+Del`.
- [ ] rename `*.cpp` files to `*.c`
- [ ] write make or cmake file
- [ ] fix the Umlaute.
- [ ] add switchable modes

## Credits
Thank goodness for [this stackoverflow post](https://stackoverflow.com/questions/20943322/accessing-keys-from-linux-input-device), it answers all of the hard (to me at least) bits and left relatively little to do. I'm a relative new-ish developer and my area of work is completely disjoint from the linux kernel land and I only recently acquired the necessary knowledge to type the right question into my favorite search engine to produce the aforementioned stackoverflow post.

It turns out, reading [the kernel documentation](https://www.kernel.org/doc/html/latest/input/uinput.html) also helps. I was trying to get this to work with a more generic UHID the last time I attempted to get the virtual keyboard thing working, and totally missed the uinput module.
