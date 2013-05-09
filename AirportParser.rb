#!/usr/bin/env ruby

# Ruby parser for Airport data.

puts "==================================================="
puts "---Welcome to the Forced Edge Detection of Flight Route Data."
puts "---This software will only show results based upon the flight data located in the United States."
puts "==================================================="



puts "Would you like to use the software for a particulare airline? (Y or N) If no, than all airlines will be used."
userInput = STDIN.gets.chomp

if userInput == "Y"
    puts "What is the airline Id?"
    airlineId = STDIN.gets.to_i
    puts "Using the airline id #{airlineId}"
    specificAirline = true
else
    puts "Using all airlines."
    specificAirline = false
end
    

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
    if specificAirline
        if line[1].to_i == airlineId
          source = line[2]
          dest = line[4]
          if airports.has_key?(source) and airports.has_key?(dest)
                  output.puts "#{airports[source].join(" ")} #{airports[dest].join(" ")}"
                  

          end
        end
    else
      source = line[2]
      dest = line[4]
      if airports.has_key?(source) and airports.has_key?(dest)
              output.puts "#{airports[source].join(" ")} #{airports[dest].join(" ")}"
              
      end
     end	
end

input.close
output.close

