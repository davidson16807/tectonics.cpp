#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries
#include "compatible.hpp"

namespace aggregated
{

	/*
	An `aggregated::Metric` aggregates elements that are subject to an `adapter::Metric`.
	It intends to provide all well-known operations and relations that could be provided simply by knowing that a `*Metric` exists.
	An `aggregated::Metric` is itself a `Metric`, but it is also in fact `Order`,
	since distance can be checked against a threshold
	*/

	template <typename ElementMetric>
	class Metric
	{
		const ElementMetric elements;
	public:
		Metric(const ElementMetric& elements):
			elements(elements)
		{}

		template <typename In1, typename In2>
		auto distance(const In1& a, const In2& b) const
		{
			assert(compatible(a,b));
			auto size = a.size();
			auto out(elements.distance(a[0], b[0]));
			for (auto i = 0*size+1; i < size; ++i)
			{
				out += elements.distance(a[i], b[i]);
			}
			return out;
		}

		template <typename In1, typename In2>
		auto max_distance(const In1& a, const In2& b) const
		{
			auto size = a.size();
			if (a.size() < 1)
			{
				throw std::out_of_range("cannot find the minimum value of an empty series");
			}
			auto out(elements.distance(a[0], b[0]));
			for (auto i=0*size; i < size; ++i)
			{
				auto delta = elements.distance(a[i], b[i]);
				out = delta > out? delta : out;
			}
			return out;
		}

		template <typename In1, typename In2>
		bool equal(const In1& a, const In2& b, const float threshold) const
		{
			return max_distance(a,b) < threshold;
		}

		template <typename In1, typename In2>
		bool equal(const In1& a, const In2& b) const
		{
			if (a.size() != b.size()){
				return false;
			}
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (a[i] != b[i]){
					return false;
				}
			}
			return true;
		}

		template <typename In1, typename In2>
		bool not_equal(const In1& a, const In2& b) const
		{
			if (a.size() != b.size()){
				return true;
			}
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (a[i] != b[i]){
					return true;
				}
			}
			return false;
		}

		template <typename In1, typename In2>
		bool greater_than(const In1& a, const In2& b) const
		{
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (a[i] <= b[i]){
					return false;
				}
			}
			return true;
		}

		template <typename In1, typename In2>
		bool less_than(const In1& a, const In2& b) const
		{
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (a[i] >= b[i]){
					return false;
				}
			}
			return true;
		}

		template <typename In1, typename In2>
		bool greater_than_equal(const In1& a, const In2& b) const
		{
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (a[i] <= b[i]){
					return false;
				}
			}
			return true;
		}

		template <typename In1, typename In2>
		bool less_than_equal(const In1& a, const In2& b) const
		{
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (a[i] >= b[i]){
					return false;
				}
			}
			return true;
		}

	};

}

