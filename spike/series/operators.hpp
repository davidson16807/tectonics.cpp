#pragma once

#include <cmath>

#include "types.hpp"
#include "relational.hpp"
#include "arithmetic.hpp"
#include "common.hpp"
#include "convenience.hpp"

/*
"convenience.hpp" contains functions that pass output as return values rather than using using output parameters
This provides convenience at the expense of performance, since now we have to call constructors for the new objects. 
See https://codeyarns.com/2010/10/21/c-return-value-versus-output-parameter/ for more info.
It is important to keep these functions separate from the rest of the library for two reasons:
 1.) It encourages good practice, since you have to explicitly opt-in to less performant convenience functions.
 2.) It provides a nice itemization of functions that will have to be created if you subclass Series<T> 
*/

namespace series
{

	template <typename T>
	std::ostream &operator<<(std::ostream &os, const Series<T>& a) { 
		os << "[";
		for (unsigned int i = 0; i < a.size(); ++i)
		{
		    os << a[i] << " ";
		}
		os << "]";
		return os;
	}


	// NOTE: all operators are suggested to be inline because they are thin wrappers of functions
	template <typename T>
	inline bool operator==(const Series<T>& a, const T b)
	{
		return series::equal(a, b);
	}
	template <typename T>
	inline bool operator!=(const Series<T>& a, const T b)
	{
		return series::notEqual(a, b);
	}
	template <typename T>
	inline bool operator==(const T a, const Series<T>& b)
	{
		return series::equal(a, b);
	}
	template <typename T>
	inline bool operator!=(const T a, const Series<T>& b)
	{
		return series::notEqual(a, b);
	}
	template <typename T>
	inline bool operator==(const Series<T>& a, const Series<T>& b)
	{
		return series::equal(a, b);
	}
	template <typename T>
	inline bool operator!=(const Series<T>& a, const Series<T>& b)
	{
		return series::notEqual(a, b);
	}
	

	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T3> operator>(const Series<T>& a, const T2 b)
	{
		return series::transform([](T ai, T2 bi){ return ai > bi; }, a, b);
	}
	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T3> operator>=(const Series<T>& a, const T2 b)
	{
		return series::transform([](T ai, T2 bi){ return ai >= bi; }, a, b);
	}
	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T3> operator<(const Series<T>& a, const T2 b)
	{
		return series::transform([](T ai, T2 bi){ return ai < bi; }, a, b);
	}
	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T3> operator<=(const Series<T>& a, const T2 b)
	{
		return series::transform([](T ai, T2 bi){ return ai <= bi; }, a, b);
	}
	

	template <typename T, typename T2, typename T3>
	inline Series<T3> operator>(const Series<T>& a, const Series<T2> b)
	{
		return series::transform([](T ai, T2 bi){ return ai > bi; }, a, b);
	}
	template <typename T, typename T2, typename T3>
	inline Series<T3> operator>=(const Series<T>& a, const Series<T2> b)
	{
		return series::transform([](T ai, T2 bi){ return ai >= bi; }, a, b);
	}
	template <typename T, typename T2, typename T3>
	inline Series<T3> operator<(const Series<T>& a, const Series<T2> b)
	{
		return series::transform([](T ai, T2 bi){ return ai < bi; }, a, b);
	}
	template <typename T, typename T2, typename T3>
	inline Series<T3> operator<=(const Series<T>& a, const Series<T2> b)
	{
		return series::transform([](T ai, T2 bi){ return ai <= bi; }, a, b);
	}
	
	// NOTE: all wrappers are suggested to be inline because they are thin wrappers of functions

	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T3> operator>(const T2 a, const Series<T>& b)
	{
		return series::transform([](T ai, T2 bi){ return ai > bi; }, a, b);
	}
	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T3> operator>=(const T2 a, const Series<T>& b)
	{
		return series::transform([](T ai, T2 bi){ return ai >= bi; }, a, b);
	}
	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T3> operator<(const T2 a, const Series<T>& b)
	{
		return series::transform([](T ai, T2 bi){ return ai < bi; }, a, b);
	}
	template <typename T, typename T2, typename T3, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T3> operator<=(const T2 a, const Series<T>& b)
	{
		return series::transform([](T ai, T2 bi){ return ai <= bi; }, a, b);
	}





