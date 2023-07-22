#pragma once

#include <cmath>

#include "types.hpp"
#include "relational.hpp"
#include "common.hpp"

/*
"convenience.hpp" contains functions that pass output as return values rather than using using output parameters
This provides convenience at the expense of performance, since now we have to call constructors for the new objects. 
See https://codeyarns.com/2010/10/21/c-return-value-versus-output-parameter/ for more info.
It is important to keep these functions separate from the rest of the library for two reasons:
 1.) It encourages good practice, since you have to explicitly opt-in to less performant convenience functions.
 2.) It provides a nice itemization of functions that will have to be created if you subclass Series<T> 
*/

namespace series
{
	template <typename T1, typename F>
	inline Series<T1> transform(const F f, const Series<T1>& a)
	{
		Series<T1> out(a.size());
		out.store(f, a); 
		return out;
	}


	template <typename T1, typename T2, typename F>
	inline Series<T1> transform(const F f, const Series<T1>& a, const Series<T2>& b)
	{
		Series<T1> out(a.size());
		out.store(f, a, b); 
		return out;
	}
	template <typename T1, typename T2, typename F, 
		std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T1> transform(const F f, const Series<T1>& a, const T2 b)
	{
		Series<T1> out(a.size());
		out.store(f, a, b); 
		return out;
	}
	template <typename T1, typename T2, typename F, 
		std::enable_if_t<!std::is_base_of<AbstractSeries, T1>::value, int> = 0>
	inline Series<T1> transform(const F f, const T1 a, const Series<T2>& b)
	{
		Series<T1> out(b.size());
		out.store(f, a, b); 
		return out;
	}




	template <typename T1, typename T2, typename T3, typename F, 
		std::enable_if_t<!std::is_base_of<AbstractSeries, F>::value, int> = 0>
	inline Series<T1> transform(const F f, const Series<T1>& a, const Series<T2>& b, const Series<T3>& c)
	{
		Series<T1> out(a.size());
		out.store(f, a, b, c); 
		return out;
	}
	template <typename T1, typename T2, typename T3, typename F, 
		std::enable_if_t<!std::is_base_of<AbstractSeries, T3>::value && !std::is_base_of<AbstractSeries, F>::value, int> = 0>
	inline Series<T1> transform(const F f, const Series<T1>& a, const Series<T2>& b, const T3 c)
	{
		Series<T1> out(a.size());
		out.store(f, a, b, c); 
		return out;
	}
	template <typename T1, typename T2, typename T3, typename F, 
		std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value && !std::is_base_of<AbstractSeries, F>::value, int> = 0>
	inline Series<T1> transform(const F f, const Series<T1>& a, const T2 b, const Series<T3>& c)
	{
		Series<T1> out(a.size());
		out.store(f, a, b, c); 
		return out;
	}
	template <typename T1, typename T2, typename T3, typename F,
		std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value && !std::is_base_of<AbstractSeries, T3>::value && !std::is_base_of<AbstractSeries, F>::value, int> = 0>
	inline Series<T1> transform(const F f, const Series<T1>& a, const T2 b, const T3 c)
	{
		Series<T1> out(a.size());
		out.store(f, a, b, c); 
		return out;
	}
	template <typename T1, typename T2, typename T3, typename F, 
		std::enable_if_t<!std::is_base_of<AbstractSeries, T1>::value && !std::is_base_of<AbstractSeries, F>::value, int> = 0>
	inline Series<T1> transform(const F f, const T1 a, const Series<T2>& b, const Series<T3>& c)
	{
		Series<T1> out(a.size());
		out.store(f, a, b, c); 
		return out;
	}
	template <typename T1, typename T2, typename T3, typename F,
		std::enable_if_t<!std::is_base_of<AbstractSeries, T1>::value && !std::is_base_of<AbstractSeries, T3>::value && !std::is_base_of<AbstractSeries, F>::value, int> = 0>
	inline Series<T1> transform(const F f, const T1 a, const Series<T2>& b, const T3 c)
	{
		Series<T1> out(a.size());
		out.store(f, a, b, c); 
		return out;
	}
	template <typename T1, typename T2, typename T3, typename F,
		std::enable_if_t<!std::is_base_of<AbstractSeries, T1>::value && !std::is_base_of<AbstractSeries, T2>::value && !std::is_base_of<AbstractSeries, F>::value, int> = 0>
	inline Series<T1> transform(const F f, const T1 a, const T2 b, const Series<T3>& c)
	{
		Series<T1> out(c.size());
		out.store(f, a, b, c); 
		return out;
	}





	template<typename T, typename Taggregator>
	Series<T> aggregate(const Series<T>& a, const Series<unsigned int>& group_ids, Taggregator aggregator)
	{
		Series<T> group_out = Series<T>(max(group_ids));
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]], a[i]);
		}
		return group_out;
	}

	template<typename T, typename Taggregator>
	Series<T> aggregate(const Series<unsigned int>& group_ids, Taggregator aggregator)
	{
		Series<T> group_out = Series<T>(max(group_ids));
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]]);
		}
		return group_out;
	}


	/// Returns x if x >= 0; otherwise, it returns -x.
	template <typename T>
	Series<T> abs(const Series<T>& a)
	{
		return series::transform([](T ai){ return ai >= 0? ai : -ai; }, a);
	}

	/// Returns 1.0 if x > 0, 0.0 if x == 0, or -1.0 if x < 0.
	template <typename T>
	Series<T> sign(const Series<T>& a)
	{
		return series::transform([](T ai){ return (T(0) < ai) - (ai < T(0)); }, a);
	}

	/// Returns a value equal to the nearest integer that is less then or equal to x.
	template <typename T>
	Series<T> floor(const Series<T>& a)
	{
		return series::transform(std::floor, a);
	}

	/// Returns a value equal to the nearest integer to x
	/// whose absolute value is not larger than the absolute value of x.
	template <typename T>
	Series<T> trunc(const Series<T>& a)
	{
		return series::transform(std::trunc, a);
	}

	/// Returns a value equal to the nearest integer to x.
	/// The fraction 0.5 will round in a direction chosen by the
	/// implementation, presumably the direction that is fastest.
	/// This includes the possibility that round(x) returns the
	/// same value as roundEven(x) for all values of x.
	template <typename T>
	Series<T> round(const Series<T>& a)
	{
		return series::transform(std::round, a);
	}

	/// Returns a value equal to the nearest integer
	/// that is greater than or equal to x.
	template <typename T>
	Series<T> ceil(const Series<T>& a)
	{
		series::transform(std::ceil, a);
	}


}