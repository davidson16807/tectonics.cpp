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
	bool equal(const T1& a, const T2& b, const float threshold)
	{
		return distance(a,b) < threshold;
	}

	template <typename T1, typename T2>
	bool equal(const T1& a, const T2& b)
	{
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
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i] == b[i]){
				return false;
			}
		}
		return true;
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

}

