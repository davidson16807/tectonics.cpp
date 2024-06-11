#pragma once

// C libraries

// 3rd-party libraries

// in-house libraries
#include <units/si.hpp>

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

	struct SiMetric
	{
		SiMetric(){}

		ADAPTER_BINARY_METHOD(si::distance, distance)

		template<typename In1, typename In2>
		inline constexpr auto length(const In1 a) const
		{
		    return std::abs(a);
		}

		template<typename In1, typename In2>
		inline constexpr auto normalize(const In1 a) const
		{
		    return a / length(a);
		}

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

