#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */
#include <array>     /* std::array */
#include <algorithm> /* std::clamp */

// in-house libraries
#include <math/special.hpp>

namespace whole
{

	template <typename In1, typename In2>
	bool compatible(const In1& a, In2& b)
	{
		auto hi = std::max(a.size(), b.size());
		auto lo = std::min(a.size(), b.size());
		return 
			hi % a.size() == 0 &&
			hi % b.size() == 0 &&
			lo > 0;
	}

	template <typename T>
	typename T::value_type min(const T& a, const bool no_nan = true, const bool no_inf = true)
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
			if (no_nan && std::isnan(a[i]))
			{
				continue;
			}
			if (no_inf && std::isinf(a[i]))
			{
				continue;
			}
			out = a[i] < out? a[i] : out;
		}
		return out;
	}

	template <typename T>
	typename T::value_type max(const T& a, const bool no_nan = true, const bool no_inf = true)
	{
		auto size = a.size();
		typedef typename T::value_type Ti;
		typedef typename T::size_type Tsize;
		if (a.size() < 1)
		{
			throw std::out_of_range("cannot find the minimum value of an empty series");
		}
		Ti out = a[0];
		for (Tsize i(0); i < size; ++i)
		{
			if (no_nan && std::isnan(a[i]))
			{
				continue;
			}
			if (no_inf && std::isinf(a[i]))
			{
				continue;
			}
			out = a[i] > out? a[i] : out;
		}
		return out;
	}

	template <typename In1, typename In2>
	double distance(const In1& a, const In2& b)
	{
		double out(0);
		assert(compatible(a,b));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out += math::distance(a[i], b[i]);
		}
		return out;
	}

	template <typename Out, typename T>
	Out sum(const T& a)
	{
		Out out(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out += a[i];
		}
		return out;
	}

	template <typename T>
	typename T::value_type mean(const T& a)
	{
		typename T::value_type out(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out += a[i];
		}
		out /= a.size();
		return out;
	}


	template <typename T>
	typename T::value_type standard_deviation(const T& a)
	{
		typename T::value_type mean_a = mean(a);

		typename T::value_type difference(0);
		typename T::value_type sum_of_squared_differences(0);
		for (typename T::value_type i(0); i<a.size(); ++i) {
			difference = (a[i] - mean_a);
			sum_of_squared_differences += difference * difference;
		}
		return std::sqrt(sum_of_squared_differences / (a.size()-1));
	}

	template <typename T>
	typename T::value_type standard_deviation(const T& a, const T& b)
	{
		typename T::value_type difference(0);
		typename T::value_type sum_of_squared_differences(0);
		for (typename T::value_type i(0); i<a.size(); ++i) {
			difference = (a[i] - b[i]);
			sum_of_squared_differences += difference * difference;
		}
		return std::sqrt(sum_of_squared_differences / (a.size()-1));
	}

}

