#pragma once

// C libraries
#include <cmath>     /* std math */

// in-house libraries

namespace adapter
{

	/*
	The following are alternate definitions of the above that allow for support of other data types using classes of the adapter pattern
	*/

	struct ScalarClosedForm
	{
		ScalarClosedForm(){}

		template<typename In1, typename In2>
		inline constexpr auto distance(const In1 a, const In2 b) const
		{
		    return std::abs(a-b);
		}

		template<typename In1, typename In2>
		inline constexpr auto length(const In1 a) const
		{
		    return std::abs(a);
		}

		template<typename In1, typename In2>
		inline constexpr auto normalize(const In1 a) const
		{
		    return a / length(a);
		}

	};

}

