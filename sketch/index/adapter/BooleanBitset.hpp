#pragma once

// C libraries

// in-house libraries

namespace adapter
{

	/*
	The following allow for scalars to be supported using classes of the adapter pattern
	*/

	struct BooleanBitset
	{
		BooleanBitset(){}

		template <typename bool, typename bool> inline auto unite     (const bool a, const bool b) const {return a || b;}
		template <typename bool, typename bool> inline auto intersect (const bool a, const bool b) const {return a && b;}
		template <typename bool, typename bool> inline auto differ    (const bool a, const bool b) const {return a && !b;}

	};

}

