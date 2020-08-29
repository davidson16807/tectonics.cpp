#pragma once

// C libraries
#include <cmath>

// in-house libraries
#include "types.hpp"
#include "morphologic.hpp"
#include "common.hpp"

namespace many
{

	const float MANY_EPSILON = 1e-4;
	
	/*
	NOTE: 
	We want a function that can be used to easily and performantly provide 
	a distance metric for objects of any data type.
	It should be publically available so that API users can test equality
	for many::series<T> that contain their own custom data types.
	As a result, it should make sense as a publically available function.
	However to prevent introducing additional hard requirements on the design,
	it should be given a name that makes it unlikely for the compiler to 
	find unintentionally similar functions from other namespaces.
	We choose "length2", mirroring the "length2" function from glm's experimental functionality.
	It represents the square of the magnitude for any scalar or vector,
	and its meaning can be extended to data types of arbitrary complexity.
	Any function with the name "length2" need only guarantee that length2(a-a) == 0 for any input a.
	*/
	template <typename T,
		std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
	float length2(const T a)
	{
		return a*a;
	}
	template <typename T,
		std::enable_if_t<std::is_arithmetic<typename T::value_type>::value, int> = 0>
	float length2(const T a)
	{
		float output(0);
		for (std::size_t i = 0; i < a.length(); ++i)
		{
			output += a[i]*a[i];
		}
		return output;
	}

	template <typename T,
		std::enable_if_t<!std::is_base_of<AbstractSeries, T>::value, int> = 0>
	inline bool equal(const T a, const T b, float threshold = MANY_EPSILON)
	{
		return length2(a-b) <= threshold;
	}

	template <typename T,
		std::enable_if_t<!std::is_base_of<AbstractSeries, T>::value, int> = 0>
	inline bool notEqual(const T a, const T b, float threshold = MANY_EPSILON)
	{
		return length2(a-b) > threshold;
	}

	template <typename T>
	bool equal(const T& a, const typename T::value_type b, float threshold = MANY_EPSILON)
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= equal( a[i], b, threshold);
		}
		return out;
	}
	template <typename T>
	bool notEqual(const T& a, const typename T::value_type b, float threshold = MANY_EPSILON)
	{
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= notEqual( a[i] , b, threshold);
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
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= equal( a[i], b[i], threshold);
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
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= notEqual( a[i] , b[i], threshold);
		}
		return out;
	}



	template <typename T, typename Tout>
	void equal(const T& a, const typename T::value_type b, Tout& out, float threshold = MANY_EPSILON)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = equal( a[i], b, threshold);
		}
	}
	template <typename T, typename Tout>
	void notEqual(const T& a, const typename T::value_type b, Tout& out, float threshold = MANY_EPSILON)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = notEqual( a[i] , b, threshold);
		}
	}
	template <typename T, typename Tout>
	void equal(const T& a, const T& b, Tout& out, float threshold = MANY_EPSILON)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = equal( a[i], b[i], threshold);
		}
	}
	template <typename T, typename Tout>
	void notEqual(const T& a, const T& b, Tout& out, float threshold = MANY_EPSILON)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = notEqual( a[i] , b[i], threshold);
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
