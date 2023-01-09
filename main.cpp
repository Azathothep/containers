#include "srcs/vector/vector.hpp"
#include "srcs/std/reverse_iterator.hpp"
#include "srcs/std/iterator_traits.hpp"
#include <vector>

int main(int argc, char **argv) {
	
	std::ofstream log;
	log.open("log.txt", std::ofstream::out | std::ofstream::trunc);
	log.close();

	{
		std::cout << "vector<int>" << std::endl;
	
		ft::vector<int> v;

		std::cout << "size = " << v.size() << " & capacity = " << v.capacity() << std::endl;

		v.push_back(0);
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);
		v.pop_back();
		v.push_back(5);
		v.push_back(6);
		v.push_back(7);

		std::cout << "const_iterator" << std::endl;

		ft::vector<int>::const_iterator cit;

		for (cit = v.cbegin(); cit != v.cend(); cit++)
			std::cout << *cit << std::endl;

		std::cout << "size = " << v.size() << " & capacity = " << v.capacity() << std::endl;

		std::cout << "reverse_iterator" << std::endl;

		ft::vector<int>::reverse_iterator rit;

		for (rit = v.rbegin(); rit != v.rend(); rit++)
			std::cout << *rit << std::endl;
	}

	{
		// std::cout << std::endl << "vector< vector<int> >" << std::endl << std::endl;

		// std::vector<int> v(1);

		// ft::vector< std::vector<int> > vov(5);

		// vov[0] = v;
		// vov[1] = v;
		// vov[2] = v;
		// vov[3] = v;
		// vov[4] = v;

		// ft::vector< std::vector<int> >::iterator it;

		// for (it = vov.begin(); it != vov.end(); it++)
		// 	std::cout << (*it)[0] << std::endl;
	}


	return 0;
}