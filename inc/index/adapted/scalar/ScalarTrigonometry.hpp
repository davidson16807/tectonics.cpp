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
	inline auto NAME (const scalar a, const scalar b, const scalar c) const\
	{\
		return METHOD(a, b, c);\
	}

	template<typename scalar>
	struct ScalarTrigonometry
	{
		const scalar pi;

		ScalarTrigonometry(const scalar pi) : pi(pi)
		{}

		ADAPTER_UNARY_METHOD(std::sin, sin)
		ADAPTER_UNARY_METHOD(std::cos, cos)
		ADAPTER_UNARY_METHOD(std::tan, tan)
		ADAPTER_UNARY_METHOD(std::asinh, asinh)
		ADAPTER_UNARY_METHOD(std::acosh, acosh)
		ADAPTER_UNARY_METHOD(std::atanh, atanh)
		ADAPTER_UNARY_METHOD(std::sinh, sinh)
		ADAPTER_UNARY_METHOD(std::cosh, cosh)
		ADAPTER_UNARY_METHOD(std::tanh, tanh)
		// ADAPTER_UNARY_METHOD(std::asec, asec)
		// ADAPTER_UNARY_METHOD(std::acsc, acsc)
		// ADAPTER_UNARY_METHOD(std::acot, acot)
		// ADAPTER_UNARY_METHOD(std::asech, asech)
		// ADAPTER_UNARY_METHOD(std::acsch, acsch)
		// ADAPTER_UNARY_METHOD(std::acoth, acoth)
		ADAPTER_BINARY_METHOD(std::atan2, atan2)

		inline auto sec(scalar a) const { return scalar(1) / std::cos(a); }
		inline auto csc(scalar a) const { return scalar(1) / std::sin(a); }
		inline auto cot(scalar a) const { return std::cos(a) / std::sin(a); }

		inline auto radians(scalar deg) const { return deg * scalar(pi) / scalar(180); }
		inline auto degrees(scalar rad) const { return rad * scalar(180) / scalar(pi); }

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

