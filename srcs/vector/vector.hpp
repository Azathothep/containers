#include "../../Debug.hpp"

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <stdexcept>
#include "../std/iterator_traits.hpp"
#include "../std/reverse_iterator.hpp"

namespace ft
{

	template <class T, class Alloc = std::allocator<T> >
	class vector
	{

		/* #region iterators */

		template <bool flag, class IsTrue, class IsFalse>
		struct choose;

		template <class IsTrue, class IsFalse>
		struct choose<true, IsTrue, IsFalse> {
			typedef IsTrue type;
		};

		template <class IsTrue, class IsFalse>
		struct choose <false, IsTrue, IsFalse> {
			typedef IsFalse type;
		};

		template <typename vector, bool is_const>
		class _vector_iterator
		{

		public:
			typedef typename std::forward_iterator_tag 									iterator_category;
			typedef typename std::ptrdiff_t 											difference_type;
			typedef typename vector::value_type 										value_type;
			typedef typename choose<is_const, const value_type *, value_type *>::type 	pointer;
			typedef typename choose<is_const, const value_type &, value_type &>::type 	reference;

		private:
			pointer _m_ptr;

		public:
			_vector_iterator() {}
			_vector_iterator(pointer __p) : _m_ptr(__p) {}
			_vector_iterator(const _vector_iterator &rhs): _m_ptr(rhs._m_ptr) {}

			reference operator*() const
			{
				return *_m_ptr;
			}

			/* #region operators */

			_vector_iterator operator+(difference_type n) const
			{
				_vector_iterator result(_m_ptr + n);
				return result;
			}

			_vector_iterator &operator++()
			{
				_m_ptr++;
				return *this;
			}

			_vector_iterator operator++(int)
			{
				_vector_iterator iterator = *this;
				++(*this);
				return iterator;
			}

			_vector_iterator &operator+=(difference_type n)
			{
				_m_ptr += n;
				return *this;
			}

			_vector_iterator operator-(difference_type n) const
			{
				_vector_iterator result(_m_ptr - n);
				return result;
			}

			_vector_iterator &operator--()
			{
				_m_ptr--;
				return *this;
			}

			_vector_iterator operator--(int)
			{
				_vector_iterator iterator = *this;
				--(*this);
				return iterator;
			}

			_vector_iterator &operator-=(difference_type n)
			{
				_m_ptr -= n;
				return *this;
			}

			pointer operator->() const
			{
				return _m_ptr;
			}

			reference operator[](int index) const
			{
				return *(_m_ptr + index);
			}

			bool operator==(const _vector_iterator &rhs) const
			{
				return _m_ptr == rhs._m_ptr;
			}

			bool operator!=(const _vector_iterator &rhs) const
			{
				return !(*this == rhs);
			}

			bool operator<(const _vector_iterator &rhs) const
			{
				return _m_ptr < rhs._m_ptr;
			}

			bool operator<=(const _vector_iterator &rhs) const
			{
				return !(*this > rhs);
			}

			bool operator>(const _vector_iterator &rhs) const
			{
				return _m_ptr > rhs._m_ptr;
			}

			bool operator>=(const _vector_iterator &rhs) const
			{
				return !(*this < rhs);
			}

			difference_type operator-(const _vector_iterator &rhs) const
			{
				return _m_ptr - rhs._m_ptr;
			}

			// operator+ (difference_type& lhs, const _vector_iterator& rhs)

			/* #endregion */
		};

		/* #endregion */

		/* #region types definition */

	public:
		typedef T value_type;
		typedef Alloc allocator_type;

		typedef typename allocator_type::reference 						reference;
		typedef typename allocator_type::const_reference 				const_reference;
		typedef typename allocator_type::pointer 						pointer;
		typedef typename allocator_type::const_pointer 					const_pointer;

