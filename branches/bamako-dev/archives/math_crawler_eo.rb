#!/usr/bin/ruby

require 'dbi'

puts "#{Time.now.to_s} Starting..."

$IMAGES_FOLDER = "/var/www/localhost/htdocs/eo/images/math/"
$BLOCK_SIZE	= 5242880 # 5MB
$DATA_PATH	= "/var/moulin/eo/math/"
INDEX_SCHEMA= "CREATE TABLE mindex (
	id INTEGER PRIMARY KEY,
	md5 VARCHAR(80) UNIQUE,
	archive INTEGER,
	startoff INTEGER
);"

$index	= DBI.connect( "DBI:Sqlite3:#{$DATA_PATH}index.db" )
$archive	= 0
$data	= File.open( "#{$DATA_PATH}#{$archive}", "w+" )

$index.do( INDEX_SCHEMA )

$all_md5 = []

def recurs_browse( folder )
	d = Dir.new( folder )
	d.each do |f|
		next if ["..","."].include? f
		file =  "#{d.path}/#{f}"
		md5 = f.gsub('.png','')
		if file =~ /^.*\.png$/ and not $all_md5.include? md5
		  puts f
    	   content = File.open( file ).read
    	   startoff = $data.pos
    	   $data.write( content )
    	   $index.do( "INSERT INTO mindex (md5, archive, startoff) VALUES ( \"#{f.gsub('.png','')}\", \"#{$archive}\", #{startoff} );" )
    	   $all_md5 << md5
    	   if $data.pos > $BLOCK_SIZE then
    	   	$data.close
    	   	$archive += 1
    	   	$data	= File.open( "#{$DATA_PATH}#{$archive}", "w+" )
    	   end
		end
		recurs_browse( file ) if File.directory? file			
	end
end

recurs_browse( $IMAGES_FOLDER )

$data.close
$index.disconnect

puts "#{Time.now.to_s} Done."

