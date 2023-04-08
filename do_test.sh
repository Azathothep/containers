#!/bin/bash

Color_Off='\033[0m'		  # Reset
IBlack='\033[0;90m'       # Black
IRed='\033[0;91m'         # Red
IGreen='\033[0;92m'       # Green
IYellow='\033[0;93m'      # Yellow
IBlue='\033[0;94m'        # Blue
IPurple='\033[0;95m'      # Purple
ICyan='\033[0;96m'        # Cyan
IWhite='\033[0;97m'       # White

logsdir="logs"

mkdir -p $logsdir

ftlog="$logsdir/ftlog.txt"
stllog="$logsdir/stllog.txt"
difflog="$logsdir/diff.txt"

./ft.out > $ftlog
./stl.out > $stllog

diff $ftlog $stllog > $difflog

none=0
cont=1
test=2
time=3

get_tag() {
	tag=$none

	if [[ "$1" == *"CONTAINER"* ]] ; then
		tag=$cont
	elif [[ "$1" == *"TESTING"* ]] ; then
		tag=$test
	elif [[ "$1" == *"TIME"* ]] ; then
		tag=$time
	fi

	echo $tag
}

min_space=20

current=$none
compare_line=0
ft_accumulator=""
stl_accumulator=""

echo -e "$ICyan--- TEST ------------ COMP - TIME $Color_Off"

while IFS= read -r ftline && IFS= read -r stlline <&3;
do
	if [[ ${ftline:0:1} == '#' ]] ; 
	then
		current=$(get_tag $ftline)
		compare_line=0
	else
		current=$none
	fi

	if [[ $current -eq $cont ]] ; then
		compare_line=0
		title=${ftline//CONTAINER/}
		echo -e "\n$IYellow $title $Color_Off"
	elif [[ $current -eq $test ]] ; then
		compare_line=1
		title=${ftline//TESTING/}
		echo -e -n "$IPurple $title $Color_Off"
		len=${#title}

		while [[ $len -lt 20 ]] ; do
			echo -n " ";
			len=$(($len+1))
		done

	elif [[ $current -eq $time ]] ; then
		compare_line=0
		if diff <(echo "$ft_accumulator") <(echo "$stl_accumulator") > /dev/null; then
			echo -e -n "✅"
		else
			echo -e -n "❌"
		fi

		fttime=$(echo "$ftline" | sed 's/[^0-9]*//g')
		stltime=$(echo "$stlline" | sed 's/[^0-9]*//g')
		difftime=$fttime
		if [[ $stltime -ne "0" ]] ; then
			difftime=$(echo "scale=2; $fttime / $stltime" | bc)
		fi
		
		if [[ $(echo "$difftime < 20" |bc -l) ]] ; then
			echo -e "    $IGreen x$difftime $Color_Off"
		else
			echo -e "    $IRed x$difftime $Color_Off"
		fi

	elif [[ $compare_line -eq 1 ]] ; then
		ft_accumulator="$ftline"
		stl_accumulator="$stlline"
	fi;
done <$ftlog 3<$stllog

exit 0