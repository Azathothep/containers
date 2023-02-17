#ifndef NODE_HPP
# define NODE_HPP

# include <iostream>
# include "ft/is_integral.hpp"
# include "ft/enable_if.hpp"

# define BLACK "30"
# define RED "31"
# define GREEN "32"
# define YELLOW "33"
# define BLUE "34"
# define MAGENTA "35"
# define CYAN "36"
# define WHITE "37"

namespace ft {

	template <typename T>
	struct node {
		bool red;
		T value;
		struct node * parent;
		struct node * right;
		struct node * left;

		node(T const & v) : value(v), parent(NULL), right(NULL), left(NULL) { }
	};

	template <typename T>
	void integral_printer(T & value) {
		std::cout << value;
	}

	template <typename T>
	void print_tree(node<T> *n, void (*printer)(T&) = &integral_printer, int level = 0) {
		print_node(n, printer, level);

		if (n && (n->right || n->left)) {
			print_tree(n->right, printer, level + 1);
			print_tree(n->left, printer, level + 1);
		}
	}

	void print_branch(std::string b) {
		std::cout << "\033[1;" << WHITE << "m" << b << "\033[0m";
	}

	template <typename T>
	void print_node(node<T> *n, void (*printer)(T&), int level) {
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
	void print_colored_value(node<T> *n, void (*printer)(T&)) {
		if (n) {
			std::cout << "\033[1;" << get_color(n) << "m";
			printer(n->value);
			std::cout << "\033[0m" << std::endl;
		}
		else
			std::cout << "." << std::endl;
	}

	template <typename T>
	std::string get_color(node<T> *n) {
		if (n->red)
			return RED;
		return BLUE;
	}

}


#endif