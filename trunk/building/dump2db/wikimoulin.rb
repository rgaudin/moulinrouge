#!/usr/bin/ruby

if ARGV[0].nil?
	puts "Usage: #{$0} SOURCE_FILE"
	exit
end

require 'rexml/document'
include REXML

MYSQL_CMD = "/usr/bin/mysql -uroot wikipedia"
$mysql = IO.popen( MYSQL_CMD, "w" )
$i = $y = 0

NAMESPACES = { 
	'Media'			=> -2,
	'Special'		=> -1,
	''				=> 0,
	'Discuter'				=> 1,
	'Utilisateur'	=> 2,
	'Discussion Utilisateur'=> 3,
	'Wikipédia'		=> 4,
	'Discussion Wikipédia'	=> 5,
	'Image'			=> 6,
	'Discussion Image'		=> 7,
	'MediaWiki'		=> 8,
	'Discussion MediaWiki'	=> 9,
	'Modèle'		=> 10,
	'Discussion Modèle'		=> 11,
	'Aide'			=> 12,
	'Discussion Aide'		=> 13,
	'Catégorie'		=> 14,
	'Discussion Catégorie'	=> 15,
	'Portail'		=> 100,
	'Discussion Portail'	=> 101,
	'Projet'		=> 102,
	'Discussion Projet'		=> 103,
	'Référence'		=> 104,
	'Discussion Référence'	=> 105
	}
	
def escape_string( str )
	str.gsub(/([\0\n\r\032\'\"\\])/) do
		case $1
			when "\0" then "\\0"
			when "\n" then "\\n"
			when "\r" then "\\r"
			when "\032" then "\\Z"
			else "\\"+$1
		end
	end
end

def sqlize( xml )
	document= Document.new( xml )
	page	= document.elements[ '//page' ]
	title	= page.elements[ 'page/title' ]
	title	= title.nil? ? title.to_s : title.text.to_s
	nsi		= title.index( ":" )
	ns 		= 0
	if not nsi.nil? and NAMESPACES.include? title[0, nsi] then
		ns 	  	= NAMESPACES[ title[0, nsi] ]
		title 	= title[nsi + 1, title.length]
	end
	title.gsub!( ' ', '_' )
	title	= escape_string( title )
	id		= page.elements[ 'page/id' ].text
	rev_id	= page.elements[ 'page/revision/id' ].text
	rev_ts	= page.elements[ 'page/revision/timestamp' ].text
	rev_c_n	= page.elements[ 'page/revision/contributor/username' ]
	rev_c_n = escape_string( rev_c_n.text.to_s ) unless rev_c_n.nil?
	rev_c_id= page.elements[ 'page/revision/contributor/id' ]
	rev_c_id= escape_string( rev_c_id.text.to_s ) unless rev_c_id.nil?
	rev_comm= page.elements[ 'page/revision/comment' ]
	rev_comm= escape_string( rev_comm.text.to_s ) unless rev_comm.nil?
	text	= page.elements[ 'page/revision/text' ]
	text	= escape_string( text.text.to_s ) unless text.nil?
	redirect= ( text[ 0, 9 ] == "#REDIRECT" ) ? 1 : 0
	len		= text.length

	req  = "INSERT INTO text (old_id, old_flags, old_text) VALUES ( '#{rev_id}', 'utf-8', \"#{text}\");"
	$mysql.puts req

	req2 = "INSERT INTO revision (rev_id, rev_page, rev_text_id, rev_comment, rev_user, rev_user_text, rev_timestamp, rev_minor_edit, rev_deleted) 
			VALUES ( '#{rev_id}', '#{id}', '#{rev_id}', \"#{rev_comm}\", '#{rev_c_id}', '#{rev_c_n}', '#{rev_ts}', 1, 0);"
	$mysql.puts req2

	req3 = "INSERT INTO page (page_id, page_namespace, page_title, page_is_redirect, page_latest, page_len) 
			VALUES ( '#{id}', '#{ns}', \"#{title}\", '#{redirect}', '#{rev_id}', '#{len}');"
	$mysql.puts req3
	
	if $i >= 500 then
		$mysql.close
		$mysql = IO.popen( MYSQL_CMD, "w" )
		$i = 0
	end
	
	$i += 1
end

STDOUT.puts Time.now.to_s

	PAGE_START_REGEXP	= /^  <page>(.*)$/
	PAGE_END_REGEXP		= /^(.*)<\/page>$/
	
	src		= File.new( ARGV[0], "r" )
	data	= ""
	in_text	= false

	while not src.eof?
	
		l = src.readline
		
    	if l[ 0, 3 ] == "  <" and PAGE_START_REGEXP.match( l ) then
    		data = "<page>" << l.sub( PAGE_START_REGEXP, '\1' )
    		in_text = true
    	end
		
		if l[ -7, 6 ] == "/page>" and PAGE_END_REGEXP.match( l ) then
    		data << l.sub( PAGE_END_REGEXP, '\1' ) << "</page>"
    		in_text	= false
			
			sqlize( data )
    	end
    	
    	if in_text then
    		data <<  l
    	end
    	
    end
    
    src.close
    $mysql.close
    

STDOUT.puts Time.now.to_s
