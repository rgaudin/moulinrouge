#!/usr/bin/ruby

STARTTIME = Time.now

# Global defines
LANG    = "fa"
RTL     = true
PROJECT = "encyclopedia"
MASTER = "encyclopedia"
NAMESPAC= {:id => 0, :name => nil}
UNIQID  = Time.now.to_i
UNIQFD  = "/var/moulin/#{LANG}/#{PROJECT}_#{UNIQID}"
MDWKFD  = "/var/www/localhost/htdocs/wiki"
DBICON  = "DBI:Mysql:#{LANG}_#{PROJECT}"
DBIUSER = "fa"
DBIPASS = "fa"
BLOCK_SIZE	= 10485760 # 10MB

# 
LOCALIZED_STRINGS = {
    :NSCategory => {:raw => "رده", :url => "%D8%B1%D8%AF%D9%87"}
    }

require '../crawler.rb'

hello()

puts "Duration: #{(Time.now - STARTTIME) /60} mn."
