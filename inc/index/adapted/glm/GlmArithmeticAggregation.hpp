#pragma once

// C libraries

// in-house libraries
#include <glm/geometric.hpp>
#include <glm/gtx/component_wise.hpp>

namespace adapted
{

	/*
	The following are alternate definitions of the above that allow for support of other data types using classes of the adapter pattern
	*/

	#define ADAPTER_UNARY_METHOD(METHOD, NAME) \
	template <typename In1>\
	inline auto NAME (const In1& a) const\
	{\
		return METHOD(a);\
	}

	#define ADAPTER_BINARY_METHOD(METHOD, NAME) \
	template <typename In1, typename In2>\
	inline auto NAME (const In1& a, const In2& b) const\
	{\
		return METHOD(a, b);\
	}

	#define ADAPTER_TRINARY_METHOD(METHOD, NAME) \
	template <typename In1, typename In2, typename In3>\
	inline auto NAME (const In1& a, const In2& b, const In3& c) const\
	{\
		return METHOD(a, b, c);\
	}

	struct GlmArithmeticAggregation
	{
		GlmArithmeticAggregation(){}

		ADAPTER_UNARY_METHOD(glm::compAdd, sum)
		ADAPTER_UNARY_METHOD(glm::compMul, product)
		ADAPTER_BINARY_METHOD(glm::dot, linear_combination)

		template <typename In1>
		inline auto mean (const In1 a) const
		{
			return glm::compAdd(a) / a.length();
		}

		template <typename In1, typename In2>
		inline auto weighted_average (const In1 weights, const In2 values) const
		{
			return glm::dot(weights, values) / glm::compAdd(weights);
		}

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

