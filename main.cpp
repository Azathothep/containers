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
#include <map>

#include <time.h>

#define NS ft

int main() {

	NS::map<int, char> my_map;
	//ft::Tree_Printer< ft::integral_printer > printer;

	clock_t start = clock();
	
	my_map.insert( NS::make_pair(98, 'a') );
	my_map.insert( NS::make_pair(8, 'b') );
	my_map.insert( NS::make_pair(81, 'c') );
	my_map.insert( NS::make_pair(90, 'd') );
	my_map.insert( NS::make_pair(5, 'e') );
	my_map.insert( NS::make_pair(23, 'f') );
	my_map.insert( NS::make_pair(20, 'g') );
	my_map.insert( NS::make_pair(52, 'h') );
	my_map.insert( NS::make_pair(99, 'i') );
	my_map.insert( NS::make_pair(2, 'j') );
	my_map.insert( NS::make_pair(91, 'k') );
	my_map.insert( NS::make_pair(105, 'l') );
	my_map.insert( NS::make_pair(18, 'm') );
	my_map.insert( NS::make_pair(1, 'n') );
	my_map.insert( NS::make_pair(19, 'o') );
	my_map.insert( NS::make_pair(0, 'p') );
	my_map.insert( NS::make_pair(4, 'q') );

	clock_t end = clock();

	std::cout << "Measured time: " << end - start << std::endl;;

	// std::cout << "Before erasures" << std::endl;

	// printer.print(my_map);

	// my_map.erase(81);
	// my_map.erase(52);
	// my_map.erase(23);
	// my_map.erase(20);

	// std::cout << std::endl << "ANSer erasures" << std::endl;

	//printer.print(my_map);

	// NS::map<int, char>::iterator it = my_map.begin();
	// NS::map<int, char>::iterator it2;
	// NS::map<int, char>::iterator ite = my_map.end();

	// for (; it != ite; it++)
	// {
	// 	it2 = it;
	// 	std::cout << "iterator: " << (*it).first << ", " << (*it).second << std::endl;
	// 	it2++;
	// 	if (it2 != ite)
	// 		my_map.erase((*it2).first);
	// }

	// //printer.print(my_map);

	// std::cout << std::endl << "Testing copy -------------------------------------" << std::endl << std::endl;

	// NS::map<int, char> sec_map;

	// std::cout << "emptiness = " << sec_map.empty() << std::endl;
	// std::cout << "size = " << sec_map.size() << std::endl;
	// std::cout << "maxsize = " << sec_map.max_size() << std::endl;

	// sec_map = my_map;

	// it = sec_map.begin();
	// ite = sec_map.end();

	// std::cout << "emptiness = " << sec_map.empty() << std::endl;
	// std::cout << "size = " << sec_map.size() << std::endl;

	// for (; it != ite; it++)
	// {
	// 	std::cout << "iterator with []: " << (*it).first << ", " << sec_map[(*it).first] << std::endl;
	// 	std::cout << "iterator with at: " << (*it).first << ", " << sec_map.at((*it).first) << std::endl;
	// }

	// try {
	// 	std::cout << "iterator at end: " << sec_map.at(189);
	// }
	// catch (std::exception &e) {
	// 	std::cout << e.what() << std::endl;
	// }

	return 0;
}