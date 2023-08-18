#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */

// in-house libraries
#include "each.hpp"

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



	template <typename In1, typename In2>
	double distance(const In1& a, const In2& b)
	{
		double out(0);
		assert(compatible(a,b));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out += each::distance(a[i], b[i]);
		}
		return out;
	}



	template <typename In1, typename In2>
	bool all(const In1& a)
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

	template <typename In1, typename In2>
	bool any(const In1& a)
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

	template <typename In1, typename In2>
	bool equal(const In1& a, const In2& b, const float threshold)
	{
		return whole::distance(a,b) < threshold;
	}

	template <typename In1, typename In2>
	bool equal(const In1& a, const In2& b)
	{
		if (a.size() != b.size()){
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

	template <typename In1, typename In2>
	bool notEqual(const In1& a, const In2& b)
	{
		if (a.size() != b.size()){
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

	template <typename In1, typename In2>
	bool greaterThan(const In1& a, const In2& b)
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

	template <typename In1, typename In2>
	bool lessThan(const In1& a, const In2& b)
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

	template <typename In1, typename In2>
	bool greaterThanEqual(const In1& a, const In2& b)
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

	template <typename In1, typename In2>
	bool lessThanEqual(const In1& a, const In2& b)
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



	#if defined(__clang__)
		const std::vector<const std::string> 
	#else
		const std::array<const std::string, 6>
	#endif
	shades {" ", "░", "▒", "▓", "█" };

	inline bool magnitude(const bool a){
		return a;
	}
	inline int magnitude(const int a){
		return a;
	}
	inline float magnitude(const float a){
		return a;
	}
	inline double magnitude(const double a){
		return a;
	}

	template<typename T>
	inline std::string character(const T a, const T lo, const T hi){
		if (std::isnan(a))
		{
			return "N";
		}
		else if (std::isinf(a))
		{
			return "∞";
		}
		else 
		{
			float shade_fraction = each::linearstep(lo, hi, a);
			int shade_id = int(std::min(float(shades.size()-1), (shades.size() * shade_fraction) ));
		    return shades[shade_id];
		}
	}
	template<>
	inline std::string character(const bool a, const bool lo, const bool hi){
		return a? "T":"F";
	}

	template<typename T>
	inline std::string legend(T sample, const T lo, const T hi){
		std::string out("");
		for (unsigned int i = 0; i < shades.size(); ++i)
		{
			auto bound = each::mix(float(lo), float(hi), float(i)/float(shades.size()));
			out += shades[i];
			out += " ≥ ";
			out += std::to_string(bound);
			out += "\n";
		}
		return out;
	}
	template<>
	inline std::string legend(bool sample, const bool lo, const bool hi){
		return std::string("");
	}


	template <typename Series, typename T>
	std::string to_string(const Series& a, const T lo, const T hi, const int line_char_width = 80)
	{
		std::string out("");
		using id = typename Series::size_type;
		for (id i = 0; i < a.size(); ++i)
		{
		    if (i % line_char_width == 0)
		    {
		    	out += "\n";
		    }
		    out += character(a[i],lo,hi);
		}
		out += "\n";
		out += legend(a[0], lo, hi);
		return out;
	}

	template <typename Series>
	std::string to_string(const Series& a, const int line_char_width = 80)
	{
		auto hi(magnitude(a[0]));
		auto lo(magnitude(a[0]));
		using id = typename Series::size_type;
		for (id i = 0; i < a.size(); ++i)
		{
			hi = std::max(hi, magnitude(a[i]));
			lo = std::min(lo, magnitude(a[i]));
		}
		return to_string(a, lo, hi, line_char_width);
	}




}

