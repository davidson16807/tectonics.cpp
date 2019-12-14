#pragma once

#include <cmath>

#include "types.hpp"

/*
"convenience.hpp" contains functions that pass output as return values rather than using using output parameters
This provides convenience at the expense of performance, since now we have to call constructors for the new objects. 
See https://codeyarns.com/2010/10/21/c-return-value-versus-output-parameter/ for more info.
It is important to keep these functions separate from the rest of the library for two reasons:
 1.) It encourages good practice, since you have to explicitly opt-in to less performant convenience functions.
 2.) It provides a nice itemization of functions that will have to be created if you subclass std::vector<T> (as we do within the rasters library)
*/

namespace many
{
	template <class T>
	std::vector<T> get(const std::vector<T>& a, const std::vector<unsigned int>& ids)
	{
		std::vector<T> out(ids.size());
		get(a, ids, out);
		return out;
	}
	template <class T>
	std::vector<T> copy(const std::vector<T>& a )
	{
		return std::vector<T>(a);
	}
	

	template <class T1, typename F>
	inline std::vector<T1> apply(const std::vector<T1>& a, F f)
	{
		std::vector<T1> out = std::vector<T1>(a.size());
		apply(a, f, out); 
		return out;
	}


	template <class T1, class T2, typename F>
	inline std::vector<T1> apply(const std::vector<T1>& a, const std::vector<T2>& b, F f)
	{
		std::vector<T1> out = std::vector<T1>(a.size());
		apply(a, b, f, out); 
		return out;
	}
	template <class T1, class T2, typename F>
	inline std::vector<T1> apply(const std::vector<T1>& a, const T2 b, F f)
	{
		std::vector<T1> out = std::vector<T1>(a.size());
		apply(a, b, f, out); 
		return out;
	}
	template <class T1, class T2, typename F>
	inline std::vector<T1> apply(const T1 a, const std::vector<T2>& b, F f)
	{
		std::vector<T1> out = std::vector<T1>(b.size());
		apply(a, b, f, out); 
		return out;
	}




	template <class T1, class T2, class T3, typename F>
	inline std::vector<T1> apply(const std::vector<T1>& a, const std::vector<T2>& b, const std::vector<T3>& c, F f)
	{
		std::vector<T1> out = std::vector<T1>(a.size());
		apply(a, b, c, f, out); 
		return out;
	}
	template <class T1, class T2, class T3, typename F>
	inline std::vector<T1> apply(const std::vector<T1>& a, const std::vector<T2>& b, const T3 c, F f)
	{
		std::vector<T1> out = std::vector<T1>(a.size());
		apply(a, b, c, f, out); 
		return out;
	}
	template <class T1, class T2, class T3, typename F>
	inline std::vector<T1> apply(const std::vector<T1>& a, const T2 b, const std::vector<T3>& c, F f)
	{
		std::vector<T1> out = std::vector<T1>(a.size());
		apply(a, b, c, f, out); 
		return out;
	}
	template <class T1, class T2, class T3, typename F>
	inline std::vector<T1> apply(const std::vector<T1>& a, const T2 b, const T3 c, F f)
	{
		std::vector<T1> out = std::vector<T1>(a.size());
		apply(a, b, c, f, out); 
		return out;
	}
	template <class T1, class T2, class T3, typename F>
	inline std::vector<T1> apply(const T1 a, const std::vector<T2>& b, const std::vector<T3>& c, F f)
	{
		std::vector<T1> out = std::vector<T1>(b.size());
		apply(a, b, c, f, out); 
		return out;
	}
	template <class T1, class T2, class T3, typename F>
	inline std::vector<T1> apply(const T1 a, const std::vector<T2>& b, const T3 c, F f)
	{
		std::vector<T1> out = std::vector<T1>(b.size());
		apply(a, b, c, f, out); 
		return out;
	}
	template <class T1, class T2, class T3, typename F>
	inline std::vector<T1> apply(const T1 a, const T2 b, const std::vector<T3>& c, F f)
	{
		std::vector<T1> out = std::vector<T1>(c.size());
		apply(a, b, c, f, out); 
		return out;
	}





