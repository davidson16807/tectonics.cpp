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
		typename T::value_type min(const T& a, const typename T::value_type lo, const typename T::value_type hi) const
		{
			typedef typename T::value_type Ti;
			if (a.size() < 1)
			{
				throw std::out_of_range("cannot find the minimum value of an empty series");
			}
			Ti extremum = aggregation.min(a[0]);
			for (auto i = 0*a.size()+1; i < a.size(); ++i)
			{
				auto ai = aggregation.min(a[i]);
				extremum = order.less_than(ai, extremum) && order.less_than(lo, ai) && order.less_than(ai, hi)? ai : extremum;
			}
			return extremum;
		}

		template <typename T>
		typename T::value_type max(const T& a, const typename T::value_type lo, const typename T::value_type hi) const
		{
			typedef typename T::value_type Ti;
			if (a.size() < 1)
			{
				throw std::out_of_range("cannot find the minimum value of an empty series");
			}
			Ti extremum = aggregation.max(a[0]);
			for (auto i = 0*a.size()+1; i < a.size(); ++i)
			{
				auto ai = aggregation.max(a[i]);
				extremum = order.greater_than(ai, extremum) && order.less_than(lo, ai) && order.less_than(ai, hi)? ai : extremum;
			}
			return extremum;
		}

		template <typename T>
		typename T::value_type min(const T& a) const
		{
			typedef typename T::value_type Ti;
			if (a.size() < 1)
			{
				throw std::out_of_range("cannot find the minimum value of an empty series");
			}
			Ti extremum = aggregation.min(a[0]);
			for (auto i = 0*a.size()+1; i < a.size(); ++i)
			{
				auto ai = aggregation.min(a[i]);
				extremum = order.less_than(ai, extremum)? ai : extremum;
			}
			return extremum;
		}

		template <typename T>
		typename T::value_type max(const T& a) const
		{
			typedef typename T::value_type Ti;
			if (a.size() < 1)
			{
				throw std::out_of_range("cannot find the minimum value of an empty series");
			}
			Ti extremum = aggregation.max(a[0]);
			for (auto i = 0*a.size()+1; i < a.size(); ++i)
			{
				auto ai = aggregation.max(a[i]);
				extremum = order.greater_than(ai, extremum)? ai : extremum;
			}
			return extremum;
		}

		// component-wise min
		template <typename T>
		typename T::size_type min_id(const T& a, const typename T::value_type lo, const typename T::value_type hi) const
		{
			typename T::value_type extremum = aggregation.min(a[0]);
			typename T::size_type extremum_id = 0;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				auto ai = aggregation.max(a[i]);
				if (order.less_than(ai, extremum) && order.less_than(lo, ai) && order.less_than(ai, hi))
				{
					extremum = ai;
					extremum_id = i;
				}
			}
			return extremum_id;
		}

		template <typename T>
		typename T::size_type max_id(const T& a, const typename T::value_type lo, const typename T::value_type hi) const
		{
			typename T::value_type extremum = aggregation.max(a[0]);
			typename T::size_type extremum_id = 0;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				auto ai = aggregation.max(a[i]);
				if (order.greater_than(ai, extremum) && order.less_than(lo, ai) && order.less_than(ai, hi))
				{
					extremum = ai;
					extremum_id = i;
				}
			}
			return extremum_id;
		}

		// component-wise min
		template <typename T>
		typename T::size_type min_id(const T& a) const
		{
			typename T::value_type extremum = aggregation.min(a[0]);
			typename T::size_type extremum_id = 0;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				auto ai = aggregation.max(a[i]);
				if (order.less_than(ai, extremum))
				{
					extremum = ai;
					extremum_id = i;
				}
			}
			return extremum_id;
		}

		template <typename T>
		typename T::size_type max_id(const T& a) const
		{
			typename T::value_type extremum = aggregation.max(a[0]);
			typename T::size_type extremum_id = 0;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				auto ai = aggregation.max(a[i]);
				if (order.greater_than(ai, extremum))
				{
					extremum = ai;
					extremum_id = i;
				}
			}
			return extremum_id;
		}

	};

}

