#ifndef MAP_HPP
# define MAP_HPP

# include "ft/pair.hpp"
# include "ft/make_pair.hpp"

namespace ft
{

	template <typename T>
	struct node<T> {
		T & value;
		node * left;
		node * right;
	};

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
	class list_iterator {
	
	public:
		typedef typename std::forward_iterator_tag 									iterator_category;
		typedef typename std::ptrdiff_t 											difference_type;
		typedef typename is_const<T, constness>::type							value_type;
		typedef	value_type*														 	pointer;
		typedef value_type&														 	reference;
	};

	template <class Key, class T, class Compare = less<Key>, class Alloc = allocator<ft::pair<const Key, T> > >
	class map {

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

	public:
		explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) { }

		template <class InputIterator>
		map (InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) { }

		map (const map& x) {
			*this = x;
		}

		~map() {}

		map& operator= (const map& x) { }
	}


}

#endif