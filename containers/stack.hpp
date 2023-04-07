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
		container_type	c;

	public:
		explicit stack (const container_type& ctnr = container_type()) : c(ctnr) {}

		bool empty() const { return this->c.empty(); }

		size_type size() const { return this->c.size(); }

		value_type& top() { return this->c.back(); }

		const value_type& top() const { return this->c.back(); }

		void push (const value_type& val) { this->c.push_back(val); }

		void pop() { this->c.pop_back(); }

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
		return lhs.c == rhs.c;
	}

	template <class T, class Container>
	bool operator!=(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs.c != rhs.c;
	}

	template <class T, class Container>
	bool operator<(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs.c < rhs.c;
	}

	template <class T, class Container>
	bool operator<=(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs.c <= rhs.c;
	}

	template <class T, class Container>
	bool operator>(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs.c > rhs.c;
	}

	template <class T, class Container>
	bool operator>=(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs.c >= rhs.c;
	}
}

#endif