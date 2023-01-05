#include "srcs/vector/vector.hpp"
#include <vector>

int main(int argc, char **argv) {
	
	{
		std::cout << "vector<int>" << std::endl;
	
		ft::vector<int> v(1);

		v.push_back(2);
		v.push_back(3);
		v.push_back(4);
		v.push_back(5);
		v.push_back(6);

		ft::vector<int>::const_iterator cit;

		for (cit = v.cbegin(); cit != v.cend(); cit++)
			std::cout << *cit << std::endl;
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