#pragma once

// C libraries

// in-house libraries
#include <unit/si.hpp>

namespace adapted
{

	/*
	The following are alternate definitions of the above that allow for support of other data types using classes of the adapter pattern
	*/

	#define ADAPTED_UNARY_METHOD(METHOD, NAME) \
	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>\
	inline auto NAME (const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a) const\
	{\
		return METHOD(a);\
	}

	#define ADAPTED_BINARY_METHOD(METHOD, NAME) \
	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>\
	inline auto NAME (const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b) const\
	{\
		return METHOD(a, b);\
	}

	#define ADAPTED_TRINARY_METHOD(METHOD, NAME) \
	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>\
	inline auto NAME (const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b, const T1 t) const\
	{\
		return METHOD(a, b, t);\
	}

	struct SiClosedForm
	{
		SiClosedForm(){}

		ADAPTED_UNARY_METHOD(si::abs,  abs)
		ADAPTED_UNARY_METHOD(si::sign, sign)
		// ADAPTED_UNARY_METHOD(si::bitsign, bitsign)

		// ADAPTED_UNARY_METHOD(si::fract, fract)
		// ADAPTED_UNARY_METHOD(si::floor, floor)
		// ADAPTED_UNARY_METHOD(si::trunc, trunc)
		// ADAPTED_UNARY_METHOD(si::round, round)
		// ADAPTED_UNARY_METHOD(si::ceil, ceil)
		// ADAPTED_BINARY_METHOD(si::modulus, modulus)  
		// ADAPTED_BINARY_METHOD(si::residue, residue)  
		// ADAPTED_BINARY_METHOD(si::remainder, remainder)

		ADAPTED_UNARY_METHOD(si::sqrt, sqrt)
		ADAPTED_UNARY_METHOD(si::cbrt, cbrt)
		// ADAPTED_UNARY_METHOD(si::pow, pow)
		// ADAPTED_UNARY_METHOD(si::inversesqrt, inversesqrt)

		ADAPTED_BINARY_METHOD(si::step, step) 

		ADAPTED_TRINARY_METHOD(si::smoothstep, smoothstep)
		ADAPTED_TRINARY_METHOD(si::linearstep, linearstep)

	};

	#undef ADAPTED_UNARY_METHOD
 	#undef ADAPTED_BINARY_METHOD
	#undef ADAPTED_TRINARY_METHOD

}

