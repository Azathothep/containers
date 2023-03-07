# include "Debug.hpp"

#ifndef B_TREE_HPP
# define B_TREE_HPP

# include "node.hpp"
# include "print_tree.hpp"

namespace ft {

	template <class Key, class T, class Compare = std::less< Key >, class Alloc = std::allocator< ft::node< ft::pair<Key, T> > > >
	class B_TREE {
		#define		KEY(n) ((node *)n)->value.first
		#define		NIL &(this->_nil)
		#define 	PRINT_TREE _printer.print(*this)
		#define		GET_COLOR(n) (char)(((node *)n == NULL) ? BLACK_NODE : n->color)

		#define		LEFT_CHILD true
		#define		RIGHT_CHILD false
		#define		CHILD_STATUS(n) (bool)((node *)n == (node *)n->parent->left)

		public:
			typedef Key									key_type;
			typedef T									value_type;
			typedef ft::pair< key_type, value_type > 	pair_type;
			typedef Alloc 								allocator_type;
			typedef Compare 							compare_type;
			typedef typename ft::node< pair_type > 		node;

		private:
			node 					*_root;
			node					_nil;
			allocator_type 			_M_alloc;
			compare_type			_M_comp;

			ft::Tree_Printer<ft::integral_printer> 		_printer;

		public:
			B_TREE(const compare_type &comp = compare_type()) : _root(NULL), _printer(Debug::Log) {
				this->_M_comp = comp;
			}

			B_TREE(pair_type const & val, const compare_type &comp = compare_type()) : _printer(Debug::Log) {
				this->_M_comp = comp;

				this->_root = this->_create_node(val);
				this->_root->color = BLACK_NODE;
			}

			~B_TREE() {
				if (this->_root)
					this->_move_destroy(this->_root);
			}

			void _move_destroy(node *current) {
				if (current->left)
					_move_destroy(current->left);
				if (current->right)
					_move_destroy(current->right);
				
				this->_delete_allocated(current);
			}

			node *root() { return this->_root; }

			pair_type const & get_pair(key_type const & key) {
				node *n = get_node(key, this->root());
				
				return n->value;
			}

			node *get_node(key_type const & key, node *n) {
				Debug::Log << "B_Tree: foward_node: checking node of key " << n->value.first << std::endl;

				if (this->_M_comp(key, KEY(n))) // if key < n
					return get_node(key, n->left);
				else if (this->_M_comp(KEY(n), key)) // if key > n
					return get_node(key, n->right);
				return n;
			}

			void insert(pair_type const & val) {
				node * n = this->_create_node(val);

				if (this->_root)
					this->_move_insert(&this->_root, n);
				else
				{
					this->_root = n;
					this->_root->color = BLACK_NODE;
				}
			}

			void erase(key_type const & key) {
				node *n = get_node(key, this->root());

				this->_erase_node(n);
			}

		private:

		/* #region utils */

			void _replace(node *n, node *r) {
				node *p = n->parent;
				
				if (p == NULL)
					this->_root = r;
				else if (n == p->left)
					p->left = r;
				else if (n == p->right)
					p->right = r;

				if (r) r->parent = p;
			}

			void _swap_colors(node *n1, node *n2) {
				int temp = n1->color;
				n1->color = n2->color;
				n2->color = temp;
			}

			void _delete_allocated(node *n) {
				_M_alloc.destroy(n);
				_M_alloc.deallocate(n, 1);
			}

			node *_create_node(pair_type const & val) {
				node *n = _M_alloc.allocate(1);
				_M_alloc.construct(n, node(val));
				return n;
			}

		/* #endregion */

		/* #region insertion */

			void _move_insert(node **current, node *insert, node *parent = NULL) {
				if (*current == NULL) {
					this->_emplace_node(current, insert, parent);
					return;
				}
				
				if (this->_M_comp(KEY(*current), KEY(insert))) // if current < insert
					this->_move_insert(&((*current)->right), insert, *current);
				else
					this->_move_insert(&((*current)->left), insert, *current);
			}

			void _emplace_node(node **current, node *insert, node *parent) {
				*current = insert;
				insert->parent = parent;
				Debug::Log << "Map: inserting element" << std::endl;
				this->_recolor(insert);
				PRINT_TREE;
			}

