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
	ft::B_TREE< int, char > tree;
	ft::Tree_Printer< ft::integral_printer > printer;

	// tree.insert( ft::make_pair(98, 'a') );
	// tree.insert( ft::make_pair(8, 'b') );
	// tree.insert( ft::make_pair(81, 'c') );
	// tree.insert( ft::make_pair(90, 'd') );
	// tree.insert( ft::make_pair(-2, 'e') );
	// tree.insert( ft::make_pair(23, 'f') );
	// tree.insert( ft::make_pair(20, 'g') );
	// tree.insert( ft::make_pair(52, 'h') );
	// tree.insert( ft::make_pair(99, 'i') );
	// tree.insert( ft::make_pair(-4, 'j') );
	tree.insert( ft::make_pair(20, 'g') );
	tree.insert( ft::make_pair(10, 'h') );
	tree.insert( ft::make_pair(30, 'i') );
		tree.insert( ft::make_pair(25, 'j') );
	tree.insert( ft::make_pair(35, 'j') );

	std::cout << "Before erasures" << std::endl;

	printer.print(tree);

	int key =  10;

	ft::pair<int, int> p = tree.get_pair(key);

	std::cout << "p value is: " << (char)p.second << std::endl;

	tree.erase(key);

	std::cout << std::endl << "After erasures" << std::endl;

	printer.print(tree);

	return 0;
}