		typedef _vector_iterator<vector<T>, false > 					iterator;
		typedef _vector_iterator<vector<T>, true > 						const_iterator;
		typedef reverse_iterator<iterator> 								reverse_iterator;
		// typedef typename reverse_iterator<const_iterator> 			const_reverse_iterator;
		typedef typename iterator_traits<iterator>::difference_type 	difference_type;
		typedef typename allocator_type::size_type 						size_type; // not sure if it's right

		/* #endregion */

		/* #region private fields */

	private:
		#define __NOTIFY_GROWTH(n) this->_M_data._M_finish += n
		#define __NOTIFY_SHRINK(n) this->_M_data._M_finish -= n
		#define BASE_CAPACITY 5

		struct _VECTOR_DATA
		{
			pointer _M_start;
			pointer _M_finish;
			pointer _M_end_of_storage;
		};

		_VECTOR_DATA	_M_data;
		allocator_type	_M_alloc;

		/* #endregion */

		/* #region public methods */
	public:
		/* #region constructors & destructor */
		explicit vector(const allocator_type &alloc = allocator_type())
		{
			Debug::Log << "Vector: Default Constructor Called" << std::endl;

			this->_M_alloc = alloc;
			
			this->_M_data._M_start = NULL;
			this->_M_data._M_finish = NULL;
			this->_M_data._M_end_of_storage = NULL;
			this->_create_storage(0);
		}

		explicit vector(size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type())
		{
			Debug::Log << "Vector: Size Constructor Called" << std::endl;

			this->_M_alloc = alloc;

			this->_create_storage(n * 2);
			this->_fill(this->_M_data._M_start, val, n);
			__NOTIFY_GROWTH(n);
		}

		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type())
		{
			Debug::Log << "Vector: Iterator Constructor Called" << std::endl;
		}

		vector(const vector &x)
		{
			Debug::Log << "Vector: Copy Constructor Called" << std::endl;
			*this = x;
		}

		~vector()
		{
			Debug::Log << "Vector: Destructor Called" << std::endl;
			
			iterator it;
			
			for (it = begin(); it != end(); it++)
				_M_alloc.destroy(&(*it));

			_M_alloc.deallocate(_M_data._M_start, this->capacity());
		}

		vector &operator=(const vector &x)
		{
			Debug::Log << "Vector: operator= Called" << std::endl;
			this->_M_alloc = x.get_allocator();

			this->_create_storage(x.capacity());


		}

		/* #endregion */

		/* #region iterators */

		iterator begin() const { return iterator(this->_M_data._M_start); }
		const_iterator cbegin() const { return const_iterator(this->_M_data._M_start); }
		iterator end() const { return iterator(this->_M_data._M_finish); }
		const_iterator cend() const { return const_iterator(this->_M_data._M_finish); }
		reverse_iterator rbegin() const { return reverse_iterator(end()); }
		// const_reverse_iterator crbegin() const;
		reverse_iterator rend() const { return reverse_iterator(begin()); }
		// const_reverse_iterator crend() const;

		/* #endregion */

		size_type size() const {
			return size_type(this->_M_data._M_finish - this->_M_data._M_start);
		}

		size_type max_size() const {
			return _M_alloc.max_size();
		}

		void	resize(size_type n, value_type val = value_type()) { 
			if (n == this->size())
				return;
			
			if (n < this->size())
			{
				pointer __new_finish = this->_M_data._M_start + n;

				if (n > this->size())
					__new_finish = this->_M_data._M_start;

				pointer __p;
				pointer __end = this->_M_data._M_finish;

				for (__p = __new_finish; __p < __end; __p++)
					this->_M_alloc.destroy(__p);

				this->_M_data._M_finish = __new_finish;
			} else {
				size_type _to_fill = n - this->size();

				for (int i = 0; i < n; i++)
					this->push_back(val);
			}
		}

		size_type capacity() const {
			return size_type(this->_M_data._M_end_of_storage - this->_M_data._M_start);
		}

		bool empty() const {
			return (this->size() == 0);
		}

		void reserve(size_type __n) // throwing exception!
		{
			if (__n <= this->capacity())
				return;

			if (__n > this->max_size())	
				throw new std::length_error("vector reserve");

			this->_realloc(__n);
		}

