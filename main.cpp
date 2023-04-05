//define DEBUG_MODE

#include "vector.hpp"
#include "ft/reverse_iterator.hpp"
#include "ft/iterator_traits.hpp"
#include <vector>

#include "ft/make_pair.hpp"
#include "ft/pair.hpp"
#include "map.hpp"
#include "set.hpp"
#include <map>
#include <set>

#include <time.h>

#include <iostream>

#define NS ft

int main() {

	clock_t start = clock();

	NS::set<int> my_set;

	my_set.insert(78);
	my_set.insert(-6);
	my_set.insert(0);
	my_set.insert(9);
	my_set.insert(58);
	my_set.insert(1119);

	NS::set<int>::iterator it = my_set.begin();
	NS::set<int>::iterator ite = my_set.end();

	for(; it != ite; it++)
		std::cout << "Iterator value: " << *it << std::endl;

	clock_t end = clock();

	std::cout << "Measured time: " << end - start << std::endl;

	return 0;
}