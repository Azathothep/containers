#include "Debug.hpp"

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <stdexcept>
#include "std/iterator_traits.hpp"
#include "std/reverse_iterator.hpp"
#include "std/enable_if.hpp"
#include "std/is_integral.hpp"
#include <stdlib.h>
#include <cstring>

namespace ft
{
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

	/* #region iterators */

	template <typename T, bool constness = false>
	class vector_iterator
	{

	public:
		typedef typename std::forward_iterator_tag 									iterator_category;
		typedef typename std::ptrdiff_t 											difference_type;
		typedef typename is_const<T, constness>::type							value_type;
		typedef	value_type*														 	pointer;
		typedef value_type&														 	reference;

	private:
		pointer _m_ptr;

	public:
		vector_iterator() {}
		vector_iterator(pointer __p) : _m_ptr(__p) {}

		template <bool c>
		vector_iterator(const vector_iterator<T, c> &rhs) : _m_ptr(&(*rhs)) {}

		template <bool c>
		vector_iterator &operator=(const vector_iterator<T, c> &rhs) {
			this->_m_ptr = &(*rhs);
			return *this;
		}

		~vector_iterator() {}

		reference operator*() const { return *_m_ptr; }

		/* #region operators */

		vector_iterator operator+(difference_type n) const
		{
			vector_iterator result(_m_ptr + n);
			return result;
		}

		reference operator[](difference_type n) const {
			return *(*this + n);
		}

		vector_iterator &operator++()
		{
			_m_ptr++;
			return *this;
		}

		vector_iterator operator++(int)
		{
			vector_iterator iterator = *this;
			++(*this);
			return iterator;
		}

		vector_iterator &operator+=(difference_type n)
		{
			_m_ptr += n;
			return *this;
		}

		vector_iterator operator-(difference_type n) const
		{
			vector_iterator result(_m_ptr - n);
			return result;
		}

		vector_iterator &operator--()
		{
			_m_ptr--;
			return *this;
		}

		vector_iterator operator--(int)
		{
			vector_iterator iterator = *this;
			--(*this);
			return iterator;
		}

		vector_iterator &operator-=(difference_type n)
		{
			_m_ptr -= n;
			return *this;
		}

		pointer operator->() const
		{
			return _m_ptr;
		}

		template <bool c>
		bool operator==(const vector_iterator<T, c> &rhs) const
		{
			return &(**this) == &(*rhs);
		}

		template <bool c>
		bool operator!=(const vector_iterator<T, c> &rhs) const
		{
			return !(*this == rhs);
		}

		template <bool c>
		bool operator<(const vector_iterator<T, c> &rhs) const
		{
			return &(**this) < &(*rhs);
		}

		template <bool c>
		bool operator<=(const vector_iterator<T, c> &rhs) const
		{
			return !(*this > rhs);
		}

		template <bool c>
		bool operator>(const vector_iterator<T, c> &rhs) const
		{
			return (!(*this < rhs) && (*this != rhs));
		}

		template <bool c>
		bool operator>=(const vector_iterator<T, c> &rhs) const
		{
			return !(*this < rhs);
		}

		template <bool c>
		difference_type operator-(const vector_iterator<T, c> &rhs) const
		{
			return &(**this) - &(*rhs);
		}

		/* #endregion */
	};

	template <typename T, bool c>
	vector_iterator<T, c> operator+ (typename vector_iterator<T, c>::difference_type lhs, const vector_iterator<T, c> & rhs) {
		vector_iterator<T, c> result = rhs + lhs;
		return result;
	}

	template <typename T, bool c>
	vector_iterator<T, c> operator- (typename vector_iterator<T, c>::difference_type lhs, const vector_iterator<T, c> & rhs) {
		vector_iterator<T, c> result = rhs - lhs;
		return result;
	}

	/* #endregion */

	template <class T, class Alloc = std::allocator<T> >
	class vector
	{
		/* #region types definition */

	public:
		typedef T value_type;
		typedef Alloc allocator_type;

		typedef typename allocator_type::reference 						reference;
		typedef typename allocator_type::const_reference 				const_reference;
		typedef typename allocator_type::pointer 						pointer;
		typedef typename allocator_type::const_pointer 					const_pointer;

