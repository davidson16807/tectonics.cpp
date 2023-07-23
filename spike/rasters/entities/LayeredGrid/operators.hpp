#pragma once

#include <cmath>

#include <series/types.hpp>
#include "LayeredRaster.hpp"
#include "convenience.hpp"

/*
"convenience.hpp" contains functions that pass output as return values rather than using using output parameters
This provides convenience at the expense of performance, since now we have to call constructors for the new objects. 
See https://codeyarns.com/2010/10/21/c-return-value-versus-output-parameter/ for more info.
It is important to keep these functions separate from the rest of the library for two reasons:
 1.) It encourages good practice, since you have to explicitly opt-in to less performant convenience functions.
 2.) It provides a nice itemization of functions that will have to be created if you subclass LayeredRaster<T,Tgrid,Tmap> 
*/

namespace rasters
{

	/*
	template <class T, rasters::mapping Tmap, class Tid, class Tfloat>
	std::ostream &operator<<(std::ostream &os, const LayeredRaster<T,SpheroidGrid<Tid,Tfloat,Tmap>>& a) { 
		os << "[";
		for (unsigned int i = 0; i < a.size(); ++i)
		{
		    os << a[i] << " ";
		}
		os << "]";
		return os;
	}
	*/


	// NOTE: all operators are suggested to be inline because they are thin wrappers of functions
	template <class Tgrid, rasters::mapping Tmap, class T>
	inline bool operator==(const LayeredRaster<T,Tgrid,Tmap>& a, const T b)
	{
		return series::equal(a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T>
	inline bool operator!=(const LayeredRaster<T,Tgrid,Tmap>& a, const T b)
	{
		return series::notEqual(a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T>
	inline bool operator==(const T a, const LayeredRaster<T,Tgrid,Tmap>& b)
	{
		return series::equal(a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T>
	inline bool operator!=(const T a, const LayeredRaster<T,Tgrid,Tmap>& b)
	{
		return series::notEqual(a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T>
	inline bool operator==(const LayeredRaster<T,Tgrid,Tmap>& a, const LayeredRaster<T,Tgrid,Tmap>& b)
	{
		return series::equal(a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T>
	inline bool operator!=(const LayeredRaster<T,Tgrid,Tmap>& a, const LayeredRaster<T,Tgrid,Tmap>& b)
	{
		return series::notEqual(a, b);
	}
	

	template <class Tgrid, rasters::mapping Tmap, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T3,Tgrid,Tmap> operator>(const LayeredRaster<T,Tgrid,Tmap>& a, const T2 b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai > bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T3,Tgrid,Tmap> operator>=(const LayeredRaster<T,Tgrid,Tmap>& a, const T2 b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai >= bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T3,Tgrid,Tmap> operator<(const LayeredRaster<T,Tgrid,Tmap>& a, const T2 b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai < bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T3,Tgrid,Tmap> operator<=(const LayeredRaster<T,Tgrid,Tmap>& a, const T2 b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai <= bi; }, a, b);
	}
	
	// NOTE: all wrappers are suggested to be inline because they are thin wrappers of functions

	template <class Tgrid, rasters::mapping Tmap, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T3,Tgrid,Tmap> operator>(const T2 a, const LayeredRaster<T,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai > bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T3,Tgrid,Tmap> operator>=(const T2 a, const LayeredRaster<T,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai >= bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T3,Tgrid,Tmap> operator<(const T2 a, const LayeredRaster<T,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai < bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2, class T3, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T3,Tgrid,Tmap> operator<=(const T2 a, const LayeredRaster<T,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai <= bi; }, a, b);
	}





	template <class Tgrid, rasters::mapping Tmap, class T>
	inline LayeredRaster<T,Tgrid,Tmap>& operator+=(LayeredRaster<T,Tgrid,Tmap>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] += b;
		}
		return a;
	}
	template <class Tgrid, rasters::mapping Tmap, class T>
	inline LayeredRaster<T,Tgrid,Tmap>& operator-=(LayeredRaster<T,Tgrid,Tmap>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] -= b;
		}
		return a;
	}
	template <class Tgrid, rasters::mapping Tmap, class T>
	inline LayeredRaster<T,Tgrid,Tmap>& operator*=(LayeredRaster<T,Tgrid,Tmap>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] *= b;
		}
		return a;
	}
	template <class Tgrid, rasters::mapping Tmap, class T>
	inline LayeredRaster<T,Tgrid,Tmap>& operator/=(LayeredRaster<T,Tgrid,Tmap>& a, const T b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] /= b;
		}
		return a;
	}


	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap>& operator+=(LayeredRaster<T,Tgrid,Tmap>& a, const LayeredRaster<T2,Tgrid,Tmap>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] += b[i];
		}
		return a;
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap>& operator-=(LayeredRaster<T,Tgrid,Tmap>& a, const LayeredRaster<T2,Tgrid,Tmap>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] -= b[i];
		}
		return a;
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap>& operator*=(LayeredRaster<T,Tgrid,Tmap>& a, const LayeredRaster<T2,Tgrid,Tmap>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] *= b[i];
		}
		return a;
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap>& operator/=(LayeredRaster<T,Tgrid,Tmap>& a, const LayeredRaster<T2,Tgrid,Tmap>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] /= b[i];
		}
		return a;
	}


	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap>& operator+=(LayeredRaster<T,Tgrid,Tmap>& a, const series::Series<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] += b[i];
		}
		return a;
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap>& operator-=(LayeredRaster<T,Tgrid,Tmap>& a, const series::Series<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] -= b[i];
		}
		return a;
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap>& operator*=(LayeredRaster<T,Tgrid,Tmap>& a, const series::Series<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] *= b[i];
		}
		return a;
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap>& operator/=(LayeredRaster<T,Tgrid,Tmap>& a, const series::Series<T2>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] /= b[i];
		}
		return a;
	}

