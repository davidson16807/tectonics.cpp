#pragma once

#include <cmath>

#include <series/relational.hpp>
#include <series/common.hpp>
#include <series/glm/geometric.hpp>

#include "LayeredRaster.hpp"

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
	template<class Tgrid, rasters::mapping Tmap, class T1, typename F>
	inline LayeredRaster<T1,Tgrid,Tmap> transform(const F f, const LayeredRaster<T1,Tgrid,Tmap>& a)
	{
		LayeredRaster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a); 
		return out;
	}


	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, typename F, 
		std::enable_if_t<!std::is_base_of<series::AbstractSeries, F>::value, int> = 0>
	inline LayeredRaster<T1,Tgrid,Tmap> transform(const F f, const LayeredRaster<T1,Tgrid,Tmap>& a, const LayeredRaster<T2,Tgrid>& b)
	{
		LayeredRaster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, typename F, 
		std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value && !std::is_base_of<series::AbstractSeries, F>::value, int> = 0>
	inline LayeredRaster<T1,Tgrid,Tmap> transform(const F f, const LayeredRaster<T1,Tgrid,Tmap>& a, const T2 b)
	{
		LayeredRaster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, typename F, 
		std::enable_if_t<!std::is_base_of<series::AbstractSeries, T1>::value && !std::is_base_of<series::AbstractSeries, F>::value, int> = 0>
	inline LayeredRaster<T1,Tgrid,Tmap> transform(const F f, const T1 a, const LayeredRaster<T2,Tgrid>& b)
	{
		auto out = b;
		out.store(f, a, b); 
		return out;
	}




	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F>
	inline LayeredRaster<T1,Tgrid,Tmap> transform(const F f, const LayeredRaster<T1,Tgrid,Tmap>& a, const LayeredRaster<T2,Tgrid>& b, const LayeredRaster<T3,Tgrid>& c)
	{
		LayeredRaster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F, 
		std::enable_if_t<!std::is_base_of<series::AbstractSeries, T3>::value && !std::is_base_of<series::AbstractSeries, F>::value, int> = 0>
	inline LayeredRaster<T1,Tgrid,Tmap> transform(const F f, const LayeredRaster<T1,Tgrid,Tmap>& a, const LayeredRaster<T2,Tgrid>& b, const T3 c)
	{
		LayeredRaster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F, 
		std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value && !std::is_base_of<series::AbstractSeries, F>::value, int> = 0>
	inline LayeredRaster<T1,Tgrid,Tmap> transform(const F f, const LayeredRaster<T1,Tgrid,Tmap>& a, const T2 b, const LayeredRaster<T3,Tgrid>& c)
	{
		LayeredRaster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F,
		std::enable_if_t<!std::is_base_of<series::AbstractSeries, T2>::value && !std::is_base_of<series::AbstractSeries, T3>::value && !std::is_base_of<series::AbstractSeries, F>::value, int> = 0>
	inline LayeredRaster<T1,Tgrid,Tmap> transform(const F f, const LayeredRaster<T1,Tgrid,Tmap>& a, const T2 b, const T3 c)
	{
		LayeredRaster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F, 
		std::enable_if_t<!std::is_base_of<series::AbstractSeries, T1>::value && !std::is_base_of<series::AbstractSeries, F>::value, int> = 0>
	inline LayeredRaster<T1,Tgrid,Tmap> transform(const F f, const T1 a, const LayeredRaster<T2,Tgrid>& b, const LayeredRaster<T3,Tgrid>& c)
	{
		auto out = b;
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F,
		std::enable_if_t<!std::is_base_of<series::AbstractSeries, T1>::value && !std::is_base_of<series::AbstractSeries, T3>::value && !std::is_base_of<series::AbstractSeries, F>::value, int> = 0>
	inline LayeredRaster<T1,Tgrid,Tmap> transform(const F f, const T1 a, const LayeredRaster<T2,Tgrid>& b, const T3 c)
	{
		auto out = b;
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F,
		std::enable_if_t<!std::is_base_of<series::AbstractSeries, T1>::value && !std::is_base_of<series::AbstractSeries, T2>::value && !std::is_base_of<series::AbstractSeries, F>::value, int> = 0>
	inline LayeredRaster<T1,Tgrid,Tmap> transform(const F f, const T1 a, const T2 b, const LayeredRaster<T3,Tgrid>& c)
	{
		auto out = c;
		out.store(f, a, b, c); 
		return out;
	}





	template<class Tgrid, rasters::mapping Tmap, class T, typename Taggregator>
	LayeredRaster<T,Tgrid,Tmap> aggregate(const LayeredRaster<T,Tgrid,Tmap>& a, const LayeredRaster<unsigned int,Tgrid>& group_ids, Taggregator aggregator)
	{
		LayeredRaster<T,Tgrid,Tmap> group_out = LayeredRaster<T,Tgrid,Tmap>(series::max(group_ids));
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]], a[i]);
		}
		return group_out;
	}

	template<class Tgrid, rasters::mapping Tmap, class T, typename Taggregator>
	LayeredRaster<T,Tgrid,Tmap> aggregate(const LayeredRaster<unsigned int,Tgrid>& group_ids, Taggregator aggregator)
	{
		LayeredRaster<T,Tgrid,Tmap> group_out = LayeredRaster<T,Tgrid,Tmap>(series::max(group_ids));
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]]);
		}
		return group_out;
	}


	/// Returns x if x >= 0; otherwise, it returns -x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	LayeredRaster<T,Tgrid,Tmap> abs(const LayeredRaster<T,Tgrid,Tmap>& a)
	{
		return transform( [](T ai){ return ai >= 0? ai : -ai; }, a);
	}

	/// Returns 1.0 if x > 0, 0.0 if x == 0, or -1.0 if x < 0.
	template<class Tgrid, rasters::mapping Tmap, class T>
	LayeredRaster<T,Tgrid,Tmap> sign(const LayeredRaster<T,Tgrid,Tmap>& a)
	{
		return transform( [](T ai){ return (T(0) < ai) - (ai < T(0)); }, a);
	}

	/// Returns a value equal to the nearest integer that is less then or equal to x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	LayeredRaster<T,Tgrid,Tmap> floor(const LayeredRaster<T,Tgrid,Tmap>& a)
	{
		return transform(std::floor, a);
	}

	/// Returns a value equal to the nearest integer to x
	/// whose absolute value is not larger than the absolute value of x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	LayeredRaster<T,Tgrid,Tmap> trunc(const LayeredRaster<T,Tgrid,Tmap>& a)
	{
		return transform(std::trunc, a);
	}

	/// Returns a value equal to the nearest integer to x.
	/// The fraction 0.5 will round in a direction chosen by the
	/// implementation, presumably the direction that is fastest.
	/// This includes the possibility that round(x) returns the
	/// same value as roundEven(x) for all values of x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	LayeredRaster<T,Tgrid,Tmap> round(const LayeredRaster<T,Tgrid,Tmap>& a)
	{
		return transform(std::round, a);
	}

	/// Returns a value equal to the nearest integer
	/// that is greater than or equal to x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	LayeredRaster<T,Tgrid,Tmap> ceil(const LayeredRaster<T,Tgrid,Tmap>& a)
	{
		return transform(std::ceil, a);
	}



	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, class T, glm::qualifier Q>
	inline LayeredRaster<T,Tgrid,Tmap> get_x(const LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& a)
	{
		LayeredRaster<T,Tgrid,Tmap> out(a.size());
		out.store([](glm::vec<L,T,Q> ai){ return ai.x; }, a);
		return out;
	}

	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, class T, glm::qualifier Q>
	inline LayeredRaster<T,Tgrid,Tmap> get_y(const LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& a)
	{
		LayeredRaster<T,Tgrid,Tmap> out(a.size());
		out.store([](glm::vec<L,T,Q> ai){ return ai.y; }, a);
		return out;
	}

	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, class T, glm::qualifier Q>
	inline LayeredRaster<T,Tgrid,Tmap> get_z(const LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& a)
	{
		LayeredRaster<T,Tgrid,Tmap> out(a.size());
		out.store([](glm::vec<L,T,Q> ai){ return ai.z; }, a);
		return out;
	}

	
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline LayeredRaster<T,Tgrid,Tmap> dot (const LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const glm::vec<L,T,Q> v ) 
	{
		LayeredRaster<T,Tgrid,Tmap> out(u.size());
		series::dot(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, typename T, glm::qualifier Q>
	inline LayeredRaster<glm::vec<3,T,Q>,Tgrid> cross (const LayeredRaster<glm::vec<3,T,Q>,Tgrid>& u, const glm::vec<3,T,Q> v ) 
	{
		LayeredRaster<glm::vec<3,T,Q>,Tgrid> out = LayeredRaster<glm::vec<3,T,Q>,Tgrid>(u.size());
		series::cross(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, typename T, glm::qualifier Q>
	inline LayeredRaster<float,Tgrid> cross (const LayeredRaster<glm::vec<2,T,Q>,Tgrid>& u, const glm::vec<2,T,Q> v ) 
	{
		LayeredRaster<float,Tgrid> out = LayeredRaster<float,Tgrid>(u.size());
		series::cross(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline LayeredRaster<T,Tgrid,Tmap> distance(const LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const glm::vec<L,T,Q> v ) 
	{
		LayeredRaster<T,Tgrid,Tmap> out(u.size());
		series::distance(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline LayeredRaster<T,Tgrid,Tmap> dot (const LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& v ) 
	{
		LayeredRaster<T,Tgrid,Tmap> out(u.size());
		series::dot(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap> cross (const LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& v ) 
	{
		LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap> out = LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>(u.size());
		series::cross(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline LayeredRaster<T,Tgrid,Tmap> distance(const LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& v ) 
	{
		LayeredRaster<T,Tgrid,Tmap> out(u.size());
		series::distance(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap> normalize(const LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& u) 
	{
		LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap> out = LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>(u.size());
		series::normalize(u, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline LayeredRaster<T,Tgrid,Tmap> length(const LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& u) 
	{
		LayeredRaster<T,Tgrid,Tmap> out(u.size());
		series::length(u, out);
		return out;
	}


/*
	template<class Tgrid, rasters::mapping Tmap, typename T, glm::qualifier Q>
	std::ostream &operator<< (std::ostream &out, const glm::vec<3,T,Q> &vec) {
	    out << "[" 
	        << vec.x << "," << vec.y << ","<< vec.z 
	        << "]";

	    return out;
	}
	template<class Tgrid, rasters::mapping Tmap, typename T, glm::qualifier Q>
	std::ostream &operator<< (std::ostream &out, const glm::vec<2,T,Q> &vec) {
	    out << "[" 
	        << vec.x << "," << vec.y 
	        << "]";

	    return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	std::ostream &operator<<(std::ostream &os, const LayeredRaster<glm::vec<L,T,Q>,Tgrid,Tmap>& a) { 
		os << "[";
		for (unsigned int i = 0; i < a.size(); ++i)
		{
		    os << a[i] << " ";
		}
		os << "]";
		return os;
	}
*/





}