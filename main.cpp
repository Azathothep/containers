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
	ft::map<int, char> my_map;
	ft::Tree_Printer< ft::integral_printer > printer;

	my_map.insert( ft::make_pair(98, 'a') );
	my_map.insert( ft::make_pair(8, 'b') );
	my_map.insert( ft::make_pair(81, 'c') );
	my_map.insert( ft::make_pair(90, 'd') );
	my_map.insert( ft::make_pair(5, 'e') );
	my_map.insert( ft::make_pair(23, 'f') );
	my_map.insert( ft::make_pair(20, 'g') );
	my_map.insert( ft::make_pair(52, 'h') );
	my_map.insert( ft::make_pair(99, 'i') );
	my_map.insert( ft::make_pair(2, 'j') );
	my_map.insert( ft::make_pair(91, 'k') );
	my_map.insert( ft::make_pair(105, 'l') );
	my_map.insert( ft::make_pair(18, 'm') );
	my_map.insert( ft::make_pair(1, 'n') );
	my_map.insert( ft::make_pair(19, 'o') );
	my_map.insert( ft::make_pair(0, 'p') );
	my_map.insert( ft::make_pair(4, 'q') );

	std::cout << "Before erasures" << std::endl;

	printer.print(my_map);

	// my_map.erase(81);
	// my_map.erase(52);
	// my_map.erase(23);
	// my_map.erase(20);

	std::cout << std::endl << "After erasures" << std::endl;

	printer.print(my_map);

	ft::map<int, char>::iterator it = my_map.begin();
	ft::map<int, char>::iterator end = it;
	ft::map<int, char>::iterator ite = my_map.end();

	for (; it != ite; it++)
	{
		end = it;
		std::cout << std::endl << "iterator: " << (*it).first << ", " << (*it).second << std::endl;
	}

	std::cout << std::endl << "---------------------" << std::endl;

	it = my_map.begin();

	for (; end != it; end--)
	{
		std::cout << std::endl << "iterator: " << (*end).first << ", " << (*end).second << std::endl;
	}

	// ft::map<int, char>::reverse_iterator rit = my_map.rbegin();
	// ft::map<int, char>::reverse_iterator rite = my_map.rend();

	// for (; rit != rite; --rit)
	// {
	// 	std::cout << std::endl << "iterator: " << (*rit).first << ", " << (*rit).second << std::endl;
	// }

	return 0;
}