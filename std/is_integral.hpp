#ifndef IS_INTEGRAL_HPP
# define IS_INTEGRAL_HPP

#include <uchar.h>

namespace ft {

	template<bool integralness, class T>
	struct is_integral_checker {
		static const bool value = integralness;
		typedef T type;
	};

	template<typename>
	struct is_integral_val : public is_integral_checker<false, bool> { };

	template<>
	struct is_integral_val<bool> : public is_integral_checker<true, bool> { };

	template<>
	struct is_integral_val<char> : public is_integral_checker<true, char> { };

	template<>
	struct is_integral_val<wchar_t> : public is_integral_checker<true, wchar_t> { };

	template<>
	struct is_integral_val<signed char> : public is_integral_checker<true, signed char> { };

	template<>
	struct is_integral_val<short int> : public is_integral_checker<true, short int> { };

	template<>
	struct is_integral_val<int> : public is_integral_checker<true, int> { };

	template<>
	struct is_integral_val<long int> : public is_integral_checker<true, long int> { };

	template<>
	struct is_integral_val<long long int> : public is_integral_checker<true, long long int> { };

	template<>
	struct is_integral_val<unsigned char> : public is_integral_checker<true, unsigned char> { };

	template<>
	struct is_integral_val<unsigned short int> : public is_integral_checker<true, unsigned short int> { };

	template<>
	struct is_integral_val<unsigned int> : public is_integral_checker<true, unsigned int> { };

	template<>
	struct is_integral_val<unsigned long int> : public is_integral_checker<true, unsigned long int> { };

	template<>
	struct is_integral_val<unsigned long long int> : public is_integral_checker<true, unsigned long long int> { };

	template<typename T>
	struct is_integral : public is_integral_val<T> { };
}

#endif