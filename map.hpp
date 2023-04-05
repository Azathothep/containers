#ifndef MAP_HPP
# define MAP_HPP

# include "ft/pair.hpp"
# include "ft/make_pair.hpp"
# include "ft/is_const.hpp"
# include "ft/reverse_iterator.hpp"
# include "ft/enable_if.hpp"
# include "ft/is_integral.hpp"
# include "ft/lexicographical_compare.hpp"

# include "utils/node.hpp"
# include "utils/binary_tree.hpp"

namespace ft
{
	/* #region iterator */

	template <typename T, bool constness = false>
	class map_iterator {

	public:
		typedef typename std::forward_iterator_tag 											iterator_category;
		typedef typename std::ptrdiff_t 													difference_type;
		typedef typename is_const<T, constness>::type										value_type;
		typedef	value_type*																 	pointer;
		typedef value_type&														 			reference;

	private:
		typedef typename ft::node< T >														node;

		node 		*_m_node;

	public:
		map_iterator(node *n = NULL) : _m_node(n) {}

		template <bool c>
		map_iterator(const map_iterator<T, c> &rhs) {
			*this = rhs;
		}

		template <bool c>
		map_iterator &operator=(const map_iterator<T, c> &rhs) {
			reference r = *rhs;
			(void)r;
			this->_m_node = rhs.data();
			return *this;
		}

		~map_iterator() {}

		node *data() const { return this->_m_node; }

		reference operator*() const {
			return *(_m_node->value);
		}

		pointer operator->() const {
			return _m_node->value;
		}

