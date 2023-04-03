#ifndef BINARY_TREE_HPP
# define BINARY_TREE_HPP

# ifdef DEBUG_MODE
#	include "Debug.hpp"
#	include "print_tree.hpp"
# endif

# include "ft/pair.hpp"
# include "node.hpp"

namespace ft {

	template < class T, class Getter, class Compare, class Alloc = std::allocator< ft::node < T > > >
	class binary_tree {
		
		#define		KEY(n) this->_M_key.get(((node *)n)->value)
		#define		NIL &(this->_nil)
		#define		GET_COLOR(n) (char)(((node *)n == NULL) ? BLACK_NODE : n->color)

		#ifdef DEBUG_MODE
			# define PRINT_TREE _printer.print(*this)
		#endif

		public:
			typedef T											value_type;
			typedef Getter										key_getter;
			typedef Alloc 										allocator_type;
			typedef Compare 									compare_type;
			typedef typename key_getter::key_type				key_type;
			typedef typename ft::node< value_type > 			node;

			int						size;

		private:
			node 					*_root;
			node					*_god_p;
			node					_god;
			node					_nil;
			key_getter				_M_key;
			allocator_type 			_M_alloc;
			compare_type			_M_comp;

			#ifdef DEBUG_MODE
				ft::Tree_Printer<ft::integral_printer>		_printer;
			#endif

		public:
			#ifdef DEBUG_MODE
			binary_tree(const compare_type &comp = compare_type(), const allocator_type &alloc = allocator_type()) : size(0), _root(NULL), _printer(Debug::Log)
			#else
			binary_tree(const compare_type &comp = compare_type(), const allocator_type &alloc = allocator_type()) : size(0), _root(NULL), _M_key(key_getter())
			#endif
			{
				this->_M_comp = comp;
				this->_M_alloc = alloc;
				this->_god_p = &this->_god;
			}

			#ifdef DEBUG_MODE
			binary_tree(value_type const & val, const compare_type &comp = compare_type()) : size(0), _printer(Debug::Log)
			#else
			binary_tree(value_type const & val, const compare_type &comp = compare_type()) : size(0)
			#endif
			{
				this->_M_comp = comp;
				this->_god_p = &this->_god;

				this->insert(val);
			}

			~binary_tree() {
				this->destroy_all();
				size = 0;
			}

