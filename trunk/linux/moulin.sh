#!/bin/sh

HERE=`dirname $0`

if [ $1 ]
then
    MOULINCD="$1"
else
    MOULINCD="./"
fi

if [ ! -f $MOULINCD/datas/index.db ]
then
	echo "CD NOT FOUND"
	exit
fi

if [ ! -f /var/run/moulin_httpd.pid ]
then
	$HERE/moulin_httpd -p 2111 -d $MOULINCD/datas/ -T utf-8 & > /dev/null
fi

$HERE/xulrunner/xulrunner --app $HERE/application.ini &


