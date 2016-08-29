#!/usr/local/bin/ruby -w

# files... don't need to change
dbFile = "#{ENV['HOME']}/.tally_db"

# fvwm-name for the menu
menuName = 'FrequentMenu'

# max entries in this menu
maxEntries = 10

# load db 
db = Hash.new
begin
	IO.readlines(dbFile).each do |line|

		prog, count, firstTime = line.split
		db[prog] = [count.to_i, firstTime.to_i]
	end
rescue
end


# save an FVWM menu
puts "AddToMenu #{menuName} \"Common Applications\" Title" 

# sort entries in decending order by usage frequency	
sorted = db.sort do |a,b| 
	numa = a[1][0].to_f
	dura = Time.now.to_f - a[1][1].to_f

	numb = b[1][0].to_f
	durb = Time.now.to_f - b[1][1].to_f

	numb / durb <=> numa / dura	
end

# write each program as a menu entry
entryCount = 0	
sorted.each do |entry|

	if entryCount < maxEntries

		name = entry[0]
		uses = entry[1][0].to_f
		dur = (Time.now.to_f - entry[1][1]) / ( 60.0 * 24.0 )
	
		freq = uses / dur

		puts "+ \"#{name}\" Exec exec tally.rb #{name} # frequency #{freq} uses/day"

		entryCount += 1
	end
end		


