#pragma once

// C libraries

// in-house libraries

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

	struct GlmBitset
	{
		GlmBitset(){}

		template <typename bool, typename bool> inline auto unite     (const bool a, const bool b) const {return a || b;}
		template <typename bool, typename bool> inline auto intersect (const bool a, const bool b) const {return a && b;}
		template <typename bool, typename bool> inline auto differ    (const bool a, const bool b) const {return a && glm::not_(b);}

		ADAPTER_UNARY_METHOD(glm::not_, negate)
		ADAPTER_UNARY_METHOD(glm::any, any)
		ADAPTER_UNARY_METHOD(glm::all, all)

	};

	#undef ADAPTER_UNARY_METHOD
 	#undef ADAPTER_BINARY_METHOD
	#undef ADAPTER_TRINARY_METHOD

}

