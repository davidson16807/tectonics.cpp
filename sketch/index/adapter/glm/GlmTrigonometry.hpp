#pragma once

// C libraries

// in-house libraries
#include <glm/trigonometric.hpp>

namespace adapter
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

	struct GlmTrigonometry
	{
		GlmTrigonometry(){}

		ADAPTER_UNARY_METHOD(glm::sin, sin)
		ADAPTER_UNARY_METHOD(glm::cos, cos)
		ADAPTER_UNARY_METHOD(glm::tan, tan)
		ADAPTER_UNARY_METHOD(glm::asinh, asinh)
		ADAPTER_UNARY_METHOD(glm::acosh, acosh)
		ADAPTER_UNARY_METHOD(glm::atanh, atanh)
		ADAPTER_UNARY_METHOD(glm::sec, sec)
		ADAPTER_UNARY_METHOD(glm::csc, csc)
		ADAPTER_UNARY_METHOD(glm::cot, cot)
		// ADAPTER_UNARY_METHOD(glm::sinh, sinh)
		// ADAPTER_UNARY_METHOD(glm::cosh, cosh)
		// ADAPTER_UNARY_METHOD(glm::tanh, tanh)
		// ADAPTER_UNARY_METHOD(glm::asec, asec)
		// ADAPTER_UNARY_METHOD(glm::acsc, acsc)
		// ADAPTER_UNARY_METHOD(glm::acot, acot)
		// ADAPTER_UNARY_METHOD(glm::asech, asech)
		// ADAPTER_UNARY_METHOD(glm::acsch, acsch)
		// ADAPTER_UNARY_METHOD(glm::acoth, acoth)
		ADAPTER_BINARY_METHOD(glm::atan2, atan2)
		ADAPTER_UNARY_METHOD(glm::radians, radians)
		ADAPTER_UNARY_METHOD(glm::degrees, degrees)

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

