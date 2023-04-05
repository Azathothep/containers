#ifndef STACK_HPP
# define STACK_HPP

# include "vector.hpp"

namespace ft {

	template <class T, class Container = ft::vector<T> >
	class stack {

	/* #region typedefs */

	public:
		typedef T 													value_type;
		typedef Container											container_type;
		typedef typename container_type::reference 					reference;
		typedef typename container_type::const_reference			const_reference;
		typedef size_t			 									size_type;

	/* #endregion */

	protected:
		container_type	_M_ctnr;

	public:
		explicit stack (const container_type& ctnr = container_type()) : _M_ctnr(ctnr) {}

		bool empty() const { return this->_M_ctnr.empty(); }

		size_type size() const { return this->_M_ctnr.size(); }

		value_type& top() { return this->_M_ctnr.back(); }

		const value_type& top() const { return this->_M_ctnr.back(); }

		void push (const value_type& val) { this->_M_ctnr.push_back(val); }

		void pop() { this->_M_ctnr.pop_back(); }

		template <class _T, class _Container>
		friend bool operator==(const stack<_T,_Container>& lhs, const stack<_T,_Container>& rhs);

		template <class _T, class _Container>
		friend bool operator!=(const stack<_T,_Container>& lhs, const stack<_T,_Container>& rhs);

		template <class _T, class _Container>
		friend bool operator<(const stack<_T,_Container>& lhs, const stack<_T,_Container>& rhs);

		template <class _T, class _Container>
		friend bool operator<=(const stack<_T,_Container>& lhs, const stack<_T,_Container>& rhs);

		template <class _T, class _Container>
		friend bool operator>(const stack<_T,_Container>& lhs, const stack<_T,_Container>& rhs);

		template <class _T, class _Container>
		friend bool operator>=(const stack<_T,_Container>& lhs, const stack<_T,_Container>& rhs);
	};

	template <class T, class Container>
	bool operator==(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs._M_ctnr == rhs._M_ctnr;
	}

	template <class T, class Container>
	bool operator!=(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs._M_ctnr != rhs._M_ctnr;
	}

	template <class T, class Container>
	bool operator<(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs._M_ctnr < rhs._M_ctnr;
	}

	template <class T, class Container>
	bool operator<=(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs._M_ctnr <= rhs._M_ctnr;
	}

	template <class T, class Container>
	bool operator>(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs._M_ctnr > rhs._M_ctnr;
	}

	template <class T, class Container>
	bool operator>=(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs._M_ctnr >= rhs._M_ctnr;
	}
}

#endif