#pragma once

#include <cmath>

#include <many/relational.hpp>
#include <many/common.hpp>
#include <many/glm/geometric.hpp>
#include "raster.hpp"

/*
"convenience.hpp" contains functions that pass output as return values rather than using using output parameters
This provides convenience at the expense of performance, since now we have to call constructors for the new objects. 
See https://codeyarns.com/2010/10/21/c-return-value-versus-output-parameter/ for more info.
It is important to keep these functions separate from the rest of the library for two reasons:
 1.) It encourages good practice, since you have to explicitly opt-in to less performant convenience functions.
 2.) It provides a nice itemization of functions that will have to be created if you subclass raster<T,Tgrid,Tmap> 
*/

namespace rasters
{
	template<class Tgrid, rasters::mapping Tmap, class T1, typename F>
	inline raster<T1,Tgrid,Tmap> transform(const raster<T1,Tgrid,Tmap>& a, F f)
	{
		raster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a); 
		return out;
	}


	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, typename F, 
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline raster<T1,Tgrid,Tmap> transform(const raster<T1,Tgrid,Tmap>& a, const raster<T2,Tgrid>& b, F f)
	{
		raster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, typename F, 
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T2>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline raster<T1,Tgrid,Tmap> transform(const raster<T1,Tgrid,Tmap>& a, const T2 b, F f)
	{
		raster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, typename F, 
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T1>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline raster<T1,Tgrid,Tmap> transform(const T1 a, const raster<T2,Tgrid>& b, F f)
	{
		auto out = b;
		out.store(f, a, b); 
		return out;
	}




	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F>
	inline raster<T1,Tgrid,Tmap> transform(const raster<T1,Tgrid,Tmap>& a, const raster<T2,Tgrid>& b, const raster<T3,Tgrid>& c, F f)
	{
		raster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F, 
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T3>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline raster<T1,Tgrid,Tmap> transform(const raster<T1,Tgrid,Tmap>& a, const raster<T2,Tgrid>& b, const T3 c, F f)
	{
		raster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F, 
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T2>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline raster<T1,Tgrid,Tmap> transform(const raster<T1,Tgrid,Tmap>& a, const T2 b, const raster<T3,Tgrid>& c, F f)
	{
		raster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F,
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T2>::value && !std::is_base_of<many::AbstractSeries, T3>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline raster<T1,Tgrid,Tmap> transform(const raster<T1,Tgrid,Tmap>& a, const T2 b, const T3 c, F f)
	{
		raster<T1,Tgrid,Tmap> out(a.grid);
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F, 
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T1>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline raster<T1,Tgrid,Tmap> transform(const T1 a, const raster<T2,Tgrid>& b, const raster<T3,Tgrid>& c, F f)
	{
		auto out = b;
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F,
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T1>::value && !std::is_base_of<many::AbstractSeries, T3>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline raster<T1,Tgrid,Tmap> transform(const T1 a, const raster<T2,Tgrid>& b, const T3 c, F f)
	{
		auto out = b;
		out.store(f, a, b, c); 
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, class T1, class T2, class T3, typename F,
		std::enable_if_t<!std::is_base_of<many::AbstractSeries, T1>::value && !std::is_base_of<many::AbstractSeries, T2>::value && !std::is_base_of<many::AbstractSeries, F>::value, int> = 0>
	inline raster<T1,Tgrid,Tmap> transform(const T1 a, const T2 b, const raster<T3,Tgrid>& c, F f)
	{
		auto out = c;
		out.store(f, a, b, c); 
		return out;
	}





	template<class Tgrid, rasters::mapping Tmap, class T, typename Taggregator>
	raster<T,Tgrid,Tmap> aggregate(const raster<T,Tgrid,Tmap>& a, const raster<unsigned int,Tgrid>& group_ids, Taggregator aggregator)
	{
		raster<T,Tgrid,Tmap> group_out = raster<T,Tgrid,Tmap>(many::max(group_ids));
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]], a[i]);
		}
		return group_out;
	}

	template<class Tgrid, rasters::mapping Tmap, class T, typename Taggregator>
	raster<T,Tgrid,Tmap> aggregate(const raster<unsigned int,Tgrid>& group_ids, Taggregator aggregator)
	{
		raster<T,Tgrid,Tmap> group_out = raster<T,Tgrid,Tmap>(many::max(group_ids));
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]]);
		}
		return group_out;
	}


	/// Returns x if x >= 0; otherwise, it returns -x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	raster<T,Tgrid,Tmap> abs(const raster<T,Tgrid,Tmap>& a)
	{
		return transform([](T ai){ return ai >= 0? ai : -ai; }, a);
	}

	/// Returns 1.0 if x > 0, 0.0 if x == 0, or -1.0 if x < 0.
	template<class Tgrid, rasters::mapping Tmap, class T>
	raster<T,Tgrid,Tmap> sign(const raster<T,Tgrid,Tmap>& a)
	{
		return transform([](T ai){ return (T(0) < ai) - (ai < T(0)); }, a);
	}

	/// Returns a value equal to the nearest integer that is less then or equal to x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	raster<T,Tgrid,Tmap> floor(const raster<T,Tgrid,Tmap>& a)
	{
		return transform(std::floor, a);
	}

	/// Returns a value equal to the nearest integer to x
	/// whose absolute value is not larger than the absolute value of x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	raster<T,Tgrid,Tmap> trunc(const raster<T,Tgrid,Tmap>& a)
	{
		return transform(std::trunc, a);
	}

	/// Returns a value equal to the nearest integer to x.
	/// The fraction 0.5 will round in a direction chosen by the
	/// implementation, presumably the direction that is fastest.
	/// This includes the possibility that round(x) returns the
	/// same value as roundEven(x) for all values of x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	raster<T,Tgrid,Tmap> round(const raster<T,Tgrid,Tmap>& a)
	{
		return transform(std::round, a);
	}

	/// Returns a value equal to the nearest integer
	/// that is greater than or equal to x.
	template<class Tgrid, rasters::mapping Tmap, class T>
	raster<T,Tgrid,Tmap> ceil(const raster<T,Tgrid,Tmap>& a)
	{
		transform(std::ceil, a);
	}



	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, class T, glm::qualifier Q>
	inline raster<T,Tgrid,Tmap> get_x(const raster<glm::vec<L,T,Q>,Tgrid,Tmap>& a)
	{
		raster<T,Tgrid,Tmap> out(a.size());
		out.store([](glm::vec<L,T,Q> ai){ return ai.x; }, a);
		return out;
	}

	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, class T, glm::qualifier Q>
	inline raster<T,Tgrid,Tmap> get_y(const raster<glm::vec<L,T,Q>,Tgrid,Tmap>& a)
	{
		raster<T,Tgrid,Tmap> out(a.size());
		out.store([](glm::vec<L,T,Q> ai){ return ai.y; }, a);
		return out;
	}

	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, class T, glm::qualifier Q>
	inline raster<T,Tgrid,Tmap> get_z(const raster<glm::vec<L,T,Q>,Tgrid,Tmap>& a)
	{
		raster<T,Tgrid,Tmap> out(a.size());
		out.store([](glm::vec<L,T,Q> ai){ return ai.z; }, a);
		return out;
	}

	
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline raster<T,Tgrid,Tmap> dot (const raster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const glm::vec<L,T,Q> v ) 
	{
		raster<T,Tgrid,Tmap> out(u.size());
		many::dot(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, typename T, glm::qualifier Q>
	inline raster<glm::vec<3,T,Q>,Tgrid> cross (const raster<glm::vec<3,T,Q>,Tgrid>& u, const glm::vec<3,T,Q> v ) 
	{
		raster<glm::vec<3,T,Q>,Tgrid> out = raster<glm::vec<3,T,Q>,Tgrid>(u.size());
		many::cross(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, typename T, glm::qualifier Q>
	inline raster<float,Tgrid> cross (const raster<glm::vec<2,T,Q>,Tgrid>& u, const glm::vec<2,T,Q> v ) 
	{
		raster<float,Tgrid> out = raster<float,Tgrid>(u.size());
		many::cross(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline raster<T,Tgrid,Tmap> distance(const raster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const glm::vec<L,T,Q> v ) 
	{
		raster<T,Tgrid,Tmap> out(u.size());
		many::distance(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline raster<T,Tgrid,Tmap> dot (const raster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const raster<glm::vec<L,T,Q>,Tgrid,Tmap>& v ) 
	{
		raster<T,Tgrid,Tmap> out(u.size());
		many::dot(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline raster<glm::vec<L,T,Q>,Tgrid,Tmap> cross (const raster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const raster<glm::vec<L,T,Q>,Tgrid,Tmap>& v ) 
	{
		raster<glm::vec<L,T,Q>,Tgrid,Tmap> out = raster<glm::vec<L,T,Q>,Tgrid,Tmap>(u.size());
		many::cross(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline raster<T,Tgrid,Tmap> distance(const raster<glm::vec<L,T,Q>,Tgrid,Tmap>& u, const raster<glm::vec<L,T,Q>,Tgrid,Tmap>& v ) 
	{
		raster<T,Tgrid,Tmap> out(u.size());
		many::distance(u, v, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline raster<glm::vec<L,T,Q>,Tgrid,Tmap> normalize(const raster<glm::vec<L,T,Q>,Tgrid,Tmap>& u) 
	{
		raster<glm::vec<L,T,Q>,Tgrid,Tmap> out = raster<glm::vec<L,T,Q>,Tgrid,Tmap>(u.size());
		many::normalize(u, out);
		return out;
	}
	template<class Tgrid, rasters::mapping Tmap, glm::length_t L, typename T, glm::qualifier Q>
	inline raster<T,Tgrid,Tmap> length(const raster<glm::vec<L,T,Q>,Tgrid,Tmap>& u) 
	{
		raster<T,Tgrid,Tmap> out(u.size());
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
	std::ostream &operator<<(std::ostream &os, const raster<glm::vec<L,T,Q>,Tgrid,Tmap>& a) { 
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