#pragma once

#include <cmath>

#include <many/relational.hpp>
#include <many/common.hpp>
#include <many/glm/geometric.hpp>

#include "layered_raster.hpp"

/*
"convenience.hpp" contains functions that pass output as return values rather than using using output parameters
This provides convenience at the expense of performance, since now we have to call constructors for the new objects. 
See https://codeyarns.com/2010/10/21/c-return-value-versus-output-parameter/ for more info.
It is important to keep these functions separate from the rest of the library for two reasons:
 1.) It encourages good practice, since you have to explicitly opt-in to less performant convenience functions.
 2.) It provides a nice itemization of functions that will have to be created if you subclass layered_raster<T,Tgrid,Tmap> 
*/

namespace rasters
{
	template<class Tgrid, rasters::mapping Tmap, class T1, typename F>
	inline layered_raster<T1,Tgrid,Tmap> transform(const F f, const layered_raster<T1,Tgrid,Tmap>& a)
	{
		layered_raster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a); 
		return out;
	}


	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, typename F, 
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline layered_raster<T1,Tgrid,Tmap> transform(const F f, const layered_raster<T1,Tgrid,Tmap>& a, const layered_raster<T2,Tgrid>& b)
	{
		layered_raster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, typename F, 
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T2>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline layered_raster<T1,Tgrid,Tmap> transform(const F f, const layered_raster<T1,Tgrid,Tmap>& a, const T2 b)
	{
		layered_raster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, typename F, 
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T1>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline layered_raster<T1,Tgrid,Tmap> transform(const F f, const T1 a, const layered_raster<T2,Tgrid>& b)
	{
		auto out = b;
		out.store(f, a, b); 
		return out;
	}




	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F>
	inline layered_raster<T1,Tgrid,Tmap> transform(const F f, const layered_raster<T1,Tgrid,Tmap>& a, const layered_raster<T2,Tgrid>& b, const layered_raster<T3,Tgrid>& c)
	{
		layered_raster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F, 
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T3>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline layered_raster<T1,Tgrid,Tmap> transform(const F f, const layered_raster<T1,Tgrid,Tmap>& a, const layered_raster<T2,Tgrid>& b, const T3 c)
	{
		layered_raster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F, 
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T2>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline layered_raster<T1,Tgrid,Tmap> transform(const F f, const layered_raster<T1,Tgrid,Tmap>& a, const T2 b, const layered_raster<T3,Tgrid>& c)
	{
		layered_raster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F,
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T2>::value && !std::is_base_of<many::AbstractSeries, T3>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline layered_raster<T1,Tgrid,Tmap> transform(const F f, const layered_raster<T1,Tgrid,Tmap>& a, const T2 b, const T3 c)
	{
		layered_raster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F, 
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T1>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline layered_raster<T1,Tgrid,Tmap> transform(const F f, const T1 a, const layered_raster<T2,Tgrid>& b, const layered_raster<T3,Tgrid>& c)
	{
		auto out = b;
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F,
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T1>::value && !std::is_base_of<many::AbstractSeries, T3>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline layered_raster<T1,Tgrid,Tmap> transform(const F f, const T1 a, const layered_raster<T2,Tgrid>& b, const T3 c)
	{
		auto out = b;
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F,
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T1>::value && !std::is_base_of<many::AbstractSeries, T2>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline layered_raster<T1,Tgrid,Tmap> transform(const F f, const T1 a, const T2 b, const layered_raster<T3,Tgrid>& c)
	{
		auto out = c;
		out.store(f, a, b, c); 
		return out;
	}





	template<class Tgrid, rasters::mapping Tmap, class T, typename Taggregator>
	layered_raster<T,Tgrid,Tmap> aggregate(const layered_raster<T,Tgrid,Tmap>& a, const layered_raster<unsigned int,Tgrid>& group_ids, Taggregator aggregator)
	{
		layered_raster<T,Tgrid,Tmap> group_out = layered_raster<T,Tgrid,Tmap>(many::max(group_ids));
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]], a[i]);
		}
		return group_out;
	}

	template<class Tgrid, rasters::mapping Tmap, class T, typename Taggregator>
	layered_raster<T,Tgrid,Tmap> aggregate(const layered_raster<unsigned int,Tgrid>& group_ids, Taggregator aggregator)
	{
		layered_raster<T,Tgrid,Tmap> group_out = layered_raster<T,Tgrid,Tmap>(many::max(group_ids));
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]]);
		}
		return group_out;
	}


	/// Returns x if x >= 0; otherwise, it returns -x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	layered_raster<T,Tgrid,Tmap> abs(const layered_raster<T,Tgrid,Tmap>& a)
	{
		return transform( [](T ai){ return ai >= 0? ai : -ai; }, a);
	}

	/// Returns 1.0 if x > 0, 0.0 if x == 0, or -1.0 if x < 0.
	template<class Tgrid, rasters::mapping Tmap, class T>
	layered_raster<T,Tgrid,Tmap> sign(const layered_raster<T,Tgrid,Tmap>& a)
	{
		return transform( [](T ai){ return (T(0) < ai) - (ai < T(0)); }, a);
	}

	/// Returns a value equal to the nearest integer that is less then or equal to x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	layered_raster<T,Tgrid,Tmap> floor(const layered_raster<T,Tgrid,Tmap>& a)
	{
		return transform(std::floor, a);
	}

	/// Returns a value equal to the nearest integer to x
	/// whose absolute value is not larger than the absolute value of x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	layered_raster<T,Tgrid,Tmap> trunc(const layered_raster<T,Tgrid,Tmap>& a)
	{
		return transform(std::trunc, a);
	}

	/// Returns a value equal to the nearest integer to x.
	/// The fraction 0.5 will round in a direction chosen by the
	/// implementation, presumably the direction that is fastest.
	/// This includes the possibility that round(x) returns the
	/// same value as roundEven(x) for all values of x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	layered_raster<T,Tgrid,Tmap> round(const layered_raster<T,Tgrid,Tmap>& a)
	{
		return transform(std::round, a);
	}

	/// Returns a value equal to the nearest integer
	/// that is greater than or equal to x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	layered_raster<T,Tgrid,Tmap> ceil(const layered_raster<T,Tgrid,Tmap>& a)
	{
		return transform(std::ceil, a);
	}



	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, class T, glm::qualifier Q>
	inline layered_raster<T,Tgrid,Tmap> get_x(const layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>& a)
	{
		layered_raster<T,Tgrid,Tmap> out(a.size());
		out.store([](glm::vec<L,T,Q> ai){ return ai.x; }, a);
		return out;
	}

	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, class T, glm::qualifier Q>
	inline layered_raster<T,Tgrid,Tmap> get_y(const layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>& a)
	{
		layered_raster<T,Tgrid,Tmap> out(a.size());
		out.store([](glm::vec<L,T,Q> ai){ return ai.y; }, a);
		return out;
	}

	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, class T, glm::qualifier Q>
	inline layered_raster<T,Tgrid,Tmap> get_z(const layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>& a)
	{
		layered_raster<T,Tgrid,Tmap> out(a.size());
		out.store([](glm::vec<L,T,Q> ai){ return ai.z; }, a);
		return out;
	}

	
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline layered_raster<T,Tgrid,Tmap> dot (const layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const glm::vec<L,T,Q> v ) 
	{
		layered_raster<T,Tgrid,Tmap> out(u.size());
		many::dot(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, typename T, glm::qualifier Q>
	inline layered_raster<glm::vec<3,T,Q>,Tgrid> cross (const layered_raster<glm::vec<3,T,Q>,Tgrid>& u, const glm::vec<3,T,Q> v ) 
	{
		layered_raster<glm::vec<3,T,Q>,Tgrid> out = layered_raster<glm::vec<3,T,Q>,Tgrid>(u.size());
		many::cross(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, typename T, glm::qualifier Q>
	inline layered_raster<float,Tgrid> cross (const layered_raster<glm::vec<2,T,Q>,Tgrid>& u, const glm::vec<2,T,Q> v ) 
	{
		layered_raster<float,Tgrid> out = layered_raster<float,Tgrid>(u.size());
		many::cross(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline layered_raster<T,Tgrid,Tmap> distance(const layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const glm::vec<L,T,Q> v ) 
	{
		layered_raster<T,Tgrid,Tmap> out(u.size());
		many::distance(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline layered_raster<T,Tgrid,Tmap> dot (const layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>& v ) 
	{
		layered_raster<T,Tgrid,Tmap> out(u.size());
		many::dot(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap> cross (const layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>& v ) 
	{
		layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap> out = layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>(u.size());
		many::cross(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline layered_raster<T,Tgrid,Tmap> distance(const layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>& v ) 
	{
		layered_raster<T,Tgrid,Tmap> out(u.size());
		many::distance(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap> normalize(const layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>& u) 
	{
		layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap> out = layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>(u.size());
		many::normalize(u, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline layered_raster<T,Tgrid,Tmap> length(const layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>& u) 
	{
		layered_raster<T,Tgrid,Tmap> out(u.size());
		many::length(u, out);
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
	std::ostream &operator<<(std::ostream &os, const layered_raster<glm::vec<L,T,Q>,Tgrid,Tmap>& a) { 
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