#pragma once

// C libraries

// in-house libraries
#include <units/si.hpp>

namespace adapted
{

	/*
	The following are alternate definitions of the above that allow for support of other data types using classes of the adapter pattern
	*/

	#define ADAPTER_UNARY_METHOD(METHOD, NAME) \
	template <typename In1>\
	inline auto NAME (const In1 a) const\
	{\
		return METHOD(a);\
	}

	#define ADAPTER_BINARY_METHOD(METHOD, NAME) \
	template <typename In1, typename In2>\
	inline auto NAME (const In1 a, const In2 b) const\
	{\
		return METHOD(a, b);\
	}

	#define ADAPTER_TRINARY_METHOD(METHOD, NAME) \
	template <typename In1, typename In2, typename In3>\
	inline auto NAME (const In1 a, const In2 b, const In3 c) const\
	{\
		return METHOD(a, b, c);\
	}

	struct SiClosedForm
	{
		SiClosedForm(){}

		ADAPTER_UNARY_METHOD(si::abs,  abs)
		ADAPTER_UNARY_METHOD(si::sign, sign)
		// ADAPTER_UNARY_METHOD(si::bitsign, bitsign)

		// ADAPTER_UNARY_METHOD(si::fract, fract)
		// ADAPTER_UNARY_METHOD(si::floor, floor)
		// ADAPTER_UNARY_METHOD(si::trunc, trunc)
		// ADAPTER_UNARY_METHOD(si::round, round)
		// ADAPTER_UNARY_METHOD(si::ceil, ceil)
		// ADAPTER_BINARY_METHOD(si::modulus, modulus)  
		// ADAPTER_BINARY_METHOD(si::residue, residue)  
		// ADAPTER_BINARY_METHOD(si::remainder, remainder)

		ADAPTER_UNARY_METHOD(si::sqrt, sqrt)
		ADAPTER_UNARY_METHOD(si::cbrt, cbrt)
		// ADAPTER_UNARY_METHOD(si::pow, pow)
		// ADAPTER_UNARY_METHOD(si::inversesqrt, inversesqrt)

		ADAPTER_TRINARY_METHOD(si::mix, mix)  
		ADAPTER_TRINARY_METHOD(si::step, step) 
		ADAPTER_TRINARY_METHOD(si::smoothstep, smoothstep)
		ADAPTER_TRINARY_METHOD(si::linearstep, linearstep)

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

