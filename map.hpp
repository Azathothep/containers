#ifndef MAP_HPP
# define MAP_HPP

# include "ft/pair.hpp"
# include "ft/make_pair.hpp"
# include "ft/is_const.hpp"

# include "node.hpp"
# include "b_tree.hpp"

namespace ft
{
	/* #region iterator */

	template <typename T, bool constness = false>
	class map_iterator {
	
	#define LEFT_BRANCH 0
	#define RIGHT_BRANCH 1

	public:
		typedef typename std::forward_iterator_tag 											iterator_category;
		typedef typename std::ptrdiff_t 													difference_type;
		typedef typename is_const<T, constness>::type								value_type;
		typedef	value_type*																 	pointer;
		typedef value_type&														 			reference;

	private:
		typedef typename ft::node< value_type >												node;

		node 	*_m_node;
		int		_prev;

	public:
		map_iterator() { }
		map_iterator(node *n, int comes_from = LEFT_BRANCH) : _m_node(n), _prev(comes_from) {}

		map_iterator &operator++() {
			if (_m_node->right) {
				_m_node = _get_far_left(_m_node->right);
				_prev = LEFT_BRANCH;
				if (_m_node == _m_node->parent->right)
					_prev = RIGHT_BRANCH;
			} else {
				if (_prev == LEFT_BRANCH) {
					_m_node = _m_node->parent;

					if (_m_node == _m_node->parent->right)
						_prev = RIGHT_BRANCH;
				} else if (_prev == RIGHT_BRANCH) {
					_m_node = _m_node->parent;

					while (_prev == RIGHT_BRANCH) {
						if (_m_node->parent == NULL || _m_node == _m_node->parent->left)
							_prev = LEFT_BRANCH;
						_m_node = _m_node->parent;
					}
				}
			}
			
			return *this;
		}

		map_iterator operator++(int) {
			map_iterator iterator = *this;
			++(*this);
			return iterator;
		}

		map_iterator operator+(difference_type n) const
		{
			map_iterator result(_m_node, _prev);

			for (int i = 0; i < n; i++) {
				++result;
			}

			return result;
		}

		map_iterator &operator--() {

			if (_m_node->left) {
				_m_node = _get_far_right(_m_node->left);
				_prev = RIGHT_BRANCH;
				if (_m_node == _m_node->parent->left)
					_prev = LEFT_BRANCH;
			} else {
				if (_prev == RIGHT_BRANCH) {
					_m_node = _m_node->parent;
					if (_m_node == _m_node->parent->left)
						_prev = LEFT_BRANCH;
				} else if (_prev == LEFT_BRANCH) {
					_m_node = _m_node->parent;

					while (_prev == LEFT_BRANCH) {
						if (_m_node->parent == NULL || _m_node == _m_node->parent->right)
							_prev = RIGHT_BRANCH;
						_m_node = _m_node->parent;
					}
				}
			}
			
			return *this;
		}

		reference operator*() const {
			return _m_node->value;
		}

					map_iterator operator--(int) {
				map_iterator iterator = *this;
				--(*this);
				return iterator;
			}

		map_iterator operator-(difference_type n) const
		{
			map_iterator result(_m_node, _prev);

			for (int i = 0; i < n; i++) {
				--result;
			}

			return result;
		}

		template <bool c>
		bool operator==(const map_iterator<T, c> &rhs) const {
			return this->_m_node == rhs._m_node;
		}

		template <bool c>
		bool operator!=(const map_iterator<T, c> &rhs) const {
			return !(*this == rhs);
		}

	private:
		node *_get_far_left(node *n) {
			while (n->left)
				n = n->left;
			
			return n;
		}

		node *_get_far_right(node *n) {
			while (n->right)
				n = n->right;
			
			return n;
		}
	};

	/* #endregion */

	/* #region map */

	template <class Key, class T, class Compare = std::less< Key >, class Alloc = std::allocator< ft::pair< const Key, T > > >
	class map {

	/* #region typedefs */
	private:

		typedef ft::map<Key, T, Compare, Alloc> 						base;

	public:

		typedef Key 													key_type;
		typedef T 														mapped_type;
		typedef ft::pair<const key_type, mapped_type>					value_type;

		typedef Compare													key_compare;
		//typedef value_compare; Nested function class to compare elements (see value_comp)
		typedef Alloc													allocator_type;

