#pragma once

#include <cmath>

#include <many/types.hpp>
#include "Layers.hpp"
#include "convenience.hpp"

/*
"convenience.hpp" contains functions that pass output as return values rather than using using output parameters
This provides convenience at the expense of performance, since now we have to call constructors for the new objects. 
See https://codeyarns.com/2010/10/21/c-return-value-versus-output-parameter/ for more info.
It is important to keep these functions separate from the rest of the library for two reasons:
 1.) It encourages good practice, since you have to explicitly opt-in to less performant convenience functions.
 2.) It provides a nice itemization of functions that will have to be created if you subclass Layers<T,N> 
*/

namespace layers
{


	// NOTE: all operators are suggested to be inline because they are thin wrappers of functions
	template <std::size_t N, class T>
	inline bool operator==(const Layers<T,N>& a, const T b)
	{
		return layers::equal(a, b);
	}
	template <std::size_t N, class T>
	inline bool operator!=(const Layers<T,N>& a, const T b)
	{
		return layers::notEqual(a, b);
	}
	template <std::size_t N, class T>
	inline bool operator==(const T a, const Layers<T,N>& b)
	{
		return layers::equal(a, b);
	}
	template <std::size_t N, class T>
	inline bool operator!=(const T a, const Layers<T,N>& b)
	{
		return layers::notEqual(a, b);
	}
	template <std::size_t N, class T>
	inline bool operator==(const Layers<T,N>& a, const Layers<T,N>& b)
	{
		return layers::equal(a, b);
	}
	template <std::size_t N, class T>
	inline bool operator!=(const Layers<T,N>& a, const Layers<T,N>& b)
	{
		return layers::notEqual(a, b);
	}
	

	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T3,N> operator>(const Layers<T,N>& a, const T2 b)
	{
		return layers::greaterThan(a, b);
	}
	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T3,N> operator>=(const Layers<T,N>& a, const T2 b)
	{
		return layers::greaterThanEqual(a, b);
	}
	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T3,N> operator<(const Layers<T,N>& a, const T2 b)
	{
		return layers::lessThan(a, b);
	}
	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T3,N> operator<=(const Layers<T,N>& a, const T2 b)
	{
		return layers::lessThanEqual(a, b);
	}
	
	// NOTE: all wrappers are suggested to be inline because they are thin wrappers of functions

	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T3,N> operator>(const T2 a, const Layers<T,N>& b)
	{
		return layers::greaterThan(a, b);
	}
	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T3,N> operator>=(const T2 a, const Layers<T,N>& b)
	{
		return layers::greaterThanEqual(a, b);
	}
	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T3,N> operator<(const T2 a, const Layers<T,N>& b)
	{
		return layers::lessThan(a, b);
	}
	template <std::size_t N, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T3,N> operator<=(const T2 a, const Layers<T,N>& b)
	{
		return layers::lessThanEqual(a, b);
	}





