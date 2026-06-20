#pragma once

// C libraries

// in-house libraries
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

	struct GlmGeometric
	{
		GlmGeometric(){}

		ADAPTER_BINARY_METHOD(glm::dot, dot)
		ADAPTER_BINARY_METHOD(glm::cross, cross)
		ADAPTER_BINARY_METHOD(glm::reflect, reflect)
		ADAPTER_TRINARY_METHOD(glm::faceforward, faceforward)

		template <typename In1, typename In2>
		inline constexpr auto similarity(const In1& U, const In2& V) const
		{
		    return glm::dot(U,V) / (glm::length(U)*glm::length(V));
		}

		template <typename In1, typename In2>
		inline constexpr auto scalar_projection (const In1& a, const In2& b) const
		{
			return glm::dot(a, glm::normalize(b));
		}

		template <typename In1, typename In2>
		inline constexpr auto vector_projection (const In1& a, const In2& b) const
		{
			auto bhat = glm::normalize(bhat);
			return glm::dot(a, bhat) * bhat;
		}

		template <typename In1, typename In2>
		inline constexpr auto scalar_rejection (const In1& a, const In2& b) const
		{
			auto bhat = glm::normalize(bhat);
			return glm::length(a - glm::dot(a, bhat) * bhat);
		}

		template <typename In1, typename In2>
		inline constexpr auto vector_rejection (const In1& a, const In2& b) const
		{
			auto bhat = glm::normalize(bhat);
			return a - glm::dot(a, bhat) * bhat;
		}

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

