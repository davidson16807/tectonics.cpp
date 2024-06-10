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

	template <typename ElementArithmetic, typename ElementAlgebraic>
	class Statistics
	{
		const ElementArithmetic arithmetic;
		const ElementAlgebraic algebraic;
	public:
		Statistics(const ElementArithmetic& arithmetic, const ElementAlgebraic& algebraic):
			arithmetic(arithmetic),
			algebraic(algebraic)
		{}

		template <typename T>
		auto sum(const T& a) const
		{
			auto out = arithmetic.zero;
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out = arithmetic.add(out, a[i]);
			}
			return out;
		}

		template <typename T>
		auto product(const T& a) const
		{
			auto out = arithmetic.one;
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out = arithmetic.add(out, a[i]);
			}
			return out;
		}

		template <typename T>
		auto mean(const T& a) const
		{
			auto out = arithmetic.zero;
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out = arithmetic.add(out, a[i]);
			}
			out = arithmetic.divide(out, a.size());
			return out;
		}

		template <typename T>
		auto standard_deviation(const T& a) const
		{
			auto mean_a = mean(a);

			auto difference = arithmetic.zero;
			auto sum_of_squared_differences = arithmetic.zero;
			for (typename T::size_type i(0); i<a.size(); ++i) {
				difference = arithmetic.subtract(a[i], mean_a);
				sum_of_squared_differences = 
					arithmetic.add(sum_of_squared_differences, arithmetic.multiply(difference, difference));
			}
			return algebraic.sqrt(arithmetic.divide(sum_of_squared_differences, a.size()-1));
		}

		template <typename T>
		auto standard_deviation(const T& a, const T& b) const
		{
			auto difference = arithmetic.zero;
			auto sum_of_squared_differences = arithmetic.zero;
			for (typename T::size_type i(0); i<a.size(); ++i) {
				difference = arithmetic.subtract(a[i], b[i]);
				sum_of_squared_differences = 
					arithmetic.add(sum_of_squared_differences, arithmetic.multiply(difference, difference));
			}
			return algebraic.sqrt(arithmetic.divide(sum_of_squared_differences, a.size()-1));
		}

		template <typename T1, typename T2>
		auto linear_combination(const T1& a, const T2& weights) const
		{
			auto out = arithmetic.zero;
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