	template <typename T>
	inline Series<T>& operator+=(Series<T>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] += b;
		}
		return a;
	}
	template <typename T>
	inline Series<T>& operator-=(Series<T>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] -= b;
		}
		return a;
	}
	template <typename T>
	inline Series<T>& operator*=(Series<T>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] *= b;
		}
		return a;
	}
	template <typename T>
	inline Series<T>& operator/=(Series<T>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] /= b;
		}
		return a;
	}


	template <typename T, typename T2>
	inline Series<T>& operator+=(Series<T>& a, const Series<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] += b[i];
		}
		return a;
	}
	template <typename T, typename T2>
	inline Series<T>& operator-=(Series<T>& a, const Series<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] -= b[i];
		}
		return a;
	}
	template <typename T, typename T2>
	inline Series<T>& operator*=(Series<T>& a, const Series<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] *= b[i];
		}
		return a;
	}
	template <typename T, typename T2>
	inline Series<T>& operator/=(Series<T>& a, const Series<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] /= b[i];
		}
		return a;
	}

	// NOTE: prefix increment/decrement
	template <typename T>
	inline Series<T>& operator++(Series<T>& a)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]++;
		}
		return a;
	}  
	template <typename T>
	inline Series<T>& operator--(Series<T>& a)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]--;
		}
		return a;
	}  

	// NOTE: postfix increment/decrement
	template <typename T>
	inline Series<T> operator++(Series<T>& a, int)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]++;
		}
		return a;
	}  
	template <typename T>
	inline Series<T> operator--(Series<T>& a, int)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]--;
		}
		return a;
	}  
	




	// NOTE: we define operators for multiple typenamees T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <typename T, typename T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T> operator+(const Series<T>& a, const T2 b)
	{
		return series::transform([](T ai, T2 bi){ return ai + bi; }, a, b);
	}
	template <typename T, typename T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T> operator-(const Series<T>& a, const T2 b)
	{
		return series::transform([](T ai, T2 bi){ return ai - bi; }, a, b);
	}
	template <typename T, typename T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T> operator*(const Series<T>& a, const T2 b)
	{
		return series::transform([](T ai, T2 bi){ return ai * bi; }, a, b);
	}
	template <typename T, typename T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T> operator/(const Series<T>& a, const T2 b)
	{
		return series::transform([](T ai, T2 bi){ return ai / bi; }, a, b);
	}
	




	// NOTE: we define operators for multiple typenamees T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <typename T, typename T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T> operator+(const T2 a, const Series<T>& b)
	{
		return series::transform([](T ai, T2 bi){ return ai + bi; }, a, b);
	}
	template <typename T, typename T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T> operator-(const T2 a, const Series<T>& b)
	{
		return series::transform([](T ai, T2 bi){ return ai - bi; }, a, b);
	}
	template <typename T, typename T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T> operator*(const T2 a, const Series<T>& b)
	{
		return series::transform([](T ai, T2 bi){ return ai * bi; }, a, b);
	}
	template <typename T, typename T2, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline Series<T> operator/(const T2 a, const Series<T>& b)
	{
		return series::transform([](T ai, T2 bi){ return ai / bi; }, a, b);
	}


	// NOTE: we define operators for multiple typenamees T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <typename T, typename T2>
	inline Series<T> operator+(const Series<T>& a, const Series<T2>& b)
	{
		return series::transform([](T ai, T2 bi){ return ai + bi; }, a, b);
	}
	template <typename T, typename T2>
	inline Series<T> operator-(const Series<T>& a, const Series<T2>& b)
	{
		return series::transform([](T ai, T2 bi){ return ai - bi; }, a, b);
	}
	template <typename T, typename T2>
	inline Series<T> operator*(const Series<T>& a, const Series<T2>& b)
	{
		return series::transform([](T ai, T2 bi){ return ai * bi; }, a, b);
	}
	template <typename T, typename T2>
	inline Series<T> operator/(const Series<T>& a, const Series<T2>& b)
	{
		return series::transform([](T ai, T2 bi){ return ai / bi; }, a, b);
	}


	template <typename T>
	inline Series<T> operator-(const Series<T>& a)
	{
		return series::transform([](T ai){ return -ai; }, a);
	}
	
	inline Series<bool> operator~(const Series<bool>& a)
	{
		return series::transform([](bool ai){ return !ai; }, a);
	}


	inline Series<bool> operator|(const Series<bool>& a, const bool b)
	{
		return series::transform([](bool ai, bool bi){ return ai || bi; }, a, b);
	}
	inline Series<bool> operator&(const Series<bool>& a, const bool b)
	{
		return series::transform([](bool ai, bool bi){ return ai && bi; }, a, b);
	}

	inline Series<bool> operator|(const Series<bool>& a, const Series<bool>& b)
	{
		return series::transform([](bool ai, bool bi){ return ai || bi; }, a, b);
	}
	inline Series<bool> operator&(const Series<bool>& a, const Series<bool>& b)
	{
		return series::transform([](bool ai, bool bi){ return ai && bi; }, a, b);
	}




	inline Series<bool>& operator|=(Series<bool>& a, const bool b){
		a.store([](bool ai, bool bi){ return ai || bi; },  a, b);
		return a;
	}
	inline Series<bool>& operator&=(Series<bool>& a, const bool b){
		a.store([](bool ai, bool bi){ return ai &&  bi; },  a, b);
		return a;
	}

	inline Series<bool>& operator|=(Series<bool>& a, const Series<bool>& b){
		a.store([](bool ai, bool bi){ return ai || bi; },  a, b);
		return a;
	}
	inline Series<bool>& operator&=(Series<bool>& a, const Series<bool>& b){
		a.store([](bool ai, bool bi){ return ai &&  bi; },  a, b);
		return a;
	}
}