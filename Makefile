SRCS	= 		main.cpp

NAME	= 		containers

OBJS_DIR 	= 	objs

FT_OBJ		= 	ftmain.o
STL_OBJ		= 	stlmain.o

FT_EXEC 	= 	ft.out
STL_EXEC 	= 	stl.out

CXX		= 		c++

FLAGS	= 		-Wall -Wextra -Werror -std=c++98

CONT_DIR = 		containers/

CONT_FILES =	vector.hpp \
				map.hpp \
				stack.hpp \
				set.hpp \

UTILS_DIR	=	utils/

UTILS_FILES =	binary_tree.hpp \
				enable_if.hpp \
				equal.hpp \
				is_const.hpp \
				is_integral.hpp \
				iterator_traits.hpp \
				lexicographical_compare.hpp \
				make_pair.hpp \
				node.hpp \
				pair.hpp \
				reverse_iterator.hpp \

INCLUDES 	= 	$(addprefix ${CONT_DIR}, ${CONT_FILES}) \
				$(addprefix ${UTILS_DIR}, ${UTILS_FILES})

OBJS = 			${FT_OBJ} ${STL_OBJ}

%main.o:		${SRCS} ${INCLUDES}
				${CXX} ${FLAGS} -I${CONT_DIR} -DSTL=0 -c ${SRCS} -o ${FT_OBJ}
				${CXX} ${FLAGS} -I${CONT_DIR} -DSTL=1 -c ${SRCS} -o ${STL_OBJ}

all:			${NAME}

${NAME}:		${FT_EXEC} ${STL_EXEC}

%.out:			%main.o
				${CXX} ${FLAGS} $< -o $@

# ${FT_EXEC}:		${FT_OBJ}
# 				${CXX} ${FLAGS} $< -o $@

# ${STL_EXEC}: 	${STL_OBJ}
# 				${CXX} ${FLAGS} $< -o $@

test:			${NAME}
				$(shell ./do_test.sh)

clean:		
			rm -f ${FT_OBJ} ${STL_OBJ}

fclean:		clean
			rm -f ${FT_EXEC} ${STL_EXEC}
			rm -rf logs


re:			fclean all

.PHONY	= all clean fclean re