#!/usr/bin/ruby

STARTTIME = Time.now

# Global defines
LANG    = "ar"
RTL     = true
PROJECT = "encyclopedia"
MASTER = "encyclopedia"
NAMESPAC= {:id => 0, :name => nil}
UNIQID  = Time.now.to_i
UNIQFD  = "/var/moulin/#{LANG}/#{PROJECT}_#{UNIQID}"
MDWKFD  = "/var/www/localhost/htdocs/wiki"
DBICON  = "DBI:Mysql:#{LANG}_#{PROJECT}"
DBIUSER = "ar"
DBIPASS = "ar"
BLOCK_SIZE	= 10485760 # 10MB

# 
LOCALIZED_STRINGS = {
    :NSCategory => {:raw => "تصنيف", :url => "%D8%AA%D8%B5%D9%86%D9%8A%D9%81"},
    :NSPortal	=> {:raw => "بوابة", :url => "%D8%A8%D9%88%D8%A7%D8%A8%D8%A9"}
    }

require '../crawler.rb'

hello()

puts "Duration: #{(Time.now - STARTTIME) /60} mn."
