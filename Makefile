NAME	= 		containers

SRCS	= 		main.cpp

FT_EXEC 	= 	ft.out
STL_EXEC 	= 	stl.out

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

OBJS_DIR 	= 	objs/
FT_OBJ		= 	ftmain.o
STL_OBJ		= 	stlmain.o
OBJS_FILES	= 	${FT_OBJ} ${STL_OBJ}
OBJS		= 	$(addprefix ${OBJS_DIR}, ${OBJS_FILES})

CXX		= 		c++

FLAGS	= 		-Wall -Wextra -Werror -std=c++98

$(OBJS_DIR)%main.o:		${SRCS} ${INCLUDES}
						@mkdir -p $(OBJS_DIR)
						${CXX} ${FLAGS} -I${CONT_DIR} -DSTL=0 -c ${SRCS} -o $(OBJS_DIR)${FT_OBJ}
						${CXX} ${FLAGS} -I${CONT_DIR} -DSTL=1 -c ${SRCS} -o $(OBJS_DIR)${STL_OBJ}

all:			${NAME}

${NAME}:		${FT_EXEC} ${STL_EXEC}

%.out:			$(OBJS_DIR)%main.o
				${CXX} ${FLAGS} $< -o $@

test:			${NAME}
				$(shell ./do_test.sh)
				@echo "Tests done: check the logs/ directory"

clean:		
			rm -rf ${OBJS_DIR}

fclean:		clean
			rm -rf logs/
			rm -f ${FT_EXEC} ${STL_EXEC}


re:			fclean all

.PHONY: all clean fclean re

.PRECIOUS: ${OBJS}