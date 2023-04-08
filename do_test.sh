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

min_space=20

current=$none
compare_line=0
ft_accumulator=""
stl_accumulator=""

get_tag() {
	tag=$none

	if [[ ${ftline:0:1} == '#' ]] ; then
		compare_line=0
		if [[ "$1" == *"CONTAINER"* ]] ; then
			tag=$cont
		elif [[ "$1" == *"TESTING"* ]] ; then
			tag=$test
		elif [[ "$1" == *"TIME"* ]] ; then
			tag=$time
		fi
	fi

	echo $tag
}

print_container_name() {
	line=$1
	title=${line//CONTAINER/}
	echo -e "\n$IYellow $title $Color_Off"
}

print_test_name() {
	line=$1
	title=${line//TESTING/}
	echo -e -n "$IPurple $title $Color_Off"
	len=${#title}

	while [[ $len -lt $min_space ]] ; do
		echo -n " ";
		len=$(($len+1))
	done
}

check_diff() {
	if diff <(echo "$1") <(echo "$2") > /dev/null; then
		echo -e -n "✅"
	else
		echo -e -n "❌"
	fi
}

print_time() {
	difftime=$1
	if [[ $2 -ne "0" ]] ; then
		difftime=$(echo "scale=2; $1 / $2" | bc)
	fi
	
	if [[ $(echo "$difftime < 20" |bc -l) ]] ; then
		echo -e "    $IGreen x$difftime $Color_Off"
	else
		echo -e "    $IRed x$difftime $Color_Off"
	fi
}

make_test() {

	echo -e "\n$ICyan--- TEST ------------ COMP - TIME $Color_Off"

	while IFS= read -r ftline && IFS= read -r stlline <&3;
	do
		current=$(get_tag $ftline)

		if [[ $current -eq $cont ]] ; then
			print_container_name "$ftline"

		elif [[ $current -eq $test ]] ; then
			compare_line=1
			print_test_name "$ftline"

		elif [[ $current -eq $time ]] ; then
			check_diff "$ft_accumulator" "$stl_accumulator"
			ft_accumulator=""
			stl_accumulator=""

			fttime=$(echo "$ftline" | sed 's/[^0-9]*//g')
			stltime=$(echo "$stlline" | sed 's/[^0-9]*//g')
			print_time "$fttime" "$stltime"

		elif [[ $compare_line -eq 1 ]] ; then
			ft_accumulator="$ftline"
			stl_accumulator="$stlline"
		fi;

	done <$ftlog 3<$stllog

}

make_test
exit 0