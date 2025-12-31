#pragma once

// C libraries

// in-house libraries

namespace adapted
{

	/*
	The following allow for scalars to be supported using classes of the adapter pattern
	*/

	#define ADAPTER_SYMBOL_METHOD(SYMBOL, NAME) \
	template <typename In1, typename In2>\
	inline auto NAME (const In1 a, const In2 b) const \
	{\
		return (a SYMBOL b);\
	}

	struct SymbolicEquivalence
	{
		SymbolicEquivalence(){}
		ADAPTER_SYMBOL_METHOD(==, equal)
		ADAPTER_SYMBOL_METHOD(!=, not_equal)

	};

	#undef ADAPTER_SYMBOL_METHOD

}

