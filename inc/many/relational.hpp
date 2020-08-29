#pragma once

#include "types.hpp"
#include "morphologic.hpp"

namespace many
{

	const float MANY_EPSILON = 1e-4;

	template <typename T>
	inline float length(const T a)
	{
		return a*a;
	}

	template <typename T>
	bool equal(const T& a, const typename T::value_type b, float threshold = MANY_EPSILON)
	{
		bool out(true);
		typename T::value_type diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out &= length(diff) <= threshold;
		}
		return out;
	}
	template <typename T>
	bool notEqual(const T& a, const typename T::value_type b, float threshold = MANY_EPSILON)
	{
		bool out(false);
		typename T::value_type diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out |= length(diff) > threshold;
		}
		return out;
	}
	template <typename T, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value, int> = 0>
	bool equal(const T& a, const T& b, float threshold = MANY_EPSILON)
	{
		if (a.size() != b.size())
		{
			return false;
		}
		bool out(true);
		typename T::value_type diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out &= length(diff) <= threshold;
		}
		return out;
	}
	template <typename T, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value, int> = 0>
	bool notEqual(const T& a, const T& b, float threshold = MANY_EPSILON)
	{
		if (a.size() != b.size())
		{
			return true;
		}
		bool out(false);
		typename T::value_type diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out |= length(diff) > threshold;
		}
		return out;
	}



	template <typename T, typename Tout>
	void equal(const T& a, const typename T::value_type b, Tout& out, float threshold = MANY_EPSILON)
	{
		typename T::value_type diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out[i] = length(diff) <= threshold;
		}
	}
	template <typename T, typename Tout>
	void notEqual(const T& a, const typename T::value_type b, Tout& out, float threshold = MANY_EPSILON)
	{
		typename T::value_type diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out[i] = length(diff) > threshold;
		}
	}
	template <typename T, typename Tout>
	void equal(const T& a, const T& b, Tout& out, float threshold = MANY_EPSILON)
	{
		typename T::value_type diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out[i] = length(diff) <= threshold;
		}
	}
	template <typename T, typename Tout>
	void notEqual(const T& a, const T& b, Tout& out, float threshold = MANY_EPSILON)
	{
		typename T::value_type diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out[i] = length(diff) > threshold;
		}
	}






	bool equal(const series<bool>& a, const bool b)
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i] == b;
		}
		return out;
	}
	bool notEqual(const series<bool>& a, const bool b)
	{
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i] != b;
		}
		return out;
	}
	bool equal(const series<bool>& a, const series<bool>& b)
	{
		if (a.size() != b.size())
		{
			return false;
		}
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i] == b[i];
		}
		return out;
	}
	bool notEqual(const series<bool>& a, const series<bool>& b)
	{
		if (a.size() != b.size())
		{
			return true;
		}
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i] != b[i];
		}
		return out;
	}



	void equal(const series<bool>& a, const bool b, series<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] == b;
		}
	}
	void notEqual(const series<bool>& a, const bool b, series<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] != b;
		}
	}
	void equal(const series<bool>& a, const series<bool>& b, series<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] == b[i];
		}
	}
	void notEqual(const series<bool>& a, const series<bool>& b, series<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] != b[i];
		}
	}










	template <typename T, typename T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void greaterThan(const series<T>& a, const T2 b, series<bool>& out)
	{
		out.store([](T ai, T2 bi){ return ai > bi; }, a, b); 
	}
	template <typename T, typename T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void greaterThanEqual(const series<T>& a, const T2 b, series<bool>& out)
	{
		out.store([](T ai, T2 bi){ return ai >= bi; }, a, b); 
	}
	template <typename T, typename T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void lessThan(const series<T>& a, const T2 b, series<bool>& out)
	{
		out.store([](T ai, T2 bi){ return ai < bi; }, a, b); 
	}
	template <typename T, typename T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void lessThanEqual(const series<T>& a, const T2 b, series<bool>& out)
	{
		out.store([](T ai, T2 bi){ return ai <= bi; }, a, b); 
	}



	template <typename T, typename T2>
	void greaterThan(const series<T>& a, const series<T2>& b, series<bool>& out)
	{
		out.store([](T ai, T2 bi){ return ai > bi; }, a, b); 
	}
	template <typename T, typename T2>
	void greaterThanEqual(const series<T>& a, const series<T2>& b, series<bool>& out)
	{
		out.store([](T ai, T2 bi){ return ai >= bi; }, a, b); 
	}
	template <typename T, typename T2>
	void lessThan(const series<T>& a, const series<T2>& b, series<bool>& out)
	{
		out.store([](T ai, T2 bi){ return ai <= bi; }, a, b); 
	}
	template <typename T, typename T2>
	void lessThanEqual(const series<T>& a, const series<T2>& b, series<bool>& out)
	{
		out.store([](T ai, T2 bi){ return ai < bi; }, a, b); 
	}

	
	bool all(const series<bool>& a)
	{
		bool out = true;
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i];
		}
		return out;
	}
	bool any(const series<bool>& a)
	{
		bool out = false;
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i];
		}
		return out;
	}
	bool none(const series<bool>& a)
	{
		bool out = false;
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i];
		}
		return !out;
	}

}