		typedef vector_iterator<T> 											iterator;
		typedef vector_iterator<T, true>		 							const_iterator;
		typedef ft::reverse_iterator<iterator> 								reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> 						const_reverse_iterator;
		typedef typename ft::iterator_traits<iterator>::difference_type 	difference_type;
		typedef typename allocator_type::size_type 							size_type; // not sure if it's right

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
			this->_M_data._M_finish += n;
		}

		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type(), typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = NULL)
		{
			Debug::Log << "Vector: Iterator Constructor Called" << std::endl;

			this->_M_alloc = alloc;

			size_type size = this->_distance(first, last);
			
			this->_create_storage(size * 2);

			InputIterator it = first;
			pointer p = this->_M_data._M_start;

			for (; it != last; it++) {
				this->_M_alloc.construct(p, *it);
				p++;
			}
			
			this->_M_data._M_finish = this->_M_data._M_start + size;
		}

		vector(const vector &x)
		{
			Debug::Log << "Vector: Copy Constructor Called" << std::endl;
			*this = x;
		}

		~vector()
		{
			Debug::Log << "Vector: Destructor Called" << std::endl;
			
			this->clear();

			this->_M_alloc.deallocate(this->_M_data._M_start, this->capacity());
		}

		vector &operator=(const vector &x)
		{
			Debug::Log << "Vector: operator= Called" << std::endl;
			
			this->_M_alloc = x.get_allocator();

			this->_create_storage(x.capacity());

			this->_range_copy(x._M_data._M_start, this->_M_data._M_start, x.size());

			this->_M_data._M_finish = this->_M_data._M_start + x.size();

			return *this;
		}

		/* #endregion */

		/* #region iterators */

		iterator begin() { return iterator(this->_M_data._M_start); }
		const_iterator begin() const { return const_iterator(this->_M_data._M_start); }
		const_iterator cbegin() const { return const_iterator(this->_M_data._M_start); }
		iterator end() { return iterator(this->_M_data._M_finish); }
		const_iterator end() const { return const_iterator(this->_M_data._M_finish); }
		const_iterator cend() const { return const_iterator(this->_M_data._M_finish); }
		reverse_iterator rbegin() const { return reverse_iterator(end()); }
		const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); };
		reverse_iterator rend() const { return reverse_iterator(begin()); }
		const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); };

		/* #endregion */

		size_type size() const {
			return size_type(this->_M_data._M_finish - this->_M_data._M_start);
		}

		size_type max_size() const {
			return _M_alloc.max_size();
		}

		void	resize(size_type n, value_type val = value_type()) { 
			size_type size = this->size();
			
			if (n == size)
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
				size_type remaining = n - size;

				for (int i = 0; i < (int)remaining; i++)
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
			return const_reference(*(this->_M_data._M_start + n));
		}

		reference at(size_type n) {
			if (n >= this->size())
				throw std::out_of_range("vector: at");
			return (*this)[n];
		}

		const_reference at (size_type n) const {
			if (n >= this->size())
				throw std::out_of_range("vector: at");
			return (*this)[n];
		 }

		reference front() {
			Debug::Log << "Vector: front()"<< std::endl;
			return *this->_M_data._M_start;
		}

		const_reference front() const {
			return const_reference(*this->_M_data._M_start);
		}

		reference back() {
			Debug::Log << "Vector: back()"<< std::endl;
			return *(this->_M_data._M_finish - 1);
		}

		const_reference back() const {
			return const_reference(*(this->_M_data._M_finish - 1));
		}

		value_type* data() {
			return _M_data._M_start;
		 }

		const value_type* data() const {
			return _M_data._M_start;
		 }

		// # modifiers

		void _clean_sized(size_type n) {
			this->clear();
			
			if (n > this->capacity()) {
				size_type new_capacity = n * 2;
				if (new_capacity == 0)
					new_capacity = BASE_CAPACITY;

				this->_M_alloc.deallocate(this->_M_data._M_start, this->capacity());

				this->_create_storage(new_capacity);
			}
		}

		template <class InputIterator>
		void assign(InputIterator first, InputIterator last, typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = NULL) {
			size_type n = this->_distance(first, last);
			
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

			if (this->_M_data._M_finish >= this->_M_data._M_end_of_storage)
			{
				Debug::Log << "Vector: size insufficient, reallocating..." << std::endl;

				size_type new_capacity = this->capacity() * 2;
				if (new_capacity == 0)
					new_capacity = BASE_CAPACITY;

				this->_realloc(new_capacity);
			}

			Debug::Log << "Vector: adding value to vector" << std::endl;
			this->_construct(this->_M_data._M_finish, __x);
			this->_M_data._M_finish += 1;
		}

		void pop_back()
		{
			Debug::Log << "Vector: pop_back()" << std::endl;

			if (this->empty())
				return;

			this->_M_alloc.destroy(this->_M_data._M_finish - 1);

			this->_M_data._M_finish -= 1;
		}
	
		iterator insert(iterator position, const value_type& val) {
			Debug::Log << "Vector: single insert" << std::endl;

			size_type target_size = this->size() + 1;
			size_type elems_before = position - this->begin();
			size_type elems_after = this->size() - elems_before;

			if (target_size <= this->capacity()) {
				pointer insert_pos = this->_M_data._M_start + elems_before;
				
				memmove(insert_pos + 1, insert_pos, elems_after * sizeof(value_type));
				this->_construct(insert_pos, val);
				this->_M_data._M_finish += 1;
				return position;
			}
			else {
				pointer old_start = this->_M_data._M_start;
				size_type old_capacity = this->capacity();

				size_type new_capacity = old_capacity * 2;
				if (new_capacity == 0)
					new_capacity = BASE_CAPACITY;

				this->_create_storage(new_capacity);

				memmove(this->_M_data._M_start, old_start, elems_before * sizeof(value_type));

				pointer insert_pos = this->_M_data._M_start + elems_before;

				this->_construct(insert_pos, val);

				memmove(this->_M_data._M_start + elems_before + 1, old_start + elems_before, elems_after * sizeof(value_type));

				this->_M_data._M_finish = this->_M_data._M_start + elems_before + elems_after + 1;

				this->_M_alloc.deallocate(old_start, old_capacity);

				return (iterator(insert_pos));			
			}
		}

		void insert(iterator position, size_type n, const value_type& val) {
			Debug::Log << "Vector: Multiple insert of size " << n << std::endl;

			size_type target_size = this->size() + n;
			size_type elems_before = position - this->begin();
			size_type elems_after = this->size() - elems_before;

			if (target_size <= this->capacity()) {
				pointer insert_pos = this->_M_data._M_start + elems_before;

				memmove(insert_pos + n, insert_pos, elems_after * sizeof(value_type));

				for (int i = 0; i < (int)n; i++)
					this->_construct(insert_pos + i, val);
				
				this->_M_data._M_finish += n;
			}
			else {
				pointer old_start = this->_M_data._M_start;
				size_type old_capacity = this->capacity();

				size_type new_capacity = target_size * 2;

				this->_create_storage(new_capacity);

				memmove(this->_M_data._M_start, old_start, elems_before * sizeof(value_type));

				pointer insert_pos = this->_M_data._M_start + elems_before;

				for (int i = 0; i < (int)n; i++)
					this->_construct(insert_pos + i, val);

				memmove(this->_M_data._M_start + elems_before + n, old_start + elems_before, elems_after * sizeof(value_type));

				this->_M_data._M_finish = this->_M_data._M_start + elems_before + elems_after + n;

				this->_M_alloc.deallocate(old_start, old_capacity);		
			}
		}

		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last, typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = NULL) {
			Debug::Log << "Vector: Iterator insert of size " << this->_distance(first, last) << std::endl;

			size_type n = this->_distance(first, last);
			size_type target_size = this->size() + n;
			size_type elems_before = position - this->begin();
			size_type elems_after = this->size() - elems_before;

			if (target_size <= this->capacity()) {
				pointer insert_pos = this->_M_data._M_start + elems_before;

				memmove(insert_pos + n, insert_pos, elems_after * sizeof(value_type));

				for (int i = 0; i < (int)n; i++)
				{
					value_type val = *(this->_add(first, i));
					this->_construct(insert_pos + i, val);
				}
				
				this->_M_data._M_finish += n;
			}
			else {
				pointer old_start = this->_M_data._M_start;
				size_type old_capacity = this->capacity();

				// estimate new capacity
				size_type new_capacity = target_size * 2;

				this->_create_storage(new_capacity);

				memmove(this->_M_data._M_start, old_start, elems_before * sizeof(value_type));

				pointer insert_pos = this->_M_data._M_start + elems_before;

				for (int i = 0; i < (int)n; i++)
				{
					value_type val = *(this->_add(first, i));
					this->_construct(insert_pos + i, val);
				}

				memmove(this->_M_data._M_start + elems_before + n, old_start + elems_before, elems_after * sizeof(value_type));

				this->_M_data._M_finish = this->_M_data._M_start + elems_before + elems_after + n;

				this->_M_alloc.deallocate(old_start, old_capacity);		
			}
		}

		// ERASE PARTIALLY WORKING - check the proper construction / destruction of elements
		iterator erase(iterator position) {
			Debug::Log << "Vector: single erase" << std::endl;

			pointer erase_pos = &(*position);
			size_type elems_after = erase_pos - this->_M_data._M_start;

			this->_M_alloc.destroy(erase_pos);
			// Does this have to reconstruct the objects ? If so, we have to destroy the elements after each relocation...
			memmove(erase_pos, erase_pos + 1, elems_after * sizeof(value_type));
			this->_M_data._M_finish -= 1;

			return (position);
		}

		iterator erase(iterator first, iterator last) {
			Debug::Log << "Vector: multiple erase of size " << this->_distance(first, last) << std::endl;

			pointer erase_pos = &(*first);
			size_type n = this->_distance(first, last);
			size_type elems_after = erase_pos - this->_M_data._M_start + n;

			for (int i = 0; i < (int)n; i++)
				this->_M_alloc.destroy(erase_pos + i);

			// Does this have to reconstruct the objects ? If so, we have to destroy the elements after each relocation...
			memmove(erase_pos, erase_pos + n, elems_after * sizeof(value_type));
			this->_M_data._M_finish -= n;

			return (iterator(this->_M_data._M_start + n));
		}

		void swap(vector& x) {
			Debug::Log << "Vector: swapping" << std::endl;

			pointer xStart = x._M_data._M_start;
			pointer xFinish = x._M_data._M_finish;
			pointer xEndOfStorage = x._M_data._M_end_of_storage;

			x._M_data._M_start = this->_M_data._M_start;
			x._M_data._M_finish = this->_M_data._M_finish;
			x._M_data._M_end_of_storage = this->_M_data._M_end_of_storage;

			this->_M_data._M_start = xStart;
			this->_M_data._M_finish = xFinish;
			this->_M_data._M_end_of_storage = xEndOfStorage;
		};

		void clear() {
			Debug::Log << "Vector: clean" << std::endl;

			this->_destroy(this->_M_data._M_start, this->_M_data._M_finish);
			this->_M_data._M_finish = this->_M_data._M_start;
		}

		allocator_type get_allocator() const {
			return this->_M_alloc;
		}

		/* #endregion */

		/* #region private methods */

	private:

		template <typename Iterator>
		size_type _distance(Iterator first, Iterator last) {
			size_type n = 0;

			for (Iterator it = first; it != last; it++)
				n++;

			return n;
		}

		template <typename Iterator>
		Iterator _add(Iterator it, size_type n) {
			for (int i = 0; i < (int)n; i++)
				it++;

			return it;
		}

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
			//pointer old_finish = this->_M_data._M_finish;
			size_type	old_capacity = this->capacity();

			const size_type old_size = this->size();

			this->_create_storage(new_capacity);

			memmove(this->_M_data._M_start, old_start, old_size * sizeof(value_type));

			this->_M_data._M_finish = this->_M_data._M_start + old_size;

			this->_M_alloc.deallocate(old_start, old_capacity);
		}

		void _range_copy(pointer _src, pointer _dest, size_type _size)
		{
			Debug::Log << "Vector: Range copy of size " << _size << std::endl;
			
			// Checking if range overlaps
			if (_src < _dest && (_src + _size > _dest))
			{
				pointer p_src = _src + _size - 1;
				pointer p_dest = _dest + _size - 1;

				// This moves pointers, it doesn't copy the object !
				for (size_type i = 0; i < _size; i++)
				{
					Debug::Log << "Copying value from " << p_src << " to " << p_dest << std::endl;
					*p_dest = *p_src;
					p_src--;
					p_dest--;
				}
			}
			else {
				pointer p_src = _src;
				pointer p_dest = _dest;

				// This moves pointers, it doesn't copy the object !
				for (size_type i = 0; i < _size; i++)
				{
					Debug::Log << "Copying value from " << p_src << " to " << p_dest << std::endl;
					*p_dest = *p_src;
					p_src++;
					p_dest++;
				}
			}
			
		}

		void _destroy(pointer start, pointer end) {
			Debug::Log << "Vector: Destroying " << (end - start) << " elements" << std::endl;
			
			pointer p;

			for (p = start; p < end; p++) {
				this->_M_alloc.destroy(p);
			}
		}

		/* #endregion */
	};
}

#endif