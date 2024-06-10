#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries

namespace aggregated
{

	template <typename ElementOrderAggregation, typename ElementAggregateOrder>
	class OrderAggregation
	{
		const ElementOrderAggregation aggregation;
		const ElementAggregateOrder order;
	public:
		OrderAggregation(const ElementOrderAggregation& aggregation, const ElementAggregateOrder& order):
			aggregation(aggregation),
			order(order)
		{}

		template <typename T>
		typename T::value_type min(const T& a, const bool no_nan = true, const bool no_inf = true) const
		{
			typedef typename T::value_type Ti;
			typedef typename T::size_type Tsize;
			if (a.size() < 1)
			{
				throw std::out_of_range("cannot find the minimum value of an empty series");
			}
			Ti out = aggregation.min(a[0]);
			for (Tsize i(1); i < a.size(); ++i)
			{
				Aggregation = aggregation.min(a[i]);
				out = order.less_than(aggregation, out)? aggregation : out;
			}
			return out;
		}

		template <typename T>
		typename T::value_type max(const T& a, const bool no_nan = true, const bool no_inf = true) const
		{
			typedef typename T::value_type Ti;
			typedef typename T::size_type Tsize;
			if (a.size() < 1)
			{
				throw std::out_of_range("cannot find the minimum value of an empty series");
			}
			Ti out = aggregation.max(a[0]);
			for (Tsize i(1); i < a.size(); ++i)
			{
				Aggregation = aggregation.max(a[i]);
				out = order.greater_than(aggregation, out)? aggregation : out;
			}
			return out;
		}

	};

}

