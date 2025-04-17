#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries

namespace aggregated
{

	template <typename ElementArithmetic>
	class Arithmetic
	{
		const ElementArithmetic elements;
	public:
		Arithmetic(const ElementArithmetic& elements):
			elements(elements)
		{}

		template <typename T>
		auto sum(const T& a) const
		{
			auto out = elements.zero;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				out = elements.add(out, a[i]);
			}
			return out;
		}

		template <typename T>
		auto product(const T& a) const
		{
			auto out = elements.one;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				out = elements.add(out, a[i]);
			}
			return out;
		}

		template <typename T>
		auto mean(const T& a) const
		{
			auto out = elements.zero;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				out = elements.add(out, a[i]);
			}
			out = elements.divide(out, a.size());
			return out;
		}

		template <typename T1, typename T2>
		auto linear_combination(const T1& a, const T2& weights) const
		{
			auto out = elements.zero;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				out = elements.add(out, elements.multiply(a[i], weights[i]));
			}
			return out;
		}

		template <typename T1, typename T2>
		auto weighted_average(const T1& a, const T2& weights) const
		{
			return elements.divide(linear_combination(a, weights), sum(weights));
		}

	};

}

