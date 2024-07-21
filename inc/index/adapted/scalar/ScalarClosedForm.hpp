#pragma once

// C libraries
#include <cmath>     /* std math */

// in-house libraries

namespace adapted
{

	/*
	The following are alternate definitions of the above that allow for support of other data types using classes of the adapter pattern
	*/

	#define ADAPTER_UNARY_METHOD(METHOD, NAME) \
	inline auto NAME (const scalar a) const\
	{\
		return METHOD(a);\
	}

	#define ADAPTER_BINARY_METHOD(METHOD, NAME) \
	inline auto NAME (const scalar a, const scalar b) const\
	{\
		return METHOD(a, b);\
	}

	#define ADAPTER_TRINARY_METHOD(METHOD, NAME) \
	inline auto NAME (const scalar a, const scalar b, const In3 c) const\
	{\
		return METHOD(a, b, c);\
	}

	template <typename scalar> 
	struct ScalarClosedForm
	{
		ScalarClosedForm(){}

		ADAPTER_UNARY_METHOD(std::abs, abs)
		ADAPTER_UNARY_METHOD(std::floor, floor)
		ADAPTER_UNARY_METHOD(std::trunc, trunc)
		ADAPTER_UNARY_METHOD(std::round, round)
		ADAPTER_UNARY_METHOD(std::ceil, ceil)

		ADAPTER_UNARY_METHOD(std::sqrt, sqrt)
		ADAPTER_UNARY_METHOD(std::cbrt, cbrt)
		ADAPTER_UNARY_METHOD(std::pow, pow)
		ADAPTER_UNARY_METHOD(std::inversesqrt, inversesqrt)

		ADAPTER_UNARY_METHOD(std::exp, exp)
		ADAPTER_UNARY_METHOD(std::exp2, exp2)
		ADAPTER_UNARY_METHOD(std::exp10, exp10)
		ADAPTER_UNARY_METHOD(std::log, log)
		ADAPTER_UNARY_METHOD(std::log2, log2)
		ADAPTER_UNARY_METHOD(std::log10, log10)

		ADAPTER_TRINARY_METHOD(std::mix, mix)  
		ADAPTER_TRINARY_METHOD(std::step, step) 
		ADAPTER_TRINARY_METHOD(std::smoothstep, smoothstep)
		ADAPTER_TRINARY_METHOD(std::linearstep, linearstep)

		inline auto inversesqrt(const scalar a) const
		{ 
			return scalar(1)/std::sqrt(a); 
		}

		inline constexpr auto distance(const scalar a, const scalar b) const
		{
		    return std::sqrt(a*a + b*b);
		}

		inline constexpr T sign(const T x) const
		{ 
			return x==T(0)? T(0) : x>T(0)? T(1) : T(-1);
		}

		inline constexpr T bitsign(const T x) const
		{ 
			return std::signbit(x)? T(-1):T(1);
		}

		inline auto fract(const scalar a)  const
		{
			return a-std::floor(a);
		}

		inline constexpr auto modulus(const scalar a, const scalar b) const
		{
		    return residue(residue(a,b) + b, b);
		}

		inline constexpr auto residue(const scalar a, const scalar b) const
		{
		    return a - b * std::floor(a / b);
		}

		inline constexpr auto remainder(const scalar a, const scalar b) const
		{
		    return a - b * std::round(a / b);
		}

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

