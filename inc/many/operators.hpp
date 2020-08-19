#pragma once

#include <cmath>

#include "types.hpp"
#include "relational.hpp"
#include "common.hpp"
#include "convenience.hpp"

/*
"convenience.hpp" contains functions that pass output as return values rather than using using output parameters
This provides convenience at the expense of performance, since now we have to call constructors for the new objects. 
See https://codeyarns.com/2010/10/21/c-return-value-versus-output-parameter/ for more info.
It is important to keep these functions separate from the rest of the library for two reasons:
 1.) It encourages good practice, since you have to explicitly opt-in to less performant convenience functions.
 2.) It provides a nice itemization of functions that will have to be created if you subclass series<T> 
*/

namespace many
{

	template <class T>
	std::ostream &operator<<(std::ostream &os, const series<T>& a) { 
		os << "[";
		for (unsigned int i = 0; i < a.size(); ++i)
		{
		    os << a[i] << " ";
		}
		os << "]";
		return os;
	}


	// NOTE: all operators are suggested to be inline because they are thin wrappers of functions
	template <class T>
	inline bool operator==(const series<T>& a, const T b)
	{
		return many::equal(a, b);
	}
	template <class T>
	inline bool operator!=(const series<T>& a, const T b)
	{
		return many::notEqual(a, b);
	}
	template <class T>
	inline bool operator==(const T a, const series<T>& b)
	{
		return many::equal(a, b);
	}
	template <class T>
	inline bool operator!=(const T a, const series<T>& b)
	{
		return many::notEqual(a, b);
	}
	template <class T>
	inline bool operator==(const series<T>& a, const series<T>& b)
	{
		return many::equal(a, b);
	}
	template <class T>
	inline bool operator!=(const series<T>& a, const series<T>& b)
	{
		return many::notEqual(a, b);
	}
	

	template <class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T3> operator>(const series<T>& a, const T2 b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai > bi; });
	}
	template <class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T3> operator>=(const series<T>& a, const T2 b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai >= bi; });
	}
	template <class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T3> operator<(const series<T>& a, const T2 b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai < bi; });
	}
	template <class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T3> operator<=(const series<T>& a, const T2 b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai <= bi; });
	}
	
	// NOTE: all wrappers are suggested to be inline because they are thin wrappers of functions

	template <class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T3> operator>(const T2 a, const series<T>& b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai > bi; });
	}
	template <class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T3> operator>=(const T2 a, const series<T>& b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai >= bi; });
	}
	template <class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T3> operator<(const T2 a, const series<T>& b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai < bi; });
	}
	template <class T, class T2, class T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T3> operator<=(const T2 a, const series<T>& b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai <= bi; });
	}





	template <class T>
	inline series<T>& operator+=(series<T>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] += b;
		}
		return a;
	}
	template <class T>
	inline series<T>& operator-=(series<T>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] -= b;
		}
		return a;
	}
	template <class T>
	inline series<T>& operator*=(series<T>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] *= b;
		}
		return a;
	}
	template <class T>
	inline series<T>& operator/=(series<T>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] /= b;
		}
		return a;
	}


	template <class T, class T2>
	inline series<T>& operator+=(series<T>& a, const series<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] += b[i];
		}
		return a;
	}
	template <class T, class T2>
	inline series<T>& operator-=(series<T>& a, const series<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] -= b[i];
		}
		return a;
	}
	template <class T, class T2>
	inline series<T>& operator*=(series<T>& a, const series<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] *= b[i];
		}
		return a;
	}
	template <class T, class T2>
	inline series<T>& operator/=(series<T>& a, const series<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] /= b[i];
		}
		return a;
	}

	// NOTE: prefix increment/decrement
	template <class T>
	inline series<T>& operator++(series<T>& a)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]++;
		}
		return a;
	}  
	template <class T>
	inline series<T>& operator--(series<T>& a)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]--;
		}
		return a;
	}  

	// NOTE: postfix increment/decrement
	template <class T>
	inline series<T> operator++(series<T>& a, int)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]++;
		}
		return a;
	}  
	template <class T>
	inline series<T> operator--(series<T>& a, int)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]--;
		}
		return a;
	}  
	




	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <class T, class T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T> operator+(const series<T>& a, const T2 b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai + bi; });
	}
	template <class T, class T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T> operator-(const series<T>& a, const T2 b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai - bi; });
	}
	template <class T, class T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T> operator*(const series<T>& a, const T2 b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai * bi; });
	}
	template <class T, class T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T> operator/(const series<T>& a, const T2 b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai / bi; });
	}
	




	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <class T, class T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T> operator+(const T2 a, const series<T>& b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai + bi; });
	}
	template <class T, class T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T> operator-(const T2 a, const series<T>& b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai - bi; });
	}
	template <class T, class T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T> operator*(const T2 a, const series<T>& b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai * bi; });
	}
	template <class T, class T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline series<T> operator/(const T2 a, const series<T>& b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai / bi; });
	}


	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <class T, class T2>
	inline series<T> operator+(const series<T>& a, const series<T2>& b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai + bi; });
	}
	template <class T, class T2>
	inline series<T> operator-(const series<T>& a, const series<T2>& b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai - bi; });
	}
	template <class T, class T2>
	inline series<T> operator*(const series<T>& a, const series<T2>& b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai * bi; });
	}
	template <class T, class T2>
	inline series<T> operator/(const series<T>& a, const series<T2>& b)
	{
		return many::transform(a, b, [](T ai, T2 bi){ return ai / bi; });
	}


	inline series<bool> operator~(const series<bool>& a)
	{
		series<bool> out = series<bool>(a.size());
		many::transform(a, [](bool ai){ return !ai; }, out);
		return out;
	}


	inline series<bool> operator|(const series<bool>& a, const bool b)
	{
		return many::transform(a, b, [](bool ai, bool bi){ return ai || bi; });
	}
	inline series<bool> operator&(const series<bool>& a, const bool b)
	{
		return many::transform(a, b, [](bool ai, bool bi){ return ai && bi; });
	}

	inline series<bool> operator|(const series<bool>& a, const series<bool>& b)
	{
		return many::transform(a, b, [](bool ai, bool bi){ return ai || bi; });
	}
	inline series<bool> operator&(const series<bool>& a, const series<bool>& b)
	{
		return many::transform(a, b, [](bool ai, bool bi){ return ai && bi; });
	}




	inline series<bool>& operator|=(series<bool>& a, const bool b){
		many::transform(a, b, [](bool ai, bool bi){ return ai || bi; }, a);
		return a;
	}
	inline series<bool>& operator&=(series<bool>& a, const bool b){
		many::transform(a, b, [](bool ai, bool bi){ return ai &&  bi; }, a);
		return a;
	}

	inline series<bool>& operator|=(series<bool>& a, const series<bool>& b){
		many::transform(a, b, [](bool ai, bool bi){ return ai || bi; }, a);
		return a;
	}
	inline series<bool>& operator&=(series<bool>& a, const series<bool>& b){
		many::transform(a, b, [](bool ai, bool bi){ return ai &&  bi; }, a);
		return a;
	}
}