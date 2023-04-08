#ifndef VECTOR_HPP
# define VECTOR_HPP

#include <cstring>

#include "../utils/iterator_traits.hpp"
#include "../utils/reverse_iterator.hpp"
#include "../utils/enable_if.hpp"
#include "../utils/is_integral.hpp"

#include "../utils/is_const.hpp"

namespace ft
{
	/* #region iterator */

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

		template <class _T, bool _ca, bool _cb>
		friend bool operator==(const vector_iterator<_T, _ca>& lhs, const vector_iterator<_T, _cb>& rhs);

		template <class _T, bool _ca, bool _cb>
		friend bool operator!=(const vector_iterator<_T, _ca>& lhs, const vector_iterator<_T, _cb>& rhs);

		template <class _T, bool _ca, bool _cb>
		friend bool operator<(const vector_iterator<_T, _ca>& lhs, const vector_iterator<_T, _cb>& rhs);

		template <class _T, bool _ca, bool _cb>
		friend bool operator<=(const vector_iterator<_T, _ca>& lhs, const vector_iterator<_T, _cb>& rhs);

		template <class _T, bool _ca, bool _cb>
		friend bool operator>(const vector_iterator<_T, _ca>& lhs, const vector_iterator<_T, _cb>& rhs);

		template <class _T, bool _ca, bool _cb>
		friend bool operator>=(const vector_iterator<_T, _ca>& lhs, const vector_iterator<_T, _cb>& rhs);

		template <bool c>
		difference_type operator-(const vector_iterator<T, c> &rhs) const
		{
			return &(**this) - &(*rhs);
		}

