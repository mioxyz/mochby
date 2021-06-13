#!/bin/sh

# this is this sub-shell-doesn't-return bullshit again.
#devices=$("grep --regexp 'pci-0000:00:14.0-usb-0:.*-event-kbd',y /dev/input/by-path");

#cd /dev/input/by-path;
#devices=$("grep --regexp \"pci-0000:00:14.0-usb-0:.*-event-kbd\"");
#devices=$(grep --regexp "pci-0000:00:14.0-usb-0:.*-event-kbd");
#
#devices=$("grep --regexp 'pci-0000:00:14.0-usb-0:.*-event-kbd' /dev/input/by-path/* ");
devices=$(grep --regexp "pci-0000:00:14.0-usb-0:.*-event-kbd" /dev/input/by-path/* );

echo "$devices";


