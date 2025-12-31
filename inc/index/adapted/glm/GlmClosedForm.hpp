#pragma once

// C libraries

// 3rd-party libraries
#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/gtx/component_wise.hpp>
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

	struct GlmClosedForm
	{
		GlmClosedForm(){}

		ADAPTER_UNARY_METHOD(glm::sign, sign)
		ADAPTER_UNARY_METHOD(glm::abs, abs)

		ADAPTER_UNARY_METHOD(glm::floor, floor)
		ADAPTER_UNARY_METHOD(glm::trunc, trunc)
		ADAPTER_UNARY_METHOD(glm::round, round)
		ADAPTER_UNARY_METHOD(glm::ceil, ceil)

		ADAPTER_UNARY_METHOD(glm::fract, fract)
		// ADAPTER_BINARY_METHOD(math::modulus, modulus)  
		// ADAPTER_BINARY_METHOD(math::residue, residue)  
		// ADAPTER_BINARY_METHOD(math::remainder, remainder)

		ADAPTER_UNARY_METHOD(glm::sqrt, sqrt)
		ADAPTER_UNARY_METHOD(glm::cbrt, cbrt)
		ADAPTER_UNARY_METHOD(glm::inversesqrt, inversesqrt)

		ADAPTER_UNARY_METHOD(glm::exp, exp)
		ADAPTER_UNARY_METHOD(glm::exp2, exp2)
		ADAPTER_UNARY_METHOD(glm::exp10, exp10)
		ADAPTER_UNARY_METHOD(glm::log, log)
		ADAPTER_UNARY_METHOD(glm::log2, log2)
		ADAPTER_UNARY_METHOD(glm::log10, log10)

		ADAPTER_BINARY_METHOD(glm::pow, pow)
		ADAPTER_BINARY_METHOD(glm::distance, distance)
		ADAPTER_BINARY_METHOD(glm::step, step) 

		ADAPTER_TRINARY_METHOD(glm::mix, mix)  
		ADAPTER_TRINARY_METHOD(glm::smoothstep, smoothstep)
		// ADAPTER_TRINARY_METHOD(glm::linearstep, linearstep)

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

