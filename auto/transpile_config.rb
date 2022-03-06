#!/bin/ruby
require 'json'
require 'ostruct'
require 'colorize'
def transpileChordMapLine(chord_map, config)
   if not chord_map&.trigger then
      chord_map.trigger = "keyPressed";
   end
   chord_map.exclusive = ((not chord_map.exclusive.nil?) and chord_map.exclusive);
   # chord_map.exclusive = ((not chord_map&.exclusive&.nil?) and config.exclusive);   

   if(chord_map.shell_command) then
      if config.shell_command_wrapper and (chord_map&.ignore_shell_command_wrap&.nil? or (not config.ignore_shell_command_wrap)) then
         chord_map.shell_command = config.shell_command_wrapper.prefix + chord_map.shell_command + config.shell_command_wrapper.suffix;
      end
      if(chord_map.virtual_keys) then
         puts "ERROR: ".red + "you can't bind both shell_commands and virtual keys to the same chord_map struct.";
         puts "raw chord_map: »" + chord_map.to_s().yellow + "«";
         exit true;
      end
      return "{ \"#{chord_map.shell_command}\" , {#{chord_map.mod_keys.join ", "}}, #{chord_map.mod_keys.count}, #{chord_map.terminal_key}, #{chord_map.trigger}, #{chord_map.exclusive} }"
   else
      return "{ nullptr, {#{chord_map.mod_keys.join ", "}}, #{chord_map.mod_keys.count.to_s}, #{chord_map.terminal_key}, #{chord_map.trigger}, #{chord_map.exclusive}, {#{chord_map.virtual_keys.join ", "}}, #{chord_map.virtual_keys.count.to_s} }"
   end
end

def transpileConfig
   if(Dir.pwd.split('/').last == 'auto') then
      filepath = "#{Dir.pwd}/../config.json";
   else
      filepath = "#{Dir.pwd}/config.json"
   end

   config = JSON.parse( File.open(filepath, "rb").read, object_class: OpenStruct)

   mod_keys = []
   virtual_keys = []

   config.chord_maps.each do |chord_map|
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

   git_commit_hash = %x(git log -1 --pretty=format:%h)

   template.sub!( '/*%__VIRTUAL_KEYS_COUNT__%*/0/*%_KEEP_ZERO_%*/', virtual_keys.count.to_s );
   template.sub!( '/*%__MOD_KEYS_COUNT__%*/0/*%_KEEP_ZERO_%*/', mod_keys.count.to_s );
   template.sub!( '/*%__CHORD_MAP_COUNT__%*/0/*_KEEP_ZERO_*/', config.chord_maps.count.to_s );
   template.sub!("/*%__VIRTUAL_KEYS_BODY__%*/", virtual_keys.join(",\n   "));
   template.sub!("/*%__MOD_KEYS_BODY__%*/",     mod_keys.join(",\n   "));
   template.sub!("/*%__CHORD_MAP_BODY__%*/", config.chord_maps.map{ |x| transpileChordMapLine(x, config) }.join(",\n   ") )
   template.sub!('/*%__TRANSPILE_TIME__%*/""', "\"#{Time.now.strftime '%Y-%m-%d %H:%M:%S'}\"")
   template.sub!('/*%__CONFIG_VERSION__%*/""', "\"#{config.version}.#{git_commit_hash}\"" )

   if(Dir.pwd.split('/').last == 'auto') then
      filepath = "#{Dir.pwd}/../config.cpp";
   else
      filepath = "#{Dir.pwd}/config.cpp"
   end
   handle = File.write(filepath, template);


   ######################################################################333
   # also write git commit hash into config.json
   if(Dir.pwd.split('/').last == 'auto') then
      filepath = "#{Dir.pwd}/../config.json";
   else
      filepath = "#{Dir.pwd}/config.json"
   end
   
   config = JSON.parse( File.open(filepath, "rb").read )

   sp = config["version"].split "."
   sp = sp[0..(config.length - 3)]
   sp.append Time.now.strftime '%y%m%d_%H%M'
   sp.append git_commit_hash
   config["version"] = sp.join "."

   handle = File.write(filepath, JSON.pretty_generate(config) );
end

if ARGV[0] == 'run' then
   transpileConfig();
end