	template <std::size_t N, class T>
	inline Layers<T,N>& operator+=(Layers<T,N>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] += b;
		}
		return a;
	}
	template <std::size_t N, class T>
	inline Layers<T,N>& operator-=(Layers<T,N>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] -= b;
		}
		return a;
	}
	template <std::size_t N, class T>
	inline Layers<T,N>& operator*=(Layers<T,N>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] *= b;
		}
		return a;
	}
	template <std::size_t N, class T>
	inline Layers<T,N>& operator/=(Layers<T,N>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] /= b;
		}
		return a;
	}


	template <std::size_t N, class T, class T2>
	inline Layers<T,N>& operator+=(Layers<T,N>& a, const Layers<T2,N>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] += b[i];
		}
		return a;
	}
	template <std::size_t N, class T, class T2>
	inline Layers<T,N>& operator-=(Layers<T,N>& a, const Layers<T2,N>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] -= b[i];
		}
		return a;
	}
	template <std::size_t N, class T, class T2>
	inline Layers<T,N>& operator*=(Layers<T,N>& a, const Layers<T2,N>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] *= b[i];
		}
		return a;
	}
	template <std::size_t N, class T, class T2>
	inline Layers<T,N>& operator/=(Layers<T,N>& a, const Layers<T2,N>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] /= b[i];
		}
		return a;
	}


	
	

	// NOTE: prefix increment/decrement
	template <std::size_t N, class T>
	inline Layers<T,N>& operator++(Layers<T,N>& a)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]++;
		}
		return a;
	}  
	template <std::size_t N, class T>
	inline Layers<T,N>& operator--(Layers<T,N>& a)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]--;
		}
		return a;
	}  

	// NOTE: postfix increment/decrement
	template <std::size_t N, class T>
	inline Layers<T,N> operator++(Layers<T,N>& a, int)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]++;
		}
		return a;
	}  
	template <std::size_t N, class T>
	inline Layers<T,N> operator--(Layers<T,N>& a, int)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]--;
		}
		return a;
	}  
	




	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <std::size_t N, class T, class T2, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T,N> operator+(const Layers<T,N>& a, const T2 b)
	{
		return layers::transform(a, b, [](T ai, T2 bi){ return ai + bi; });
	}
	template <std::size_t N, class T, class T2, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T,N> operator-(const Layers<T,N>& a, const T2 b)
	{
		return layers::transform(a, b, [](T ai, T2 bi){ return ai - bi; });
	}
	template <std::size_t N, class T, class T2, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T,N> operator*(const Layers<T,N>& a, const T2 b)
	{
		return layers::transform(a, b, [](T ai, T2 bi){ return ai * bi; });
	}
	template <std::size_t N, class T, class T2, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T,N> operator/(const Layers<T,N>& a, const T2 b)
	{
		return layers::transform(a, b, [](T ai, T2 bi){ return ai / bi; });
	}
	




	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <std::size_t N, class T, class T2, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T,N> operator+(const T2 a, const Layers<T,N>& b)
	{
		return layers::transform(a, b, [](T ai, T2 bi){ return ai + bi; });
	}
	template <std::size_t N, class T, class T2, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T,N> operator-(const T2 a, const Layers<T,N>& b)
	{
		return layers::transform(a, b, [](T ai, T2 bi){ return ai - bi; });
	}
	template <std::size_t N, class T, class T2, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T,N> operator*(const T2 a, const Layers<T,N>& b)
	{
		return layers::transform(a, b, [](T ai, T2 bi){ return ai * bi; });
	}
	template <std::size_t N, class T, class T2, std::enable_if_t<!std::is_base_of<layers::AbstractLayers, T2>::value, int> = 0>
	inline Layers<T,N> operator/(const T2 a, const Layers<T,N>& b)
	{
		return layers::transform(a, b, [](T ai, T2 bi){ return ai / bi; });
	}


	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <std::size_t N, class T, class T2>
	inline Layers<T,N> operator+(const Layers<T,N>& a, const Layers<T2,N>& b)
	{
		return layers::transform(a, b, [](T ai, T2 bi){ return ai + bi; });
	}
	template <std::size_t N, class T, class T2>
	inline Layers<T,N> operator-(const Layers<T,N>& a, const Layers<T2,N>& b)
	{
		return layers::transform(a, b, [](T ai, T2 bi){ return ai - bi; });
	}
	template <std::size_t N, class T, class T2>
	inline Layers<T,N> operator*(const Layers<T,N>& a, const Layers<T2,N>& b)
	{
		return layers::transform(a, b, [](T ai, T2 bi){ return ai * bi; });
	}
	template <std::size_t N, class T, class T2>
	inline Layers<T,N> operator/(const Layers<T,N>& a, const Layers<T2,N>& b)
	{
		return layers::transform(a, b, [](T ai, T2 bi){ return ai / bi; });
	}


	template <std::size_t N>
	inline Layers<bool,N> operator~(const Layers<bool,N>& a)
	{
		Layers<bool,N> out = Layers<bool,N>(a.size());
		layers::transform(a, [](bool ai){ return !ai; }, out);
		return out;
	}


	template <std::size_t N>
	inline Layers<bool,N> operator|(const Layers<bool,N>& a, const bool b)
	{
		return layers::transform(a, b, [](bool ai, bool bi){ return ai || bi; });
	}
	template <std::size_t N>
	inline Layers<bool,N> operator&(const Layers<bool,N>& a, const bool b)
	{
		return layers::transform(a, b, [](bool ai, bool bi){ return ai && bi; });
	}

	template <std::size_t N>
	inline Layers<bool,N> operator|(const Layers<bool,N>& a, const Layers<bool,N>& b)
	{
		return layers::transform(a, b, [](bool ai, bool bi){ return ai || bi; });
	}
	template <std::size_t N>
	inline Layers<bool,N> operator&(const Layers<bool,N>& a, const Layers<bool,N>& b)
	{
		return layers::transform(a, b, [](bool ai, bool bi){ return ai && bi; });
	}




	template <std::size_t N>
	inline Layers<bool,N>& operator|=(Layers<bool,N>& a, const bool b){
		layers::transform(a, b, [](bool ai, bool bi){ return ai || bi; }, a);
		return a;
	}
	template <std::size_t N>
	inline Layers<bool,N>& operator&=(Layers<bool,N>& a, const bool b){
		layers::transform(a, b, [](bool ai, bool bi){ return ai &&  bi; }, a);
		return a;
	}

	template <std::size_t N>
	inline Layers<bool,N>& operator|=(Layers<bool,N>& a, const Layers<bool,N>& b){
		layers::transform(a, b, [](bool ai, bool bi){ return ai || bi; }, a);
		return a;
	}
	template <std::size_t N>
	inline Layers<bool,N>& operator&=(Layers<bool,N>& a, const Layers<bool,N>& b){
		layers::transform(a, b, [](bool ai, bool bi){ return ai &&  bi; }, a);
		return a;
	}
}