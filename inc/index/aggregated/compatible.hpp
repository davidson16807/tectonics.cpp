#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */

// in-house libraries

namespace aggregated
{

	template <typename In1, typename In2>
	bool compatible(const In1& a, const In2& b)
	{
		return 
			std::max(a.size(), b.size()) % a.size() == 0 &&
			std::max(a.size(), b.size()) % b.size() == 0 &&
			a.size() > 0 &&
			b.size() > 0 &&
		true;
	}

	template <typename In1, typename In2, typename In3, typename Out>
	bool compatible(const In1& a, const In2& b, const In3& c, Out& out)
	{
		return 
			std::max(a.size(), std::max(b.size(), c.size())) % a.size() == 0 &&
			std::max(a.size(), std::max(b.size(), c.size())) % b.size() == 0 &&
			a.size() > 0 &&
			b.size() > 0 &&
		true;
	}

}

