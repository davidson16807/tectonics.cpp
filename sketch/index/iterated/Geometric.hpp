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
		assert(compatible(a,b,out));\
		auto size = out.size();\
		using size_type = typename Out::size_type;\
		for (size_type i = 0; i < size; ++i)\
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
		using size_type = typename Out::size_type;\
		for (size_type i = 0; i < size; ++i)\
		{\
			out[i] = METHOD(a[i], b[i]);\
		}\
	}

	#define ITERATED_TRINARY_METHOD(METHOD, NAME) \
	template <typename In1, typename In2, typename In3, typename Out>\
	void NAME (const In1& a, const In2& b, const In3& c, Out& out) const\
	{\
		assert(compatible(a,b,c,out));\
		auto size = out.size();\
		using size_type = typename Out::size_type;\
		for (size_type i = 0; i < size; ++i)\
		{\
			out[i] = METHOD(a[i], b[i], c[i]);\
		}\
	}

	template <typename ElementGeometric>
	class Geometric
	{
		const ElementGeometric elements;
	public:
		Geometric(const ElementGeometric& elements):
			elements(elements)
		{}
		ADAPTER_UNARY_METHOD(elements.length, length)
		ADAPTER_UNARY_METHOD(elements.normalize, normalize)

		ADAPTER_BINARY_METHOD(elements.reflect, reflect)
		ADAPTER_BINARY_METHOD(elements.cross, cross)
		ADAPTER_BINARY_METHOD(elements.dot, dot)

		ADAPTER_BINARY_METHOD(elements.similarity, similarity)
		ADAPTER_BINARY_METHOD(elements.scalar_projection, scalar_projection)
		ADAPTER_BINARY_METHOD(elements.vector_projection, vector_projection)
		ADAPTER_BINARY_METHOD(elements.scalar_rejection, scalar_rejection)
		ADAPTER_BINARY_METHOD(elements.vector_rejection, vector_rejection)

		ADAPTER_TRINARY_METHOD(elements.faceforward, faceforward)
	};

	#undef ITERATED_UNARY_METHOD
	#undef ITERATED_BINARY_METHOD
	#undef ITERATED_TRINARY_METHOD

}

