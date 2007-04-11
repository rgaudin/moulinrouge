
CREATE TABLE user (
  user_id int(5) unsigned NOT NULL auto_increment,
  user_name varchar(255) binary NOT NULL default '',
  user_real_name varchar(255) binary NOT NULL default '',
  user_password tinyblob NOT NULL default '',
  user_newpassword tinyblob NOT NULL default '',
  user_email tinytext NOT NULL default '',
  user_options blob NOT NULL default '',
  user_touched char(14) binary NOT NULL default '',
  user_token char(32) binary NOT NULL default '',
  user_email_authenticated CHAR(14) BINARY,
  user_email_token CHAR(32) BINARY,
  user_email_token_expires CHAR(14) BINARY,
  user_registration CHAR(14) BINARY,
  PRIMARY KEY user_id (user_id),
  UNIQUE INDEX user_name (user_name),
  INDEX (user_email_token)
) TYPE=MyISAM;

CREATE TABLE user_groups (
  ug_user int(5) unsigned NOT NULL default '0',
  ug_group char(16) NOT NULL default '',  
  PRIMARY KEY (ug_user,ug_group),
  KEY (ug_group)
) TYPE=MyISAM;

CREATE TABLE user_newtalk (
  user_id int(5) NOT NULL default '0',
  user_ip varchar(40) NOT NULL default '',
  INDEX user_id (user_id),
  INDEX user_ip (user_ip)
);

CREATE TABLE page (
  page_id int(8) unsigned NOT NULL auto_increment,
  page_namespace int NOT NULL,
  page_title varchar(255) binary NOT NULL,
  page_restrictions tinyblob NOT NULL default '',
  page_counter bigint(20) unsigned NOT NULL default '0',
  page_is_redirect tinyint(1) unsigned NOT NULL default '0',
  page_is_new tinyint(1) unsigned NOT NULL default '0',
  page_random real unsigned NOT NULL,
  page_touched char(14) binary NOT NULL default '',
  page_latest int(8) unsigned NOT NULL,
  page_len int(8) unsigned NOT NULL,
  PRIMARY KEY page_id (page_id),
  UNIQUE INDEX name_title (page_namespace,page_title),
  INDEX (page_random),
  INDEX (page_len)
) TYPE=MyISAM;

CREATE TABLE revision (
  rev_id int(8) unsigned NOT NULL auto_increment,
  rev_page int(8) unsigned NOT NULL,
  rev_text_id int(8) unsigned NOT NULL,
  rev_comment tinyblob NOT NULL default '',
  rev_user int(5) unsigned NOT NULL default '0',
  rev_user_text varchar(255) binary NOT NULL default '',
  rev_timestamp char(14) binary NOT NULL default '',
  rev_minor_edit tinyint(1) unsigned NOT NULL default '0',
  rev_deleted tinyint(1) unsigned NOT NULL default '0',  
  PRIMARY KEY rev_page_id (rev_page, rev_id),
  UNIQUE INDEX rev_id (rev_id),
  INDEX rev_timestamp (rev_timestamp),
  INDEX page_timestamp (rev_page,rev_timestamp),
  INDEX user_timestamp (rev_user,rev_timestamp),
  INDEX usertext_timestamp (rev_user_text,rev_timestamp)
) TYPE=MyISAM;

CREATE TABLE text (
  old_id int(8) unsigned NOT NULL auto_increment,
  old_text mediumblob NOT NULL default '',
  old_flags tinyblob NOT NULL default '',  
  PRIMARY KEY old_id (old_id)
) TYPE=MyISAM;