			void swap(binary_tree & rhs) {
				#ifdef DEBUG_MODE
					Debug::Log << "B_TREE swap called" << std::endl;
				#endif

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

			compare_type key_comp() const { return _M_comp; }

			allocator_type get_allocator() const { return _M_alloc; }

			void destroy_all() {
				if (this->root())
					this->_move_destroy(this->_root);

				this->_god.left = NULL;
				this->_root = NULL;

				#ifdef DEBUG_MODE
					PRINT_TREE;
				#endif
			}

			void _move_destroy(node *current) {
				if (current->left)
					_move_destroy(current->left);
				if (current->right)
					_move_destroy(current->right);
				
				this->_delete_allocated(current);
			}

			node *get_node(key_type const & key) const {
				node *n = this->root();

				return get_node(key, n);
			}

			node *get_node(key_type const & key, node *n) const {
				if(n == NULL)
					return n;

				#ifdef DEBUG_MODE
					Debug::Log << "B_Tree: foward_node: checking node of key " << n->value.first << std::endl;
				#endif

				if (this->_M_comp(key, KEY(n))) // if key < n
					return get_node(key, n->left);
				else if (this->_M_comp(KEY(n), key)) // if key > n
					return get_node(key, n->right);

				return n;
			}

			ft::pair< ft::node<value_type> *, bool> insert(value_type const & val) {
				node *n = this->get_node(val.first, this->root());
				ft::pair< ft::node<value_type> *, bool> return_pair;
				
				if (n) {
					return_pair.first = n;
					return_pair.second = false;
					return return_pair;
				}

				n = this->_create_node(val);

				if (this->_root) {
					#ifdef DEBUG_MODE
						Debug::Log << "Move_inserting node" << std::endl;
					#endif

					this->_move_insert(&this->_root, n);
				}
				else
				{
					#ifdef DEBUG_MODE
						Debug::Log << "Replacing root" << std::endl;
					#endif

					this->_root = n;
					this->_root->color = BLACK_NODE;

					this->_god.left = this->_root;
					this->_root->parent = &(this->_god);
				}

				size++;
				
				return_pair.first = n;
				return_pair.second = true;

				return return_pair;
			}

			node *insert_from(value_type const & val, key_type const & from) {
				node *n = this->get_node(val.first, this->root());

				if (n)
					return n;
				
				node *to_insert = this->get_node(from, this->root());

				if (to_insert && _M_comp(to_insert->value.first, from)) {
					n = this->_create_node(val);
					this->_move_insert(&n, to_insert, to_insert->parent);
					return n;
				}

				return insert(val).first;
			}

			bool erase(key_type const & key) {
				node *n = get_node(key, this->root());

				if (n == NULL)
					return false;

				this->_erase_node(n);

				return true;
			}

			node *smallest() const {
				node *n = this->root();

				if (n == NULL)
					return past_the_end();
				
				if (n->left)
				{
					n = n->left;
					while (n->left)
						n = n->left;
				}

				return n;
			}

			node *past_the_end() const {
				return this->_god_p;
			}

			node *root() const { return this->_root; }

		private:

		/* #region utils */

			void _replace(node *n, node *r) {
				node *p = n->parent;
				
				if (p == &(this->_god)) {
					this->_root = r;
					this->_god.left = this->_root;
				}
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
				#ifdef DEBUG_MODE
					Debug::Log << "Deleting node of key " << n->value.first << std::endl;
				#endif

				n->left = NULL;
				n->right = NULL;
				n->parent = NULL;
				_M_alloc.destroy(n);
				_M_alloc.deallocate(n, 1);
				n = NULL;
				size--;
			}

			node *_create_node(value_type const & val) {
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
				#ifdef DEBUG_MODE
					Debug::Log << "Map: inserting element" << std::endl;
				#endif

				this->_recolor(insert);

				#ifdef DEBUG_MODE
					PRINT_TREE;
				#endif
			}

			void _recolor(node *n) {
				if (n == this->_root) {
					#ifdef DEBUG_MODE
						Debug::Log << "Map: recolouring root" << std::endl;
					#endif

					n->color = BLACK_NODE;

					#ifdef DEBUG_MODE
						PRINT_TREE;
					#endif
					
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
					#ifdef DEBUG_MODE
						PRINT_TREE;
					#endif

					this->_recolor(n->grandparent());
				}
				else {
					#ifdef DEBUG_MODE
						Debug::Log << "Map: need rotation from " << n->value.first << std::endl;
					#endif

					this->_perform_rotation(n);
				}
			}

			void _perform_rotation(node *n) {
				node *p = n->parent;
				node *g = n->grandparent();

				#ifdef DEBUG_MODE
					PRINT_TREE;
				#endif

				if (p == g->left && n == p->left)
				{
					#ifdef DEBUG_MODE
						Debug::Log << "DETECTING LL CASE" << std::endl;
					#endif

					this->_rotate_right(g);
					this->_swap_colors(g, p);
				}
				else if (p == g->right && n == p->right)
				{
					#ifdef DEBUG_MODE
						Debug::Log << "DETECTING RR CASE" << std::endl;
					#endif
					
					this->_rotate_left(g);
					this->_swap_colors(g, p);
				}
				else if (p == g->left && n == p->right)
				{
					#ifdef DEBUG_MODE
						Debug::Log << "DETECTING LR CASE" << std::endl;
					#endif
					
					this->_rotate_left(p);
					this->_rotate_right(g);
					this->_swap_colors(g, n);
				}
				else if (p == g->right && n == p->left)
				{
					#ifdef DEBUG_MODE
						Debug::Log << "DETECTING RL CASE" << std::endl;
					#endif
					
					this->_rotate_right(p);
					this->_rotate_left(g);
					this->_swap_colors(g, n);
				}
				#ifdef DEBUG_MODE
				else
					Debug::Log << "No case found, " << p->value.first << ",  " << g->value.first << std::endl;
				#endif

				#ifdef DEBUG_MODE
					PRINT_TREE;
				#endif
			}

		/* #endregion */

		/* #region rotations */

			void _rotate_left(node *n) {
				#ifdef DEBUG_MODE
					Debug::Log << "Map: rotate left" << std::endl;
				#endif
				
				node *traitor = n->right;
				node *vassal = traitor->left;

				this->_replace(n, traitor);
				n->parent = traitor;
				traitor->left = n;
				n->right = vassal;
				if (vassal)
					vassal->parent = n;

				#ifdef DEBUG_MODE
					PRINT_TREE;
				#endif
			}

			void _rotate_right(node *n) {
				#ifdef DEBUG_MODE
					Debug::Log << "Map: rotate right" << std::endl;
				#endif
				
				node *traitor = n->left;
				node *vassal = traitor->right;

				this->_replace(n, traitor);
				n->parent = traitor;
				traitor->right = n;
				n->left = vassal;
				if (vassal)
					vassal->parent = n;

				#ifdef DEBUG_MODE
					PRINT_TREE;
				#endif
			}

		/* #endregion */

		/* #region erasure */

			void _erase_node(node *n) {
				char originalColor = n->color;

				node *heir = this->_delete_node(n, &originalColor);
			
				#ifdef DEBUG_MODE
					Debug::Log << "OriginalColor is " << (int)originalColor << std::endl;
				#endif

				if (originalColor == BLACK_NODE) {
					if (GET_COLOR(heir) == RED_NODE)
					{
						#ifdef DEBUG_MODE
							Debug::Log << "PAINTING RED HEIR " << KEY(heir) << " IN BLACK" << std::endl;
						#endif

						heir->color = BLACK_NODE;
					}
					else if (heir == this->root())
						heir->color = BLACK_NODE;
					else
						this->_solve_double_black(heir);
				}

				if (heir == NIL)
					this->_replace(heir, NULL);
				
				#ifdef DEBUG_MODE
					Debug::Log << "Erasure complete" << std::endl;
					PRINT_TREE;
				#endif
			}

			node *_delete_node(node *n, char *originalColor) {
				#ifdef DEBUG_MODE
					Debug::Log << "Deleting node" << std::endl;
				#endif

				if (!n->left && !n->right)
					return _delete_leaf(n);
				else if (!n->left || !n->right)
					return _delete_one_child(n);
				return _delete_two_children(n, originalColor);
			}

			node *_delete_leaf(node *n) {
				#ifdef DEBUG_MODE
					Debug::Log << "Deleting leaf" << std::endl;
				#endif

				this->_replace(n, NIL);
				this->_delete_allocated(n);

				return NIL;
			}

			node *_delete_one_child(node *n) {
				#ifdef DEBUG_MODE
					Debug::Log << "Deleting one child" << std::endl;
				#endif
				
				node *child = n->left;
				if (n->left == NULL)
					child = n->right;
				this->_replace(n, child);
				this->_delete_allocated(n);

				return child;
			}

			node *_delete_two_children(node *n, char *originalColor) {
				#ifdef DEBUG_MODE
					Debug::Log << "Deleting two children" << std::endl;
				#endif

				node *subnode = n->left; // n->right

				while (subnode->right) // subnode->left
					subnode = subnode->right; // subnode->left

				#ifdef DEBUG_MODE
					Debug::Log << "Subnode key: " << KEY(subnode) << std::endl;
				#endif

				node *heir = subnode->left; // subnode->right
				if (heir == NULL)
					heir = NIL;
				this->_replace(subnode, heir);

				this->_replace(n, subnode);
				this->_inherit(n, subnode);
				
				*originalColor = subnode->color; // TO VERIFY

				#ifdef DEBUG_MODE
					Debug::Log << "Replacing subnode color by " << (int)n->color << std::endl;
				#endif

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
				#ifdef DEBUG_MODE
					Debug::Log << "SOLVING DOUBLE BLACK ON: " << KEY(db) << std::endl;
					PRINT_TREE;
				#endif

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
				#ifdef DEBUG_MODE
					Debug::Log << "ENTERING RED SIBLING CASE" << std::endl;
				#endif

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
				#ifdef DEBUG_MODE
					Debug::Log << "ENTERING BLACK FAMILY CASE" << std::endl;
				#endif
				
				node *sibling = db->sibling();
				node *parent = db->parent;

				sibling->color = RED_NODE;
				if (GET_COLOR(parent) == RED_NODE)
					parent->color = BLACK_NODE;
				else
					this->_solve_double_black(parent);
			}

			void _close_nephew_red_case(node *db, bool child_status) {
				#ifdef DEBUG_MODE
					Debug::Log << "ENTERING CLOSE NEPHEW RED CASE" << std::endl;
				#endif

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
				#ifdef DEBUG_MODE
					Debug::Log << "ENTERING FAR NEWPHEW RED CASE" << std::endl;
				#endif

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