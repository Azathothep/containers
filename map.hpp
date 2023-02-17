#ifndef MAP_HPP
# define MAP_HPP

# include "ft/pair.hpp"
# include "ft/make_pair.hpp"

# include "node.hpp"
# include "b_tree.hpp"

namespace ft
{

	/* #region map_iterator */

	#define IS_CONST true

	template <typename T, bool constness>
	struct is_const;

	template <typename T>
	struct is_const<T, false> {
		typedef T type;
	};

	template <typename T>
	struct is_const<T, true> {
		typedef const T type;
	};

	template <typename T, bool constness = false>
	class map_iterator {
	
	public:
		typedef typename std::forward_iterator_tag 									iterator_category;
		typedef typename std::ptrdiff_t 											difference_type;
		typedef typename is_const<T, constness>::type								value_type;
		typedef	value_type*														 	pointer;
		typedef value_type&														 	reference;
	};

	/* #endregion */

	/* #region map */

	template <class Key, class T, class Compare = less<Key>, class Alloc = allocator<ft::pair<const Key, T> > >
	class map {

	/* #region typedefs */

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

		typedef list_iterator<T>										iterator;
		typedef list_iterator<T, true>									const_iterator;
		typedef ft::reverse_iterator<iterator>							reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>					const_reverse_iterator;

		typedef typename ft::iterator_traits<iterator>::difference_type	difference_type;
		typedef typename allocator_type::size_type						size_type;

	/* #endregion */

	private:
		ft::B_TREE _M_tree;

	/* #region intialization */

	public:
		explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) { }

		template <class InputIterator>
		map (InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : root(NULL) { }

		map (const map& x) {
			*this = x;
		}

		~map() {}

		map& operator= (const map& x) { }

	/* #endregion */

	/* #region iterators */

	public:
		iterator begin() { }
		const_iterator begin() const { }
		const_iterator cbegin() const { }
		iterator end() { }
		const_iterator end() const { }
		const_iterator cend() const { }
		reverse_iterator rbegin() const { }
		const_reverse_iterator crbegin() const { }
		reverse_iterator rend() const { }
		const_reverse_iterator crend() const { }

	/* #endregion */

	/* #region capacity */

	public:
		bool empty const() { }

		size_type size() const { }

		size_type max_size() const { }

	/* #endregion */

	/* #region element_access */

	public:
		mapped_type& operator[] (const key_type& k) { }

		mapped_type& at (const key_type& k) { }

		const mapped_type& at (const key_type& k) const { }

	/* #endregion */

	/* #region modifiers */

	public:
		ft::pair<iterator, bool> insert (const value_type& val) {
			// _tree.insert();

			
		}

		iterator insert (iterator position, const value_type& val) { }

		template <class InputIterator>
		void insert (InputIterator first, InputIterator last) { }

		void erase (iterator position) { }

		size_type erase (const key_type& k) { }

		void erase (iterator first, iterator last) { }

		void swap (map& x) { }

		void clear() { }

	/* #endregion */

	/* #region observers */

	public:
		key_compare key_comp() const { }

		value_compare value_comp() const { }

	/* #endregion */

	/* #region operations */

	public:
		iterator find (const key_type& k) { }

		const_iterator find (const key_type& k) const { }

		size_type count (const key_type& k) const { }

		iterator lower_bound (const key_type& k) { }

		const_iterator lower_bound (const key_type& k) const { }

		iterator upper_bound (const key_type& k) { }

		const_iterator upper_bound (const key_type& k) const { }

		ft::pair<const_iterator, const_iterator> equal_range (const key_type& k) const { }

		ft::pair<iterator, iterator> equal_range (const key_type& k) { }

	/* #endregion */

	public:
		allocator_type get_allocator() const { }
	}

	/* #endregion */
}

#endif