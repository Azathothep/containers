#include "vector.hpp"
#include "ft/reverse_iterator.hpp"
#include "ft/iterator_traits.hpp"
#include <vector>

#include "node.hpp"
#include "ft/make_pair.hpp"
#include "ft/pair.hpp"
#include "b_tree.hpp"

int main() {
	ft::node< ft::pair<int, bool> > n(ft::make_pair<int, bool>(0, true));
	ft::node< ft::pair<int, bool> > n1(ft::make_pair<int, bool>(1, true));
	ft::node< ft::pair<int, bool> > n2(ft::make_pair<int, bool>(2, true));
	ft::node< ft::pair<int, bool> > n3(ft::make_pair<int, bool>(3, true));
	ft::node< ft::pair<int, bool> > n4(ft::make_pair<int, bool>(4, true));
	ft::node< ft::pair<int, bool> > n5(ft::make_pair<int, bool>(5, true));

	n.red = false;
	n1.red = true;
	n2.red = false;
	n3.red = true;
	n4.red = false;
	n5.red = true;

	n.right = &n1;
	n1.right = &n2;
	n.left = &n3;
	n3.right = &n4;
	n4.left = &n2;

	ft::print_tree(&n, &ft::pair_printer);

	ft::B_TREE< int > tree( 0 );

	return 0;
}