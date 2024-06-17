#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */

// in-house libraries
#include "compatible.hpp"

namespace iterated
{

	/*
	The following are alternate definitions of the above that allow for support of other data types using classes of the adapter pattern
	*/

	#define ITERATED_UNARY_METHOD(METHOD, NAME) \
	template <typename In1, typename Out>\
	void NAME (const In1& a, Out& out) const\
	{\
		assert(compatible(a,out));\
		auto size = out.size();\
		for (auto i = 0*size; i < size; ++i)\
		{\
			out[i] = METHOD(a[i]);\
		}\
	}

	#define ITERATED_BINARY_METHOD(METHOD, NAME) \
	template <typename In1, typename In2, typename Out>\
	void NAME (const In1& a, const In2& b, Out& out) const\
	{\
		assert(compatible(a,b,out));\
		auto size = out.size();\
		for (auto i = 0*size; i < size; ++i)\
		{\
			out[i] = METHOD(a[i], b[i]);\
		}\
	}

	#define ITERATED_TRINARY_METHOD(METHOD, NAME) \
	template <typename In1, typename In2, typename In3, typename Out>\
	void NAME (const In1& a, const In2& b, const In3& c, Out& out) const\
	{\
		assert(compatible(a,b,out));\
		auto size = out.size();\
		for (auto i = 0*size; i < size; ++i)\
		{\
			out[i] = METHOD(a[i], b[i], c[i]);\
		}\
	}

	template <typename ElementTrigonometry>
	class Trigonometry
	{
		const ElementTrigonometry elements;
	public:
		Trigonometry(const ElementTrigonometry& elements):
			elements(elements)
		{}
		ITERATED_UNARY_FUNCTION(elements.sin, sin)
		ITERATED_UNARY_FUNCTION(elements.cos, cos)
		ITERATED_UNARY_FUNCTION(elements.tan, tan)
		ITERATED_UNARY_FUNCTION(elements.asinh, asinh)
		ITERATED_UNARY_FUNCTION(elements.acosh, acosh)
		ITERATED_UNARY_FUNCTION(elements.atanh, atanh)
		ITERATED_UNARY_METHOD(elements.sec, sec)
		ITERATED_UNARY_METHOD(elements.csc, csc)
		ITERATED_UNARY_METHOD(elements.cot, cot)
		ITERATED_UNARY_METHOD(elements.sinh, sinh)
		ITERATED_UNARY_METHOD(elements.cosh, cosh)
		ITERATED_UNARY_METHOD(elements.tanh, tanh)
		// ITERATED_UNARY_METHOD(elements.asec, asec)
		// ITERATED_UNARY_METHOD(elements.acsc, acsc)
		// ITERATED_UNARY_METHOD(elements.acot, acot)
		// ITERATED_UNARY_METHOD(elements.asech, asech)
		// ITERATED_UNARY_METHOD(elements.acsch, acsch)
		// ITERATED_UNARY_METHOD(elements.acoth, acoth)
		ITERATED_UNARY_METHOD(elements.radians, radians)
		ITERATED_UNARY_METHOD(elements.degrees, degrees)
		ITERATED_BINARY_METHOD(elements.atan2, atan2)
	};

	#undef ITERATED_UNARY_METHOD
	#undef ITERATED_BINARY_METHOD
	#undef ITERATED_TRINARY_METHOD

}

