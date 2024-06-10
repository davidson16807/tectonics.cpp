#pragma once

// C libraries

// in-house libraries

namespace adapter
{

	/*
	The following allow for scalars to be supported using classes of the adapter pattern
	*/

	#define ADAPTER_SYMBOL_METHOD(SYMBOL, NAME) \
	template <typename In1, typename In2>\
	inline auto NAME (const In1 a, const In2 b) const \
	{\
		return (a[i] SYMBOL b[i]);\
	}

	template <typename T>
	struct SymbolicArithmetic
	{
		const T zero;
		const T one;
		SymbolicArithmetic(const T zero, const T one):
			zero(zero),
			one(one)
		{}
		ADAPTER_SYMBOL_METHOD(+,  add)
		ADAPTER_SYMBOL_METHOD(-,  sub)
		ADAPTER_SYMBOL_METHOD(*,  mult)
		ADAPTER_SYMBOL_METHOD(/,  div)

		template <typename In1, int N>
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

