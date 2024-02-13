#!/usr/bin/ruby
require 'open3'

%x[mkdir -p ./bin ]
# -w
stdout, stderr, status = Open3.capture3 "g++ -std=c++20 mochby.cpp config.cpp -o ./bin/mod_chord_bypass"

if status.success?
   puts "built with success."
   puts stdout
   puts status
else
   puts stderr
   puts "ERROR build faild."
   exit 0
end

