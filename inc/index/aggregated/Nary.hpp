#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries

namespace aggregated
{

	template <typename T, typename Op>
	class LeftBinary
	{
		const Op op;
		const T initial;
	public:
		LeftBinary(const Op& op, T& initial):
			op(op),
			initial(initial)
		{}

		auto operator() (const T& a) const
		{
			auto out = initial;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				out = op(out, a[i]);
			}
			return out;
		}

		// // comp

	};

	template <typename T, typename Op>
	class RightBinary
	{
		const Op op;
		const T initial;
	public:
		RightBinary(const Op& op, T& initial):
			op(op),
			initial(initial)
		{}

		auto operator() (const T& a) const
		{
			auto out = initial;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				out = op(a[i], out);
			}
			return out;
		}

	};

}

