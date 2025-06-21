#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries

namespace aggregated
{

	template <typename ElementArithmetic>
	class Reduction
	{
		const ElementArithmetic arithmetic;
	public:
		Reduction(const ElementArithmetic& arithmetic):
			arithmetic(arithmetic)
		{}
		Reduction():
			arithmetic()
		{}

		template <typename As>
		auto count(const As& a) const
		{
			return a.size();
		}

		template <typename As>
		auto sum(const As& a) const
		{
			using A = typename As::value_type;
			A out(0);
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out = arithmetic.add(out, a[i]);
			}
			return out;
		}

		template <typename As>
		auto product(const As& a) const
		{
			using A = typename As::value_type;
			A out(1);
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out = arithmetic.add(out, a[i]);
			}
			return out;
		}

		template <typename As>
		auto mean(const As& a) const
		{
			using A = typename As::value_type;
			A out(0);
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out = arithmetic.add(out, a[i]);
			}
			out = arithmetic.divide(out, a.size());
			return out;
		}

		template <typename As, typename T2>
		auto linear_combination(const As& a, const T2& weights) const
		{
			using A = typename As::value_type;
			A out(0);
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out = arithmetic.add(out, arithmetic.multiply(a[i], weights[i]));
			}
			return out;
		}

		template <typename T1, typename T2>
		auto weighted_average(const T1& a, const T2& weights) const
		{
			return arithmetic.divide(linear_combination(a, weights), sum(weights));
		}

		// // comp

	};

}

