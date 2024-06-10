#pragma once

// C libraries
#include <cmath>     /* std math */

// in-house libraries

namespace adapter
{

	/*
	The following are alternate definitions of the above that allow for support of other data types using classes of the adapter pattern
	*/

	#define ADAPTER_UNARY_METHOD(METHOD, NAME) \
	template <typename In1>\
	inline auto NAME (const In1 a) const\
	{\
		return METHOD(a[i]);\
	}

	#define ADAPTER_BINARY_METHOD(METHOD, NAME) \
	template <typename In1, typename In2>\
	inline auto NAME (const In1 a, const In2 b) const\
	{\
		return METHOD(a[i], b[i]);\
	}

	#define ADAPTER_TRINARY_METHOD(METHOD, NAME) \
	template <typename In1, typename In2, typename In3>\
	inline auto NAME (const In1 a, const In2 b, const In3 c) const\
	{\
		return METHOD(a[i], b[i], c[i]);\
	}

	struct ScalarClosedForm
	{
		ScalarClosedForm(){}

		ADAPTER_UNARY_METHOD(std::isnan, isnan)
		ADAPTER_UNARY_METHOD(std::isinf, isinf)

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

		template <typename In1> 
		inline auto inversesqrt(const In1 a) const
		{ 
			return In1(1)/std::sqrt(a); 
		}

		template<typename In1, typename In2>
		inline constexpr auto distance(const In1 a, const In2 b) const
		{
		    return std::sqrt(a*a + b*b);
		}

		template <typename T>
		inline constexpr T sign(const T x) const
		{ 
			return x==T(0)? T(0) : x>T(0)? T(1) : T(-1);
		}

		template <typename T>
		inline constexpr T bitsign(const T x) const
		{ 
			return std::signbit(x)? T(-1):T(1);
		}

		template <typename In1> 
		inline auto fract(const In1 a)  const
		{
			return a-std::floor(a);
		}

		template<typename In1, typename In2>
		inline constexpr auto modulus(const In1 a, const In2 b) const
		{
		    return residue(residue(a,b) + b, b);
		}

		template<typename In1, typename In2>
		inline constexpr auto residue(const In1 a, const In2 b) const
		{
		    return a - b * std::floor(a / b);
		}

		template<typename In1, typename In2>
		inline constexpr auto remainder(const In1 a, const In2 b) const
		{
		    return a - b * std::round(a / b);
		}

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

