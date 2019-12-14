#pragma once

#include <cmath>

#include "types.hpp"

namespace many
{
	template <class T, class T2, class T3>
	void add(const std::vector<T>& a, const T2 b, std::vector<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai + bi; }, out); 
	}
	template <class T, class T2, class T3>
	void sub(const std::vector<T>& a, const T2 b, std::vector<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai - bi; }, out); 
	}
	template <class T, class T2, class T3>
	void mult(const std::vector<T>& a, const T2 b, std::vector<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai * bi; }, out); 
	}
	template <class T, class T2, class T3>
	void div(const std::vector<T>& a, const T2 b, std::vector<T3>& out)
	{
		const T2 binv = T2(1.)/b;
		transform(a, binv, [](T ai, T2 bi){ return ai * bi; }, out); 
	}



	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <class T, class T2, class T3>
	void add(const std::vector<T>& a, const std::vector<T2>& b, std::vector<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai + bi; }, out); 
	}
	template <class T, class T2, class T3>
	void sub(const std::vector<T>& a, const std::vector<T2>& b, std::vector<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai - bi; }, out); 
	}
	template <class T, class T2, class T3>
	void mult(const std::vector<T>& a, const std::vector<T2>& b, std::vector<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai * bi; }, out); 
	}
	template <class T, class T2, class T3>
	void div(const std::vector<T>& a, const std::vector<T2>& b, std::vector<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai / bi; }, out); 
	}
	template <class T, class T2, class T3>
	void div(const T a, const std::vector<T2>& b, std::vector<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai / bi; }, out); 
	}
}