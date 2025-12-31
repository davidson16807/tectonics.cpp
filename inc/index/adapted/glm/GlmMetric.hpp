#pragma once

// C libraries

// 3rd-party libraries
#include <glm/common.hpp>
#include <glm/geometric.hpp>

// in-house libraries
#include <math/glm/special.hpp>

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

	struct GlmMetric
	{
		GlmMetric(){}

		ADAPTER_BINARY_METHOD(glm::distance, distance)
		ADAPTER_UNARY_METHOD(glm::length, length)
		ADAPTER_UNARY_METHOD(glm::normalize, normalize)

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

