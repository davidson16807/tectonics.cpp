#pragma once

// C libraries

// in-house libraries
#include <glm/vector_relational.hpp>

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

	struct GlmOrderIteration
	{
		GlmOrderIteration(){}

		ADAPTER_UNARY_METHOD(glm::isnan, isnan)
		ADAPTER_UNARY_METHOD(glm::isinf, isinf)

		ADAPTER_BINARY_METHOD(glm::equal, equal)
		ADAPTER_BINARY_METHOD(glm::notEqual, not_equal)
		ADAPTER_BINARY_METHOD(glm::greaterThan, greater_than)
		ADAPTER_BINARY_METHOD(glm::lessThan, less_than)
		ADAPTER_BINARY_METHOD(glm::greaterThanEqual, greater_than_equal)
		ADAPTER_BINARY_METHOD(glm::lessThanEqual, less_than_equal)

		ADAPTER_BINARY_METHOD(glm::min, min) 
		ADAPTER_BINARY_METHOD(glm::max, max) 

		ADAPTER_TRINARY_METHOD(glm::clamp, clamp) 

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

