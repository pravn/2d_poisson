#!/usr/bin/env ruby 
#usage: ./build.rb 0 to print debug messages
#./build.rb 1 if no debug messages are desired


if(ARGV[0]) 
	DEBUG=ARGV[0]
else
	DEBUG="0"
end


if(DEBUG.chomp.to_i==1)
  cmd = "make DEBUG=1 2>&1|tee build.log"
else 
  cmd = "make DEBUG=0 2>&1|tee build.log"
end

if(DEBUG=="clean")
  cmd = "make clean 2>&1|tee build.log"
end

print cmd, "\n"
print system(cmd)

  
