#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */

// in-house libraries

namespace iterated
{

	template <typename In1, typename Out>
	bool compatible(const In1& a, Out& out)
	{
		return 
			out.size() >= a.size() &&
			out.size() % a.size() == 0 &&
			a.size() > 0 &&
		true;
	}

	template <typename In1, typename In2, typename Out>
	bool compatible(const In1& a, const In2& b, Out& out)
	{
		return 
			out.size() >= a.size() &&
			out.size() >= b.size() &&
			out.size() % a.size() == 0 &&
			out.size() % b.size() == 0 &&
			a.size() > 0 &&
			b.size() > 0 &&
		true;
	}

	template <typename In1, typename In2, typename In3, typename Out>
	bool compatible(const In1& a, const In2& b, const In3& c, Out& out)
	{
		return 
			out.size() >= a.size() &&
			out.size() >= b.size() &&
			out.size() >= c.size() &&
			out.size() % a.size() == 0 &&
			out.size() % b.size() == 0 &&
			out.size() % c.size() == 0 &&
			a.size() > 0 &&
			b.size() > 0 &&
			c.size() > 0 &&
		true;
	}

}

