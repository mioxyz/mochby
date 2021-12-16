#!/bin/ruby
# requires fstrcmp to be installed.

cmds = []

def fuzzyUnique(haystack, needle)
   haystack.each do |other|
		if Float(%x[fstrcmp \"#{needle}\" \"#{other}\"]) < 0.95  then
         return false
      end
   end
   return true
end

%x[ls -1 /dev/input/by-path/].split("\n").each do |dev|
   if dev.include? "-kbd" and fuzzyUnique(cmds, dev) then
      cmds.push "/usr/local/bin/mod_chord_bypass/mod_chord_bypass /dev/input/by-path/#{dev}"
   end
end

%x[ #{cmds.join(" & ")} ]
