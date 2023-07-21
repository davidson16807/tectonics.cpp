#pragma once

// C libraries
#include <cmath>     /* assert */
#include <assert.h>  /* assert */

// std libraries
#include <initializer_list>	// initializer_list
#include <iterator>			// std::distance
#include <vector>           // std::vector
#include <memory>

#include "metric.hpp"
#include "compatibility.hpp"


namespace data
{


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
		return distance(a,b) < threshold;
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

	template <typename T1, typename T2, typename Tout>
	void equal(const T1& a, const T2& b, Tout& out, const float threshold)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = distance(a[i], b[i]) < threshold;
		}
	}

	template <typename T1, typename T2, typename Tout>
	void equal(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] == b[i];
		}
	}

	template <typename T1, typename T2, typename Tout>
	void notEqual(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] != b[i];
		}
	}

	template <typename T1, typename T2, typename Tout>
	void greaterThan(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] > b[i];
		}
	}

	template <typename T1, typename T2, typename Tout>
	void lessThan(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] < b[i];
		}
	}

	template <typename T1, typename T2, typename Tout>
	void greaterThanEqual(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] >= b[i];
		}
	}

	template <typename T1, typename T2, typename Tout>
	void lessThanEqual(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] <= b[i];
		}
	}

}

