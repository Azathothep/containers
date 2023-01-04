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

		template <typename vector>
		class _vector_iterator
		{

		public:
			typedef typename std::forward_iterator_tag iterator_category;
			typedef typename std::ptrdiff_t difference_type;
			typedef typename vector::value_type value_type;
			typedef value_type *pointer;
			typedef value_type &reference;

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

		typedef _vector_iterator<vector<T> > 							iterator;
		// typedef _vector_const_iterator< vector<T> > 					const_iterator;
		// typedef typename reverse_iterator<iterator> 					reverse_iterator;
		// typedef typename reverse_iterator<const_iterator> 			const_reverse_iterator;
		typedef typename iterator_traits<iterator>::difference_type 	difference_type;
		typedef typename allocator_type::size_type 						size_type; // not sure if it's right

		/* #endregion */

		/* #region private fields */

	private:
		struct _vector_data
		{
			pointer _M_start;
			pointer _M_finish;
			pointer _M_end_of_storage;

			pointer _M_end_addr() {
				return (_M_end_of_storage + 1);
			}
		};

		allocator_type 	_allocator;
		pointer 		_memory;

		_vector_data _M_data;

		unsigned int _size;
		unsigned int _capacity;

		/* #endregion */

		/* #region public methods */
	public:
		/* #region constructors & destructor */
		explicit vector(const allocator_type &alloc = allocator_type())
		{
			Debug::Log("Vector: Default Constructor Called");
		}

		explicit vector(size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type())
		{
			Debug::Log("Vector: Size Constructor Called");

			_size = n;
			_allocator = alloc;

			_memory = _allocator.allocate(n * sizeof(value_type));
			Debug::Log(std::string("Vector: " + Debug::ToStr(n) + " elements allocated at address " + Debug::ToStr(_memory)));

			for (int i = 0; i < n; i++)
			{
				Debug::Log(std::string("Vector: constructing element " + Debug::ToStr(i) + " with value " + Debug::ToStr(val)));
				_allocator.construct(_memory + i, val);
			}
		}

		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type())
		{
			Debug::Log("Vector: Iterator Constructor Called");
		}

		vector(const vector &x)
		{
			Debug::Log("Vector: Copy Constructor Called");
		}

		~vector()
		{
			Debug::Log("Vector: Destructor Called");
			
			iterator it;
			
			for (it = begin(); it != end(); it++)
				_allocator.destroy(&(*it));

			_allocator.deallocate(_memory, _size);
		}

		/* #endregion */

		vector &operator=(const vector &x)
		{
			Debug::Log("Vector: operator= Called");
		}

		void push_back(const value_type &__x)
		{
			Debug::Log("Vector: push_back())");

			if (this->_M_data._M_finish != this->_M_data._M_end_of_storage)
			{
				Debug::Log("Vector: adding value to vector")
				// add value
			}
			else
			{
				Debug::Log("Vector: reallocating");
				// reallocate
			}
		}

		void pop_back()
		{
			Debug::Log("Vector: pop_back()");

			// is the vector non empty ?
			if (empty())
				return;
			
			// If non empty, the function never throws exceptions
			// _Alloc_traits::destroy(this->_M_data, this->_M_data._M_finish);
			// shrink size by 1
		}

		reference front()
		{
			_requires_nonempty();
			return *begin()
		}

		reference back()
		{
			Debug::Log("Vector: back()");
			return *(end() - 1);
		}

		bool empty() const {
			Debug::Log("Vector: empty()");
			return (size == 0);
		}

		size_type size() const {
			return size_type(end() - begin());
		}

		size_type capacity() const {
			return size_type(const_iterator(this->_M_data._M_end_addr(), 0) - begin());
		}

		void reserve(size_type __n) // throwing exception!
		{
			//if (__n > max_size())
				//_throw_length_error(__N("vector::reserve"));

			//if (capacity() < __n)
				//_M_reallocate(__n);
		}

		/* #endregion */

		/* #region private methods */

	private:
		void realloc_insert(iterator __position, value_type val)
		{

			pointer __old_start = this->_M_data._M_start;
			pointer __old_finish = this->_M_data._M_finish;

			const size_type __elems_efore = __position - begin();

			// pointer __new_start(this->_M_allocate(__len));
			// pointer __new_finish(__new_start);

			try
			{
				// allocate memory & copy original vector to the new one
			}
			catch (std::exception &e)
			{
			}

			// deallocate

			// this->_M_data._M_start = __new_start;
			// this->_M_data._M_finish = __new_finish;
			// this->_M_data._M_end_of_storage = __new_start + __len;
		}

		void _requires_nonempty()
		{
			_DEBUG_VERIFY(! this->empty(), "not empty");
		}

		void _DEBUG_VERIFY(bool _v, std::string _msg)
		{
			if (_v)
				std::cerr << _msg << std::endl; // how to undifine behaviour ?
		}

		/* #endregion */

		/* #region iterators  */

	public:
		iterator begin() { return iterator(this->_M_data._M_start); }
		// const_iterator begin() const { return iterator(_memory); }
		iterator end() { return iterator(this->_M_data._M_finish); }
		// const_iterator end() const { return iterator(&_memory[_size]); }
		// reverse_iterator rbegin();
		// const_reverse_iterator rbegin() const;
		// reverse_iterator rend();
		// const_reverse_iterator rend() const;
		// const_iterator cbegin() const noexcept;
		// const_iterator cend() const noexcept;
		// const_reverse_iterator crbegin() const noexcept;
		// const_reverse_iterator crend() const noexcept;

		/* #endregion */
	};
}

#endif