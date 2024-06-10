#pragma once

// C libraries

// 3rd-party libraries
#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/geometric.hpp>

// in-house libraries
#include <math/glm/special.hpp>

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

	struct GlmMetric
	{
		GlmMetric(){}

		ADAPTER_BINARY_METHOD(glm::distance, distance)

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

