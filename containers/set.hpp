#ifndef SET_HPP
# define SET_HPP

# include "../ft/pair.hpp"
# include "../ft/make_pair.hpp"
# include "../ft/reverse_iterator.hpp"
# include "../ft/enable_if.hpp"
# include "../ft/is_integral.hpp"
# include "../ft/lexicographical_compare.hpp"

# include "../utils/is_const.hpp"
# include "../utils/node.hpp"
# include "../utils/binary_tree.hpp"

namespace ft {

	template < class T, class Compare = std::less< T >, class Alloc = std::allocator< T > >
	class set {

		/* #region typedefs */

	public:

		typedef T 															key_type;
		typedef T 															value_type;
		typedef Compare														key_compare;
		typedef Compare 													value_compare;
		typedef Alloc 														allocator_type;

		typedef typename allocator_type::reference 							reference;
		typedef typename allocator_type::const_reference					const_reference;
		typedef typename allocator_type::pointer							pointer;
		typedef typename allocator_type::const_pointer 						const_pointer;

		typedef typename ft::binary_tree_iterator<T, IS_CONST>				iterator;
		typedef typename ft::binary_tree_iterator<T, IS_CONST>				const_iterator;
		typedef ft::reverse_iterator<iterator>								reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>						const_reverse_iterator;

		typedef typename ft::iterator_traits<iterator>::difference_type 	difference_type;
		typedef typename allocator_type::size_type							size_type;

		/* #endregion */

		private:

			class set_key_getter {
				public:
					typedef value_type		key_type;

					key_type const & get(value_type const *val) const { return *val; }
			};

			typedef ft::binary_tree<value_type, set_key_getter, key_compare>	binary_tree;

			key_compare			_M_comp;
			allocator_type	 	_M_alloc;
			binary_tree 		_M_tree;

		/* #region initialization */

		public:
			explicit set (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : _M_comp(comp), _M_alloc(alloc), _M_tree(comp) {}

			template <class InputIterator>
			set (InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type(), typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = NULL) : _M_comp(comp), _M_alloc(alloc), _M_tree(comp) {
				this->_range_copy(first, last);
			}

			set (const set& x) : _M_comp(x._M_comp), _M_alloc(x._M_alloc), _M_tree(x._M_comp) {
				*this = x;
			}

			~set() {
				this->clear();
			}

			set& operator= (const set& x) {
				if (this == &x)
					return *this;

				this->clear();

				this->_range_copy(x.begin(), x.end());

				return *this;
			}

		/* #endregion */

		/* #region iterators */
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

		void _delete_value(const value_type *p) {
			pointer to_del = (pointer)p;
			this->_M_alloc.destroy(to_del);
			this->_M_alloc.deallocate(to_del, 1);
		}

		void _deep_delete(typename binary_tree::node *n) {
			pointer p = n->value;
			this->_M_tree.erase(n);
			this->_delete_value(p);
		}

	/* #region modifiers */

	public:
		ft::pair<iterator, bool> insert(const value_type& val) {
			typename binary_tree::node *n = _M_tree.get_node(val);

			if (n)
				return ft::pair<iterator, bool>(iterator(n), false);
			
			pointer p = this->_create_value(val);
			
			n = _M_tree.insert(p);

			return ft::pair<iterator, bool>(iterator(n), true);
		}

		iterator insert(iterator position, const value_type & val) {
			typename binary_tree::node *n = _M_tree.get_node(val);
			
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
		void insert (InputIterator first, InputIterator last) {
			this->_range_copy(first, last);
		}

		void erase (iterator position) {
			typename binary_tree::node *n = _M_tree.get_node(*position);
			this->_deep_delete(n);
		}

		size_type erase (const value_type& val) {
			typename binary_tree::node *n = _M_tree.get_node(val);

			if (n == NULL)
				return 0;

			this->_deep_delete(n);

			return 1;
		}

		void erase (iterator first, iterator last) {
			while (first != last) {
				iterator to_del = first;
				first++;

				typename binary_tree::node *n = _M_tree.get_node(*to_del);

				this->_deep_delete(n);
			}
		}

		void swap (set & x) {
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

	public:
		key_compare key_comp() const { return this->_M_comp; }

		value_compare value_comp() const { return this->_M_comp; }

	public:
		iterator find (const value_type& val) const {
			typename binary_tree::node *n = _M_tree.get_node(val);
			
			if (n == NULL)
				n = _M_tree.past_the_end();

			return iterator(n);
		}

		size_type count (const value_type& val) const {
			typename binary_tree::node *n = this->_M_tree.get_node(val);
			
			if (n)
				return 1;

			return 0;
		}

		iterator lower_bound (const value_type& val) const {
			iterator it = this->begin();
			iterator ite = this->end();
			
			for (; it != ite; it++) {
				if (this->_M_comp(val, *it) || this->_M_comp(*it, val) == false) // if (key < it) or if (key >= it && it >= key)
					return it;
			}

			return ite;
		}

		iterator upper_bound (const value_type& val) const {
			iterator it = this->begin();
			iterator ite = this->end();
			
			for (; it != ite; it++) {
				if (this->_M_comp(val, *it)) // if (key < it)
					return it;
			}

			return ite;
		}

		ft::pair<iterator,iterator> equal_range (const value_type& val) const {
			ft::pair<iterator, iterator> range(this->lower_bound(val), this->upper_bound(val));

			return range;
		}

		public:
			allocator_type get_allocator() const { return this->_M_alloc; }
	};

	/* #region relation operators */

	template <class T, class Compare, class Alloc>
	bool operator==( const set<T,Compare,Alloc>& lhs, const set<T,Compare,Alloc>& rhs) {
		if (lhs.size() != rhs.size())
			return false;

		typename set<T,Compare,Alloc>::const_iterator lit = lhs.cbegin();
		typename set<T,Compare,Alloc>::const_iterator lite = lhs.cend();
		typename set<T,Compare,Alloc>::const_iterator rit = rhs.cbegin();
		typename set<T,Compare,Alloc>::const_iterator rite = rhs.cend();

		for(; lit != lite; lit++) {
			if (*lit != *rit)
				return false;
			rit++;
		}

		return true;
	}

	template <class T, class Compare, class Alloc>
	bool operator!=( const set<T,Compare,Alloc>& lhs, const set<T,Compare,Alloc>& rhs) {
		return !(lhs == rhs);
	}

	template <class T, class Compare, class Alloc>
	bool operator<( const set<T,Compare,Alloc>& lhs, const set<T,Compare,Alloc>& rhs) {
		typename set<T,Compare,Alloc>::const_iterator lit = lhs.cbegin();
		typename set<T,Compare,Alloc>::const_iterator lite = lhs.cend();
		typename set<T,Compare,Alloc>::const_iterator rit = rhs.cbegin();
		typename set<T,Compare,Alloc>::const_iterator rite = rhs.cend();

		return lexicographical_compare(lit, lite, rit, rite);
	}

	template <class T, class Compare, class Alloc>
	bool operator>( const set<T,Compare,Alloc>& lhs, const set<T,Compare,Alloc>& rhs) {
		return rhs < lhs;
	}

	template <class T, class Compare, class Alloc>
	bool operator>=( const set<T,Compare,Alloc>& lhs, const set<T,Compare,Alloc>& rhs) {
		return !(lhs < rhs);
	}

	template <class T, class Compare, class Alloc>
	bool operator<=( const set<T,Compare,Alloc>& lhs, const set<T,Compare,Alloc>& rhs) {
		return !(lhs > rhs);
	}

	/* #endregion */
}

#endif