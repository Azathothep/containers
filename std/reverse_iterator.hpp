#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

# include "iterator_traits.hpp"

namespace ft {
	template <class Iterator>
	class reverse_iterator {
		public:
			typedef 			Iterator										iterator_type;
			typedef typename 	iterator_traits<Iterator>::iterator_category 	iterator_category;
			typedef typename 	iterator_traits<Iterator>::value_type 			value_type;
			typedef typename 	iterator_traits<Iterator>::difference_type 		difference_type;
			typedef typename 	iterator_traits<Iterator>::pointer 				pointer;
			typedef typename 	iterator_traits<Iterator>::reference 			reference;

		private:
			Iterator _base_it;

		public:
			reverse_iterator() {}
			explicit reverse_iterator (iterator_type it) : _base_it(it) { }

			template <class Iter> reverse_iterator (const reverse_iterator<Iter>& rev_it) : _base_it(rev_it.base()) {}

			iterator_type 		base() const { return _base_it; }

			reference 			operator*() const {
				//return *_base_it;
				return *(_base_it - 1);
			}

			reverse_iterator 	operator+ (difference_type n) const {
				return reverse_iterator(_base_it - n);
			}

			reverse_iterator& 	operator++() {
				_base_it--;
				return *this;
			}

			reverse_iterator 	operator++(int) {
				reverse_iterator it = *this;
				++(*this);
				return it;
			}

			reverse_iterator& 	operator+= (difference_type n) {
				_base_it -= n;
				return *this;
			}

			reverse_iterator 	operator-(difference_type n) const {
				return reverse_iterator(_base_it + n);
			}

			reverse_iterator& 	operator--() {
				_base_it++;
				return *this;
			}

			reverse_iterator 	operator--(int) {
				reverse_iterator it = *this;
				--(*this);
				return it;
			}

			reverse_iterator& 	operator-= (difference_type n) {
				_base_it += n;
				return *this;
			}

			pointer 			operator->() const {
				return &**this;
			}

			reference 			operator[] (difference_type n) const {
				return *(*this + n);
			}

			template <class It>
			bool operator== (const reverse_iterator<It>& rhs) const {
				return (this->base() == rhs.base());
			}

			template <class It>
			bool operator!= (const reverse_iterator<It>& rhs) const {
				return (this->base() != rhs.base());
			}

			template <class It>
			bool operator<  (const reverse_iterator<It>& rhs) const {
				return (this->base() > rhs.base());
			}

			template <class It>
			bool operator<= (const reverse_iterator<It>& rhs) const {
				return (this->base() >= rhs.base());
			}

			template <class It>
			bool operator>  (const reverse_iterator<It>& rhs) const {
				return (this->base() < rhs.base());
			}

			template <class It>
			bool operator>= (const reverse_iterator<It>& rhs) const {
				return (this->base() <= rhs.base());
			}

			template <class It>
			difference_type operator- (const reverse_iterator<It>& rhs) const {
				return rhs.base() - this->base();
			}
	};

	template <class It>
	reverse_iterator<It> operator+ (typename reverse_iterator<It>::difference_type n, const reverse_iterator<It>& rev_it) {
		reverse_iterator<It> result = rev_it + n;
		return result;
	}
}

#endif