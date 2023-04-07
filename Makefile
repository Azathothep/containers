SRCS	= main.cpp \

NAME	= containers

FT_OBJ		= ftmain.o

FT_EXEC 	= ft.out

STL_OBJ		= stlmain.o

STL_EXEC 	= stl.out

CXX		= c++

FLAGS	= -Wall -Wextra -Werror -std=c++98

INC_DIR = containers/

INC_FILES =		vector.hpp \
				map.hpp \
				stack.hpp \
				set.hpp \

INCLUDES 	= $(addprefix ${INC_DIR}, ${INC_FILES});

OBJS = 	${FT_OBJ} ${STL_OBJ}

%main.o:		main.cpp ${INCLUDES}
				${CXX} ${FLAGS} -I${INC_DIR} -DSTL=0 -c main.cpp -o ${FT_OBJ}
				${CXX} ${FLAGS} -I${INC_DIR} -DSTL=1 -c main.cpp -o ${STL_OBJ}

all:			${NAME}

${NAME}:		${FT_EXEC} ${STL_EXEC}

%.out:			%main.o
				${CXX} ${FLAGS} $< -o $@

# ${FT_EXEC}:		${FT_OBJ}
# 				${CXX} ${FLAGS} ${FT_OBJ} -o ${FT_EXEC}

# ${STL_EXEC}: 	${STL_OBJ}
# 				${CXX} ${FLAGS} ${STL_OBJ} -o ${STL_EXEC}

test:			${NAME}
				$(shell ./do_test.sh)

clean:		
			rm -f ${FT_OBJ} ${STL_OBJ}

fclean:		clean
			rm -f ${FT_EXEC}
			rm -f ${STL_EXEC}

re:			fclean all

.PHONY	= all clean fclean re