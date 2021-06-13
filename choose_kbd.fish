#!/bin/fish
set devices ( ls /dev/input/by-path/ | grep --regexp "pci-0000:00:14.0-usb-0:.*-event-kbd" | sort );
if test (count $devices) = 0 
	echo "no devices";
end
echo $devices[1];

