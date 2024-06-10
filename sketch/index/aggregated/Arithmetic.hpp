#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */
#include <array>     /* std::array */
#include <algorithm> /* std::clamp */

// in-house libraries
#include <math/special.hpp>

#include "each.hpp"

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
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out = elements.add(out, a[i]);
			}
			return out;
		}

		template <typename T>
		auto product(const T& a) const
		{
			auto out = elements.one;
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out = elements.add(out, a[i]);
			}
			return out;
		}

		template <typename T>
		auto mean(const T& a) const
		{
			auto out = elements.zero;
			for (unsigned int i = 0; i < a.size(); ++i)
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
			for (unsigned int i = 0; i < a.size(); ++i)
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

		// // comp

	};

}

