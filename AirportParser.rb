#!/usr/bin/env ruby

# Ruby parser for Airport data. 

input = File.open("airports.dat.txt", "r")
if !input
	puts "Failed to open the airports data."
	exit
end

airports = Hash.new 

while line = input.gets
	line = line.split(",")
	id = line[4]
	id = id.split(/\"/);
	id = id[1]
	latitude = line[6].to_f + 180
	longitude = line[7].to_f + 90
	airports[id] = ["#{latitude} #{longitude}"]
end

input.close

input = File.open("routes.dat.txt", "r")
output = File.open("edges.txt", "w")

if !input and !output
	puts "Failed to open the airports data."
	exit
end



while line = input.gets
	line = line.split(",")
	source = line[2]
	dest = line[4]
	if airports.has_key?(source) and airports.has_key?(dest)
		output.puts "#{airports[source]} #{airports[dest]}"
	end
end

input.close
output.close