			void _recolor(node *n) {
				if (n == this->_root) {
					Debug::Log << "Map: recolouring root" << std::endl;
					n->color = BLACK_NODE;
					PRINT_TREE;
					return;
			 	} 
				
				if (GET_COLOR(n->parent) == BLACK_NODE)
					return;

				if (n->grandparent() == NULL)
					return;
				
				if (GET_COLOR(n->aunt()) == RED_NODE)
				{
					Debug::Log << "Map: recolouring " << n->value.first << ", aunt is RED" << std::endl;
					n->parent->color = BLACK_NODE;
					n->aunt()->color = BLACK_NODE;
					n->grandparent()->color = RED_NODE;
					PRINT_TREE;
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

				PRINT_TREE;

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

				PRINT_TREE;
			}

		/* #endregion */

		/* #region rotations */

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

				PRINT_TREE;
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

				PRINT_TREE;
			}

		/* #endregion */

		/* #region erasure */

			void _erase_node(node *n) {
				char originalColor = n->color;

				node *heir = this->_delete_node(n);
				
				if (originalColor == BLACK_NODE) {
					if (GET_COLOR(heir) == RED_NODE)
						heir->color = BLACK_NODE;
					else if (heir == this->root())
						heir->color = BLACK_NODE;
					else
						this->_solve_double_black(heir);
				}

				if (heir == NIL)
					this->_replace(heir, NULL);

				Debug::Log << "Erasure complete" << std::endl;
				PRINT_TREE;
			}

			node *_delete_node(node *n) {
				Debug::Log << "Deleting node" << std::endl;

				if (!n->left && !n->right)
					return _delete_leaf(n);
				else if (!n->left || !n->right)
					return _delete_one_child(n);
				return _delete_two_children(n);
			}

			node *_delete_leaf(node *n) {
				Debug::Log << "Deleting leaf" << std::endl;
				this->_replace(n, NIL);
				this->_delete_allocated(n);

				return NIL;
			}

			node *_delete_one_child(node *n) {
				Debug::Log << "Deleting one child" << std::endl;
				node *child = n->left;
				if (n->left == NULL)
					child = n->right;
				this->_replace(n, child);
				this->_delete_allocated(n);

				return child;
			}

			node *_delete_two_children(node *n) {
				Debug::Log << "Deleting two children" << std::endl;
				node *subnode = n->left; // n->right

				while (subnode->right) // subnode->left
					subnode = subnode->right; // subnode->left
				Debug::Log << "Subnode key: " << KEY(subnode) << std::endl;

				node *heir = subnode->left; // subnode->right
				this->_replace(subnode, heir);

				this->_replace(n, subnode);
				this->_inherit(n, subnode);

				this->_delete_allocated(n);

				return subnode;
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
				Debug::Log << "SOLVING DOUBLE BLACK ON: " << KEY(db) << std::endl;
				PRINT_TREE;

				node *sibling = db->sibling();
				bool child_status = CHILD_STATUS(db);

				if (sibling == NULL) // check for this case
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
				Debug::Log << "ENTERING RED SIBLING CASE" << std::endl;
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
				Debug::Log << "ENTERING BLACK FAMILY CASE" << std::endl;
				node *sibling = db->sibling();
				node *parent = db->parent;

				sibling->color = RED_NODE;
				if (GET_COLOR(parent) == RED_NODE)
					parent->color = BLACK_NODE;
				else
					this->_solve_double_black(parent);
			}

			void _close_nephew_red_case(node *db, bool child_status) {
				Debug::Log << "ENTERING CLOSE NEPHEW RED CASE" << std::endl;
				node *sibling = db->sibling();

				if (child_status == LEFT_CHILD)
				{
					this->_swap_colors(sibling, sibling->left);
					this->_rotate_right(sibling);
				}
				else {
					this->_swap_colors(sibling, sibling->right);
					this->_rotate_left(sibling);
				}
			}

			void _far_nephew_red_case(node *db, bool child_status) {
				Debug::Log << "ENTERING FAR NEWPHEW RED CASE" << std::endl;
				node *sibling = db->sibling();
				node *parent = db->parent;

				sibling->color = parent->color;
				parent->color = BLACK_NODE;

				if (child_status == LEFT_CHILD) {
					sibling->right->color = BLACK_NODE;
					this->_rotate_left(parent);
				}
				else {
					sibling->left->color = BLACK_NODE;
					this->_rotate_right(parent);
				}
			}
			
			/* #endregion */
		
		/* #endregion */
	};

	
}


#endif