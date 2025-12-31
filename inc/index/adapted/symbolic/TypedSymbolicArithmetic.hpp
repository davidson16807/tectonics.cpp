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

	template <typename T>
	struct TypedSymbolicArithmetic
	{
		const T zero;
		const T one;
		TypedSymbolicArithmetic(const T zero, const T one):
			zero(zero),
			one(one)
		{}
		TypedSymbolicArithmetic():
			zero(0),
			one(1)
		{}
		ADAPTER_SYMBOL_METHOD(+,  add)
		ADAPTER_SYMBOL_METHOD(-,  subtract)
		ADAPTER_SYMBOL_METHOD(*,  multiply)
		ADAPTER_SYMBOL_METHOD(/,  divide)

		template <int N, typename In1>
		inline auto pow (const In1 a) const
		{
			return 
				N < 0?  one/pow<-N>(a) 
			  : N == 0? one
			  :         a * pow<N-1>(a);
		}
	};

	#undef ADAPTER_SYMBOL_METHOD

}

