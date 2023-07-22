#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */

// in-house libraries
#include "compatibility.hpp"
#include "each.hpp"

namespace whole
{


	template <typename T>
	typename T::value_type min(const T& a, const bool no_nan = true, const bool no_inf = true)
	{
		typedef typename T::value_type Ti;
		typedef typename T::size_type Tsize;
		if (a.size() < 1)
		{
			throw std::out_of_range("cannot find the minimum value of an empty composite");
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
			throw std::out_of_range("cannot find the minimum value of an empty composite");
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



	template <typename T1, typename T2>
	double distance(const T1& a, const T2& b)
	{
		double out(0);
		// assert(compatible(a,b));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out += each::distance(a[i], b[i]);
		}
		return out;
	}



	template <typename T1, typename T2>
	bool all(const T1& a)
	{
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (!a[i]){
				return false;
			}
		}
		return true;
	}

	template <typename T1, typename T2>
	bool any(const T1& a)
	{
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i]){
				return false;
			}
		}
		return true;
	}

	template <typename T1, typename T2>
	bool equal(const T1& a, const T2& b, const float threshold)
	{
		return whole::distance(a,b) < threshold;
	}

	template <typename T1, typename T2>
	bool equal(const T1& a, const T2& b)
	{
		if (!compatible(a,b)){
			return false;
		}
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i] != b[i]){
				return false;
			}
		}
		return true;
	}

	template <typename T1, typename T2>
	bool notEqual(const T1& a, const T2& b)
	{
		if (!compatible(a,b)){
			return true;
		}
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i] != b[i]){
				return true;
			}
		}
		return false;
	}

	template <typename T1, typename T2>
	bool greaterThan(const T1& a, const T2& b)
	{
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i] <= b[i]){
				return false;
			}
		}
		return true;
	}

	template <typename T1, typename T2>
	bool lessThan(const T1& a, const T2& b)
	{
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i] >= b[i]){
				return false;
			}
		}
		return true;
	}

	template <typename T1, typename T2>
	bool greaterThanEqual(const T1& a, const T2& b)
	{
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i] <= b[i]){
				return false;
			}
		}
		return true;
	}

	template <typename T1, typename T2>
	bool lessThanEqual(const T1& a, const T2& b)
	{
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i] >= b[i]){
				return false;
			}
		}
		return true;
	}



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



	namespace 
	{
		#if defined(__clang__)
			const std::vector<const std::string> 
		#else
			const std::array<const std::string, 6>
		#endif
		shades {" ", "░", "▒", "▓", "█" };
	} 

	template <typename T>
	std::string to_string(const T& a, const typename T::value_type lo, const typename T::value_type hi, const int line_char_width = 80)
	{
		std::string out("");
		for (unsigned int i = 0; i < a.size(); ++i)
		{
		    if (i % line_char_width == 0)
		    {
		    	out += "\n";
		    }
		    
			if (std::isnan(a[i]))
			{
				out += "N";
			}
			else if (std::isinf(a[i]))
			{
				out += "∞";
			}
			else 
			{
				float shade_fraction = each::linearstep(lo, hi, a[i]);
				int shade_id = int(std::min(float(shades.size()-1), (shades.size() * shade_fraction) ));
			    out += shades[shade_id];
			}
		}
		out += "\n";
		for (unsigned int i = 0; i < shades.size(); ++i)
		{
			out += shades[i];
			out += " ≥ ";
			out += std::to_string(each::mix(float(lo), float(hi), float(i)/float(shades.size())));
			out += "\n";
			// out += ", ";
		}
		return out;
	}

	template <typename T>
	std::string to_string(const T& a, const int line_char_width = 80)
	{
		return to_string(a, whole::min(a), whole::max(a), line_char_width);
	}




}

