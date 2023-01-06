#include "../../Debug.hpp"

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include "../std/iterator_traits.hpp"

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
			typedef typename std::forward_iterator_tag iterator_category;
			typedef typename std::ptrdiff_t difference_type;
			typedef typename vector::value_type value_type;
			typedef typename choose<is_const, const value_type *, value_type *>::type pointer;
			typedef typename choose<is_const, const value_type &, value_type &>::type reference;

		private:
			pointer _m_ptr;

		public:
			_vector_iterator() {}
			_vector_iterator(pointer __p) : _m_ptr(__p) {}
			_vector_iterator(const _vector_iterator &rhs) {}

			reference operator*() const
			{
				return *_m_ptr;
			}

			/* #region operators */

			_vector_iterator operator+(difference_type n) const
			{
				_vector_iterator result(_m_ptr + n);
				return *result;
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
				return *result;
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

		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;

		typedef _vector_iterator<vector<T>, false > 					iterator;
		typedef _vector_iterator< vector<T>, true > 					const_iterator;
		// typedef typename reverse_iterator<iterator> 					reverse_iterator;
		// typedef typename reverse_iterator<const_iterator> 			const_reverse_iterator;
		typedef typename iterator_traits<iterator>::difference_type 	difference_type;
		typedef typename allocator_type::size_type 						size_type; // not sure if it's right

		/* #endregion */

		/* #region private fields */

	private:
		#define __NOTIFY_GROWTH(n) this->_M_data._M_finish += n
		#define __NOTIFY_SHRINK(n) this->_M_data._M_finish -= n

		struct _vector_data
		{
			pointer _M_start;
			pointer _M_finish;
			pointer _M_end_of_storage;

			pointer _M_end_addr() const {
				return (_M_end_of_storage);
			}
		};

		_vector_data	_M_data;
		allocator_type	_M_alloc;

		/* #endregion */

		/* #region public methods */
	public:
		/* #region constructors & destructor */
		explicit vector(const allocator_type &alloc = allocator_type())
		{
			Debug::Log << "Vector: Default Constructor Called" << std::endl;

			_M_alloc = alloc;
			
			this->__create_storage(0);
		}

		explicit vector(size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type())
		{
			Debug::Log << "Vector: Size Constructor Called" << std::endl;

			_M_alloc = alloc;

			this->__create_storage(n * 2);
			this->_fill(this->_M_data._M_start, n, val);
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
		}

		~vector()
		{
			Debug::Log << "Vector: Destructor Called" << std::endl;
			
			iterator it;
			
			for (it = begin(); it != end(); it++)
				_M_alloc.destroy(&(*it));

			_M_alloc.deallocate(_M_data._M_start, this->capacity());
		}

		/* #endregion */

		vector &operator=(const vector &x)
		{
			Debug::Log << "Vector: operator= Called" << std::endl;
		}

		reference operator[](size_type n) {
			Debug::Log << "Vector: operator[" << n << "]" << std::endl;
			return *(this->_M_data._M_start + n);
		}

		void push_back(const value_type &__x)
		{
			Debug::Log << "Vector: push_back()" << std::endl;

			if (this->_M_data._M_finish != this->_M_data._M_end_of_storage)
			{
				// add value
				Debug::Log << "Vector: adding value to vector" << std::endl;
				this->__construct(this->_M_data._M_finish, __x);
				__NOTIFY_GROWTH(1);
			}
			else
			{
				Debug::Log << "Vector: size insufficient, reallocating..." << std::endl;
				_realloc_insert(end(), __x);
			}
		}

		void pop_back()
		{
			Debug::Log << "Vector: pop_back()" << std::endl;

			// is the vector non empty ?
			if (empty())
				return;
			
			_shrink(1);
			// If non empty, the function never throws exceptions
			// _Alloc_traits::destroy(this->_M_data, this->_M_data._M_finish);
			// shrink size by 1
		}

		reference front()
		{
			_requires_nonempty();
			return *begin();
		}

		reference back()
		{
			Debug::Log << "Vector: back()"<< std::endl;
			return *(end() - 1);
		}

		bool empty() const {
			Debug::Log << "Vector: empty()" << std::endl;
			return (this->size() == 0);
		}

		size_type size() const {
			return size_type(end() - begin());
		}

		size_type capacity() const {
			return size_type(iterator(this->_M_data._M_end_addr()) - begin());
		}

		void reserve(size_type __n) // throwing exception!
		{
			//if (__n > max_size())
				//_throw_length_error(__N("vector::reserve"));

			//if (capacity() < __n)
				//_M_reallocate(__n);
		}

		size_type max_size() const {
			return _M_alloc.max_size();
		}

		/* #endregion */

		/* #region private methods */

	private:
		void _fill(pointer __start, size_t __n, value_type __val)
		{
			Debug::Log << "Vector: Filling " << __n << " elements" << std::endl;

			iterator it;
			iterator ite = iterator(__start + __n);

			for (it = iterator(__start); it != ite; it++)
				__construct(&(*it), __val);
		}

		pointer __allocate(size_type __n)
		{
			Debug::Log << "Vector: allocating for " << __n << " elements" << std::endl;
			return _M_alloc.allocate(__n);
		}

		void __construct(pointer __elem, value_type __val)
		{
			Debug::Log << "Vector: constructing element at address " << __elem << std::endl;
			_M_alloc.construct(__elem, __val);
		}

		void __create_storage(size_t __n) {
			Debug::Log << "Vector: creating storage of capacity " << __n << std::endl;

			this->_M_data._M_start = this->__allocate(__n);
			this->_M_data._M_finish = this->_M_data._M_start;
			this->_M_data._M_end_of_storage = this->_M_data._M_start + __n;
		}

		void _realloc_insert(iterator __position, value_type val)
		{
			Debug::Log << "Vector: realloc_inserting value " << val << " at position " << &(*__position) << std::endl;

			pointer __old_start = this->_M_data._M_start;
			pointer __old_finish = this->_M_data._M_finish;

			const size_type __elems_before = __position - begin();
			const size_type __elems_after = end() - __position;

			try
			{
				// allocate memory & copy original vector to the new one
				size_type _new_capacity = (this->capacity() == 0 ? 5 : this->capacity() * 2);

				this->__create_storage(_new_capacity);

				this->_range_copy(__old_start, _M_data._M_start, __elems_before);

				pointer __insert_position = _M_data._M_start + __elems_before;
				*__insert_position = val;

				this->_range_copy(__old_start + __elems_before, __insert_position + 1, __elems_after);

				__NOTIFY_GROWTH(__elems_before + __elems_after + 1);
			}
			catch (std::exception &e)
			{
				std::cout << e.what() << std::endl;
			}

			// deallocate
			_delete(__old_start, __old_finish);
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

		void _delete(pointer __start, pointer __end)
		{
			iterator it;
			iterator ite = iterator(__end);

			Debug::Log << "Vector: Deleting " << (__end - __start) << " elements" << std::endl;

			for (it = iterator(__start); it != ite; it++)
			{
				Debug::Log << "Vector: Destroying element at address " << &(*it) << std::endl;
				_M_alloc.destroy(&(*it));
			}

			size_type __n = __end - __start;

			Debug::Log << "Vector: Deallocating..." << std::endl;
			_M_alloc.deallocate(__start, __n);
		}

		void _shrink(size_type __n)
		{
			Debug::Log << "Vector: Shrinking memory by " << __n << std::endl;
			pointer __new_finish = _M_data._M_finish - __n;

			if (__n > size())
				__new_finish = _M_data._M_start;

			iterator it;
			iterator ite = iterator(_M_data._M_finish);

			for (it = iterator(__new_finish); it != ite; it++)
				this->_M_alloc.destroy(&(*it));

			__NOTIFY_SHRINK(__n);
		}

		void _requires_nonempty()
		{
			_DEBUG_VERIFY(! this->empty(), "not empty");
		}

		void _DEBUG_VERIFY(bool _v, std::string _msg)
		{
			if (_v)
				std::cerr << _msg << std::endl; // how to undefine behaviour ?
		}

		/* #endregion */

		/* #region iterators  */

	public:
		iterator begin() const { return iterator(this->_M_data._M_start); }
		const_iterator cbegin() const { return const_iterator(this->_M_data._M_start); }
		iterator end() const { return iterator(this->_M_data._M_finish); }
		const_iterator cend() const { return const_iterator(this->_M_data._M_finish); }
		// reverse_iterator rbegin();
		// const_reverse_iterator crbegin() const;
		// reverse_iterator rend();
		// const_reverse_iterator crend() const;

		/* #endregion */
	};
}

#endif