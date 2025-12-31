#pragma once

// C libraries
#include <cmath>     /* std math */

// in-house libraries
#include <math/special.hpp>

namespace adapted
{

	/*
	The following are alternate definitions of the above that allow for support of other data types using classes of the adapter pattern
	*/

	#define ADAPTED_UNARY_METHOD(METHOD, NAME) \
	inline auto NAME (const scalar a) const\
	{\
		return METHOD(a);\
	}

	#define ADAPTED_BINARY_METHOD(METHOD, NAME) \
	inline auto NAME (const scalar a, const scalar b) const\
	{\
		return METHOD(a, b);\
	}

	#define ADAPTED_TRINARY_METHOD(METHOD, NAME) \
	inline auto NAME (const scalar a, const scalar b, const scalar c) const\
	{\
		return METHOD(a, b, c);\
	}

	template <typename scalar> 
	struct ScalarClosedForm
	{
		ScalarClosedForm(){}

		ADAPTED_UNARY_METHOD(std::abs, abs)
		ADAPTED_UNARY_METHOD(std::floor, floor)
		ADAPTED_UNARY_METHOD(std::trunc, trunc)
		ADAPTED_UNARY_METHOD(std::round, round)
		ADAPTED_UNARY_METHOD(std::ceil, ceil)

		ADAPTED_UNARY_METHOD(std::sqrt, sqrt)
		ADAPTED_UNARY_METHOD(std::cbrt, cbrt)

		ADAPTED_UNARY_METHOD(std::exp, exp)
		ADAPTED_UNARY_METHOD(std::exp2, exp2)
		ADAPTED_UNARY_METHOD(math::exp10, exp10)
		ADAPTED_UNARY_METHOD(std::log, log)
		ADAPTED_UNARY_METHOD(std::log2, log2)
		ADAPTED_UNARY_METHOD(std::log10, log10)

		ADAPTED_BINARY_METHOD(std::pow, pow)

		ADAPTED_UNARY_METHOD(math::inversesqrt, inversesqrt)
		ADAPTED_UNARY_METHOD(math::sign, sign)
		ADAPTED_UNARY_METHOD(math::bitsign, bitsign)
		ADAPTED_UNARY_METHOD(math::fract, fract)

		ADAPTED_BINARY_METHOD(math::distance, distance)
		ADAPTED_BINARY_METHOD(math::modulus, modulus)
		ADAPTED_BINARY_METHOD(math::residue, residue)
		ADAPTED_BINARY_METHOD(math::remainder, remainder)
		ADAPTED_BINARY_METHOD(math::step, step) 

		ADAPTED_TRINARY_METHOD(math::mix, mix)  
		ADAPTED_TRINARY_METHOD(math::smoothstep, smoothstep)
		ADAPTED_TRINARY_METHOD(math::linearstep, linearstep)

	};

	#undef ADAPTED_UNARY_METHOD
 	#undef ADAPTED_BINARY_METHOD
	#undef ADAPTED_TRINARY_METHOD

}

