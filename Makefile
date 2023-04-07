SRCS	= main.cpp \

OBJS	= ${SRCS:.cpp=.o}

NAME	= ft

FT_EXEC 	= ft_tests

STL_EXEC 	= stl_tests

CC		= c++

FLAGS	= -Wall -Wextra -Werror -std=c++98

INC_DIR = containers

INCLUDES =	${INC_DIR}/vector.hpp \
			${INC_DIR}/map.hpp \
			${INC_DIR}/stack.hpp \
			${INC_DIR}/set.hpp \

%.o:		${SRCS}%.cpp ${INCLUDES}
			${CC} ${FLAGS} -c $< -o $@ -Icontainers/

all:	${NAME}

${NAME}:	main.cpp ${INCLUDES}
			${CC} ${FLAGS} main.cpp -DSTL=0 -Icontainers/ -o ${FT_EXEC}
			${CC} ${FLAGS} main.cpp -DSTL=1 -Icontainers/ -o ${STL_EXEC}

clean:		
			rm -f ${OBJS}

fclean:		clean
			rm -f ${FT_EXEC}
			rm -f ${STL_EXEC}

re:			fclean all

.PHONY	= all clean fclean re