		void shrink_to_fit() {
			size_type remaining_capacity = this->capacity() - this->size();

			if (remaining_capacity == 0)
				return;

			this->_M_alloc.deallocate(this->_M_data._M_finish, remaining_capacity);

			this->_M_data._M_end_of_storage = this->_M_data._M_finish;
		 }

		// # element access

		reference operator[](size_type n) {
			Debug::Log << "Vector: operator[" << n << "]" << std::endl;
			return *(this->_M_data._M_start + n);
		}

		const_reference operator[](size_type n) const {
			Debug::Log << "Vector: const operator[" << n << "]" << std::endl;
			return const_reference((*this)[n]);
		}

		reference at(size_type n) {
			if (n >= this->size())
				throw new std::out_of_range("vector: at");
			return (*this)[n];
		}

		const_reference at (size_type n) const {
			if (n >= this->size())
				throw new std::out_of_range("vector: at");
			return (*this)[n];
		 }

		reference front() {
			Debug::Log << "Vector: front()"<< std::endl;
			return *(_M_data._M_start);
		}

		const_reference front() const {
			return const_reference(this->front());
		}

		reference back() {
			Debug::Log << "Vector: back()"<< std::endl;
			return *(_M_data._M_finish - 1);
		}

		const_reference back() const {
			return const_reference(this->back());
		}

		value_type* data() {
			return _M_data._M_start;
		 }

		const value_type* data() const {
			return _M_data._M_start;
		 }

		// # modifiers

		void _clean_sized(size_type n) {
			if (n <= this->capacity()) {
				this->_destroy(this->_M_data._M_start, this->_M_data._M_finish);
			} else {
				size_type new_capacity = n * 2;
				if (new_capacity == 0)
					new_capacity = BASE_CAPACITY;

				this->_destroy(this->_M_data._M_start, this->_M_data._M_finish);

				this->_M_alloc.deallocate(this->_M_data._M_start, this->capacity());

				this->_create_storage(new_capacity);
			}
		}

		template <class InputIterator>
		void assign(InputIterator first, InputIterator last) {
			size_type n = last - first;
			
			this->_clean_sized(n);
			
			InputIterator it;
			pointer p = this->_M_data._M_start;

			for (it = first; it != last; it++) {
				this->_M_alloc.construct(p, *it);
				p++;
			}

			this->_M_data._M_finish = this->_M_data._M_start + n;
		}

		void assign(size_type n, const value_type& val) {
			this->_clean_sized(n);

			this->_fill(this->_M_data._M_start, val, n);

			this->_M_data._M_finish = this->_M_data._M_start + n;
		}

		void push_back(const value_type &__x)
		{
			Debug::Log << "Vector: push_back()" << std::endl;

			if (this->_M_data._M_finish < this->_M_data._M_end_of_storage) {
				Debug::Log << "Vector: adding value to vector" << std::endl;
				this->_construct(this->_M_data._M_finish, __x);
				this->_M_data._M_finish += 1;
			}
			else {
				Debug::Log << "Vector: size insufficient, reallocating..." << std::endl;

				size_type new_capacity = this->capacity() * 2;
				if (new_capacity == 0)
					new_capacity = BASE_CAPACITY;

				this->_realloc(new_capacity);

				this->push_back(__x);
			}
		}

		void pop_back()
		{
			Debug::Log << "Vector: pop_back()" << std::endl;

			if (this->empty())
				return;

			this->_M_alloc.destroy(this->_M_data._M_finish - 1);

			this->_M_data._M_finish -= 1;
		}
	
		iterator insert(iterator position, const value_type& val) { }

