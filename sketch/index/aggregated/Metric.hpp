#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries

namespace aggregated
{

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
			for (std::size_t i = 1; i < size; ++i)
			{
				out += elements.distance(a[i], b[i]);
			}
			return out;
		}

		template <typename In1, typename In2>
		auto max_distance(const In1& a, const In2& b) const
		{
			auto size = a.size();
			typedef typename In1::size_type Tsize;
			if (a.size() < 1)
			{
				throw std::out_of_range("cannot find the minimum value of an empty series");
			}
			auto out(elements.distance(a[0], b[0]));
			for (Tsize i(1); i < size; ++i)
			{
				auto delta = elements.distance(a[i], b[i]);
				out = delta > out? delta : out;
			}
			return out;
		}

	};

}

