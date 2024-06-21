#pragma once

// C libraries
#include <cmath>     /* std math */

// in-house libraries

namespace adapted
{

	/*
	The following are alternate definitions of the above that allow for support of other data types using classes of the adapter pattern
	*/

	struct ScalarMetric
	{
		ScalarMetric(){}

		/*
		in line with its formal mathematical definition, `distance(a,b)` always returns a nonnegative number,
		it always satisfies the triangle equality, and it equals 0 iff a==b.
		*/
		template<typename In1, typename In2>
		inline constexpr auto distance(const In1 a, const In2 b) const
		{
		    return std::abs(a-b);
		}

		/*
		`length(a)` is defined here as `distance(a,o)` for some "zero" value o
		*/
		template<typename In1>
		inline constexpr auto length(const In1 a) const
		{
		    return std::abs(a);
		}

		/*
		`normalize(a)` is defined here as `a/length(a)`
		*/
		template<typename In1>
		inline constexpr auto normalize(const In1 a) const
		{
		    return a / length(a);
		}

	};

}

