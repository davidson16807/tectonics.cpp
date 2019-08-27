#pragma once

#include "composite.hpp"

namespace many
{

	const float MANY_EPSILON = 1e-4;

	template <class T>
	bool equal(const composite<T>& a, const T b)
	{
		bool out(true);
		T diff(0);
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out &= diff*diff <= threshold;
		}
		return out;
	}
	template <class T>
	bool notEqual(const composite<T>& a, const T b)
	{
		bool out(false);
		T diff(0);
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out |= diff*diff > threshold;
		}
		return out;
	}
	template <class T>
	bool equal(const composite<T>& a, const composite<T>& b)
	{
		if (a.size() != b.size())
		{
			return false;
		}
		bool out(true);
		T diff(0);
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out &= diff*diff <= threshold;
		}
		return out;
	}
	template <class T>
	bool notEqual(const composite<T>& a, const composite<T>& b)
	{
		if (a.size() != b.size())
		{
			return true;
		}
		bool out(false);
		T diff(0);
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out |= diff*diff > threshold;
		}
		return out;
	}



	template <class T>
	void equal(const composite<T>& a, const T b, composite<bool>& out)
	{
		T diff(0);
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out[i] = diff*diff <= threshold;
		}
	}
	template <class T>
	void notEqual(const composite<T>& a, const T b, composite<bool>& out)
	{
		T diff(0);
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out[i] = diff*diff > threshold;
		}
	}
	template <class T>
	void equal(const composite<T>& a, const composite<T>& b, composite<bool>& out)
	{
		T diff(0);
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out[i] = diff*diff <= threshold;
		}
	}
	template <class T>
	void notEqual(const composite<T>& a, const composite<T>& b, composite<bool>& out)
	{
		T diff(0);
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out[i] = diff*diff > threshold;
		}
	}

	template <class T, class T2>
	void greaterThan(const composite<T>& a, const T2 b, composite<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai > bi; }, out); 
	}
	template <class T, class T2>
	void greaterThanEqual(const composite<T>& a, const T2 b, composite<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai >= bi; }, out); 
	}
	template <class T, class T2>
	void lessThan(const composite<T>& a, const T2 b, composite<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai < bi; }, out); 
	}
	template <class T, class T2>
	void lessThanEqual(const composite<T>& a, const T2 b, composite<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai <= bi; }, out); 
	}



	template <class T, class T2>
	void greaterThan(const composite<T>& a, const composite<T2>& b, composite<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai > bi; }, out); 
	}
	template <class T, class T2>
	void greaterThanEqual(const composite<T>& a, const composite<T2>& b, composite<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai >= bi; }, out); 
	}
	template <class T, class T2>
	void lessThan(const composite<T>& a, const composite<T2>& b, composite<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai <= bi; }, out); 
	}
	template <class T, class T2>
	void lessThanEqual(const composite<T>& a, const composite<T2>& b, composite<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai < bi; }, out); 
	}
}
