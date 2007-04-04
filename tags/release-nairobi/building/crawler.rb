#!/usr/bin/ruby

@sp=["Â","Á","À","È","É","Ê","Ë","Í","Ì","Î","Ò","Ó","Ô","Œ","Ñ","Ú","Ù","Ü","Û","Ÿ","Ç","«","»","¿","¡","—","â","á","à","è","é","ê","ë","í","ì","ï","î","ò","ó","ô","œ","ñ","ú","ü","ù","û","ÿ","ç"]
@std=["A","A","A","E","E","E","E","I","I","I","O","O","O","OE","N","U","U","U","U","Y","C","<",">","?","!","-","a","a","a","e","e","e","e","i","i","i","i","o","o","o","oe","n","u","u","u","u","y","c"]

def sp2std( str )
    @sp.each_index do |i|
        str.gsub!("#{@sp[i]}","#{@std[i]}")
    end
    return str
end

require 'dbi'

puts "#{Time.now.to_s} Starting..."

def escape_string( str )
	str.gsub(/"/, '' )
end

def get_html_for( title )
	
	puts "#{title}\n"	
	File.open( "/tmp/title", "w+" ) { |f| f.write title }
	# process the wikipedia
	result = `/usr/local/php/bin/php -f cmd.php /tmp/title`

	# exclude the top and left frames
	result.gsub!( /.*<div id=\"column-content\">(.*)/m, '\1' )
	result.gsub!( /(.*)<div id=\"column-one\">.*/m, '\1' )
	# remove the siteNotice (donate to wikipedia...)
	result.gsub!( /<div id=\"siteNotice\">.*<h1 class=\"firstHeading\">/m, '<h1 class="firstHeading">' )
	# Remove the Images links
	result.gsub!( /<a href=\"[^"><]*\" class=\"new\" title=\"Image:[^"><]*\">Image\:[^<]*<\/a>/, '' )
	# Remove the links to the article in other-languages 
	result.gsub!( /<a href=\"[^"><]*\" class=\"new\" title=\"[a-zA-Z]{2}:[^"><]*\">[a-z]{2}:([^<]*)<\/a>/, '' )
	# Remove the links to non-existent articles
	result.gsub!( /<a href=\"[^"><]*\" class=\"new\" title=\"[^"><]*\">([^<]*)<\/a>/, '\1' )
	# correct links to math images
	result.gsub!( /\/images\/math\/[a-z0-9]{1}\/[a-z0-9]{1}\/[a-z0-9]{1}\/([a-z0-9]*)\.png/, '_WikiMathImage:\1' )
	result
end

def get_redir_for( latest )
db = DBI.connect("DBI:Mysql:wikipedia", "root", "moulinmonk")
begin
text = db.select_one( "select old_text from text where old_id='#{latest}';" )[0]
rescue
text = "BadRedirection"
end
db.disconnect
return text.gsub(/\{\{\portail.*\}\}/i, "").gsub( /\[\[Catégorie\:.*\]\]/i, "" ).gsub(/\n.*/m, "").strip.gsub( /^#REDIRECT(ION)?.{0,2}\:?.{0,2}\[\[/i, "" ).gsub( /\]\]$/, "" )

end

puts "usage: #{$0} THREADS_NB SPLIT_INDEX\n" ; exit(1) unless ARGV[1]

# TMP
TOTAL_ARTICLES = 536931 # 6.12
NB_THREADS     = ARGV[0].to_i
NB_ARTFORME    = TOTAL_ARTICLES / ARGV[0].to_i
START_INDEX    = ARGV[1].to_i
ROW_START_INDEX= (START_INDEX * NB_ARTFORME)

BLOCK_SIZE	= 10485760 # 10MB
DATA_PATH	= "/home/reg/datas/ark/"
INDEX_SCHEMA= "CREATE TABLE windex (
	id INTEGER PRIMARY KEY,
	title VARCHAR(250),
	archive INTEGER,
	startoff INTEGER,
	redirect VARCHAR(250),
	stdtitle VARCHAR(250)
);"

Dir.chdir( "/usr/local/apache2/htdocs/" )

index	= DBI.connect( "DBI:Sqlite3:/home/reg/datas/index#{START_INDEX}.db" )
archive	= 0
#data	= Zlib::GzipWriter.open( "#{DATA_PATH}#{START_INDEX}-#{archive}.gz" )
data	= File.open( "#{DATA_PATH}#{START_INDEX}-#{archive}", "w+" )

index.do( INDEX_SCHEMA )

dbh = DBI.connect("DBI:Mysql:wikipedia", "root", "moulinmonk")
res = dbh.execute("SELECT page_title, page_is_redirect, page_latest FROM page WHERE page_namespace=0 ORDER BY page_title ASC LIMIT #{ROW_START_INDEX},#{NB_ARTFORME} ")
while row = res.fetch do
	title = row[0]
	is_redirect = row[1].to_i
	latest = row[2]
	if is_redirect == 1
		redir = get_redir_for( latest )
		startoff = data.pos
	else
		content = get_html_for( title )
		startoff = data.pos
		data.write( content )
		redir = ""
	end
	index.do( "INSERT INTO windex (title, archive, startoff, redirect, stdtitle) VALUES ( \"#{escape_string( title )}\", \"#{START_INDEX}-#{archive}\", #{startoff}, \"#{escape_string( redir )}\", \"#{escape_string(sp2std(title))}\" );" )
	if data.pos > BLOCK_SIZE then
		data.close
		Thread.new{ system("/usr/bin/bzip2 #{DATA_PATH}#{START_INDEX}-#{archive}") }
		archive += 1
		#data	= Zlib::GzipWriter.open( "#{DATA_PATH}#{START_INDEX}-#{archive}.gz" )
		data	= File.open( "#{DATA_PATH}#{START_INDEX}-#{archive}", "w+" )
	end
end

res.finish
data.close
index.disconnect
dbh.disconnect

puts "#{Time.now.to_s} Done."

