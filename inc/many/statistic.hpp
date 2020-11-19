#pragma once

#include <cmath> 		// sqrt, etc
#include <algorithm>	// std::sort
#include <functional>	// std::function

#include "types.hpp"

namespace many
{




	// component-wise min
	template <typename T>
	std::size_t min_id(const T& a)
	{
		typename T::value_type min_value = a[0];
		std::size_t min_id = 0;
		for (std::size_t i = 0; i < a.size(); ++i)
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
	std::size_t max_id(const T& a)
	{
		typename T::value_type min_value = a[0];
		std::size_t max_id = 0;
		for (std::size_t i = 0; i < a.size(); ++i)
		{
			if (a[i] < min_value)
			{
				min_value = a[i];
				max_id = i;
			}
		}
		return max_id;
	}


	// component-wise min
	template <typename T>
	typename T::value_type sum(const T& a)
	{
		typename T::value_type out(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out += a[i];
		}
		return out;
	}
	// component-wise min
	int sum(const Series<bool>& a)
	{
		int out = 0;
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			if (a[i])
			{
				out++;
			}
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
	typename T::value_type weighted_average(const T& a, const T& weights)
	{
		typename T::value_type out(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out += a[i] * weights[i];
		}
		out /= sum(weights);
		return out;
	}

	// TODO: vector version
	template <typename T>
	void rescale(const T& a, T& out, typename T::value_type max_new = 1., typename T::value_type min_new = 0.)
	{
	    typename T::value_type max_old = max(a);
	    typename T::value_type min_old = min(a);
		typename T::value_type range_old = max_old - min_old;
		typename T::value_type range_new = max_new - min_new;

		typename T::value_type scaling_factor = range_new / range_old;

		for (unsigned int i=0; i<a.size(); ++i) {
			out[i] = scaling_factor * (a[i] - min_old) + min_new;
		}
	}
	// // component-wise min
	// template <typename T>
	// typename T::value_type median(const T& a)
	// {
	// 	const typename T::value_type temp(a);
	// 	std::sort(std::begin(temp), std::end(temp));
	// 	return a[a.size()/2];
	// }
// 
	// template <typename T>
	// typename T::value_type mode(const T& a)
	// {
	// 	const typename T::value_type temp(a);
	// 	std::sort(std::begin(temp), std::end(temp));
	//     int value = a[0];
	//     int max = a[0];
	//     int min = a[0];
	//     int mode;
	//     for (int i = 1, mode_count = 1, count = 1; i < a.size(); ++i) {
	//         if (a[i] == value)
	//             ++mode_count;
	//         if (mode_count > count) {
	//             count = mode_count;
	//             mode = value;
	//         }
	//         if (a[i] > max)
	//             max = a[i];
	//         if (a[i] < min)
	//             min = a[i];
	//         value = a[i];
	//     }
	//     return mode;
	// }
} //namespace many
