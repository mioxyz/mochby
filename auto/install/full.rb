#!/usr/bin/ruby
require 'open3'
require_relative '../transpile_config'

if !%x[whoami].chomp.eql? "root" then
   puts "you need to run this install-script as root."
   exit 0
end

# check in which folder we are
puts "before if..."
if "mochby" != %x[pwd].chomp.split("/").last then
   puts "run this script in the main directory."
   exit 0
end

puts "transpile config (config.json is transpiled into config.cpp and will overwrite config.cpp)? ? [Y/n]"
yesses = ["y", "Y", "yes", "Yes", 1, "j", "ja", "yolo", ""]
if yesses.include? gets.chomp then
   print " ...yes."
   transpileConfig()
else
   puts "nope."
end

%x[mkdir -p ./bin ]
# -w
stdout, stderr, status = Open3.capture3 "g++ -std=c++20 mochby.cpp config.cpp -o ./bin/mod_chord_bypass "

if status.success?
   puts stdout
else
   puts stderr
   puts "ERROR build faild."
   exit 0
end

maybeService=%[systemctl list-units --full -all | grep "mod-chord-bypass.service"]
if 0 < maybeService.length then
   %x[systemctl stop mod-chord-bypass]
else
   puts "it seems like this is the first time you are installing mod-chord-bypass, skipping the part where the old service is disabled."
end

%x[killall mod_chord_bypass]
%x[mkdir -p /usr/local/bin/mod_chord_bypass]

stdout, stderr, status = Open3.capture3 "install ./bin/mod_chord_bypass /usr/local/bin/mod_chord_bypass/mod_chord_bypass"

if status.success?
   puts stdout
else
   puts stderr
   puts "ERROR: installing mod_chord_bypass to /usr/local/bin/mod_chord_bypass unsuccessful. servie has been disabled, you need to manually restart it."
   exit 0
end

stdout, stderr, status = Open3.capture3 "install ./auto/mod-chord-bypass.service /etc/systemd/system/mod-chord-bypass.service"

if status.success?
   puts stdout
else
   puts stderr
   puts "ERROR: installing new service to /etc/systemd/mod-chord-bypass.service unsuccessful. service file might have been corrupted, you need to manually copy it to the systemd folder."
   exit 0
end

#install scripts
stdout, stderr, status = Open3.capture3 "install ./auto/launcher.rb /usr/local/bin/mod_chord_bypass/launcher.rb"

if status.success?
   puts stdout
else
   puts stderr
   puts "ERROR: during installation of launcher script to /usr/local/bin/mod_chord_bypass/launcher.rb"
   exit 0
end

%x[systemctl daemon-reload; systemctl start mod-chord-bypass; sleep 1; systemctl status mod-chord-bypass];

puts "show service status? [Y/n]"
yesses = ["y", "Y", "yes", "Yes", 1, "j", "ja", "yolo", ""]
if yesses.include? gets.chomp then
   print " ...yes."
   system "systemctl status mod-chord-bypass"
else
   puts "nope."
end

puts "Set keyboard repeat rate and repeat delay to ass-blazing-fast mode (with the following command)?"
puts "      »xset r rate 152 44«"
puts "[y/N] "

if  yesses.include? gets.chomp then
   print " ...yes."
   %x[xset r rate 152 44]
else
   puts "nope."
end

