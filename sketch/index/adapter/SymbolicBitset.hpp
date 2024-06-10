#pragma once

// C libraries

// in-house libraries

namespace adapter
{

	/*
	The following allow for scalars to be supported using classes of the adapter pattern
	*/

	struct SymbolicBitset
	{
		SymbolicBitset(){}

		template <typename In1, typename In2> inline auto unite     (const In1 a, const In2 b) const {return a | b;}
		template <typename In1, typename In2> inline auto intersect (const In1 a, const In2 b) const {return a & b;}
		template <typename In1, typename In2> inline auto differ    (const In1 a, const In2 b) const {return a & ~b;}

	};

}

