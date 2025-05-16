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

	template <typename ElementClosedForm>
	class ClosedForm
	{
		const ElementClosedForm elements;
	public:
		ClosedForm(const ElementClosedForm& elements):
			elements(elements)
		{}
		ClosedForm():
			elements()
		{}

		ITERATED_UNARY_METHOD(elements.sign,    sign)
		ITERATED_UNARY_METHOD(elements.bitsign, bitsign)
		ITERATED_UNARY_METHOD(elements.abs,     abs)

		ITERATED_UNARY_METHOD (elements.floor,     floor)
		ITERATED_UNARY_METHOD (elements.trunc,     trunc)
		ITERATED_UNARY_METHOD (elements.round,     round)
		ITERATED_UNARY_METHOD (elements.ceil,      ceil)
		ITERATED_UNARY_METHOD (elements.fract,     fract)
		ITERATED_BINARY_METHOD(elements.modulus,   modulus)  
		ITERATED_BINARY_METHOD(elements.residue,   residue)  
		ITERATED_BINARY_METHOD(elements.remainder, remainder)

		ITERATED_UNARY_METHOD(elements.sqrt, sqrt)
		ITERATED_UNARY_METHOD(elements.cbrt, cbrt)
		ITERATED_UNARY_METHOD(elements.inversesqrt, inversesqrt)
		ITERATED_BINARY_METHOD(elements.pow, pow)

		ITERATED_UNARY_METHOD(elements.exp,   exp)
		ITERATED_UNARY_METHOD(elements.exp2,  exp2)
		ITERATED_UNARY_METHOD(elements.exp10, exp10)
		ITERATED_UNARY_METHOD(elements.log,   log)
		ITERATED_UNARY_METHOD(elements.log2,  log2)
		ITERATED_UNARY_METHOD(elements.log10, log10)

		ITERATED_TRINARY_METHOD(elements.mix,        mix)  
		ITERATED_TRINARY_METHOD(elements.step,       step) 
		ITERATED_TRINARY_METHOD(elements.smoothstep, smoothstep)
		ITERATED_TRINARY_METHOD(elements.linearstep, linearstep)

	};

	#undef ITERATED_UNARY_METHOD
	#undef ITERATED_BINARY_METHOD
	#undef ITERATED_TRINARY_METHOD

}

