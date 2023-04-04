#ifndef BINARY_TREE_HPP
# define BINARY_TREE_HPP

# include "ft/pair.hpp"
# include "node.hpp"

namespace ft {

	template < class T, class Getter, class Compare, class Alloc = std::allocator < ft::node < T > > >
	class binary_tree {
		
		#define		KEY(v) this->_M_key.get(v)
		#define		NIL &(this->_nil)
		#define		GET_COLOR(n) (char)(((node *)n == NULL) ? BLACK_NODE : n->color)

		public:
			typedef T											value_type;
			typedef Getter										key_getter;
			typedef Alloc				 						allocator_type;
			typedef Compare 									compare_type;
			typedef typename key_getter::key_type				key_type;
			typedef typename ft::node< value_type > 			node;
			typedef typename allocator_type::size_type			size_type;

			int						size;

		private:
			key_getter				_M_key;
			compare_type			_M_comp;
			allocator_type		 	_M_alloc;

			node 					*_root;
			node					*_god_p;
			node					_god;
			node					_nil;

		public:

			binary_tree(const compare_type &comp = compare_type()) : size(0), _M_key(key_getter()), _M_comp(comp), _root(NULL) {
				this->_god_p = &this->_god;
			}

			~binary_tree() {
				this->destroy_all();
			}

			void swap(binary_tree & rhs) {
				node *t_root = this->_root;
				
				this->_root = rhs._root;
				rhs._root = t_root;

				this->_god.left = this->_root;
				rhs._god.left = rhs._root;

				this->_root->parent = this->_god_p;
				rhs._root->parent = rhs._god_p;

				int t_size = this->size;
				this->size = rhs.size;
				rhs.size = t_size;
			}

			void destroy_all() {
				if (this->_root)
					this->_move_destroy(this->_root);

				this->_god.left = NULL;
				this->_root = NULL;
			}

			node *get_node(key_type const & key) const {
				node *n = this->_root;

				return this->_get_node_recursive(key, n);
			}

			node *insert(value_type *val) {
				node *n = this->_create_node(val);

				if (this->_root) {
					this->_move_insert(&this->_root, n);
				}
				else {
					this->_root = n;
					this->_root->color = BLACK_NODE;

					this->_god.left = this->_root;
					this->_root->parent = &(this->_god);
				}

				size++;

				return n;
			}

			node *insert_from(value_type *val, value_type *from) {				
				node *to_insert;
				
				to_insert = this->get_node(KEY(from));
				
				if (to_insert && this->_M_comp(KEY(val), KEY(from))) {
					node *n = this->_create_node(val);
					this->_move_insert(&n, to_insert, to_insert->parent);
					return n;
				}

				return insert(val);
			}

			void erase(node *n) {
				this->_erase_node(n);
			}

			node *smallest() const {
				node *n = this->_root;

				if (n == NULL)
					return past_the_end();
				
				if (n->left) {
					n = n->left;
					while (n->left)
						n = n->left;
				}

				return n;
			}

			node *past_the_end() const {
				return this->_god_p;
			}

			size_type max_size() const { return this->_M_alloc.max_size(); }

		private:

		/* #region utils */

			node *_get_node_recursive(key_type const & key, node *n) const {
				if (n == NULL)
					return n;

				if (this->_M_comp(key, KEY(n->value))) // if key < n
					return this->_get_node_recursive(key, n->left);
				else if (this->_M_comp(KEY(n->value), key)) // if key > n
					return this->_get_node_recursive(key, n->right);

				return n;
			}

			void _replace(node *n, node *r) {
				node *p = n->parent;
				
				if (p == this->_god_p) {
					this->_root = r;
					this->_god.left = this->_root;
				} else if (n == p->left)
					p->left = r;
				else if (n == p->right)
					p->right = r;

				if (r)
					r->parent = p;
			}

			void _swap_colors(node *n1, node *n2) {
				int temp = n1->color;
				n1->color = n2->color;
				n2->color = temp;
			}

			void _delete_allocated(node *n) {
				_M_alloc.destroy(n);
				_M_alloc.deallocate(n, 1);
				n = NULL;
				size--;
			}

			node *_create_node(value_type *val) {
				node *n = _M_alloc.allocate(1);
				_M_alloc.construct(n, node(val));
				return n;
			}

			void _move_destroy(node *current) {
				if (current->left)
					_move_destroy(current->left);
				if (current->right)
					_move_destroy(current->right);
				
				this->_delete_allocated(current);
			}

		/* #endregion */

		/* #region insertion */

			void _move_insert(node **current, node *insert, node *parent = NULL) {
				
				if (*current == NULL) {
					this->_emplace_node(current, insert, parent);
					return;
				}
				
				if (this->_M_comp(KEY((*current)->value), KEY(insert->value))) // if current < insert
					this->_move_insert(&((*current)->right), insert, *current);
				else
					this->_move_insert(&((*current)->left), insert, *current);
			}

			void _emplace_node(node **current, node *insert, node *parent) {
				*current = insert;
				insert->parent = parent;

				this->_recolor(insert);
			}

			void _recolor(node *n) {
				if (n == this->_root) {
					n->color = BLACK_NODE;
					return;
			 	} 
				
				if (GET_COLOR(n->parent) == BLACK_NODE)
					return;

				if (n->grandparent() == NULL || n->grandparent() == &(this->_god))
					return;

				if (GET_COLOR(n->aunt()) == RED_NODE)
				{
					n->parent->color = BLACK_NODE;
					n->aunt()->color = BLACK_NODE;
					n->grandparent()->color = RED_NODE;

					this->_recolor(n->grandparent());
				}
				else {
					this->_perform_rotation(n);
				}
			}

