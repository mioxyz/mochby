# mochby
Mod-Chord-Bypass or "mochby" intercepts raw key-presses from a keyboard device and triggers an action if a set of user defined keys have been depressed in unison. This allows for a different input style more akin to playing chords on a piano, in contrast to toggled systems. In turn, this allows us to bypass the window manager completely, by reading directly from `/dev/path-to-keyboard-device` and proceeding to do whatever we want, which may or may not be a good idea.

## Setup & Install
Edit the `config.json` to fit your needs, then run `sudo ./auto/install_full.rb` which will first transpile the `config.json` into `config.cpp` and will then build and install mod-chord-bypass on your machine. 

### Editing the Config.json file
`config.json` contains a key `chord_maps` which in turn contains chord objects:
```json
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
Above are two example **chord_maps**. The **chord_map** represents an event that should be triggered if a set of keys are pressed. 
A **chord_map**s event can either be a **shell_command** or **virtual_keys** but not both. When all **mod_keys** and the **terminal_key** are depressed simultaneously, the **shell_command** is executed or the **virtual_keys** are pressed respectively. A **chord_map** can have an **exclusive** key which is `false` by default. When a **chord_map**s `exclusive` key is set to `true` and this particular **chord_map** is triggered, all subsequent **chord_maps** are skipped. This is particularly useful if you have two different **chord_map**s which share a **terminal_key** and have similar **mod_keys**. However this also means that any preceding **chord_maps** that can be triggered will trigger, so it isn't truly "exclusive" in the literal meaning of the word.

A **chord_map** can also have a **trigger** key, which is by default **keyPressed** and can alternatively be set to **keyReleased** or **keyHeld**, which will cause the **chord_map** to trigger when the **terminal_key** key is released or held (the mod keys are presumed to be all pressed, if you want more complicated behavior, I'd suggest modifying the C code directly).

**virtual_keys** and **mod_keys** are based on the `linux/input-event-codes.h` key macros.

Editing the generated `config.cpp` file directly is no longer necessary.


### Passthrough Mode
Mochby has a mode in which the physical keyboard is made invisible for the current X session and all Keyboard presses are passed through to the virtual keyboard. The reason you might want to do that is for Applications such as Remmina, VirtualBox or all of the IntelliJ products, which ignore the virtual keyboard as long as there is a physical slave keyboard available to it or do even less predicatble things. By 'floating' the physical keyboard you can have the virtual keyboard press exactly those keys you want to be pressed at any given point. 
To do this enable the `PASSTHROUGH_MODE`. I haven't modified the ruby transpiler script to account for this mode, so you'll have to edit the cpp by hand. I would not recommend installing mochby as a service with PASSTHROUGH_MODE enabled, though since there are some drawbacks.
Instead, disable the service when you need `PASSTHROUGH_MODE`, and run:
```
xinput float <physical-keyboard-slave-id> ; sudo g++ -std=c++20 mochby.cpp config.cpp -o ./bin/mod_chord_bypass && sudo ./bin/mod_chord_bypass /dev/input/by-path/,<your-physical-keyboard-device>; xinput reattach <physical-keyboard-slave-id> <master-id>
```
I do this when I use Remmina RDP client. The `xinput reattach` command reenables your keyboard device. Be careful, you can easily render all input devices useless when floating physical keyboards. You can figure out the physical keyboard's slave-id by running `xinput list`.
You can also run something like: `xinput float 9; sleep 60; xinput reattach 9 3` for testing.

### Requirements
You'll need the following ruby gems: OpenSturct (`ostruct`), `colorize` and `json`. A C compiler is also necessary.

## Gotchas
My keyboard allows for multiple keys to be pressed simultaneously, but certain keys become mutually exclusive, due to the hardware, for example of the three keys [capslock, A, S] if you press any of the two in this set it will register, but pressing the third key will always not register. I believe this is due to how the keys share wiring lanes to the keyboard micro-controller- I'm not 100% sure, though.

## Why?
1. map backspace to »`Mod4`+`U`«. X11's xkb extension for one reason or another doesn't allow remapping backspace (though `Delete` is remappable) to the normal `Enter` key (though the keypad enter key can be remapped to »`Mod4`+`;`« for example).

2. map »`Mod4`+`A`« to Ctrl. This might seem a bit confusing if you haven't tried this before, but it becomes quickly unconfusing if tried in practice. Pressing »`Mod4`+`A`« acts as if you would be pressing `Mod4` and `Ctrl` (and not the A-key) allowing for additional `Ctrl`+`Mod4`'ed keypresses to be performed. If you have »`Mod4`+`l`« mapped to the right arrow key, then pressing »`Mod4`+`A`+`L`« (in that sequence without releasing the pressed keys) will be as if pressing »`Ctrl`+`→`«.

## Is this Cursed?
Yes. Yes it is.

## Open
- [+] I would like to add a virtual keyboard output device to the mix, as to bypass xdotool (and not have to do the ugly `system("stuff");` calls) and to guarantee that the keystrokes arriving in the X11 window do not mix with the causal mod-chord. For example, if I would like to map »`Mod4`+`d`« to »`Shift`+`t`« for whatever reason, I would prefer if the Window Manager and X11 would not mangle the keystrokes together. I'm not entirely sure if this makes sense, I'll have to try it out.~~
- [x] update the config.json for stuff like mapping `Capslock+A+O` onto `Ctrl+Shift+Del`.
- [x] fix the Umlaute.
- [x] add map `Capslock+A+C` to `Shift+End` and `Capslock+A+X` to `Shift+Home`
- [ ] add switchable modes

## Clopen
- [!] config transpilation has proven too cursed and now shouldn't be used anylonger.

## Open Problems
There is no way to suppress the physical keyboard's output (see **Update** below). For example, mapping `Capslock+Space` to the `_`-character can be done in two ways, either by using virtual keys (which is faster than calling xdotool and simulates key-press and key-release events):
```json
{
   "virtual_keys": [ "KEY_LEFTSHIFT", "KEY_MINUS" ],
   "mod_keys": [ "KEY_CAPSLOCK" ],
   "terminal_key": "KEY_SPACE",
   "exclusive": false
}
```
or using *xdotool*:
```json
{
   "shell_command": "xdotool type '_'",
   "mod_keys": [ "KEY_CAPSLOCK" ],
   "terminal_key": "KEY_SPACE",
   "exclusive": false
}
```
The problem that arises is that the Space symbol is not being suppressed, resulting in ` _`« being printed instead of the desired output »`_`« . I usually solve this by mapping `Capslock+<some_key>` to `Escape` or nothing in the xkb symbols file, but this is not possible for the Space key (and other 'special' keys). The current workaround is to send a `BackSpace` keypress to remove the space character, which works fine in most editors, but is quite janky. The only way I see how this can be fixed is if I bake this application directly into the linux kernel (making the keyboard devices "invisible" or simply hacking whatever is controlling /dev/input directly), which might be a very dumb idea, but eh.

**Update**: I found a better "Do-Nothing" key which now replaces most of the `Escape` key symbols which were previously on the US-Intl. symbolic key layout you can find in `./x11_keyboard_layouts/us.new`: `Num_Lock`.
The key `Num_Lock` thankfully does absolutely nothing in any of the applications I frequently use, `Escape` on the other hand often has side effects, e.g. if you're typing on a website using Chromium or Firefox, pressing `Escape` often has some meaning on the site you're using, similarly some curses-based terminal-emulator applications such as `btop` and the `kakoune` editor interpret the `Escape` key in some way. There are seemingly other "Do-Nothing" keys, such as `Caps` and `Multi` (which are valid xkb symbols), however they are ignored by most applications and the symbol on the first mod level is printed in its stead (though I'm not sure if the application or X11 is doing this, in any case it doesn't work), but `Num_Lock` is the only one which is accepted as an actual keypress and not simply ignored by X11 or the application. 
To make a long story short: placing `Num_Lock` on the 3rd mod Level in your xkb file will cause X11 to seemingly send no direct keystrokes to the active window, and allow mod-chord-bypass to intercept the raw keypresses and then perform whatever custom action you wish.
Despite this success, certain special keys such as the Space key still require the same workarounds since they don't have mod levels as described above.

## Credits
Thank goodness for [this stackoverflow post](https://stackoverflow.com/questions/20943322/accessing-keys-from-linux-input-device), it answers all of the hard (to me at least) bits and left relatively little to do. I'm a relative new-ish developer and my area of work is completely disjoint from the linux kernel land and I only recently acquired the necessary knowledge to type the right question into my favorite search engine to produce the aforementioned stackoverflow post.

It turns out, reading [the kernel documentation](https://www.kernel.org/doc/html/latest/input/uinput.html) also helps. I was trying to get this to work with a more generic UHID the last time I attempted to get the virtual keyboard thing working and totally missed the uinput module.
