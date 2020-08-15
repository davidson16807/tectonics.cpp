#pragma once

#include <cmath>

#include "types.hpp"
#include "relational.hpp"
#include "common.hpp"

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
	series<T> get(const series<T>& a, const series<unsigned int>& ids)
	{
		series<T> out(ids.size());
		get(a, ids, out);
		return out;
	}
	template <class T>
	series<T> copy(const series<T>& a )
	{
		return series<T>(a);
	}
	

	template <class T1, typename F>
	inline series<T1> transform(const series<T1>& a, F f)
	{
		series<T1> out = series<T1>(a.size());
		transform(a, f, out); 
		return out;
	}


	template <class T1, class T2, typename F>
	inline series<T1> transform(const series<T1>& a, const series<T2>& b, F f)
	{
		series<T1> out = series<T1>(a.size());
		transform(a, b, f, out); 
		return out;
	}
	template <class T1, class T2, typename F>
	inline series<T1> transform(const series<T1>& a, const T2 b, F f)
	{
		series<T1> out = series<T1>(a.size());
		transform(a, b, f, out); 
		return out;
	}
	template <class T1, class T2, typename F>
	inline series<T1> transform(const T1 a, const series<T2>& b, F f)
	{
		series<T1> out = series<T1>(b.size());
		transform(a, b, f, out); 
		return out;
	}




	template <class T1, class T2, class T3, typename F>
	inline series<T1> transform(const series<T1>& a, const series<T2>& b, const series<T3>& c, F f)
	{
		series<T1> out = series<T1>(a.size());
		transform(a, b, c, f, out); 
		return out;
	}
	template <class T1, class T2, class T3, typename F>
	inline series<T1> transform(const series<T1>& a, const series<T2>& b, const T3 c, F f)
	{
		series<T1> out = series<T1>(a.size());
		transform(a, b, c, f, out); 
		return out;
	}
	template <class T1, class T2, class T3, typename F>
	inline series<T1> transform(const series<T1>& a, const T2 b, const series<T3>& c, F f)
	{
		series<T1> out = series<T1>(a.size());
		transform(a, b, c, f, out); 
		return out;
	}
	template <class T1, class T2, class T3, typename F>
	inline series<T1> transform(const series<T1>& a, const T2 b, const T3 c, F f)
	{
		series<T1> out = series<T1>(a.size());
		transform(a, b, c, f, out); 
		return out;
	}
	template <class T1, class T2, class T3, typename F>
	inline series<T1> transform(const T1 a, const series<T2>& b, const series<T3>& c, F f)
	{
		series<T1> out = series<T1>(b.size());
		transform(a, b, c, f, out); 
		return out;
	}
	template <class T1, class T2, class T3, typename F>
	inline series<T1> transform(const T1 a, const series<T2>& b, const T3 c, F f)
	{
		series<T1> out = series<T1>(b.size());
		transform(a, b, c, f, out); 
		return out;
	}
	template <class T1, class T2, class T3, typename F>
	inline series<T1> transform(const T1 a, const T2 b, const series<T3>& c, F f)
	{
		series<T1> out = series<T1>(c.size());
		transform(a, b, c, f, out); 
		return out;
	}





	template<class T, typename Taggregator>
	series<T> aggregate(const series<T>& a, const series<unsigned int>& group_ids, Taggregator aggregator)
	{
		series<T> group_out = series<T>(max(group_ids));
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]], a[i]);
		}
		return group_out;
	}

	template<class T, typename Taggregator>
	series<T> aggregate(const series<unsigned int>& group_ids, Taggregator aggregator)
	{
		series<T> group_out = series<T>(max(group_ids));
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]]);
		}
		return group_out;
	}


	/// Returns x if x >= 0; otherwise, it returns -x.
	template <class T>
	series<T> abs(const series<T>& a)
	{
		return transform(a, [](T ai){ return ai >= 0? ai : -ai; });
	}

	/// Returns 1.0 if x > 0, 0.0 if x == 0, or -1.0 if x < 0.
	template <class T>
	series<T> sign(const series<T>& a)
	{
		return transform(a, [](T ai){ return (T(0) < ai) - (ai < T(0)); });
	}

	/// Returns a value equal to the nearest integer that is less then or equal to x.
	template <class T>
	series<T> floor(const series<T>& a)
	{
		return transform(a, std::floor);
	}

	/// Returns a value equal to the nearest integer to x
	/// whose absolute value is not larger than the absolute value of x.
	template <class T>
	series<T> trunc(const series<T>& a)
	{
		return transform(a, std::trunc);
	}

	/// Returns a value equal to the nearest integer to x.
	/// The fraction 0.5 will round in a direction chosen by the
	/// implementation, presumably the direction that is fastest.
	/// This includes the possibility that round(x) returns the
	/// same value as roundEven(x) for all values of x.
	template <class T>
	series<T> round(const series<T>& a)
	{
		return transform(a, std::round);
	}

	/// Returns a value equal to the nearest integer
	/// that is greater than or equal to x.
	template <class T>
	series<T> ceil(const series<T>& a)
	{
		transform(a, std::ceil);
	}



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
		return equal(a, b);
	}
	template <class T>
	inline bool operator!=(const series<T>& a, const T b)
	{
		return notEqual(a, b);
	}
	template <class T>
	inline bool operator==(const T a, const series<T>& b)
	{
		return equal(a, b);
	}
	template <class T>
	inline bool operator!=(const T a, const series<T>& b)
	{
		return notEqual(a, b);
	}
	template <class T>
	inline bool operator==(const series<T>& a, const series<T>& b)
	{
		return equal(a, b);
	}
	template <class T>
	inline bool operator!=(const series<T>& a, const series<T>& b)
	{
		return notEqual(a, b);
	}
	

	template <class T, class T2, class T3>
	inline series<T3> operator>(const series<T>& a, const T2 b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai > bi; });
	}
	template <class T, class T2, class T3>
	inline series<T3> operator>=(const series<T>& a, const T2 b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai >= bi; });
	}
	template <class T, class T2, class T3>
	inline series<T3> operator<(const series<T>& a, const T2 b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai < bi; });
	}
	template <class T, class T2, class T3>
	inline series<T3> operator<=(const series<T>& a, const T2 b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai <= bi; });
	}
	
	// NOTE: all wrappers are suggested to be inline because they are thin wrappers of functions

	template <class T, class T2, class T3>
	inline series<T3> operator>(const T2 a, const series<T>& b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai > bi; });
	}
	template <class T, class T2, class T3>
	inline series<T3> operator>=(const T2 a, const series<T>& b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai >= bi; });
	}
	template <class T, class T2, class T3>
	inline series<T3> operator<(const T2 a, const series<T>& b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai < bi; });
	}
	template <class T, class T2, class T3>
	inline series<T3> operator<=(const T2 a, const series<T>& b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai <= bi; });
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
	template <class T, class T2>
	inline series<T> operator+(const series<T>& a, const T2 b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai + bi; });
	}
	template <class T, class T2>
	inline series<T> operator-(const series<T>& a, const T2 b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai - bi; });
	}
	template <class T, class T2>
	inline series<T> operator*(const series<T>& a, const T2 b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai * bi; });
	}
	template <class T, class T2>
	inline series<T> operator/(const series<T>& a, const T2 b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai / bi; });
	}
	




	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <class T, class T2>
	inline series<T> operator+(const T2 a, const series<T>& b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai + bi; });
	}
	template <class T, class T2>
	inline series<T> operator-(const T2 a, const series<T>& b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai - bi; });
	}
	template <class T, class T2>
	inline series<T> operator*(const T2 a, const series<T>& b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai * bi; });
	}
	template <class T, class T2>
	inline series<T> operator/(const T2 a, const series<T>& b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai / bi; });
	}


	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <class T, class T2>
	inline series<T> operator+(const series<T>& a, const series<T2>& b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai + bi; });
	}
	template <class T, class T2>
	inline series<T> operator-(const series<T>& a, const series<T2>& b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai - bi; });
	}
	template <class T, class T2>
	inline series<T> operator*(const series<T>& a, const series<T2>& b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai * bi; });
	}
	template <class T, class T2>
	inline series<T> operator/(const series<T>& a, const series<T2>& b)
	{
		return transform(a, b, [](T ai, T2 bi){ return ai / bi; });
	}


	inline series<bool> operator~(const series<bool>& a)
	{
		series<bool> out = series<bool>(a.size());
		transform(a, [](bool ai){ return !ai; }, out);
		return out;
	}


	inline series<bool> operator|(const series<bool>& a, const bool b)
	{
		return transform(a, b, [](bool ai, bool bi){ return ai || bi; });
	}
	inline series<bool> operator&(const series<bool>& a, const bool b)
	{
		return transform(a, b, [](bool ai, bool bi){ return ai && bi; });
	}

	inline series<bool> operator|(const series<bool>& a, const series<bool>& b)
	{
		return transform(a, b, [](bool ai, bool bi){ return ai || bi; });
	}
	inline series<bool> operator&(const series<bool>& a, const series<bool>& b)
	{
		return transform(a, b, [](bool ai, bool bi){ return ai && bi; });
	}




	inline series<bool>& operator|=(series<bool>& a, const bool b){
		transform(a, b, [](bool ai, bool bi){ return ai || bi; }, a);
		return a;
	}
	inline series<bool>& operator&=(series<bool>& a, const bool b){
		transform(a, b, [](bool ai, bool bi){ return ai &&  bi; }, a);
		return a;
	}

	inline series<bool>& operator|=(series<bool>& a, const series<bool>& b){
		transform(a, b, [](bool ai, bool bi){ return ai || bi; }, a);
		return a;
	}
	inline series<bool>& operator&=(series<bool>& a, const series<bool>& b){
		transform(a, b, [](bool ai, bool bi){ return ai &&  bi; }, a);
		return a;
	}
}