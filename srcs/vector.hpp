#include "../Debug.hpp"

#ifndef VECTOR_HPP
# define VECTOR_HPP

#include <memory>

namespace ft {

	template < typename vector >
	class _vector_iterator {

		public:

			typedef typename 	vector::value_type 	value_type;
			typedef 			value_type* 		pointer;
			typedef 			value_type& 		reference;

		private:
			pointer _pointer;

		public:

			_vector_iterator(pointer p) : _pointer(p) {}

			_vector_iterator& operator++() {
				_pointer++;
				return *this;
			}

			_vector_iterator operator++(int) {
				_vector_iterator iterator = *this;
				++(*this);
				return iterator;
			}

			_vector_iterator& operator--() {
				_pointer--;
				return *this;
			}

			_vector_iterator operator--(int) {
				_vector_iterator iterator = *this;
				--(*this);
				return iterator;
			}

			reference operator[](int index) {
				return *(_pointer + index);
			}

			pointer operator->() {
				return _pointer;
			}

			reference operator*() {
				return *_pointer;
			}

			bool operator==(const _vector_iterator& rhs) const {
				return _pointer == rhs._pointer;
			}

			bool operator!=(const _vector_iterator& rhs) const {
				return !(*this == rhs);
			}
	};

	template < class T, class Alloc = std::allocator<T> >
	class vector
	{
		public:

			typedef 			T 									value_type;
			typedef 			Alloc 								allocator_type;

			typedef typename 	allocator_type::reference 			reference;
			typedef typename 	allocator_type::const_reference 	const_reference;
			typedef typename 	allocator_type::pointer 			pointer;
			typedef typename 	allocator_type::const_pointer 		const_pointer;

			typedef 			_vector_iterator< vector<T> >		iterator;
			//typedef typename {a random access iterator to const value_type} const_iterator;
			//typedef typename reverse_iterator<iterator> 		reverse_iterator;
			//typedef typename reverse_iterator<const_iterator> 	const_reverse_iterator;
			//typedef typename iterator_traits<iterator>::difference_type 	difference_type;
			//typedef typename {an unsigned integral type that can represent any non-negative value of difference_type} size_type;
			typedef 			size_t								size_type;

		private:
			pointer 		_memory;
			unsigned int 	_size;

		public:

			// Constructors
			explicit vector (const allocator_type& alloc = allocator_type()) {
				Debug::Log("vector Default Constructor Called");
			}

			explicit vector (size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()) {
				Debug::Log("vector Size Constructor Called");

				_size = n;
				_memory = alloc.allocate(n * sizeof(value_type));
				Debug::Log(std::string(Debug::ToStr(n) + " elements allocated at address " + Debug::ToStr(_memory)));

				alloc.construct(_memory, val);
			}

			template <class InputIterator> vector (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()) {
				Debug::Log("vector Iterator Constructor Called");
			}

			vector (const vector& x) {
				Debug::Log("vector Copy Constructor Called");
			}

			~vector() {
				Debug::Log("vector Destructor Called");
			}

			vector& operator= (const vector& x) {
				Debug::Log("vector operator= Called");
			}

			iterator begin() { return iterator(_memory); }
			iterator end() { return iterator(_memory[_size]); }
	};

}

#endif