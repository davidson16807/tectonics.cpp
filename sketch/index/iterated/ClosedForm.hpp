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

	template <typename ElementClosedForm>
	class ClosedForm
	{
		const ElementClosedForm elements;
	public:
		ClosedForm(const ElementClosedForm& elements):
			elements(elements)
		{}

		ADAPTER_UNARY_METHOD(elements.isnan, isnan)
		ADAPTER_UNARY_METHOD(elements.isinf, isinf)

		ADAPTER_UNARY_METHOD(elements.sign,    sign)
		ADAPTER_UNARY_METHOD(elements.bitsign, bitsign)
		ADAPTER_UNARY_METHOD(elements.abs,     abs)

		ADAPTER_UNARY_METHOD (elements.abs,       abs)
		ADAPTER_UNARY_METHOD (elements.floor,     floor)
		ADAPTER_UNARY_METHOD (elements.trunc,     trunc)
		ADAPTER_UNARY_METHOD (elements.round,     round)
		ADAPTER_UNARY_METHOD (elements.ceil,      ceil)
		ADAPTER_UNARY_METHOD (elements.fract,     fract)
		ADAPTER_BINARY_METHOD(elements.modulus,   modulus)  
		ADAPTER_BINARY_METHOD(elements.residue,   residue)  
		ADAPTER_BINARY_METHOD(elements.remainder, remainder)

		ADAPTER_UNARY_METHOD(elements.sqrt, sqrt)
		ADAPTER_UNARY_METHOD(elements.cbrt, cbrt)
		ADAPTER_UNARY_METHOD(elements.inversesqrt, inversesqrt)
		ADAPTER_BINARY_METHOD(elements.pow, pow)

		ADAPTER_UNARY_METHOD(elements.exp,   exp)
		ADAPTER_UNARY_METHOD(elements.exp2,  exp2)
		ADAPTER_UNARY_METHOD(elements.exp10, exp10)
		ADAPTER_UNARY_METHOD(elements.log,   log)
		ADAPTER_UNARY_METHOD(elements.log2,  log2)
		ADAPTER_UNARY_METHOD(elements.log10, log10)

		ADAPTER_TRINARY_METHOD(elements.mix,        mix)  
		ADAPTER_TRINARY_METHOD(elements.step,       step) 
		ADAPTER_TRINARY_METHOD(elements.smoothstep, smoothstep)
		ADAPTER_TRINARY_METHOD(elements.linearstep, linearstep)

	};

	#undef ITERATED_UNARY_METHOD
	#undef ITERATED_BINARY_METHOD
	#undef ITERATED_TRINARY_METHOD

}

