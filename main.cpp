#include "vector.hpp"
#include "ft/reverse_iterator.hpp"
#include "ft/iterator_traits.hpp"
#include <vector>

#include "node.hpp"
#include "ft/make_pair.hpp"
#include "ft/pair.hpp"
#include "b_tree.hpp"
#include "print_tree.hpp"
#include "map.hpp"

int main() {
	ft::B_TREE< int, char > tree;
	ft::Tree_Printer< ft::integral_printer > printer;

	tree.insert( ft::make_pair(98, 'a') );
	tree.insert( ft::make_pair(8, 'b') );
	tree.insert( ft::make_pair(81, 'c') );
	tree.insert( ft::make_pair(90, 'd') );
	tree.insert( ft::make_pair(5, 'e') );
	tree.insert( ft::make_pair(23, 'f') );
	tree.insert( ft::make_pair(20, 'g') );
	tree.insert( ft::make_pair(52, 'h') );
	tree.insert( ft::make_pair(99, 'i') );
	tree.insert( ft::make_pair(2, 'j') );
	tree.insert( ft::make_pair(91, 'k') );
	tree.insert( ft::make_pair(105, 'k') );
	tree.insert( ft::make_pair(18, 'k') );
	tree.insert( ft::make_pair(0, 'k') );
	tree.insert( ft::make_pair(19, 'k') );
	tree.insert( ft::make_pair(1, 'k') );

	std::cout << "Before erasures" << std::endl;

	printer.print(tree);

	tree.erase(81);
	tree.erase(52);
	tree.erase(23);
	tree.erase(20);

	std::cout << std::endl << "After erasures" << std::endl;

	printer.print(tree);

	ft::map<int, bool> my_map;

	return 0;
}