	template<class T, typename Taggregator>
	std::vector<T> aggregate(const std::vector<T>& a, const std::vector<unsigned int>& group_ids, Taggregator aggregator)
	{
		std::vector<T> group_out = std::vector<T>(max(group_ids));
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]], a[i]);
		}
		return group_out;
	}

	template<class T, typename Taggregator>
	std::vector<T> aggregate(const std::vector<unsigned int>& group_ids, Taggregator aggregator)
	{
		std::vector<T> group_out = std::vector<T>(max(group_ids));
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]]);
		}
		return group_out;
	}


	/// Returns x if x >= 0; otherwise, it returns -x.
	template <class T>
	std::vector<T> abs(const std::vector<T>& a)
	{
		return apply(a, [](T ai){ return ai >= 0? ai : -ai; });
	}

	/// Returns 1.0 if x > 0, 0.0 if x == 0, or -1.0 if x < 0.
	template <class T>
	std::vector<T> sign(const std::vector<T>& a)
	{
		return apply(a, [](T ai){ return (T(0) < ai) - (ai < T(0)); });
	}

	/// Returns a value equal to the nearest integer that is less then or equal to x.
	template <class T>
	std::vector<T> floor(const std::vector<T>& a)
	{
		return apply(a, std::floor);
	}

	/// Returns a value equal to the nearest integer to x
	/// whose absolute value is not larger than the absolute value of x.
	template <class T>
	std::vector<T> trunc(const std::vector<T>& a)
	{
		return apply(a, std::trunc);
	}

	/// Returns a value equal to the nearest integer to x.
	/// The fraction 0.5 will round in a direction chosen by the
	/// implementation, presumably the direction that is fastest.
	/// This includes the possibility that round(x) returns the
	/// same value as roundEven(x) for all values of x.
	template <class T>
	std::vector<T> round(const std::vector<T>& a)
	{
		return apply(a, std::round);
	}

	/// Returns a value equal to the nearest integer
	/// that is greater than or equal to x.
	template <class T>
	std::vector<T> ceil(const std::vector<T>& a)
	{
		apply(a, std::ceil);
	}



	template <class T>
	std::ostream &operator<<(std::ostream &os, const std::vector<T>& a) { 
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
	inline bool operator==(const std::vector<T>& a, const T b)
	{
		return equal(a, b);
	}
	template <class T>
	inline bool operator!=(const std::vector<T>& a, const T b)
	{
		return notEqual(a, b);
	}
	template <class T>
	inline bool operator==(const T a, const std::vector<T>& b)
	{
		return equal(a, b);
	}
	template <class T>
	inline bool operator!=(const T a, const std::vector<T>& b)
	{
		return notEqual(a, b);
	}
	template <class T>
	inline bool operator==(const std::vector<T>& a, const std::vector<T>& b)
	{
		return equal(a, b);
	}
	template <class T>
	inline bool operator!=(const std::vector<T>& a, const std::vector<T>& b)
	{
		return notEqual(a, b);
	}
	

	template <class T, class T2, class T3>
	inline std::vector<T3> operator>(const std::vector<T>& a, const T2 b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai > bi; });
	}
	template <class T, class T2, class T3>
	inline std::vector<T3> operator>=(const std::vector<T>& a, const T2 b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai >= bi; });
	}
	template <class T, class T2, class T3>
	inline std::vector<T3> operator<(const std::vector<T>& a, const T2 b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai < bi; });
	}
	template <class T, class T2, class T3>
	inline std::vector<T3> operator<=(const std::vector<T>& a, const T2 b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai <= bi; });
	}
	
	// NOTE: all wrappers are suggested to be inline because they are thin wrappers of functions

	template <class T, class T2, class T3>
	inline std::vector<T3> operator>(const T2 a, const std::vector<T>& b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai > bi; });
	}
	template <class T, class T2, class T3>
	inline std::vector<T3> operator>=(const T2 a, const std::vector<T>& b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai >= bi; });
	}
	template <class T, class T2, class T3>
	inline std::vector<T3> operator<(const T2 a, const std::vector<T>& b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai < bi; });
	}
	template <class T, class T2, class T3>
	inline std::vector<T3> operator<=(const T2 a, const std::vector<T>& b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai <= bi; });
	}





	template <class T>
	inline std::vector<T>& operator+=(std::vector<T>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] += b;
		}
		return a;
	}
	template <class T>
	inline std::vector<T>& operator-=(std::vector<T>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] -= b;
		}
		return a;
	}
	template <class T>
	inline std::vector<T>& operator*=(std::vector<T>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] *= b;
		}
		return a;
	}
	template <class T>
	inline std::vector<T>& operator/=(std::vector<T>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] /= b;
		}
		return a;
	}


	template <class T, class T2>
	inline std::vector<T>& operator+=(std::vector<T>& a, const std::vector<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] += b[i];
		}
		return a;
	}
	template <class T, class T2>
	inline std::vector<T>& operator-=(std::vector<T>& a, const std::vector<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] -= b[i];
		}
		return a;
	}
	template <class T, class T2>
	inline std::vector<T>& operator*=(std::vector<T>& a, const std::vector<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] *= b[i];
		}
		return a;
	}
	template <class T, class T2>
	inline std::vector<T>& operator/=(std::vector<T>& a, const std::vector<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] /= b[i];
		}
		return a;
	}

	// NOTE: prefix increment/decrement
	template <class T>
	inline std::vector<T>& operator++(std::vector<T>& a)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]++;
		}
		return a;
	}  
	template <class T>
	inline std::vector<T>& operator--(std::vector<T>& a)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]--;
		}
		return a;
	}  

	// NOTE: postfix increment/decrement
	template <class T>
	inline std::vector<T> operator++(std::vector<T>& a, int)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]++;
		}
		return a;
	}  
	template <class T>
	inline std::vector<T> operator--(std::vector<T>& a, int)  
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
	inline std::vector<T> operator+(const std::vector<T>& a, const T2 b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai + bi; });
	}
	template <class T, class T2>
	inline std::vector<T> operator-(const std::vector<T>& a, const T2 b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai - bi; });
	}
	template <class T, class T2>
	inline std::vector<T> operator*(const std::vector<T>& a, const T2 b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai * bi; });
	}
	template <class T, class T2>
	inline std::vector<T> operator/(const std::vector<T>& a, const T2 b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai / bi; });
	}
	




	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <class T, class T2>
	inline std::vector<T> operator+(const T2 a, const std::vector<T>& b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai + bi; });
	}
	template <class T, class T2>
	inline std::vector<T> operator-(const T2 a, const std::vector<T>& b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai - bi; });
	}
	template <class T, class T2>
	inline std::vector<T> operator*(const T2 a, const std::vector<T>& b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai * bi; });
	}
	template <class T, class T2>
	inline std::vector<T> operator/(const T2 a, const std::vector<T>& b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai / bi; });
	}


	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <class T, class T2>
	inline std::vector<T> operator+(const std::vector<T>& a, const std::vector<T2>& b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai + bi; });
	}
	template <class T, class T2>
	inline std::vector<T> operator-(const std::vector<T>& a, const std::vector<T2>& b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai - bi; });
	}
	template <class T, class T2>
	inline std::vector<T> operator*(const std::vector<T>& a, const std::vector<T2>& b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai * bi; });
	}
	template <class T, class T2>
	inline std::vector<T> operator/(const std::vector<T>& a, const std::vector<T2>& b)
	{
		return apply(a, b, [](T ai, T2 bi){ return ai / bi; });
	}


	inline std::vector<bool> operator~(const std::vector<bool>& a)
	{
		std::vector<bool> out = std::vector<bool>(a.size());
		apply(a, [](bool ai){ return !ai; }, out);
		return out;
	}


	inline std::vector<bool> operator|(const std::vector<bool>& a, const bool b)
	{
		return apply(a, b, [](bool ai, bool bi){ return ai || bi; });
	}
	inline std::vector<bool> operator&(const std::vector<bool>& a, const bool b)
	{
		return apply(a, b, [](bool ai, bool bi){ return ai && bi; });
	}

	inline std::vector<bool> operator|(const std::vector<bool>& a, const std::vector<bool>& b)
	{
		return apply(a, b, [](bool ai, bool bi){ return ai || bi; });
	}
	inline std::vector<bool> operator&(const std::vector<bool>& a, const std::vector<bool>& b)
	{
		return apply(a, b, [](bool ai, bool bi){ return ai && bi; });
	}




	inline std::vector<bool>& operator|=(std::vector<bool>& a, const bool b){
		apply(a, b, [](bool ai, bool bi){ return ai || bi; }, a);
		return a;
	}
	inline std::vector<bool>& operator&=(std::vector<bool>& a, const bool b){
		apply(a, b, [](bool ai, bool bi){ return ai &&  bi; }, a);
		return a;
	}

	inline std::vector<bool>& operator|=(std::vector<bool>& a, const std::vector<bool>& b){
		apply(a, b, [](bool ai, bool bi){ return ai || bi; }, a);
		return a;
	}
	inline std::vector<bool>& operator&=(std::vector<bool>& a, const std::vector<bool>& b){
		apply(a, b, [](bool ai, bool bi){ return ai &&  bi; }, a);
		return a;
	}
}