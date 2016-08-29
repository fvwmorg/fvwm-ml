#!/usr/local/bin/ruby -w

# files... don't need to change
dbFile = "#{ENV['HOME']}/.tally_db"
fvwmFile = "#{ENV['HOME']}/.fvwm_dynamic_menu"

# fvwm-name for the menu
menuName = 'FrequentMenu'

# determine name of command being executed
commandName = ARGV[0]

# update the program usage database

# load db and increment/insert the program we are running
db = { commandName => 1 }
begin
	IO.readlines(dbFile).each do |line|

		prog, count = line.split
		count = count.to_i
		count += 1 if prog == commandName	
		db[prog] = count
	end
rescue
end


begin
	# save the database
	File.open(dbFile, 'w') do |fout|

		db.each_pair do |key,val|
			fout.syswrite "#{key} #{val}\n"
		end
	end

	# save an FVWM menu
	File.open(fvwmFile, 'w') do |fout|

		fout.syswrite "AddToMenu #{menuName} \"Common Applications\" Title\n" 

		# sort entries in decending order by occurance	
		sorted = db.sort { |a,b| b[1] <=> a[1] }

		# write each program as a menu entry	
		sorted.each do |entry|
		
			fout.syswrite "+ \"#{entry[0]}\" Exec exec tally.rb #{entry[0]}\n"
		end		
	end
rescue
	puts "Cannot open for writing"
end


# execute the program as normal
exec ARGV.join(' ')

