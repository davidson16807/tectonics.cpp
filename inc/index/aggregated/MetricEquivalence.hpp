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
	class MetricEquivalence
	{
		const ElementMetric elements;
		const T threshold;
	public:
		MetricEquivalence(const ElementMetric& elements, const T threshold):
			elements(elements),
			threshold(threshold)
		{}

		template <typename In1, typename In2>
		bool equal(const In1& a, const In2& b) const
		{
			if (a.size() != b.size()){
				return false;
			}
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (elements.distance(a[i], b[i]) > threshold){
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
				if (elements.distance(a[i], b[i]) > threshold){
					return true;
				}
			}
			return false;
		}

	};

}