		/* #endregion */
	};

	/* #region iterator relational operators */

	template <class T, bool ca, bool cb>
	bool operator==(const vector_iterator<T, ca>& lhs, const vector_iterator<T, cb>& rhs) {
		return &(*lhs) == &(*rhs);
	}

	template <class T, bool ca, bool cb>
	bool operator!=(const vector_iterator<T, ca>& lhs, const vector_iterator<T, cb>& rhs) {
		return !(lhs == rhs);
	}

	template <class T, bool ca, bool cb>
	bool operator<(const vector_iterator<T, ca>& lhs, const vector_iterator<T, cb>& rhs) {
		return &(*lhs) < &(*rhs);
	}

	template <class T, bool ca, bool cb>
	bool operator<=(const vector_iterator<T, ca>& lhs, const vector_iterator<T, cb>& rhs) {
		return !(lhs > rhs);
	}

	template <class T, bool ca, bool cb>
	bool operator>(const vector_iterator<T, ca>& lhs, const vector_iterator<T, cb>& rhs) {
		return (rhs < lhs);
	}

	template <class T, bool ca, bool cb>
	bool operator>=(const vector_iterator<T, ca>& lhs, const vector_iterator<T, cb>& rhs) {
		return !(lhs < rhs);
	}

	/* #endregion */

	template <typename T, bool c>
	vector_iterator<T, c> operator+ (typename vector_iterator<T, c>::difference_type n, const vector_iterator<T, c> & rhs) {
		vector_iterator<T, c> result = rhs + n;
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
		typedef vector_iterator<T, IS_CONST>		 							const_iterator;
		typedef ft::reverse_iterator<iterator> 								reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> 						const_reverse_iterator;
		typedef typename ft::iterator_traits<iterator>::difference_type 	difference_type;
		typedef typename allocator_type::size_type 							size_type; // not sure if it's right

		/* #endregion */

		/* #region private fields */

	private:
		#define __NOTIFY_GROWTH(n) this->_M_data._M_finish += n
		#define __NOTIFY_SHRINK(n) this->_M_data._M_finish -= n
		#define BASE_CAPACITY 1

		struct _VECTOR_DATA
		{
			pointer _M_start;
			pointer _M_finish;
			pointer _M_end_of_storage;

			_VECTOR_DATA() {
				_M_start = NULL;
				_M_finish = NULL;
				_M_end_of_storage = NULL;
			}

			_VECTOR_DATA& operator=(const _VECTOR_DATA& OTHER) {
				this->_M_start = OTHER._M_start;
				this->_M_finish = OTHER._M_finish;
				this->_M_end_of_storage = OTHER._M_end_of_storage;

				return *this;
			}
		};

		_VECTOR_DATA	_M_data;
		allocator_type	_M_alloc;

		/* #endregion */

		/* #region public methods */
	public:
		/* #region constructors & destructor */
		explicit vector(const allocator_type &alloc = allocator_type()) : _M_data(_VECTOR_DATA())
		{
			this->_M_alloc = alloc;
		}

		explicit vector(size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type()) : _M_data(_VECTOR_DATA())
		{
			this->_M_alloc = alloc;

			this->_create_storage(n * 2);
			this->_fill(this->_M_data._M_start, val, n);
			this->_M_data._M_finish += n;
		}

		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type(), typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = NULL) : _M_data(_VECTOR_DATA())
		{
			this->_M_alloc = alloc;

			size_type size = this->_distance(first, last);
			
			this->_create_storage(size * 2);

			// sort of a range copy
			pointer p = this->_M_data._M_start;
			InputIterator it = first;

			for (; it != last; it++) {
				this->_M_alloc.construct(p, *it);
				p++;
			}

			this->_M_data._M_finish = this->_M_data._M_start + size;
		}

		vector(const vector &x) : _M_data(_VECTOR_DATA())
		{
			*this = x;
		}

		~vector()
		{	
			if (this->capacity())
				this->_wipe_memory();
		}

		vector &operator=(const vector &x)
		{
			if (this == &x)
				return *this;

			this->assign(x.begin(), x.end());

			return *this;
		}

		/* #endregion */

		/* #region iterators */

		iterator begin() { return iterator(this->_M_data._M_start); }
		const_iterator begin() const { return cbegin(); }
		const_iterator cbegin() const { return const_iterator(this->_M_data._M_start); }
		iterator end() { return iterator(this->_M_data._M_finish); }
		const_iterator end() const { return cend(); }
		const_iterator cend() const { return const_iterator(this->_M_data._M_finish); }
		reverse_iterator rbegin() { return reverse_iterator(iterator(this->_M_data._M_finish)); }
		const_reverse_iterator rbegin() const { return crbegin(); }
		const_reverse_iterator crbegin() const { return const_reverse_iterator(this->cend()); };
		reverse_iterator rend() { return reverse_iterator(iterator(this->_M_data._M_start)); }
		const_reverse_iterator rend() const { return crend(); }
		const_reverse_iterator crend() const { return const_reverse_iterator(this->cbegin()); };

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
				throw std::length_error("vector::reserve");

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
			return *(this->_M_data._M_start + n);
		}

		const_reference operator[](size_type n) const {
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
			return *this->_M_data._M_start;
		}

		const_reference front() const {
			return const_reference(*this->_M_data._M_start);
		}

		reference back() {
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
			if (this->_M_data._M_finish >= this->_M_data._M_end_of_storage)
			{
				size_type new_capacity = this->capacity() * 2;
				if (new_capacity == 0)
					new_capacity = BASE_CAPACITY;

				this->_realloc(new_capacity);
			}

			this->_M_alloc.construct(this->_M_data._M_finish, __x);
			this->_M_data._M_finish += 1;
		}

		void pop_back()
		{
			if (this->empty())
				return;

			this->_M_alloc.destroy(this->_M_data._M_finish - 1);

			this->_M_data._M_finish -= 1;
		}

		

		iterator insert(iterator position, const value_type& val) {
			return this->_insert(position, 1, val);
		}

		void insert(iterator position, size_type n, const value_type& val) {
			this->_insert(position, n, val);
		}

		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last, typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = NULL) {
			size_type n = this->_distance(first, last);
			size_type target_size = this->size() + n;
			size_type elems_before = position - this->begin();
			size_type elems_after = this->size() - elems_before;

			if (target_size <= this->capacity()) {
				pointer insert_pos = this->_M_data._M_start + elems_before;

				this->_move(insert_pos + n, insert_pos, elems_after);

				for (int i = 0; i < (int)n; i++)
				{
					value_type val = *(this->_add(first, i));
					this->_M_alloc.construct(insert_pos + i, val);
				}
				
				this->_M_data._M_finish += n;
			}
			else {
				pointer old_start = this->_M_data._M_start;
				size_type old_capacity = this->capacity();

				// estimate new capacity
				size_type new_capacity = target_size * 2;

				this->_create_storage(new_capacity);

				this->_move(this->_M_data._M_start, old_start, elems_before);

				pointer insert_pos = this->_M_data._M_start + elems_before;

				for (int i = 0; i < (int)n; i++)
				{
					value_type val = *(this->_add(first, i));
					this->_M_alloc.construct(insert_pos + i, val);
				}

				this->_move(this->_M_data._M_start + elems_before + n, old_start + elems_before, elems_after);

				this->_M_data._M_finish = this->_M_data._M_start + elems_before + elems_after + n;

				this->_M_alloc.deallocate(old_start, old_capacity);		
			}
		}

		iterator erase(iterator position) {
			return this->_erase(position, position + 1);
		}

		iterator erase(iterator first, iterator last) {
			return this->_erase(first, last);
		}

		void swap(vector& x) {
			_VECTOR_DATA temp = x._M_data;
			x._M_data = this->_M_data;
			this->_M_data = temp;
		};

		void clear() {
			this->_destroy(this->_M_data._M_start, this->_M_data._M_finish);
			this->_M_data._M_finish = this->_M_data._M_start;
		}

		allocator_type get_allocator() const {
			return this->_M_alloc;
		}

		/* #endregion */

		/* #region private methods */

	private:

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

		iterator _erase(iterator first, iterator last) {
			pointer erase_pos = &(*first);
			size_type n = last - first;
			size_type elems_after = this->_M_data._M_finish - (erase_pos + n);

			for (int i = 0; i < (int)n; i++)
				this->_M_alloc.destroy(erase_pos + i);

			this->_move(erase_pos, erase_pos + n, elems_after);

			this->_M_data._M_finish -= n;

			return (first);
		}

		void _move(pointer dest, pointer src, size_type n) {
			if (dest == src)
				return;
			
			if (dest < src)
			{
				for (int i = 0; i < (int)n; i++) {
					this->_M_alloc.construct(dest + i, src[i]);
					this->_M_alloc.destroy(src + i);
				}
			}
			else {
				for (int i = n - 1; i >= 0; i--) {
					this->_M_alloc.construct(dest + i, src[i]);
					this->_M_alloc.destroy(src + i);
				}
			}

		}

		iterator _insert(iterator position, size_type n, const value_type& val) {
			size_type target_size = this->size() + n;
			size_type elems_before = position - this->begin();
			size_type elems_after = this->size() - elems_before;

			if (target_size <= this->capacity()) {
				pointer insert_pos = this->_M_data._M_start + elems_before;

				this->_move(insert_pos + n, insert_pos, elems_after);

				for (int i = 0; i < (int)n; i++)
					this->_M_alloc.construct(insert_pos + i, val);
				
				this->_M_data._M_finish += n;

				return position;
			}
			else {
				pointer old_start = this->_M_data._M_start;
				size_type old_capacity = this->capacity();

				size_type new_capacity = target_size * 2;

				this->_create_storage(new_capacity);

				this->_move(this->_M_data._M_start, old_start, elems_before);

				pointer insert_pos = this->_M_data._M_start + elems_before;

				for (int i = 0; i < (int)n; i++)
					this->_M_alloc.construct(insert_pos + i, val);

				this->_move(this->_M_data._M_start + elems_before + n, old_start + elems_before, elems_after);

				this->_M_data._M_finish = this->_M_data._M_start + elems_before + elems_after + n;

				this->_M_alloc.deallocate(old_start, old_capacity);

				return iterator(insert_pos);
			}
		}

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
			if (__n == 0)
				return;

			pointer __p;
			pointer __end = __start + __n;

			for (__p = __start; __p < __end; __p++)
				this->_M_alloc.construct(__p, __val);
		}

		void _create_storage(size_t __n) {
			if (__n == 0)
				return;

			this->_M_data._M_start = this->_M_alloc.allocate(__n);
			this->_M_data._M_finish = this->_M_data._M_start;
			this->_M_data._M_end_of_storage = this->_M_data._M_start + __n;
		}

		void _realloc(size_type new_capacity) {
			pointer old_start = this->_M_data._M_start;
			size_type	old_capacity = this->capacity();

			const size_type old_size = this->size();

			this->_create_storage(new_capacity);

			//memmove((void *)this->_M_data._M_start, (void *)old_start, old_size * sizeof(value_type));
			this->_range_move(this->_M_data._M_start, old_start, old_size);

			this->_M_data._M_finish = this->_M_data._M_start + old_size;

			for (size_type i = 0; i < old_size; i++)
				this->_M_alloc.destroy(old_start + i);

			this->_M_alloc.deallocate(old_start, old_capacity);
		}

		void _range_move(pointer dest, pointer src, size_type size) {
			pointer buffer = this->_M_alloc.allocate(size);
			
			for (size_type i = 0; i < size; i++)
				this->_M_alloc.construct(buffer + i, src[i]);
			// for (size_type i = 0; i < size; i++)
			// 	this->_M_alloc.destroy(src + i);
			for (size_type i = 0; i < size; i++)
				this->_M_alloc.construct(dest + i, buffer[i]);
			
			for (size_type i = 0; i < size; i++)
				this->_M_alloc.destroy(buffer + i);

			this->_M_alloc.deallocate(buffer, size);
		}

		void _destroy(pointer start, pointer end) {
			pointer p;

			for (p = start; p < end; p++) {
				this->_M_alloc.destroy(p);
			}
		}

		void _wipe_memory() {
			this->clear();

			this->_M_alloc.deallocate(this->_M_data._M_start, this->capacity());

			this->_M_data._M_start = NULL;
			this->_M_data._M_finish = NULL;
			this->_M_data._M_end_of_storage = NULL;
		}

		/* #endregion */
	};

	/* #region relational operators */

	template <class T, class Alloc> 
	bool operator== (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {		
		if (lhs.size() != rhs.size())
			return false;

		typename ft::vector<T, Alloc>::const_iterator lit = lhs.cbegin();
		typename ft::vector<T, Alloc>::const_iterator rit = rhs.cbegin();

		for (; (lit != lhs.cend() && rit != rhs.cend()); lit++) {
			if (*lit != *rit)
				return false;
			rit++;
		}

		if (lit != lhs.cend() || rit != rhs.cend())
			return false;

		return true;
	}

	template <class T, class Alloc>
	bool operator!= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
		return !(lhs == rhs);
	}

	template <class T, class Alloc>
	bool operator<  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
		typename ft::vector<T, Alloc>::const_iterator lit = lhs.cbegin();
		typename ft::vector<T, Alloc>::const_iterator rit = rhs.cbegin();

		while (lit != lhs.cend())
		{
			if (rit == rhs.cend() || *lit > *rit)
				return false;
			else if (*lit < *rit)
				return true;

			lit++; rit++;
		}

		return rit != rhs.cend();
	}

	template <class T, class Alloc>
	bool operator<= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
		return !(lhs > rhs);
	}

	template <class T, class Alloc>
	bool operator>  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
		return rhs < lhs;
	}

	template <class T, class Alloc>
	bool operator>= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
		return !(lhs < rhs);
	}

	template <class T, class Alloc>
	void swap (vector<T,Alloc>& x, vector<T,Alloc>& y)
	{
		x.swap(y);
	}

	/* #endregion */
}

#endif