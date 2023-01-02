#include "srcs/vector/vector.hpp"

int main(int argc, char **argv) {
	ft::vector<int> v(5);

	ft::vector<int>::iterator it;

	for (it = v.begin(); it != v.end(); it++)
		std::cout << *it << std::endl;

	return 0;
}