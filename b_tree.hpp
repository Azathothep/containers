#ifndef B_TREE_HPP
# define B_TREE_HPP

# include "node.hpp"

namespace ft {
	template <typename T, class Compare = std::less<T>, class Alloc = std::allocator<ft::node<T> > >
	class B_TREE {
		public:
			typedef T 								value_type;
			typedef Alloc 							allocator_type;
			typedef Compare 						compare;
			typedef typename ft::node<value_type> 	node;

		private:
			ft::node< value_type > *_root;
			allocator_type 			_M_alloc;

		public:
			B_TREE() : _root(NULL) { }

			B_TREE(value_type const & val) {
				this->_root = _M_alloc.allocate(1);
				_M_alloc.construct(this->_root, node(val));

				ft::print_tree(_root);
			}

			~B_TREE() {
				_M_alloc.destroy(this->_root);
				_M_alloc.deallocate(this->_root, 1);
			}
	};
}


#endif