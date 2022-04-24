#!/bin/ruby
require 'open3'
require 'colorize'

puts "This script copies and overwrites the symbolic US keyboard layout into the X11 us xkb folder and subsequently enables it. You can edit this script and copy out the relevant pieces into your window manager's startup script for convenience."

if !%x[whoami].chomp.eql? "root" then
   puts "Error: you need to run this install-script as root."
   exit 0
end

if "mochby" != %x[pwd].chomp.split("/").last then
   puts "execute this script from main working directory."
   exit 0
end

puts "\nDo you want to make a backup copy of the current US keyboard layout? This will overwrite any files which start with the same timestamp (today, excluding hours, minutes etc.) in the »" + "./x11_keyboard_layouts".light_white + "« folder."

puts "[Y/n]"

yesses = ["y", "Y", "yes", "Yes", 1, "j", "ja", "yolo"]

if  ([""] | yesses).include? gets.chomp then
   date=%x[date +%Y%m%d].chomp
   %x[cp /usr/share/X11/xkb/symbols/us ./x11_keyboard_layouts/#{date}.us.bak]
   puts "backup placed in »".green + "#{`pwd`.chomp}/#{date}.us.bak".light_white + "«.".green
end

%x[cp ./x11_keyboard_layouts/us.new /usr/share/X11/xkb/symbols/us]

puts "Set keyboard layout to »" + "us".light_white + "« variant symbolic with mod4 set to capslock with the following command?"
puts "       »" + "setxkbmap -layout us -option lv3:caps_switch -option keyboard:pointerkeys -variant symbolic".light_white + "«"
puts "[y/N] "

if yesses.include? gets.chomp then
   stdout, stderr, status = Open3.capture3 "setxkbmap -layout us -option lv3:caps_switch -option keyboard:pointerkeys -variant symbolic"

   if status.success?
      puts "setxkbmap successfully changed keyboard layout without errors".green
   else
      puts "!!! ".red + "WARNING WARNING WARNING".yellow + " !!!".red
      puts "setxkbmap failed with message: »" + stderr.to_s().chomp().yellow + "«"
      puts "ERROR: something seems to be wrong with the keyboard layout! You need to manually copy a backup of the us symbols file onto /usr/share/X11/xkb/symbols/us immediately, or your keyboard will not work at reboot if this specific layout is loaded.".red
      exit 0
   end

   errormsg= "Error loading new keyboard description"

   puts "done."
else
   puts "nope."
end

puts "Set keyboard repeat rate and repeat delay to ass-blazing-fast mode (with the following command)?"
puts "      »" + "xset r rate 152 44".light_white + "«"
puts "[y/N] "

if  yesses.include? gets.chomp then
   %x[xset r rate 152 44]
else
   puts "nope."
end

puts "All done."