		void insert(iterator position, size_type n, const value_type& val) { }

		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last) { }

		iterator erase(iterator position) { }

		iterator erase(iterator first, iterator last) { }

		void swap(vector& x);

		void clear() { }

		// # allocator

		allocator_type get_allocator() const {
			return this->_M_alloc;
		}

		/* #endregion */

		/* #region private methods */

	private:
		void _fill(pointer __start, value_type __val, size_type __n)
		{
			Debug::Log << "Vector: Filling " << __n << " elements" << std::endl;

			if (__n == 0)
				return;

			pointer __p;
			pointer __end = __start + __n;

			for (__p = __start; __p < __end; __p++)
				this->_construct(__p, __val);
		}

		pointer _allocate(size_type __n)
		{
			Debug::Log << "Vector: allocating for " << __n << " elements" << std::endl;
			return _M_alloc.allocate(__n);
		}

		void _construct(pointer __elem, value_type __val)
		{
			Debug::Log << "Vector: constructing element at address " << __elem << std::endl;
			_M_alloc.construct(__elem, __val);
		}

		void _create_storage(size_t __n) {
			Debug::Log << "Vector: creating storage of capacity " << __n << std::endl;

			// !!!!! MAX SIZE !!!!!

			this->_M_data._M_start = this->_allocate(__n);
			this->_M_data._M_finish = this->_M_data._M_start;
			this->_M_data._M_end_of_storage = this->_M_data._M_start + __n;
		}

		void _realloc(size_type new_capacity) {
			pointer old_start = this->_M_data._M_start;
			pointer old_finish = this->_M_data._M_finish;

			const size_type old_size = this->size();

			this->_create_storage(new_capacity);

			this->_range_copy(old_start, this->_M_data._M_start, old_size);

			this->_M_data._M_finish = this->_M_data._M_start + old_size;

			this->_destroy_and_deallocate(old_start, old_finish);
		}

		void _realloc_insert(pointer position, value_type val, size_type N = 1) {
			Debug::Log << "Vector: realloc_inserting " << N << " values " << val << " at position " << position << std::endl;

			pointer old_start = this->_M_data._M_start;
			pointer old_finish = this->_M_data._M_finish;

			const size_type elems_before = position - this->_M_data._M_start;
			const size_type elems_after = this->_M_data._M_finish - position;

			// allocate memory & copy original vector to the new one
			size_type new_capacity = this->capacity() + N;

			this->_create_storage(new_capacity);

			this->_range_copy(old_start, this->_M_data._M_start, elems_before);

			pointer insert_position = this->_M_data._M_start + elems_before;

			this->_fill(insert_position, val, N);

			this->_range_copy(old_start + elems_before, insert_position + N, elems_after);

			this->_M_data._M_finish = elems_before + N + elems_after;

			this->_destroy_and_deallocate(old_start, old_finish);
		}

		void _range_copy(pointer _src, pointer _dest, size_type _size)
		{
			Debug::Log << "Vector: Range copy of size " << _size << std::endl;

			pointer p_src = _src;
			pointer p_dest = _dest;

			// Move pointers, don't copy the object !
			for (size_type i = 0; i < _size; i++)
			{
				Debug::Log << "Copying value from " << p_src << " to " << p_dest << std::endl;
				*p_dest = *p_src;
				p_src++;
				p_dest++;
			}
		}

		void _destroy(pointer start, pointer end) {
			Debug::Log << "Vector: Destroying" << (end - start) << " elements" << std::endl;
			
			pointer p;

			for (p = start; p < end; p++) {
				this->_M_alloc.destroy(p);
			}
		}

		void _destroy_and_deallocate(pointer __start, pointer __end)
		{
			this->_destroy(__start, __end);

			size_type __n = __end - __start;

			Debug::Log << "Vector: Deallocating..." << std::endl;
			_M_alloc.deallocate(__start, __n);
		}

		void _shrink_by(size_type __n)
		{
			Debug::Log << "Vector: Shrinking memory by " << __n << std::endl;
			pointer __new_finish = _M_data._M_finish - __n;

			if (__n > this->size())
				__new_finish = _M_data._M_start;

			pointer __p;
			pointer __end = this->_M_data._M_finish;

			for (__p = __new_finish; __p < __end; __p++)
				this->_M_alloc.destroy(__p);
		}

		/* #endregion */
	};
}

#endif