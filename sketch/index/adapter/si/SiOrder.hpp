#pragma once

// C libraries

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

	struct SiComponentOrder
	{
		SiComponentOrder(){}

		ADAPTER_BINARY_METHOD(si::equal, equal)
		ADAPTER_BINARY_METHOD(si::notEqual, notEqual)
		ADAPTER_BINARY_METHOD(si::greaterThan, greaterThan)
		ADAPTER_BINARY_METHOD(si::lessThan, lessThan)
		ADAPTER_BINARY_METHOD(si::greaterThanEqual, greaterThanEqual)
		ADAPTER_BINARY_METHOD(si::lessThanEqual, lessThanEqual)

		ADAPTER_BINARY_METHOD(si::min, min)  
		ADAPTER_BINARY_METHOD(si::max, max)  

		ADAPTER_TRINARY_METHOD(si::clamp, clamp)  

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

