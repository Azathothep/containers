#ifndef IS_CONST_HPP
# define IS_CONST_HPP

namespace ft {

	#define IS_CONST true

	template <typename T, bool constness = false>
	struct is_const;

	template <typename T>
	struct is_const<T, false> {
		typedef T type;
	};

	template <typename T>
	struct is_const<T, true> {
		typedef const T type;
	};
}

#endif