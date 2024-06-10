#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries

namespace aggregated
{

	template <typename ElementOrder>
	class Order
	{
		const ElementOrder elements;
	public:
		Order(const ElementOrder& elements):
			elements(elements)
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
			Ti out = a[0];
			for (Tsize i(0); i < a.size(); ++i)
			{
				if (no_nan && elements.isnan(a[i]))
				{
					continue;
				}
				if (no_inf && elements.isinf(a[i]))
				{
					continue;
				}
				out = elements.less_than(a[i], out)? a[i] : out;
			}
			return out;
		}

		template <typename T>
		typename T::value_type max(const T& a, const bool no_nan = true, const bool no_inf = true) const
		{
			auto size = a.size();
			typedef typename T::value_type Ti;
			typedef typename T::size_type Tsize;
			if (a.size() < 1)
			{
				throw elements.out_of_range("cannot find the minimum value of an empty series");
			}
			Ti out = a[0];
			for (Tsize i(0); i < size; ++i)
			{
				if (no_nan && elements.isnan(a[i]))
				{
					continue;
				}
				if (no_inf && elements.isinf(a[i]))
				{
					continue;
				}
				out = elements.greater_than(a[i], out)? a[i] : out;
			}
			return out;
		}

		// component-wise min
		template <typename T>
		typename T::size_type min_id(const T& a) const
		{
			typename T::value_type min_value = a[0];
			typename T::size_type min_id = 0;
			for (typename T::size_type i = 0; i < a.size(); ++i)
			{
				if (a[i] < min_value)
				{
					min_value = a[i];
					min_id = i;
				}
			}
			return min_id;
		}



		template <typename T>
		typename T::size_type max_id(const T& a) const
		{
			typename T::value_type max_value = a[0];
			typename T::size_type max_id = 0;
			for (typename T::size_type i = 0; i < a.size(); ++i)
			{
				if (a[i] > max_value)
				{
					max_value = a[i];
					max_id = i;
				}
			}
			return max_id;
		}

		template <typename In1, typename In2>
		bool equal(const In1& a, const In2& b, const float threshold) const
		{
			return whole::distance(a,b) < threshold;
		}

		template <typename In1, typename In2>
		bool equal(const In1& a, const In2& b) const
		{
			if (a.size() != b.size()){
				return false;
			}
			auto size = a.size();
			for (typename T::size_type i = 0; i < size; ++i)
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
			for (typename T::size_type i = 0; i < size; ++i)
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
			for (typename T::size_type i = 0; i < size; ++i)
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
			for (typename T::size_type i = 0; i < size; ++i)
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
			for (typename T::size_type i = 0; i < size; ++i)
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
			for (typename T::size_type i = 0; i < size; ++i)
			{
				if (a[i] >= b[i]){
					return false;
				}
			}
			return true;
		}

	};

}

