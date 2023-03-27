#!/bin/bash

if [ $# -ne 0 ] && [ "$1" = "-D" ]; then
	sed -i 's-//define DEBUG_MODE-#define DEBUG_MODE-g' main.cpp
else
	sed -i 's-#define DEBUG_MODE-//define DEBUG_MODE-g' main.cpp
fi

rm a.out 2> /dev/null
rm log.txt 2> /dev/null
c++ -Wall -Wextra -Werror --std=c++98 -g main.cpp