/*
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap>& operator+=(series::Series<T>& a, const LayeredRaster<T2,Tgrid,Tmap>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] += b[i];
		}
		return a;
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap>& operator-=(series::Series<T>& a, const LayeredRaster<T2,Tgrid,Tmap>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] -= b[i];
		}
		return a;
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap>& operator*=(series::Series<T>& a, const LayeredRaster<T2,Tgrid,Tmap>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] *= b[i];
		}
		return a;
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap>& operator/=(series::Series<T>& a, const LayeredRaster<T2,Tgrid,Tmap>& b) 
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i] /= b[i];
		}
		return a;
	}
*/

	
	// NOTE: all operators are suggested to be inline because they are thin wrappers of functions
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& operator+=(LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& a, const T b) 
	{
		add(a, b, a);
		return a;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& operator-=(LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& a, const T b) 
	{
		sub(a, b, a);
		return a;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& operator*=(LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& a, const T b) 
	{
		mult(a, b, a);
		return a;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& operator/=(LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& a, const T b) 
	{
		div(a, b, a);
		return a;
	}

	

	// NOTE: prefix increment/decrement
	template <class Tgrid, rasters::mapping Tmap, class T>
	inline LayeredRaster<T,Tgrid,Tmap>& operator++(LayeredRaster<T,Tgrid,Tmap>& a)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]++;
		}
		return a;
	}  
	template <class Tgrid, rasters::mapping Tmap, class T>
	inline LayeredRaster<T,Tgrid,Tmap>& operator--(LayeredRaster<T,Tgrid,Tmap>& a)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]--;
		}
		return a;
	}  

	// NOTE: postfix increment/decrement
	template <class Tgrid, rasters::mapping Tmap, class T>
	inline LayeredRaster<T,Tgrid,Tmap> operator++(LayeredRaster<T,Tgrid,Tmap>& a, int)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]++;
		}
		return a;
	}  
	template <class Tgrid, rasters::mapping Tmap, class T>
	inline LayeredRaster<T,Tgrid,Tmap> operator--(LayeredRaster<T,Tgrid,Tmap>& a, int)  
	{  
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			a[i]--;
		}
		return a;
	}  
	




	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <class Tgrid, rasters::mapping Tmap, class T, class T2, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T,Tgrid,Tmap> operator+(const LayeredRaster<T,Tgrid,Tmap>& a, const T2 b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai + bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T,Tgrid,Tmap> operator-(const LayeredRaster<T,Tgrid,Tmap>& a, const T2 b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai - bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T,Tgrid,Tmap> operator*(const LayeredRaster<T,Tgrid,Tmap>& a, const T2 b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai * bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T,Tgrid,Tmap> operator/(const LayeredRaster<T,Tgrid,Tmap>& a, const T2 b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai / bi; }, a, b);
	}
	




	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <class Tgrid, rasters::mapping Tmap, class T, class T2, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T,Tgrid,Tmap> operator+(const T2 a, const LayeredRaster<T,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai + bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T,Tgrid,Tmap> operator-(const T2 a, const LayeredRaster<T,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai - bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T,Tgrid,Tmap> operator*(const T2 a, const LayeredRaster<T,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai * bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2, std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
	inline LayeredRaster<T,Tgrid,Tmap> operator/(const T2 a, const LayeredRaster<T,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai / bi; }, a, b);
	}


	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap> operator+(const LayeredRaster<T,Tgrid,Tmap>& a, const LayeredRaster<T2,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai + bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap> operator-(const LayeredRaster<T,Tgrid,Tmap>& a, const LayeredRaster<T2,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai - bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap> operator*(const LayeredRaster<T,Tgrid,Tmap>& a, const LayeredRaster<T2,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai * bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap> operator/(const LayeredRaster<T,Tgrid,Tmap>& a, const LayeredRaster<T2,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai / bi; }, a, b);
	}

	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap> operator+(const series::Series<T>& a, const LayeredRaster<T2,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai + bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap> operator-(const series::Series<T>& a, const LayeredRaster<T2,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai - bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap> operator*(const series::Series<T>& a, const LayeredRaster<T2,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai * bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap> operator/(const series::Series<T>& a, const LayeredRaster<T2,Tgrid,Tmap>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai / bi; }, a, b);
	}

	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap> operator+(const LayeredRaster<T,Tgrid,Tmap>& a, const series::Series<T2>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai + bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap> operator-(const LayeredRaster<T,Tgrid,Tmap>& a, const series::Series<T2>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai - bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap> operator*(const LayeredRaster<T,Tgrid,Tmap>& a, const series::Series<T2>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai * bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap, class T, class T2>
	inline LayeredRaster<T,Tgrid,Tmap> operator/(const LayeredRaster<T,Tgrid,Tmap>& a, const series::Series<T2>& b)
	{
		return rasters::transform([](T ai, T2 bi){ return ai / bi; }, a, b);
	}


	template <class Tgrid, rasters::mapping Tmap>
	inline LayeredRaster<bool,Tgrid,Tmap> operator~(const LayeredRaster<bool,Tgrid,Tmap>& a)
	{
		LayeredRaster<bool,Tgrid,Tmap> out = LayeredRaster<bool,Tgrid,Tmap>(a.size());
		rasters::transform(a,[](bool ai){ return !ai; },  out);
		return out;
	}


	template <class Tgrid, rasters::mapping Tmap>
	inline LayeredRaster<bool,Tgrid,Tmap> operator|(const LayeredRaster<bool,Tgrid,Tmap>& a, const bool b)
	{
		return rasters::transform([](bool ai, bool bi){ return ai || bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap>
	inline LayeredRaster<bool,Tgrid,Tmap> operator&(const LayeredRaster<bool,Tgrid,Tmap>& a, const bool b)
	{
		return rasters::transform([](bool ai, bool bi){ return ai && bi; }, a, b);
	}

	template <class Tgrid, rasters::mapping Tmap>
	inline LayeredRaster<bool,Tgrid,Tmap> operator|(const LayeredRaster<bool,Tgrid,Tmap>& a, const LayeredRaster<bool,Tgrid,Tmap>& b)
	{
		return rasters::transform([](bool ai, bool bi){ return ai || bi; }, a, b);
	}
	template <class Tgrid, rasters::mapping Tmap>
	inline LayeredRaster<bool,Tgrid,Tmap> operator&(const LayeredRaster<bool,Tgrid,Tmap>& a, const LayeredRaster<bool,Tgrid,Tmap>& b)
	{
		return rasters::transform([](bool ai, bool bi){ return ai && bi; }, a, b);
	}




	template <class Tgrid, rasters::mapping Tmap>
	inline LayeredRaster<bool,Tgrid,Tmap>& operator|=(LayeredRaster<bool,Tgrid,Tmap>& a, const bool b){
		rasters::transform(a,[](bool ai, bool bi){ return ai || bi; },  b, a);
		return a;
	}
	template <class Tgrid, rasters::mapping Tmap>
	inline LayeredRaster<bool,Tgrid,Tmap>& operator&=(LayeredRaster<bool,Tgrid,Tmap>& a, const bool b){
		rasters::transform(a,[](bool ai, bool bi){ return ai &&  bi; },  b, a);
		return a;
	}

	template <class Tgrid, rasters::mapping Tmap>
	inline LayeredRaster<bool,Tgrid,Tmap>& operator|=(LayeredRaster<bool,Tgrid,Tmap>& a, const LayeredRaster<bool,Tgrid,Tmap>& b){
		rasters::transform(a,[](bool ai, bool bi){ return ai || bi; },  b, a);
		return a;
	}
	template <class Tgrid, rasters::mapping Tmap>
	inline LayeredRaster<bool,Tgrid,Tmap>& operator&=(LayeredRaster<bool,Tgrid,Tmap>& a, const LayeredRaster<bool,Tgrid,Tmap>& b){
		rasters::transform(a,[](bool ai, bool bi){ return ai &&  bi; },  b, a);
		return a;
	}
}