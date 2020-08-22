#pragma once

#include <cmath> 		// sqrt, etc
#include <algorithm>	// std::sort
#include <functional>	// std::function

#include "types.hpp"

namespace many
{




	// component-wise min
	template <typename T>
	unsigned int min_id(const series<T>& a)
	{
		T min_value = a[0];
		unsigned int min_id = 0;
		for (unsigned int i = 0; i < a.size(); ++i)
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
	unsigned int max_id(const series<T>& a)
	{
		T min_value = a[0];
		unsigned int max_id = 0;
		for (unsigned int i = 0; i < a.size(); ++i)
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
	T sum(const series<T>& a)
	{
		T out = T(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out += a[i];
		}
		return out;
	}
	// component-wise min
	int sum(const series<bool>& a)
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
	T mean(const series<T>& a)
	{
		T out = T(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out += a[i];
		}
		out /= a.size();
		return out;
	}


	template <typename T>
	T standard_deviation(const series<T>& a)
	{
		T mean_a = mean(a);

		T difference(0);
		T sum_of_squared_differences(0);
		for (T i(0); i<a.size(); ++i) {
			difference = (a[i] - mean_a);
			sum_of_squared_differences += difference * difference;
		}
		return std::sqrt(sum_of_squared_differences / (a.size()-1));
	}

	template <typename T>
	T weighted_average(const series<T>& a, const series<T>& weights)
	{
		T out = T(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out += a[i] * weights[i];
		}
		out /= sum(weights);
		return out;
	}

	// TODO: vector version
	template <typename T>
	void rescale(const series<T>& a, series<T>& out, T max_new = 1., T min_new = 0.)
	{
	    T max_old = max(a);
	    T min_old = min(a);
		T range_old = max_old - min_old;
		T range_new = max_new - min_new;

		T scaling_factor = range_new / range_old;

		for (unsigned int i=0; i<a.size(); ++i) {
			out[i] = scaling_factor * (a[i] - min_old) + min_new;
		}
	}
	// // component-wise min
	// template <typename T>
	// T median(const series<T>& a)
	// {
	// 	const series<T> temp = series<T>(a);
	// 	std::sort(std::begin(temp), std::end(temp));
	// 	return a[a.size()/2];
	// }
// 
	// template <typename T>
	// T mode(const series<T>& a)
	// {
	// 	const series<T> temp = series<T>(a);
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
