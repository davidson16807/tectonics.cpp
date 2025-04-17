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
	It is important to note that the names of classes in the `aggregated::` namespace 
	simply describe the structures they aggregate, and need not reflect the structures they are,
	so it is left as an excercise for the reader to determine whether `aggregated::Metric` is itself a `Metric`
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

	};

}

