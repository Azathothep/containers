#ifndef PRINT_TREE_HPP
# define PRINT_TREE_HPP

# include <iostream>
# include "ft/is_integral.hpp"
# include "ft/enable_if.hpp"

# include "node.hpp"
# include "b_tree.hpp"

# define P_BLACK "30"
# define P_RED "31"
# define P_GREEN "32"
# define P_YELLOW "33"
# define P_BLUE "34"
# define P_MAGENTA "35"
# define P_CYAN "36"
# define P_WHITE "37"

template <typename T>
void integral_printer(T & value) {
	std::cout << value;
}

template <typename T>
std::string get_color(ft::node<T> *n) {
	if (n->color == RED)
		return P_RED;
	return P_BLUE;
}

void print_branch(std::string b) {
	std::cout << "\033[1;" << P_WHITE << "m" << b << "\033[0m";
}


template <typename T>
void print_colored_value(ft::node<T> *n, void (*printer)(T&)) {
	if (n) {
		std::cout << "\033[1;" << get_color(n) << "m";
		printer(n->value);
		std::cout << "\033[0m" << std::endl;
	}
	else
		std::cout << "." << std::endl;
}

template <typename T>
void print_node(ft::node<T> *n, void (*printer)(T&), int level) {
	if (level == 0) {
		print_colored_value(n, printer);
		return;
	}
	
	while (level > 1) {
		print_branch("|   ");
		level--;
	}

	print_branch("|___");
	print_colored_value(n, printer);
}

template <typename T>
void print_tree(ft::node<T> *n, void (*printer)(T&) = &integral_printer, int level = 0) {
	print_node(n, printer, level);

	if (n && (n->right || n->left)) {
		print_tree(n->right, printer, level + 1);
		print_tree(n->left, printer, level + 1);
	}
}

template <typename T, class U >
void print_tree(ft::B_TREE<T, U> & t, void (*printer)(T&) = &integral_printer) {
	print_tree(t.root(), printer);
}

#endif