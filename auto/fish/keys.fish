#!/bin/fish
function keys
   set mochby_dir "/home/mio/code/projects/mochby"
   set current_dir (pwd);
   cd $mochby_dir;
   ruby ./install/auto.rb --su install silent;
   if test "$current_dir" != "$mochby_dir"
      cd -;
   end
end

