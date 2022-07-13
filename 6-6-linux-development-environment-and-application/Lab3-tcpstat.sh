#!/bin/bash

command_res=`netstat --statistics`
last_recv=`expr "$command_res" : '^.* \([0-9][0-9]*\) segments received'`
last_sent=`expr "$command_res" : '^.* \([0-9][0-9]*\) segments sent out'`
last_total=0
#echo "$last_recv" "$last_sent"

first_min=1

while true
do
	sleep 60
	command_res=`netstat --statistics`
	recv=`expr "$command_res" : '^.* \([0-9][0-9]*\) segments received'`
	sent=`expr "$command_res" : '^.* \([0-9][0-9]*\) segments sent out'`
	date=`date "+%Y-%m-%d %H:%M"`
	
	recv_delta=`expr $recv - $last_recv`
	sent_delta=`expr $sent - $last_sent`
	total=`expr $recv_delta + $sent_delta`
	total_delta=`expr $total - $last_total`

	if [ $first_min = 1 ]
	then
		echo $date $sent_delta $recv_delta $total
		first_min=0
	else
		if [ $total_delta -ge 10 ]
		then
			echo $date $sent_delta $recv_delta $total +
		elif [ $total_delta -le 0 ]
		then
			echo $date $sent_delta $recv_delta $total -
		else
			echo $date $sent_delta $recv_delta $total -
		fi
	fi
	last_recv=$recv
	last_sent=$sent
	last_total=$total
done
