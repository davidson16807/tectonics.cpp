#pragma once

// C libraries

// in-house libraries

namespace adapted
{

	/*
	The following allow for scalars to be supported using classes of the adapter pattern
	*/

	struct BooleanBitset
	{
		BooleanBitset(){}

		inline auto unite     (const bool a, const bool b) const {return a || b;}
		inline auto intersect (const bool a, const bool b) const {return a && b;}
		inline auto differ    (const bool a, const bool b) const {return a && !b;}
		inline auto negate    (const bool a) const {return a;}

		inline auto all       (const bool a) const {return a;}
		inline auto any       (const bool a) const {return a;}

	};

}

