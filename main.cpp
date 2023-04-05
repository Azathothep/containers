//define DEBUG_MODE

#include "vector.hpp"
#include "ft/reverse_iterator.hpp"
#include "ft/iterator_traits.hpp"
#include <vector>

#include "ft/make_pair.hpp"
#include "ft/pair.hpp"
#include "map.hpp"
#include "set.hpp"
#include "stack.hpp"

#include <map>
#include <set>
#include <stack>

#include <time.h>

#include <iostream>

#define NS ft

int main() {

	clock_t start = clock();

	NS::stack<int> my_stack;

	my_stack.push(78);
	my_stack.push(-6);
	my_stack.push(0);
	my_stack.push(9);
	my_stack.push(58);
	my_stack.push(1119);

	while (my_stack.empty() == false) {
		std::cout << "Stack top: " << my_stack.top() << std::endl;
		my_stack.pop();
	}

	clock_t end = clock();

	std::cout << "Measured time: " << end - start << std::endl;

	return 0;
}