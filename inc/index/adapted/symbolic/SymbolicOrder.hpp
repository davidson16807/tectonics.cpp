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

	#define ADAPTER_UNARY_METHOD(METHOD, NAME) \
	template <typename In1>\
	inline auto NAME (const In1 a) const\
	{\
		return METHOD(a);\
	}

	struct SymbolicOrder
	{
		SymbolicOrder(){}
		ADAPTER_SYMBOL_METHOD(>, greater_than)
		ADAPTER_SYMBOL_METHOD(<, less_than)
		ADAPTER_SYMBOL_METHOD(>=, greater_than_equal)
		ADAPTER_SYMBOL_METHOD(<=, less_than_equal)
		ADAPTER_SYMBOL_METHOD(==, equal)
		ADAPTER_SYMBOL_METHOD(!=, not_equal)

		template <typename In1, typename In2>
		inline auto max (const In1 a, const In2 b) const 
		{
			return a > b? a : b;
		}

		template <typename In1, typename In2>
		inline auto min (const In1 a, const In2 b) const 
		{
			return a < b? a : b;
		}

		template <typename In1, typename In2>
		inline auto clamp (const In1 a, const In2 lo, const In2 hi) const 
		{
			return max(lo, min(hi, a));
		}

	};

	#undef ADAPTER_SYMBOL_METHOD

}

