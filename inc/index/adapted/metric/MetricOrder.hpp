#pragma once

// C libraries

// in-house libraries

namespace adapted
{

	/*
	A `MetricOrder` is an `*Order` that is defined using the `length` function of a `*Metric`
	*/

	#define ADAPTER_SYMBOL_METHOD(SYMBOL, NAME) \
	template <typename In1, typename In2>\
	inline auto NAME (const In1 a, const In2 b) const \
	{\
		return (metric.length(a) SYMBOL metric.length(b));\
	}

	template<typename Metric>
	struct MetricOrder
	{
		const Metric metric;

		MetricOrder(const Metric& metric):
			metric(metric)
		{}
		MetricOrder():
			metric()
		{}
		ADAPTER_SYMBOL_METHOD(>, greater_than)
		ADAPTER_SYMBOL_METHOD(<, less_than)
		ADAPTER_SYMBOL_METHOD(>=, greater_than_equal)
		ADAPTER_SYMBOL_METHOD(<=, less_than_equal)
		ADAPTER_SYMBOL_METHOD(==, equal)
		ADAPTER_SYMBOL_METHOD(!=, not_equal)

		template <typename In1, typename In2>
		inline auto max (const In1 a, const In2 b) const 
		{
			return metric.length(a) > metric.length(b)? a : b;
		}

		template <typename In1, typename In2>
		inline auto min (const In1 a, const In2 b) const 
		{
			return metric.length(a) < metric.length(b)? a : b;
		}

		template <typename In1, typename In2>
		inline auto clamp (const In1 a, const In2 lo, const In2 hi) const 
		{
			return max(lo, min(hi, a));
		}

	};

	#undef ADAPTER_SYMBOL_METHOD

}