		map_iterator &operator++() {

			if (_m_node->right) {
				_m_node = _get_far_left(_m_node->right);
			} else { // if has no right child...
				if (_m_node->parent == NULL) { // ...and this is already god
					return *this;
				} else if (_m_node->child_status() == LEFT_CHILD) { // ...and is a left child
					_m_node = _m_node->parent;
				} else { // ...and is a right child
					// start tests from grandparent, because here the parent is, in any case, inferior
					_m_node = _m_node->parent;

					while (_m_node->parent) { // while node is not god
						if (_m_node->child_status() == LEFT_CHILD) {// if node branch is a left branch, then the parent is higher than the original
							_m_node = _m_node->parent;
							break;
						}
						
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
			map_iterator result(_m_node);

			for (int i = 0; i < n; i++) {
				++result;
			}

			return result;
		}

		map_iterator operator+=(difference_type n) const
		{
			*this = *this + n;
			return *this;
		}

		map_iterator &operator--() {

			if (_m_node->left) {
				_m_node = _get_far_right(_m_node->left);
			} else { // if has no left child...
				if (_m_node->parent == NULL) { // ...and is god -> tree is empty
					return *this;
				} else if (_m_node->child_status() == RIGHT_CHILD) {
					_m_node = _m_node->parent;
				} else { // ...and is a left child
					_m_node = _m_node->parent;

					while (_m_node->parent) { // while node is not god
						if (_m_node->child_status() == RIGHT_CHILD) {
							_m_node = _m_node->parent;
							break;
						}
						
						_m_node = _m_node->parent;
					}
				}
			}
			 
			return *this;
		}

		map_iterator operator--(int) {
				map_iterator iterator = *this;
				--(*this);
				return iterator;
			}

		map_iterator operator-(difference_type n) const
		{
			map_iterator result(_m_node);

			for (int i = 0; i < n; i++) {
				--result;
			}

			return result;
		}

		map_iterator operator-=(difference_type n) const
		{
			*this = *this - n;
			return *this;
		}

		template <typename U, bool c>
		bool operator==(const map_iterator<U, c> &rhs) const {
			return this->_m_node == rhs.data();
		}

		template <typename U, bool c>
		bool operator!=(const map_iterator<U, c> &rhs) const {
			return !(*this == rhs);
		}

	private:
		node *_get_far_left(node *n) {
			while (n->left) {
				n = n->left;
			}
			
			return n;
		}

		node *_get_far_right(node *n) {
			while (n->right) {
				n = n->right;
			}
			
			return n;
		}
	};

	/* #endregion */

	/* #region key_getter */


	/* #endregion */

	/* #region map */

	template <class Key, class T, class Compare = std::less< Key >, class Alloc = std::allocator< ft::pair< const Key, T > > >
	class map {

	/* #region typedefs */

	public:

		typedef Key 													key_type;
		typedef T 														mapped_type;
		typedef ft::pair<const key_type, mapped_type>					value_type;

		typedef Compare													key_compare;
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
		
		class map_key_getter {
			public:
				typedef typename value_type::first_type 						key_type;

				key_type const & get(value_type const *val) const { return val->first; }
		};

		typedef ft::binary_tree<value_type, map_key_getter, key_compare>		binary_tree;

		key_compare 		_M_comp;
		allocator_type		_M_alloc;
		binary_tree 		_M_tree;

	/* #region intialization */

	public:
		explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : _M_comp(comp), _M_alloc(alloc), _M_tree(comp) {}

		template <class InputIterator>
		map (InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type(), typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = NULL) : _M_comp(comp), _M_alloc(alloc), _M_tree(comp) {
			this->_range_copy(first, last);
		}

		map (const map& x) : _M_comp(x._M_comp), _M_alloc(x._M_alloc), _M_tree(x._M_comp) {
			*this = x;
		}

		~map() {
			this->clear();
		}

		map& operator=(const map& x) {
			if (this == &x)
				return *this;

			this->clear();

			this->_range_copy(x.begin(), x.end());

			return *this;
		}

	public:
		iterator begin() { return iterator(_M_tree.smallest() ); }
	 	const_iterator begin() const { return cbegin(); }
	 	const_iterator cbegin() const { return const_iterator(_M_tree.smallest() ); }
		iterator end() { return iterator(_M_tree.past_the_end()); }
		const_iterator end() const { return cend(); }
	 	const_iterator cend() const { return const_iterator(_M_tree.past_the_end()); }
	 	reverse_iterator rbegin() { return reverse_iterator(iterator(_M_tree.past_the_end())); }
		const_reverse_iterator rbegin() const { return crbegin(); }
		const_reverse_iterator crbegin() const { return const_reverse_iterator(iterator(_M_tree.past_the_end())); }
		reverse_iterator rend() { return reverse_iterator(iterator(_M_tree.smallest())); }
		const_reverse_iterator rend() const { return crend(); }
		const_reverse_iterator crend() const { return const_reverse_iterator(iterator(_M_tree.smallest())); }

	/* #endregion */

	/* #region capacity */

	public:
		bool empty() const {
			return _M_tree.size == 0;
		}

		size_type size() const {
			return _M_tree.size;
		};

		size_type max_size() const {
			return this->_M_tree.max_size();
		};

	/* #endregion */

	/* #region element_access */

	public:
		mapped_type& operator[] (const key_type& k) {
			typename binary_tree::node *n = _M_tree.get_node(k);

			if (n)
				return n->value->second;

			ft::pair<iterator, bool> new_node = this->insert( value_type(k, mapped_type()) );

			return new_node.first->second;
		}

		mapped_type& at (const key_type& k) {
			typename binary_tree::node *n = _M_tree.get_node(k);

			if (n == NULL)
				throw std::out_of_range("map: at");

			return n->value->second;
		}

		const mapped_type& at (const key_type& k) const {
			typename binary_tree::node *n = _M_tree.get_node(k);

				if (n == NULL)
					throw std::out_of_range("map: at");

			return n->value->second;
		}

	/* #endregion */

	/* #region modifiers */

	private:

		pointer _create_value(value_type const & val) {
			pointer p = _M_alloc.allocate(1);
			_M_alloc.construct(p, val);

			return p;
		}

		template <class InputIterator>
		void _range_copy(InputIterator first, InputIterator last) {
			for (; first != last; first++) {
				this->insert(*first);
			}
		}

		void _delete_value(pointer p) {
			this->_M_alloc.destroy(p);
			this->_M_alloc.deallocate(p, 1);
		}

		void _deep_delete(typename binary_tree::node *n) {
			pointer p = n->value;
			this->_M_tree.erase(n);
			this->_delete_value(p);
		}

	public:
		ft::pair<iterator, bool> insert (const value_type& val) {
			typename binary_tree::node *n = _M_tree.get_node(val.first);

			if (n)
				return ft::pair<iterator, bool>(iterator(n), false);
			
			pointer p = this->_create_value(val);
			
			n = _M_tree.insert(p);

			return ft::pair<iterator, bool>(iterator(n), true);
		}

		iterator insert (iterator position, const value_type& val) {
			typename binary_tree::node *n = _M_tree.get_node(val.first);
			
			if (n == NULL) {
				pointer p = this->_create_value(val);

				if (position == end())
					n = _M_tree.insert(p);
				else
					n = _M_tree.insert_from(p, &(*position));
			}

			return iterator(n);
		}

		template <class InputIterator>
		void insert (InputIterator first, InputIterator last, typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = NULL) {
			this->_range_copy(first, last);
		}

	public:

		void erase (iterator position) {
			typename binary_tree::node *n = _M_tree.get_node(position->first);
			this->_deep_delete(n);
		}

		size_type erase (const key_type& k) {
			typename binary_tree::node *n = _M_tree.get_node(k);

			if (n == NULL)
				return 0;

			this->_deep_delete(n);

			return 1;
		}

		void erase (iterator first, iterator last) {
			while (first != last) {
				iterator to_del = first;
				first++;

				typename binary_tree::node *n = _M_tree.get_node(to_del->first);

				this->_deep_delete(n);
			}
		}

		void swap (map& x) {
			this->_M_tree.swap(x._M_tree);
		}

		void clear() {
			iterator it = begin();
			iterator ite = end();

			for (; it != ite; it++) {
				this->_delete_value(&(*it));
			}

			this->_M_tree.destroy_all();
		}

	/* #endregion */

	/* #region observers */
	public:
		class value_compare : std::binary_function<value_type, value_type, bool> {
			protected:
				Compare comp;

			public:
				typedef bool result_type;
				typedef value_type first_argument_type;
				typedef value_type second_argument_type;

				value_compare (Compare c) : comp(c) {}

				bool operator() (const value_type& x, const value_type& y) const {
					return comp(x.first, y.first);
				}		
		};

		key_compare key_comp() const { return this->_M_comp; }

		value_compare value_comp() const { return value_compare(_M_comp); }

	/* #endregion */

	/* #region operations */

	// public:
	 	iterator find (const key_type& k) {
			typename binary_tree::node *n = _M_tree.get_node(k);
			
			if (n == NULL)
				n = _M_tree.past_the_end();

			return iterator(n);
		}

	 	const_iterator find (const key_type& k) const {
			typename binary_tree::node *n = _M_tree.get_node(k);
			
			if (n == NULL)
				n = _M_tree.past_the_end();

			return const_iterator(n);
		}

	 	size_type count (const key_type& k) const {
			typename binary_tree::node *n = this->_M_tree.get_node(k);
			
			if (n)
				return 1;

			return 0;
		}

	 	iterator lower_bound (const key_type& k) {
			iterator it = this->begin();
			iterator ite = this->end();
			
			for (; it != ite; it++) {
				if (this->_M_comp(k, it->first) || this->_M_comp(it->first, k) == false) // if (key < it) or if (key >= it && it >= key)
					return it;
			}

			return ite;
		}

	 	const_iterator lower_bound (const key_type& k) const {
			const_iterator it = this->begin();
			const_iterator ite = this->end();
			
			for (; it != ite; it++) {
				if (this->_M_comp(k, it->first) || this->_M_comp(it->first, k) == false) // if (key < it) or if (key >= it && it >= key)
					return it;
			}

			return ite;
		}

	 	iterator upper_bound (const key_type& k) {
			iterator it = this->begin();
			iterator ite = this->end();
			
			for (; it != ite; it++) {
				if (this->_M_comp(k, it->first)) // if (key < it)
					return it;
			}

			return ite;
		}

	 	const_iterator upper_bound (const key_type& k) const {
			const_iterator it = this->begin();
			const_iterator ite = this->end();
			
			for (; it != ite; it++) {
				if (this->_M_comp(k, it->first)) // if (key < it)
					return it;
			}

			return ite;
		}

	 	ft::pair<const_iterator, const_iterator> equal_range (const key_type& k) const {
			ft::pair<const_iterator, const_iterator> const_range(this->lower_bound(k), this->upper_bound(k));

			return const_range;
		}

	 	ft::pair<iterator, iterator> equal_range (const key_type& k) {
			ft::pair<iterator, iterator> range(this->lower_bound(k), this->upper_bound(k));

			return range;
		}

	/* #endregion */

	public:
		allocator_type get_allocator() const { return this->_M_alloc; }

	};

	/* #endregion */

	/* #region relational operators */

	template <class Key, class T, class Compare, class Alloc>
	bool operator==( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs) {
		if (lhs.size() != rhs.size())
			return false;

		typename map<Key,T,Compare,Alloc>::const_iterator lit = lhs.cbegin();
		typename map<Key,T,Compare,Alloc>::const_iterator lite = lhs.cend();
		typename map<Key,T,Compare,Alloc>::const_iterator rit = rhs.cbegin();
		typename map<Key,T,Compare,Alloc>::const_iterator rite = rhs.cend();

		for(; lit != lite; lit++) {
			if (*lit != *rit)
				return false;
			rit++;
		}

		return true;
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator!=( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs) {
		return !(lhs == rhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator<( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs) {
		typename map<Key,T,Compare,Alloc>::const_iterator lit = lhs.cbegin();
		typename map<Key,T,Compare,Alloc>::const_iterator lite = lhs.cend();
		typename map<Key,T,Compare,Alloc>::const_iterator rit = rhs.cbegin();
		typename map<Key,T,Compare,Alloc>::const_iterator rite = rhs.cend();

		return lexicographical_compare(lit, lite, rit, rite);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator>( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs) {
		return rhs < lhs;
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator>=( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs) {
		return !(lhs < rhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator<=( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs) {
		return !(lhs > rhs);
	}

	/* #endregion */
}

#endif