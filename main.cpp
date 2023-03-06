#include "vector.hpp"
#include "ft/reverse_iterator.hpp"
#include "ft/iterator_traits.hpp"
#include <vector>

#include "node.hpp"
#include "ft/make_pair.hpp"
#include "ft/pair.hpp"
#include "b_tree.hpp"
#include "print_tree.hpp"

int main() {
	ft::B_TREE< int, bool > tree;
	ft::Tree_Printer< ft::integral_printer > printer;

	tree.insert( ft::make_pair(98, true) );
	tree.insert( ft::make_pair(8, true) );
	tree.insert( ft::make_pair(81, true) );
	tree.insert( ft::make_pair(90, true) );
	tree.insert( ft::make_pair(-2, true) );
	tree.insert( ft::make_pair(23, true) );
	tree.insert( ft::make_pair(20, true) );
	tree.insert( ft::make_pair(52, true) );
	tree.insert( ft::make_pair(99, true) );

	printer.print(tree);

	return 0;
}