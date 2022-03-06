#!/bin/ruby
require 'json'
require 'ostruct'
require 'colorize'

def transpileChordMapLine(chord_map)
   if(chord_map.shell_command) then
      if(chord_map.virtual_keys) then
         puts "ERROR: ".red + "you can't bind both shell_commands and virtual keys to the same chord_map struct.";
         puts "raw chord_map: »" + chord_map.to_s().yellow + "«";
         exit true;
      end
      return "{ \"#{chord_map.shell_command}\" , {#{chord_map.mod_keys.join ", "}}, 1, #{chord_map.terminal_key}, #{chord_map.trigger}, #{chord_map.exclusive} }"
   else
      return "{ nullptr, {#{chord_map.mod_keys.join ", "}}, #{[0, chord_map.mod_keys.count - 1].max.to_s}, #{chord_map.terminal_key}, #{chord_map.trigger}, #{chord_map.exclusive}, {#{chord_map.virtual_keys.join ", "}}, #{chord_map.virtual_keys.count.to_s} }"
   end
end


def transpileConfig
   if(Dir.pwd.split('/').last == 'auto') then
      filepath = "#{Dir.pwd}/../config.json";
   else
      filepath = "#{Dir.pwd}/config.json"
   end

   data = JSON.parse( File.open(filepath, "rb").read, object_class: OpenStruct)

   data.chord_maps.each do |chord_map|
      puts "---"
      puts chord_map
      puts "==="
   end

   mod_keys = []
   virtual_keys = []

   data.chord_maps.each do |chord_map|
      if(chord_map.mod_keys) then
         mod_keys.append chord_map.mod_keys
      end
      if(chord_map.virtual_keys) then
         virtual_keys.append chord_map.virtual_keys
      end
   end

   mod_keys.flatten!().uniq!
   virtual_keys.flatten!().uniq!

   if(Dir.pwd.split('/').last == 'auto') then
      filepath = "#{Dir.pwd}/../config.template.cpp";
   else
      filepath = "#{Dir.pwd}/config.template.cpp"
   end

   template = File.open(filepath, "rb").read

   template.sub!( '/*%__VIRTUAL_KEYS_COUNT__%*/0/*%_KEEP_ZERO_%*/', virtual_keys.count.to_s );
   template.sub!( '/*%__MOD_KEYS_COUNT__%*/0/*%_KEEP_ZERO_%*/', mod_keys.count.to_s );
   template.sub!( '/*%__CHORD_MAP_COUNT__%*/0/*_KEEP_ZERO_*/', data.chord_maps.count.to_s );   
   template.sub!("/*%__VIRTUAL_KEYS_BODY__%*/", virtual_keys.join(",\n"))
   template.sub!("/*%__MOD_KEYS_BODY__%*/",     mod_keys.join(",\n"))
   template.sub!("/*%__CHORD_MAP_BODY__%*/", data.chord_maps.map{|x| transpileChordMapLine x }.join ",\n" )

   if(Dir.pwd.split('/').last == 'auto') then
      filepath = "#{Dir.pwd}/../config.cpp";
   else
      filepath = "#{Dir.pwd}/config.cpp"
   end

   handle = File.write(filepath, template);
end

transpileConfig();
