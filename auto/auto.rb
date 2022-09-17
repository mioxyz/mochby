#!/bin/ruby
require 'colorize'

paths = %x(find ./auto "-type" f).split("\n");

run_as_su=false;

if(ARGV[0] == "--su") then
   ARGV.shift();
   run_as_su=true;
end

if ARGV.length == 0 then
   paths.each do |path|
      words = path.split("/");
      ln = "";
      for k in 2..(words.length - 1) do
         case (k)
         when words.length - 1
            ln += words[k].split(".")[0].red + " ";
         when 2 then
            ln += words[k].green + " ";
         when 3 then
            ln += words[k].yellow + " ";
         when 4 then
            ln += words[k].magenta + " ";
         else
            ln += words[k] + " ";
         end
      end
      puts ln;
   end
   exit 0;
end

s = "./auto/" + ARGV.join("/");

while 0 != ARGV.length
   paths.each do |p|
      # puts p[0, s.length];
      if p[0, s.length] == s and [".sh", ".rb", ".fish", ".js", ".ts", ".py", ".pl" ].include? p[s.length, p.length] then
         if run_as_su then
            exec("sudo " + p);
         else
            exec(p);
         end
      end
   end
   ARGV.pop();
end

STDERR.puts "ERROR: no auto script found!"

