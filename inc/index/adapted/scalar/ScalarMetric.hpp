#pragma once

// C libraries
#include <cmath>     /* std math */

// in-house libraries

namespace adapted
{

	/*
	The following are alternate definitions of the above that allow for support of other data types using classes of the adapter pattern
	*/

	template<typename scalar>
	struct ScalarMetric
	{
		ScalarMetric(){}

		/*
		in line with its formal mathematical definition, `distance(a,b)` always returns a nonnegative number,
		it always satisfies the triangle equality, and it equals 0 iff a==b.
		*/
		inline constexpr auto distance(const scalar a, const scalar b) const
		{
		    return std::abs(a-b);
		}

		/*
		`length(a)` is defined here as `distance(a,o)` for some "zero" value o
		*/
		inline constexpr auto length(const scalar a) const
		{
		    return std::abs(a);
		}

		/*
		`normalize(a)` is defined here as `a/length(a)`
		*/
		inline constexpr auto normalize(const scalar a) const
		{
		    return a / length(a);
		}

	};

}

