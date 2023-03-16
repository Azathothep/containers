#ifndef PRINT_TREE_HPP
# define PRINT_TREE_HPP

# include <iostream>
# include "ft/is_integral.hpp"
# include "ft/enable_if.hpp"
# include "ft/pair.hpp"

# include "node.hpp"

# define P_BLACK "30"
# define P_RED "31"
# define P_GREEN "32"
# define P_YELLOW "33"
# define P_BLUE "34"
# define P_MAGENTA "35"
# define P_CYAN "36"
# define P_WHITE "37"

namespace ft {
	template <class Key, class T, class Compare, class Alloc>
	class B_TREE;

	template <class Key, class T, class Compare, class Alloc>
	class map;
}

namespace ft {
	class integral_printer {
		public:
			template <typename T>
			void out(T const & val, std::ostream & o = std::cout) {
				o << val;
			}

			template <typename T>
			void out(T const & value, std::ostream & o = std::cout, typename ft::enable_if< !ft::is_integral<T>::value >::type* = NULL) {
				(void)value;
				o << "Type not printable!";
			}
	};

	template <typename key_printer>
	class Tree_Printer {
		private:
			std::ostream	&_out;
			key_printer 		_key_printer;

		public:
			Tree_Printer(std::ostream & o = std::cout, key_printer const & k = integral_printer()) : _out(o), _key_printer(k) { }

			template <class T, class U, class V, class W>
			void print(ft::B_TREE<T, U, V, W> & t) {
				this->print(t.root());
			}

			template <class T, class U, class V, class W>
			void print(ft::map<T, U, V, W> & m) {
				this->print(m.root());
			}

			template <typename pair>
			void print(ft::node<pair> *n, int level = 0) {
				this->print_node(n, level);

				if (n && (n->right || n->left)) {
					this->print(n->right, level + 1);
					this->print(n->left, level + 1);
				}
			}

			template <typename T>
			void print_node(ft::node<T> *n, int level) {
				if (level == 0) {
					this->print_colored_value(n);
					return;
				}
				
				while (level > 1) {
					this->print_branch("|   ");
					level--;
				}

				this->print_branch("|___");
				this->print_colored_value(n);
			}

			template <typename T>
			void print_colored_value(ft::node<T> *n) {
				if (n) {
					if (_out == std::cout) _out << "\033[1;" << get_color(n) << "m";
					else _out << this->get_color_str(n) << ":";
					this->_key_printer.out(n->value.first, _out);
					if (_out == std::cout) _out << "\033[0m";
					_out << std::endl;
				}
				else
					_out << "." << std::endl;
			}

			void print_branch(std::string b) {
				if (_out == std::cout) _out << "\033[1;" << P_WHITE << "m";
				_out << b;
				if (_out == std::cout) _out << "\033[0m";
			}

			template <typename T>
			std::string get_color(ft::node<T> *n) {
				if (n->color == RED_NODE)
					return P_RED;
				return P_BLUE;
			}

			template <typename T>
			std::string get_color_str(ft::node<T> *n) {
				if (n->color == RED_NODE)
					return "R";
				return "B";
			}
	};
}

template <typename pair>
void pair_printer(pair & value, std::ostream & o) {
	o << value.first;
}

template <typename pair>
void pair_printer(pair & value, std::ostream & o, typename ft::enable_if< !ft::is_integral<typename pair::first_type>::value >::type* = NULL) {
	(void)value;
	o << "type not printable!";
}
#endif