CREATE TABLE archive (
  ar_namespace int NOT NULL default '0',
  ar_title varchar(255) binary NOT NULL default '',
  ar_text mediumblob NOT NULL default '',
  ar_comment tinyblob NOT NULL default '',
  ar_user int(5) unsigned NOT NULL default '0',
  ar_user_text varchar(255) binary NOT NULL,
  ar_timestamp char(14) binary NOT NULL default '',
  ar_minor_edit tinyint(1) NOT NULL default '0',
  ar_flags tinyblob NOT NULL default '',
  ar_rev_id int(8) unsigned,
  ar_text_id int(8) unsigned,  
  KEY name_title_timestamp (ar_namespace,ar_title,ar_timestamp)
) TYPE=MyISAM;

CREATE TABLE pagelinks (
  pl_from int(8) unsigned NOT NULL default '0',
  pl_namespace int NOT NULL default '0',
  pl_title varchar(255) binary NOT NULL default '',  
  UNIQUE KEY pl_from(pl_from,pl_namespace,pl_title),
  KEY (pl_namespace,pl_title)
) TYPE=MyISAM;

CREATE TABLE templatelinks (
  tl_from int(8) unsigned NOT NULL default '0',
  tl_namespace int NOT NULL default '0',
  tl_title varchar(255) binary NOT NULL default '',  
  UNIQUE KEY tl_from(tl_from,tl_namespace,tl_title),
  KEY (tl_namespace,tl_title)
) TYPE=MyISAM;

CREATE TABLE imagelinks (
  il_from int(8) unsigned NOT NULL default '0',
  il_to varchar(255) binary NOT NULL default '',  
  UNIQUE KEY il_from(il_from,il_to),
  KEY (il_to)
) TYPE=MyISAM;

CREATE TABLE categorylinks (
  cl_from int(8) unsigned NOT NULL default '0',
  cl_to varchar(255) binary NOT NULL default '',
  cl_sortkey varchar(86) binary NOT NULL default '',
  cl_timestamp timestamp NOT NULL,  
  UNIQUE KEY cl_from(cl_from,cl_to),
  KEY cl_sortkey(cl_to,cl_sortkey),
  KEY cl_timestamp(cl_to,cl_timestamp)
) TYPE=MyISAM;

CREATE TABLE externallinks (
  el_from int(8) unsigned NOT NULL default '0',
  el_to blob NOT NULL default '',
  el_index blob NOT NULL default '',  
  KEY (el_from, el_to(40)),
  KEY (el_to(60), el_from),
  KEY (el_index(60))
) TYPE=MyISAM;

CREATE TABLE site_stats (
  ss_row_id int(8) unsigned NOT NULL,
  ss_total_views bigint(20) unsigned default '0',
  ss_total_edits bigint(20) unsigned default '0',
  ss_good_articles bigint(20) unsigned default '0',
  ss_total_pages bigint(20) default '-1',
  ss_users bigint(20) default '-1',
  ss_admins int(10) default '-1',
  ss_images int(10) default '0',
  UNIQUE KEY ss_row_id (ss_row_id)
) TYPE=MyISAM;

CREATE TABLE hitcounter (
  hc_id INTEGER UNSIGNED NOT NULL
) TYPE=HEAP MAX_ROWS=25000;

CREATE TABLE ipblocks (
  ipb_id int(8) NOT NULL auto_increment,
  ipb_address varchar(40) binary NOT NULL default '',
  ipb_user int(8) unsigned NOT NULL default '0',
  ipb_by int(8) unsigned NOT NULL default '0',
  ipb_reason tinyblob NOT NULL default '',
  ipb_timestamp char(14) binary NOT NULL default '',
  ipb_auto tinyint(1) NOT NULL default '0',
  ipb_expiry char(14) binary NOT NULL default '',
  ipb_range_start varchar(32) NOT NULL default '',
  ipb_range_end varchar(32) NOT NULL default '',  
  PRIMARY KEY ipb_id (ipb_id),
  INDEX ipb_address (ipb_address),
  INDEX ipb_user (ipb_user),
  INDEX ipb_range (ipb_range_start(8), ipb_range_end(8))
) TYPE=MyISAM;

