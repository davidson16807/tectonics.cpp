#pragma once

#include <cmath>

#include "types.hpp"

namespace series
{
	template <typename T, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value && !std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void add(const T& a, const T2 b, Tout& output)
	{
		output.store([](typename T::value_type ai, T2 bi){ return ai + bi; }, a, b); 
	}
	template <typename T, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value && !std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void sub(const T& a, const T2 b, Tout& output)
	{
		output.store([](typename T::value_type ai, T2 bi){ return ai - bi; }, a, b); 
	}
	template <typename T, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value && !std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void mult(const T& a, const T2 b, Tout& output)
	{
		output.store([](typename T::value_type ai, T2 bi){ return ai * bi; }, a, b); 
	}
	template <typename T, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value && !std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void div(const T& a, const T2 b, Tout& output)
	{
		const T2 binv = T2(1.)/b;
		output.store([](typename T::value_type ai, T2 bi){ return ai * bi; }, a, binv); 
	}


	template <typename T, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value && !std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void add(const T2 a, const T& b, Tout& output)
	{
		output.store([](T2 ai, typename T::value_type bi){ return ai + bi; }, a, b); 
	}
	template <typename T, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value && !std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void sub(const T2 a, const T& b, Tout& output)
	{
		output.store([](T2 ai, typename T::value_type bi){ return ai - bi; }, a, b); 
	}
	template <typename T, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value && !std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void mult(const T2 a, const T& b, Tout& output)
	{
		output.store([](T2 ai, typename T::value_type bi){ return ai * bi; }, a, b); 
	}
	template <typename T, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value && !std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void div(const T2 a, const T& b, Tout& output)
	{
		output.store([](T2 ai, typename T::value_type bi){ return ai / bi; }, a, b); 
	}



	// NOTE: we define operators for multiple typenames T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <typename T, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value && std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void add(const T& a, const T2& b, Tout& output)
	{
		output.store([](typename T::value_type ai, typename T2::value_type bi){ return ai + bi; }, a, b); 
	}
	template <typename T, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value && std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void sub(const T& a, const T2& b, Tout& output)
	{
		output.store([](typename T::value_type ai, typename T2::value_type bi){ return ai - bi; }, a, b); 
	}
	template <typename T, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value && std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void mult(const T& a, const T2& b, Tout& output)
	{
		output.store([](typename T::value_type ai, typename T2::value_type bi){ return ai * bi; }, a, b); 
	}
	template <typename T, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value && std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void div(const T& a, const T2& b, Tout& output)
	{
		output.store([](typename T::value_type ai, typename T2::value_type bi){ return ai / bi; }, a, b); 
	}
}