#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries
#include "compatible.hpp"  /* compatible */

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
			return sum(a, arithmetic.zero);
		}

		template <typename As, typename A>
		auto sum(const As& a, const A zero) const
		{
			A out(zero);
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out = arithmetic.add(out, a[i]);
			}
			return out;
		}

		template <typename As>
		inline auto product(const As& a) const
		{
			return product(a, arithmetic.one);
		}

		template <typename As, typename A>
		auto product(const As& a, const A one) const
		{
			A out(one);
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out = arithmetic.multiply(out, a[i]);
			}
			return out;
		}

		template <typename As>
		auto mean(const As& a) const
		{
			return mean(a, arithmetic.zero);
		}

		template <typename As, typename A>
		auto mean(const As& a, const A zero) const
		{
			A out(zero);
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out = arithmetic.add(out, a[i]);
			}
			out = arithmetic.divide(out, T(a.size()));
			return out;
		}

		template <typename As, typename T2>
		auto linear_combination(const As& a, const T2& weights) const
		{
			return linear_combination(a, weights, arithmetic.zero);
		}

		template <typename As, typename Bs, typename C>
		auto linear_combination(const As& a, const Bs& weights, const C zero) const
		{
			assert(compatible(a,weights));
			C out(zero);
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out = arithmetic.add(out, arithmetic.multiply(a[i], weights[i]));
			}
			return out;
		}

		template <typename T1, typename T2>
		auto weighted_average(const T1& a, const T2& weights) const
		{
			assert(compatible(a,weights));
			return arithmetic.divide(linear_combination(a, weights), sum(weights));
		}

		// // comp

	};

}

