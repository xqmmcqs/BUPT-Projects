#!/bin/bash

if [ $# = 0 ]
then
    from_page=1
    to_page=235
elif [ $# = 2 ]
then
    from_page=$1
    to_page=$2
else
    echo "Usage: $0 <from_page> <to_page>"
    exit 1
fi

if [ ! -d html ]
then
	mkdir html
fi

if [ ! -d temp ]
then
	mkdir temp
fi

if [ ! -d bing ]
then
	mkdir bing
fi

for i in `seq $from_page $to_page`
do
	if [ ! -f html/$i.html ]
	then
		echo -e "\033[33m Downloading... \033[0m html/$i.html"
		wget --no-verbose http://bing.plmeizi.com/?page=$i -O html/$i.html
		echo -e "\033[32m Finished! \033[0m html/$i.html"
	else
		echo -e "\033[34m Existed! \033[0m html/$i.html"
	fi

	if [ ! -f temp/$i.txt ]
	then
		echo -e "\033[33m Downloading... \033[0m temp/$i.txt"
		cat html/$i.html | \
		sed 's/\(<a class="item\)/\n\1/g' | \
		grep 'class="item"' | \
		sed 's#^.*<img src=\(//.*\.jpg\)-listpic .*<p>\(.*\) (.*) (\([0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\}\)).*#http:\1$\2$\3#g' > temp/$i.txt
		echo -e "\033[32m Finished! \033[0m temp/$i.txt"
	else
		echo -e "\033[34m Existed! \033[0m temp/$i.txt"
	fi

	while read line
	do
	{
		url=`echo "$line" | sed 's/\(.*\)$.*$.*/\1/g'`
		img=`echo "$line" | sed 's#.*$\(.*\)$\(.*\)#bing/\2 \1.jpg#g'`
		if [ ! -f "$img" ]
		then
			tmp="$img.downloading$!"
			echo -e "\033[33m Downloading... \033[0m $tmp"
			wget --no-verbose --referer="http://bing.plmeizi.com/" -O "$tmp" $url
			if [ ! $? = 0 ]
			then
				echo -e "\033[31m Failed! \033[0m $tmp"
				rm "$tmp"
				continue
			fi

			if [ ! -f "$img" ]
			then
				echo -e "\033[33m Renaming... \033[0m $tmp to $img"
				mv "$tmp" "$img"
			else
				echo -e "\033[34m Existed! \033[0m $img"
				rm "$tmp"
			fi
			echo -e "\033[32m Finished! \033[0m $img"
		else
			echo -e "\033[34m Existed! \033[0m $img"
		fi
	}&
	done < temp/$i.txt
	wait
done

