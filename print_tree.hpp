#ifndef PRINT_TREE_HPP
# define PRINT_TREE_HPP

# include <iostream>
# include "ft/is_integral.hpp"
# include "ft/enable_if.hpp"

# include "node.hpp"
//# include "b_tree.hpp"

# define P_BLACK "30"
# define P_RED "31"
# define P_GREEN "32"
# define P_YELLOW "33"
# define P_BLUE "34"
# define P_MAGENTA "35"
# define P_CYAN "36"
# define P_WHITE "37"

namespace ft {
	template <typename T, class Compare, class Alloc>
	class B_TREE;
}

template <typename T>
void integral_printer(T & value, std::ostream & o) {
	o << value;
}

template <typename T>
std::string get_color(ft::node<T> *n) {
	if (n->color == RED)
		return P_RED;
	return P_BLUE;
}

template <typename T>
std::string get_color_str(ft::node<T> *n) {
	if (n->color == RED)
		return "R";
	return "B";
}

void print_branch(std::string b, std::ostream & o) {
	if (o == std::cout) o << "\033[1;" << P_WHITE << "m";
	o << b;
	if (o == std::cout) o << "\033[0m";
}


template <typename T>
void print_colored_value(ft::node<T> *n, std::ostream & o, void (*printer)(T&, std::ostream &)) {
	if (n) {
		if (o == std::cout) o << "\033[1;" << get_color(n) << "m";
		else o << get_color_str(n) << ":";
		printer(n->value, o);
		if (o == std::cout) o << "\033[0m";
		o << std::endl;
	}
	else
		o << "." << std::endl;
}

template <typename T>
void print_node(ft::node<T> *n, std::ostream & o, void (*printer)(T&, std::ostream &), int level) {
	if (level == 0) {
		print_colored_value(n, o, printer);
		return;
	}
	
	while (level > 1) {
		print_branch("|   ", o);
		level--;
	}

	print_branch("|___", o);
	print_colored_value(n, o, printer);
}

template <typename T>
void print_tree(ft::node<T> *n, std::ostream & o = std::cout, void (*printer)(T&, std::ostream &) = &integral_printer, int level = 0) {
	print_node(n, o, printer, level);

	if (n && (n->right || n->left)) {
		print_tree(n->right, o, printer, level + 1);
		print_tree(n->left, o, printer, level + 1);
	}
}

template <typename T, class U, class V >
void print_tree(ft::B_TREE<T, U, V> & t, std::ostream & o = std::cout, void (*printer)(T&, std::ostream &) = &integral_printer) {
	print_tree(t.root(), o, printer);
}

#endif