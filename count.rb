#!/bin/ruby

input = File.open("routes.dat.txt","r")

if !input
puts "FAILED"
exit
end

i = 0

while line = input.gets
i = i + 1
end

input.close

puts i
