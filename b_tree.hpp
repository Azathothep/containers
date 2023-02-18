# include "Debug.hpp"

#ifndef B_TREE_HPP
# define B_TREE_HPP

# include "node.hpp"

namespace ft {
	template <typename T, class Compare = std::less<T>, class Alloc = std::allocator<ft::node<T> > >
	class B_TREE {
		public:
			typedef T 								value_type;
			typedef Alloc 							allocator_type;
			typedef Compare 						compare_type;
			typedef typename ft::node<value_type> 	node;

		private:
			node 					*_root;
			allocator_type 			_M_alloc;
			compare_type			_M_comp;

		public:
			B_TREE() : _root(NULL) { }

			B_TREE(value_type const & val, const compare_type &comp = compare_type()) {
				this->_M_comp = comp;

				this->_root = this->_create_node(val);
			}

			~B_TREE() {
				_M_alloc.destroy(this->_root);
				_M_alloc.deallocate(this->_root, 1);
			}

			node *root() { return this->_root; }

			void insert(value_type const & val) {
				node * n = this->_create_node(val);

				this->_move_insert(&this->_root, n);
			}

		private:
			node *_create_node(value_type const & val) {
				node *n = _M_alloc.allocate(1);
				_M_alloc.construct(n, node(val));
				return n;
			}

			void _move_insert(node **current, node *insert, node *parent = NULL) {
				if (*current == NULL) {
					*current = insert;
					insert->parent = parent;
					this->_color_node(insert);
					return;
				}
				
				if (this->_M_comp((*current)->value, insert->value))
					this->_move_insert(&((*current)->left), insert, *current);
				else
					this->_move_insert(&((*current)->right), insert, *current);
			}

			void _color_node(node *n) {
				n->color = !n->parent->color;
			}
	};
}


#endif