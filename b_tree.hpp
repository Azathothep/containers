# include "Debug.hpp"

#ifndef B_TREE_HPP
# define B_TREE_HPP

# include "node.hpp"
# include "print_tree.hpp"

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
			B_TREE(const compare_type &comp = compare_type()) : _root(NULL) {
				this->_M_comp = comp;
			}

			B_TREE(value_type const & val, const compare_type &comp = compare_type()) {
				this->_M_comp = comp;

				this->_root = this->_create_node(val);
				this->_root->color = BLACK;
			}

			~B_TREE() {
				this->_move_destroy(this->_root);
			}

			void _move_destroy(node *current) {
				if (current->left)
					_move_destroy(current->left);
				if (current->right)
					_move_destroy(current->right);
				
				this->_delete(current);
			}

			node *root() { return this->_root; }

			void insert(value_type const & val) {
				node * n = this->_create_node(val);

				if (this->_root)
					this->_move_insert(&this->_root, n);
				else
				{
					this->_root = n;
					this->_root->color = BLACK;
				}
			}

		private:
			node *_create_node(value_type const & val) {
				node *n = _M_alloc.allocate(1);
				_M_alloc.construct(n, node(val));
				return n;
			}

			void _move_insert(node **current, node *insert, node *parent = NULL) {
				if (*current == NULL) {
					this->_emplace_node(current, insert, parent);
					return;
				}
				
				if (this->_M_comp((*current)->value, insert->value))
					this->_move_insert(&((*current)->right), insert, *current);
				else
					this->_move_insert(&((*current)->left), insert, *current);
			}

			void _emplace_node(node **current, node *insert, node *parent) {
				*current = insert;
				insert->parent = parent;
				Debug::Log << "Map: inserting element" << std::endl;
				print_tree(*this, Debug::Log, &ft::pair_printer<value_type>);
				this->_recolor(insert);
			}

			void _recolor(node *n) {
				if (n == this->_root) {
					Debug::Log << "Map: recolouring root" << std::endl;
					n->color = BLACK;
					this->_print_tree();
					return;
			 	} 
				
				if (n->parent->color == BLACK)
					return;

				if (n->grandparent() == NULL)
					return;
				
				if (n->aunt() && n->aunt()->color == RED)
				{
					Debug::Log << "Map: recolouring " << n->value.first << ", aunt is RED" << std::endl;
					n->parent->color = BLACK;
					n->aunt()->color = BLACK;
					n->grandparent()->color = RED;
					this->_print_tree();
					this->_recolor(n->grandparent());
				}
				else {
					Debug::Log << "Map: need rotation from " << n->value.first << std::endl;
					this->_perform_rotation(n);
				}
			}

			void _perform_rotation(node *n) {
				node *p = n->parent;
				node *g = n->grandparent();

				this->_print_tree();

				if (p == g->left && n == p->left)
				{
					Debug::Log << "DETECTING LL CASE" << std::endl;
					this->_rotate_right(g);
					this->_swap_colors(g, p);
				}
				else if (p == g->right && n == p->right)
				{
					Debug::Log << "DETECTING RR CASE" << std::endl;
					this->_rotate_left(g);
					this->_swap_colors(g, p);
				}
				else if (p == g->left && n == p->right)
				{
					Debug::Log << "DETECTING LR CASE" << std::endl;
					this->_rotate_left(p);
					this->_rotate_right(g);
					this->_swap_colors(g, n);
				}
				else if (p == g->right && n == p->left)
				{
					Debug::Log << "DETECTING RL CASE" << std::endl;
					this->_rotate_right(p);
					this->_rotate_left(g);
					this->_swap_colors(g, n);
				}
				else
					Debug::Log << "No case found, " << p->value.first << ",  " << g->value.first << std::endl;

				this->_print_tree();

				//then recolour
			}

			void _rotate_left(node *n) {
				Debug::Log << "Map: rotate left" << std::endl;
				node *traitor = n->right;
				node *vassal = traitor->left;

				this->_replace(n, traitor);
				n->parent = traitor;
				traitor->left = n;
				n->right = vassal;
				if (vassal)
					vassal->parent = n;

				this->_print_tree();
				
			}

			void _rotate_right(node *n) {
				Debug::Log << "Map: rotate right" << std::endl;
				node *traitor = n->left;
				node *vassal = traitor->right;

				this->_replace(n, traitor);
				n->parent = traitor;
				traitor->right = n;
				n->left = vassal;
				if (vassal)
					vassal->parent = n;

				this->_print_tree();
			}

			void _replace(node *n, node *r) {
				if (n == this->_root)
				{
					this->_root = r;
					r->parent = NULL;
					return;
				}
				
				node *p = n->parent;

				if (p == NULL)
					return;

				if (n == p->left)
				{
					p->left = r;
					r->parent = p;
				}
				else if (n == p->right)
				{
					p->right = r;
					r->parent = p;
				}
			}

			void _swap_colors(node *n1, node *n2) {
				int temp = n1->color;
				n1->color = n2->color;
				n2->color = temp;
			}

			void _delete(node *n) {
				_M_alloc.destroy(n);
				_M_alloc.deallocate(n, 1);
			}

			void _delete_node(node *n) {
				Debug::Log << "Deleting node" << std::endl;

				if (!n->left && !n->right)
					_delete_leaf(n);
				else if (!n->left || !n->right)
					_delete_one_child(n);
				else
					_delete_two_children(n);
			}

			void _delete_leaf(node *n) {
				Debug::Log << "Deleting leaf" << std::endl;
			}

			void _delete_one_child(node *n) {
				Debug::Log << "Deleting one child" << std::endl;
			}

			void _delete_two_children(node *n) {
				Debug::Log << "Deleting two children" << std::endl;
			}

			void _print_tree() {
				print_tree(*this, Debug::Log, &ft::pair_printer<value_type>);
			}
	};
}


#endif