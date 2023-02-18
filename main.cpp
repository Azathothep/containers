#include "vector.hpp"
#include "ft/reverse_iterator.hpp"
#include "ft/iterator_traits.hpp"
#include <vector>

#include "node.hpp"
#include "ft/make_pair.hpp"
#include "ft/pair.hpp"
#include "b_tree.hpp"
#include "print_tree.hpp"

template <class T>
struct integral_key_compare {
	bool operator() (const T& x, const T& y) const { return x.first < y.first; }
};

int main() {
	typedef ft::pair<int, bool> m_pair;

	ft::B_TREE< m_pair, integral_key_compare< m_pair > > tree;

	tree.insert( ft::make_pair<int, bool>(1, true) );
	tree.insert( ft::make_pair<int, bool>(2, true) );
	tree.insert( ft::make_pair<int, bool>(-1, true) );
	tree.insert( ft::make_pair<int, bool>(798, true) );
	tree.insert( ft::make_pair<int, bool>(-77, true) );
	tree.insert( ft::make_pair<int, bool>(-78, true) );
	tree.insert( ft::make_pair<int, bool>(-67, true) );

	print_tree(tree, &ft::pair_printer<m_pair>);

	return 0;
}