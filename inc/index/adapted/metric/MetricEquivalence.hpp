#pragma once

// C libraries

// in-house libraries

namespace adapted
{

	/*
	A `MetricEquivalence` is an `*Equivalence` that is defined by whether the `distance` function of a `*Metric` exceeds a `threshold`
	*/

	#define ADAPTER_SYMBOL_METHOD(SYMBOL, NAME) \
	template <typename In1, typename In2>\
	inline auto NAME (const In1 a, const In2 b) const \
	{\
		return (metric.length(a) SYMBOL metric.length(b));\
	}

	template <typename T, typename Metric>
	struct MetricEquivalence
	{
		const T threshold;
		const Metric metric;
		MetricEquivalence(const T threshold, const Metric& metric):
			threshold(threshold),
			metric(metric)
		{}
		MetricEquivalence(const T threshold):
			threshold(threshold),
			metric()
		{}

		template <typename In1, typename In2>
		inline bool equal (const In1 a, const In2 b) const 
		{
			return metric.distance(a,b) <= threshold;
		}

		template <typename In1, typename In2>
		inline bool not_equal (const In1 a, const In2 b) const 
		{
			return metric.distance(a,b) > threshold;
		}

	};

	#undef ADAPTER_SYMBOL_METHOD

}

