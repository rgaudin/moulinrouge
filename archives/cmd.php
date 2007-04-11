<?php

$_SERVER['SERVER_NAME'] = "moulinmonk";
$_SERVER['REMOTE_ADDR'] = "127.0.0.1";
$_SERVER['PATH_INFO'] = "/" . str_replace("\n","", file_get_contents( $argv[1] ) );

include 'index.php';

?>
