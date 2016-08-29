#!/usr/local/bin/ruby -w

# files... don't need to change
dbFile = "#{ENV['HOME']}/.tally_db"

# determine name of command being executed
commandName = ARGV[0]

# update the program usage database

# load db and increment/insert the program we are running
db = { commandName => [1, Time.now.to_i] }
begin
	IO.readlines(dbFile).each do |line|

		prog, count, firstTime = line.split
		count = count.to_i
		count += 1 if prog == commandName	
		db[prog] = [count, firstTime.to_i]
	end
rescue
end


begin
	# save the database
	File.open(dbFile, 'w') do |fout|

		db.each_pair do |key,val|
			fout.syswrite "#{key} #{val[0]} #{val[1]}\n"
		end
	end

rescue
	puts "Cannot open for writing"
end


# execute the program as normal
exec ARGV.join(' ')

