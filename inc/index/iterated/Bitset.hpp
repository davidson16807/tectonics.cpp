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

	template <typename ElementBitset>
	class Bitset
	{
		const ElementBitset elements;
	public:
		Bitset(const ElementBitset& elements):
			elements(elements)
		{}
		Bitset():
			elements()
		{}
		ITERATED_BINARY_METHOD(elements.unite,     unite)
		ITERATED_BINARY_METHOD(elements.intersect, intersect)
		ITERATED_BINARY_METHOD(elements.differ,    differ)
		ITERATED_UNARY_METHOD (elements.negate,    negate)
		ITERATED_UNARY_METHOD (elements.copy,      copy)
	};

	#undef ITERATED_UNARY_METHOD
	#undef ITERATED_BINARY_METHOD
	#undef ITERATED_TRINARY_METHOD

}

