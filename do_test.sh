sed -i "s/define STL 1/define STL 0/g" main.cpp
clang++ -Wall -Wextra -Werror -std=c++98 main.cpp -o ft.out
sed -i "s/define STL 0/define STL 1/g" main.cpp
clang++ -Wall -Wextra -Werror -std=c++98 main.cpp -o std.out
sed -i "s/define STL 1/define STL 0/g" main.cpp

./ft.out $1 > ftlog.txt
./std.out $1 > stdlog.txt

diff ftlog.txt stdlog.txt

rm ft.out std.out