CREATE TABLE image (
  img_name varchar(255) binary NOT NULL default '',
  img_size int(8) unsigned NOT NULL default '0',
  img_width int(5)  NOT NULL default '0',
  img_height int(5)  NOT NULL default '0',
  img_metadata mediumblob NOT NULL,
  img_bits int(3)  NOT NULL default '0',
  img_media_type ENUM("UNKNOWN", "BITMAP", "DRAWING", "AUDIO", "VIDEO", "MULTIMEDIA", "OFFICE", "TEXT", "EXECUTABLE", "ARCHIVE") default NULL,
  img_major_mime ENUM("unknown", "application", "audio", "image", "text", "video", "message", "model", "multipart") NOT NULL default "unknown",
  img_minor_mime varchar(32) NOT NULL default "unknown",
  img_description tinyblob NOT NULL default '',
  img_user int(5) unsigned NOT NULL default '0',
  img_user_text varchar(255) binary NOT NULL default '',
  img_timestamp char(14) binary NOT NULL default '',  
  PRIMARY KEY img_name (img_name),
  INDEX img_size (img_size),
  INDEX img_timestamp (img_timestamp)
) TYPE=MyISAM;

CREATE TABLE oldimage (
  oi_name varchar(255) binary NOT NULL default '',
  oi_archive_name varchar(255) binary NOT NULL default '',
  oi_size int(8) unsigned NOT NULL default 0,
  oi_width int(5) NOT NULL default 0,
  oi_height int(5) NOT NULL default 0,
  oi_bits int(3) NOT NULL default 0,
  oi_description tinyblob NOT NULL default '',
  oi_user int(5) unsigned NOT NULL default '0',
  oi_user_text varchar(255) binary NOT NULL default '',
  oi_timestamp char(14) binary NOT NULL default '',
  INDEX oi_name (oi_name(10))
) TYPE=MyISAM;

CREATE TABLE recentchanges (
  rc_id int(8) NOT NULL auto_increment,
  rc_timestamp varchar(14) binary NOT NULL default '',
  rc_cur_time varchar(14) binary NOT NULL default '',
  rc_user int(10) unsigned NOT NULL default '0',
  rc_user_text varchar(255) binary NOT NULL default '',
  rc_namespace int NOT NULL default '0',
  rc_title varchar(255) binary NOT NULL default '',
  rc_comment varchar(255) binary NOT NULL default '',
  rc_minor tinyint(3) unsigned NOT NULL default '0',
  rc_bot tinyint(3) unsigned NOT NULL default '0',  
  rc_new tinyint(3) unsigned NOT NULL default '0',
  rc_cur_id int(10) unsigned NOT NULL default '0',
  rc_this_oldid int(10) unsigned NOT NULL default '0',
  rc_last_oldid int(10) unsigned NOT NULL default '0',
  rc_type tinyint(3) unsigned NOT NULL default '0',
  rc_moved_to_ns tinyint(3) unsigned NOT NULL default '0',
  rc_moved_to_title varchar(255) binary NOT NULL default '',
  rc_patrolled tinyint(3) unsigned NOT NULL default '0',
  rc_ip char(15) NOT NULL default '',  
  PRIMARY KEY rc_id (rc_id),
  INDEX rc_timestamp (rc_timestamp),
  INDEX rc_namespace_title (rc_namespace, rc_title),
  INDEX rc_cur_id (rc_cur_id),
  INDEX new_name_timestamp(rc_new,rc_namespace,rc_timestamp),
  INDEX rc_ip (rc_ip)
) TYPE=MyISAM;

CREATE TABLE watchlist (
  wl_user int(5) unsigned NOT NULL,
  wl_namespace int NOT NULL default '0',
  wl_title varchar(255) binary NOT NULL default '',
  wl_notificationtimestamp varchar(14) binary,  
  UNIQUE KEY (wl_user, wl_namespace, wl_title),
  KEY namespace_title (wl_namespace,wl_title)
) TYPE=MyISAM;