		typedef typename allocator_type::reference 						reference;
		typedef typename allocator_type::const_reference 				const_reference;
		typedef typename allocator_type::pointer 						pointer;
		typedef typename allocator_type::const_pointer 					const_pointer;

		typedef map_iterator<value_type>								iterator;
		typedef map_iterator<value_type, IS_CONST>						const_iterator;
		typedef ft::reverse_iterator<iterator>							reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>					const_reverse_iterator;

		typedef typename ft::iterator_traits<iterator>::difference_type	difference_type;
		typedef typename allocator_type::size_type						size_type;

	/* #endregion */

	private:
		key_compare			_M_comp;
		allocator_type		_M_alloc;

		ft::B_TREE<key_type, mapped_type, key_compare> _M_tree;

	/* #region intialization */

	public:
		explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : _M_tree(_M_comp) {
			_M_comp = comp;
			_M_alloc = alloc;
		}

		template <class InputIterator>
		map (InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : _M_tree(_M_comp) {
			(void)first;
			(void)last;
			_M_comp = comp;
			_M_alloc = alloc;
		}

		map (const map& x) : _M_tree(_M_comp) {
			*this = x;
		}

		~map() {}

		map& operator= (const map& x) {
			return *this;
		}

	/* #endregion */

	public:
		ft::node< value_type > *root() { return _M_tree.root(); }

	/* #region iterators */

	public:
		iterator begin() { return iterator(_M_tree.smallest() ); }
	 	// const_iterator begin() const { return const_iterator(_M_tree.smallest() ); }
	 	// const_iterator cbegin() const { return const_iterator(_M_tree.smallest() ); }
		iterator end() { return ++iterator(_M_tree.biggest(), RIGHT_BRANCH); }
		const_iterator end() const { return ++iterator(_M_tree.biggest(), RIGHT_BRANCH); }
	 	const_iterator cend() const { return ++iterator(_M_tree.biggest(), RIGHT_BRANCH); }
	 	reverse_iterator rbegin() const { return reverse_iterator(++iterator(_M_tree.biggest(), RIGHT_BRANCH)); }
		// const_reverse_iterator crbegin() const { }
		reverse_iterator rend() const { return reverse_iterator(iterator(_M_tree.smallest())); }
		const_reverse_iterator crend() const { }

	/* #endregion */

	/* #region capacity */

	public:
		bool empty() const;

		size_type size() const;

		size_type max_size() const;

	/* #endregion */

	/* #region element_access */

	public:
		mapped_type& operator[] (const key_type& k) {
			Debug::Log << "Map: operator[" << k << "]";

			return _M_tree.get_pair(k).second;
		}

		mapped_type& at (const key_type& k) { }

		const mapped_type& at (const key_type& k) const { }

	/* #endregion */

	/* #region modifiers */

	public:
		ft::pair<iterator, bool> insert (const value_type& val) {
			_M_tree.insert(val);
			ft::pair<iterator, bool> result(iterator(), true);
			// iterator pointing to the newly inserted element or to the element with an equivalent key in the map
			// bool element is set to true if a new element was inserted or false if an equivalent key already existed

			return result;
		}

	// 	iterator insert (iterator position, const value_type& val) { }

	// 	template <class InputIterator>
	// 	void insert (InputIterator first, InputIterator last) { }

	// 	void erase (iterator position) { }

	size_type erase (const key_type& k) {
		_M_tree.erase(k);

		return 0;
	}

	// 	void erase (iterator first, iterator last) { }

	// 	void swap (map& x) { }

	// 	void clear() { }

	/* #endregion */

	/* #region observers */

	public:
		key_compare key_comp() const { }

		//value_compare value_comp() const { }

	/* #endregion */

	/* #region operations */

	// public:
	// 	iterator find (const key_type& k) { }

	// 	const_iterator find (const key_type& k) const { }

	// 	size_type count (const key_type& k) const { }

	// 	iterator lower_bound (const key_type& k) { }

	// 	const_iterator lower_bound (const key_type& k) const { }

	// 	iterator upper_bound (const key_type& k) { }

	// 	const_iterator upper_bound (const key_type& k) const { }

	// 	ft::pair<const_iterator, const_iterator> equal_range (const key_type& k) const { }

	// 	ft::pair<iterator, iterator> equal_range (const key_type& k) { }

	/* #endregion */

	public:
		allocator_type get_allocator() const { }

	};

	/* #endregion */
}

#endif