			void _perform_rotation(node *n) {
				node *p = n->parent;
				node *g = n->grandparent();

				if (p == g->left && n == p->left)
				{
					this->_rotate_right(g);
					this->_swap_colors(g, p);
				} else if (p == g->right && n == p->right) {	
					this->_rotate_left(g);
					this->_swap_colors(g, p);
				} else if (p == g->left && n == p->right) {
					this->_rotate_left(p);
					this->_rotate_right(g);
					this->_swap_colors(g, n);
				} else if (p == g->right && n == p->left) {
					this->_rotate_right(p);
					this->_rotate_left(g);
					this->_swap_colors(g, n);
				}
			}

		/* #endregion */

		/* #region rotations */

			void _rotate_left(node *n) {
				node *traitor = n->right;
				node *vassal = traitor->left;

				this->_replace(n, traitor);
				n->parent = traitor;
				traitor->left = n;
				n->right = vassal;
				if (vassal)
					vassal->parent = n;
			}

			void _rotate_right(node *n) {
				node *traitor = n->left;
				node *vassal = traitor->right;

				this->_replace(n, traitor);
				n->parent = traitor;
				traitor->right = n;
				n->left = vassal;
				if (vassal)
					vassal->parent = n;
			}

		/* #endregion */

		/* #region erasure */

			void _erase_node(node *n) {
				char originalColor = n->color;

				node *heir = this->_delete_node(n, &originalColor);

				if (originalColor == BLACK_NODE) {
					if (GET_COLOR(heir) == RED_NODE)
						heir->color = BLACK_NODE;
					else if (heir == this->_root)
						heir->color = BLACK_NODE;
					else
						this->_solve_double_black(heir);
				}

				if (heir == NIL)
					this->_replace(heir, NULL);
			}

			node *_delete_node(node *n, char *originalColor) {
				if (!n->left && !n->right)
					return _delete_leaf(n);
				else if (!n->left || !n->right)
					return _delete_one_child(n);
				return _delete_two_children(n, originalColor);
			}

			node *_delete_leaf(node *n) {
				this->_replace(n, NIL);
				this->_delete_allocated(n);

				return NIL;
			}

			node *_delete_one_child(node *n) {
				node *child = n->left;
				if (n->left == NULL)
					child = n->right;
				this->_replace(n, child);
				this->_delete_allocated(n);

				return child;
			}

			node *_delete_two_children(node *n, char *originalColor) {
				node *subnode = n->left; // n->right

				while (subnode->right) // subnode->left
					subnode = subnode->right; // subnode->left

				node *heir = subnode->left; // subnode->right
				if (heir == NULL)
					heir = NIL;
				this->_replace(subnode, heir);

				this->_replace(n, subnode);
				this->_inherit(n, subnode);
				
				*originalColor = subnode->color; // TO VERIFY

				subnode->color = n->color;

				this->_delete_allocated(n);

				return heir; //is that ok ?
			}

			void _inherit(node *n, node *heir) {
				if (!n || !heir)
					return;
				
				if (heir != n->right) {
					heir->right = n->right;
					if (heir->right) heir->right->parent = heir; // check
				}

				if (heir != n->left) {
					heir->left = n->left;
					if (heir->left) heir->left->parent = heir;
				}
			}

			/* #region double black */

			void _solve_double_black(node *db) {
				node *sibling = db->sibling();
				bool child_status = db->child_status();

				if (sibling == NULL || db == this->_root) // check for this case
					return;

				if (GET_COLOR(sibling) == RED_NODE)
					this->_red_sibling_case(db, child_status);
				else {
					if (GET_COLOR(sibling->right) == BLACK_NODE && GET_COLOR(sibling->left) == BLACK_NODE)
						this->_black_family_case(db);
					else {
						if (child_status == LEFT_CHILD) {
							if (GET_COLOR(sibling->right) == BLACK_NODE)
								this->_close_nephew_red_case(db, child_status);
							this->_far_nephew_red_case(db, child_status);
						} else {
							if (GET_COLOR(sibling->left) == BLACK_NODE)
								this->_close_nephew_red_case(db, child_status);
							this->_far_nephew_red_case(db, child_status);
						}
					}
				}
			}

			void _red_sibling_case(node *db, bool child_status) {
				node *sibling = db->sibling();
				node *parent = db->parent;

				this->_swap_colors(sibling, parent);

				if (child_status == LEFT_CHILD)
					this->_rotate_left(parent);
				else
					this->_rotate_right(parent);

				this->_solve_double_black(db);
			}

			void _black_family_case(node *db) {
				node *sibling = db->sibling();
				node *parent = db->parent;

				sibling->color = RED_NODE;
				if (GET_COLOR(parent) == RED_NODE)
					parent->color = BLACK_NODE;
				else
					this->_solve_double_black(parent);
			}

			void _close_nephew_red_case(node *db, bool child_status) {
				node *sibling = db->sibling();

				if (child_status == LEFT_CHILD) {
					this->_swap_colors(sibling, sibling->left);
					this->_rotate_right(sibling);
				} else {
					this->_swap_colors(sibling, sibling->right);
					this->_rotate_left(sibling);
				}
			}

			void _far_nephew_red_case(node *db, bool child_status) {
				node *sibling = db->sibling();
				node *parent = db->parent;

				sibling->color = parent->color;
				parent->color = BLACK_NODE;

				if (child_status == LEFT_CHILD) {
					sibling->right->color = BLACK_NODE;
					this->_rotate_left(parent);
				} else {
					sibling->left->color = BLACK_NODE;
					this->_rotate_right(parent);
				}
			}
			
			/* #endregion */
		
		/* #endregion */
	};

}


#endif