CREATE TABLE math (
  math_inputhash varchar(16) NOT NULL,
  math_outputhash varchar(16) NOT NULL,
  math_html_conservativeness tinyint(1) NOT NULL,
  math_html text,
  math_mathml text,  
  UNIQUE KEY math_inputhash (math_inputhash)
) TYPE=MyISAM;

CREATE TABLE searchindex (
  si_page int(8) unsigned NOT NULL,
  si_title varchar(255) NOT NULL default '',
  si_text mediumtext NOT NULL default '',  
  UNIQUE KEY (si_page),
  FULLTEXT si_title (si_title),
  FULLTEXT si_text (si_text)
) TYPE=MyISAM;

CREATE TABLE interwiki (
  iw_prefix char(32) NOT NULL,
  iw_url char(127) NOT NULL,
  iw_local BOOL NOT NULL,
  iw_trans TINYINT(1) NOT NULL DEFAULT 0,  
  UNIQUE KEY iw_prefix (iw_prefix)
) TYPE=MyISAM;

CREATE TABLE querycache (
  qc_type char(32) NOT NULL,
  qc_value int(5) unsigned NOT NULL default '0',
  qc_namespace int NOT NULL default '0',
  qc_title char(255) binary NOT NULL default '',  
  KEY (qc_type,qc_value)
) TYPE=MyISAM;

CREATE TABLE objectcache (
  keyname char(255) binary not null default '',
  value mediumblob,
  exptime datetime,
  unique key (keyname),
  key (exptime)
) TYPE=MyISAM;

CREATE TABLE transcache (
	tc_url		VARCHAR(255) NOT NULL,
	tc_contents	TEXT,
	tc_time		INT NOT NULL,
	UNIQUE INDEX tc_url_idx(tc_url)
) TYPE=MyISAM;

CREATE TABLE validate (
  val_user int(11) NOT NULL default '0',
  val_page int(11) unsigned NOT NULL default '0',
  val_revision int(11) unsigned NOT NULL default '0',
  val_type int(11) unsigned NOT NULL default '0',
  val_value int(11) default '0',
  val_comment varchar(255) NOT NULL default '',
  val_ip varchar(20) NOT NULL default '',
  KEY val_user (val_user,val_revision)
) TYPE=MyISAM;


CREATE TABLE logging (
  log_type char(10) NOT NULL default '',
  log_action char(10) NOT NULL default '',
  log_timestamp char(14) NOT NULL default '19700101000000',
  log_user int unsigned NOT NULL default 0,
  log_namespace int NOT NULL default 0,
  log_title varchar(255) binary NOT NULL default '',
  log_comment varchar(255) NOT NULL default '',
  log_params blob NOT NULL default '',
  KEY type_time (log_type, log_timestamp),
  KEY user_time (log_user, log_timestamp),
  KEY page_time (log_namespace, log_title, log_timestamp),
  KEY times (log_timestamp)
) TYPE=MyISAM;

CREATE TABLE trackbacks (
	tb_id integer AUTO_INCREMENT PRIMARY KEY,
	tb_page	integer REFERENCES page(page_id) ON DELETE CASCADE,
	tb_title varchar(255) NOT NULL,
	tb_url	varchar(255) NOT NULL,
	tb_ex text,
	tb_name varchar(255),
	INDEX (tb_page)
) TYPE=MyISAM;

CREATE TABLE job (
  job_id int(9) unsigned NOT NULL auto_increment,
  job_cmd varchar(245) NOT NULL default '',
  job_namespace int NOT NULL,
  job_title varchar(245) binary NOT NULL,
  job_params blob NOT NULL default '',
  PRIMARY KEY job_id (job_id),
  KEY (job_cmd, job_namespace, job_title)
) TYPE=MyISAM;

CREATE TABLE querycache_info (
        qci_type varchar(32) NOT NULL default '',
        qci_timestamp char(14) NOT NULL default '19700101000000',
        UNIQUE KEY ( qci_type )
) TYPE=MyISAM;
