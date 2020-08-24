#pragma once

#include <cmath>

#include "types.hpp"

namespace many
{
	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void add(const series<T>& a, const T2 b, series<T3>& out)
	{
		out.store([](T ai, T2 bi){ return ai + bi; }, a, b); 
	}
	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void sub(const series<T>& a, const T2 b, series<T3>& out)
	{
		out.store([](T ai, T2 bi){ return ai - bi; }, a, b); 
	}
	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void mult(const series<T>& a, const T2 b, series<T3>& out)
	{
		out.store([](T ai, T2 bi){ return ai * bi; }, a, b); 
	}
	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void div(const series<T>& a, const T2 b, series<T3>& out)
	{
		const T2 binv = T2(1.)/b;
		out.store([](T ai, T2 bi){ return ai * bi; }, a, binv); 
	}


	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void add(const T2 a, const series<T>& b, series<T3>& out)
	{
		out.store([](T ai, T2 bi){ return ai + bi; }, a, b); 
	}
	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void sub(const T2 a, const series<T>& b, series<T3>& out)
	{
		out.store([](T ai, T2 bi){ return ai - bi; }, a, b); 
	}
	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void mult(const T2 a, const series<T>& b, series<T3>& out)
	{
		out.store([](T ai, T2 bi){ return ai * bi; }, a, b); 
	}
	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void div(const T2 a, const series<T>& b, series<T3>& out)
	{
		out.store([](T ai, T2 bi){ return ai / bi; }, a, b); 
	}



	// NOTE: we define operators for multiple typenames T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <typename T, typename T2, typename T3>
	void add(const series<T>& a, const series<T2>& b, series<T3>& out)
	{
		out.store([](T ai, T2 bi){ return ai + bi; }, a, b); 
	}
	template <typename T, typename T2, typename T3>
	void sub(const series<T>& a, const series<T2>& b, series<T3>& out)
	{
		out.store([](T ai, T2 bi){ return ai - bi; }, a, b); 
	}
	template <typename T, typename T2, typename T3>
	void mult(const series<T>& a, const series<T2>& b, series<T3>& out)
	{
		out.store([](T ai, T2 bi){ return ai * bi; }, a, b); 
	}
	template <typename T, typename T2, typename T3>
	void div(const series<T>& a, const series<T2>& b, series<T3>& out)
	{
		out.store([](T ai, T2 bi){ return ai / bi; }, a, b); 
	}
}