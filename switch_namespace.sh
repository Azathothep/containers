if [ $# -eq 0 ]
  	then
		std=0
	else
		std=$1
fi

if [ $std -eq 1 ]
	then sed -i "s/define STL 0/define STL 1/g" main.cpp
	else sed -i "s/define STL 1/define STL 0/g" main.cpp
fi