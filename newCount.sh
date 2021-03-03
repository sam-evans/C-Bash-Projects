#!/bin/sh

LOCATION = $1
COUNT = 0
DIRCOUNT = 0

if [ "$#" -lt "1" ]
then 
	echo "Usage: ./test2.sh <directory>"
fi

for item in $LOCATION
do
if [ -f "$item" ]
	then 
		FILECOUNT = $[$COUNT + 1]
	elif [ -d "$item" ]
		then 
		DIRCOUNT = $[$DIRCOUNT + 1]
fi
done
echo "File count : " $COUNT
echo "Dir count : " $DIRCOUNT 

