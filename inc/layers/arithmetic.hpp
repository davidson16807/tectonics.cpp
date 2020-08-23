#pragma once

#include <cmath>

#include "Layers.hpp"

namespace layers
{
	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	void add(const Layers<T,N>& a, const T2 b, Layers<T3,N>& out)
	{
		layers::transform(a, b, [](T ai, T2 bi){ return ai + bi; }, out); 
	}
	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	void sub(const Layers<T,N>& a, const T2 b, Layers<T3,N>& out)
	{
		layers::transform(a, b, [](T ai, T2 bi){ return ai - bi; }, out); 
	}
	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	void mult(const Layers<T,N>& a, const T2 b, Layers<T3,N>& out)
	{
		layers::transform(a, b, [](T ai, T2 bi){ return ai * bi; }, out); 
	}
	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	void div(const Layers<T,N>& a, const T2 b, Layers<T3,N>& out)
	{
		const T2 binv = T2(1.)/b;
		layers::transform(a, binv, [](T ai, T2 bi){ return ai * bi; }, out); 
	}


	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	void add(const T2 a, const Layers<T,N>& b, Layers<T3,N>& out)
	{
		layers::transform(a, b, [](T ai, T2 bi){ return ai + bi; }, out); 
	}
	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	void sub(const T2 a, const Layers<T,N>& b, Layers<T3,N>& out)
	{
		layers::transform(a, b, [](T ai, T2 bi){ return ai - bi; }, out); 
	}
	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	void mult(const T2 a, const Layers<T,N>& b, Layers<T3,N>& out)
	{
		layers::transform(a, b, [](T ai, T2 bi){ return ai * bi; }, out); 
	}
	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	void div(const T2 a, const Layers<T,N>& b, Layers<T3,N>& out)
	{
		layers::transform(a, b, [](T ai, T2 bi){ return ai / bi; }, out); 
	}



	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <std::size_t N, class T, class T2, class T3>
	void add(const Layers<T,N>& a, const Layers<T2,N>& b, Layers<T3,N>& out)
	{
		layers::transform(a, b, [](T ai, T2 bi){ return ai + bi; }, out); 
	}
	template <std::size_t N, class T, class T2, class T3>
	void sub(const Layers<T,N>& a, const Layers<T2,N>& b, Layers<T3,N>& out)
	{
		layers::transform(a, b, [](T ai, T2 bi){ return ai - bi; }, out); 
	}
	template <std::size_t N, class T, class T2, class T3>
	void mult(const Layers<T,N>& a, const Layers<T2,N>& b, Layers<T3,N>& out)
	{
		layers::transform(a, b, [](T ai, T2 bi){ return ai * bi; }, out); 
	}
	template <std::size_t N, class T, class T2, class T3>
	void div(const Layers<T,N>& a, const Layers<T2,N>& b, Layers<T3,N>& out)
	{
		layers::transform(a, b, [](T ai, T2 bi){ return ai / bi; }, out); 
	}
}