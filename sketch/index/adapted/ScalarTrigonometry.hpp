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

	struct ScalarTrigonometry
	{
		ScalarTrigonometry(){}

		ADAPTER_UNARY_METHOD(std::sin, sin)
		ADAPTER_UNARY_METHOD(std::cos, cos)
		ADAPTER_UNARY_METHOD(std::tan, tan)
		ADAPTER_UNARY_METHOD(std::asinh, asinh)
		ADAPTER_UNARY_METHOD(std::acosh, acosh)
		ADAPTER_UNARY_METHOD(std::atanh, atanh)
		ADAPTER_UNARY_METHOD(std::sec, sec)
		ADAPTER_UNARY_METHOD(std::csc, csc)
		ADAPTER_UNARY_METHOD(std::cot, cot)
		// ADAPTER_UNARY_METHOD(std::sinh, sinh)
		// ADAPTER_UNARY_METHOD(std::cosh, cosh)
		// ADAPTER_UNARY_METHOD(std::tanh, tanh)
		// ADAPTER_UNARY_METHOD(std::asec, asec)
		// ADAPTER_UNARY_METHOD(std::acsc, acsc)
		// ADAPTER_UNARY_METHOD(std::acot, acot)
		// ADAPTER_UNARY_METHOD(std::asech, asech)
		// ADAPTER_UNARY_METHOD(std::acsch, acsch)
		// ADAPTER_UNARY_METHOD(std::acoth, acoth)
		ADAPTER_BINARY_METHOD(std::atan2, atan2)
		ADAPTER_UNARY_METHOD(std::radians, radians)
		ADAPTER_UNARY_METHOD(std::degrees, degrees)

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

