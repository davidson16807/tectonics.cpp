#pragma once

#include <cmath>

#include "../many.hpp"

/*
"convenience.hpp" contains functions that pass output as return values rather than using using output parameters
This provides convenience at the expense of performance, since now we have to call constructors for the new objects. 
See https://codeyarns.com/2010/10/21/c-return-value-versus-output-parameter/ for more info.
It is important to keep these functions separate from the rest of the library for two reasons:
 1.) It encourages good practice, since you have to explicitly opt-in to less performant convenience functions.
 2.) It provides a nice itemization of functions that will have to be created if you subclass many<T> (as we do within the rasters library)
*/

namespace composites
{
	// NOTE: Here we have convenience functions that are stand-ins for operators
	//  we do this because there are no operators that can express them succinctly

	// NOTE: all operators and convenience functions are marked inline,
	//  because they are thin wrappers of static functions



	template <length_t L, class T, qualifier Q>
	inline many<T> get_x(const many<vec<L,T,Q>>& a)
	{
		return transform(a, [](vec<L,T,Q> ai){ return ai.x; });
	}

	template <length_t L, class T, qualifier Q>
	inline many<T> get_y(const many<vec<L,T,Q>>& a)
	{
		return transform(a, [](vec<L,T,Q> ai){ return ai.y; });
	}

	template <length_t L, class T, qualifier Q>
	inline many<T> get_z(const many<vec<L,T,Q>>& a)
	{
		return transform(a, [](vec<L,T,Q> ai){ return ai.z; });
	}

	
	template<length_t L, typename T, qualifier Q>
	inline many<T> dot (const many<vec<L,T,Q>>& u, const vec<L,T,Q> v ) 
	{
		vec<L,T,Q> out = vec<L,T,Q>(u.size());
		dot(u, v, out);
		return out;
	}
	template<typename T, qualifier Q>
	inline many<vec<3,T,Q>> cross (const many<vec<3,T,Q>>& u, const vec<3,T,Q> v ) 
	{
		many<vec<3,T,Q>> out = many<vec<3,T,Q>>(u.size());
		cross(u, v, out);
		return out;
	}
	template<typename T, qualifier Q>
	inline many<float> cross (const many<vec<2,T,Q>>& u, const vec<2,T,Q> v ) 
	{
		many<float> out = many<float>(u.size());
		cross(u, v, out);
		return out;
	}
	template<length_t L, typename T, qualifier Q>
	inline many<T> distance(const many<vec<L,T,Q>>& u, const vec<L,T,Q> v ) 
	{
		many<T> out = many<T>(u.size());
		distance(u, v, out);
		return out;
	}
	template<length_t L, typename T, qualifier Q>
	inline many<T> dot (const many<vec<L,T,Q>>& u, const many<vec<L,T,Q>>& v ) 
	{
		vec<L,T,Q> out = vec<L,T,Q>(u.size());
		dot(u, v, out);
		return out;
	}
	template<length_t L, typename T, qualifier Q>
	inline many<vec<L,T,Q>> cross (const many<vec<L,T,Q>>& u, const many<vec<L,T,Q>>& v ) 
	{
		many<vec<L,T,Q>> out = many<vec<L,T,Q>>(u.size());
		cross(u, v, out);
		return out;
	}
	template<length_t L, typename T, qualifier Q>
	inline many<T> distance(const many<vec<L,T,Q>>& u, const many<vec<L,T,Q>>& v ) 
	{
		many<T> out = many<T>(u.size());
		distance(u, v, out);
		return out;
	}
	template<length_t L, typename T, qualifier Q>
	inline many<vec<L,T,Q>> normalize(const many<vec<L,T,Q>>& u) 
	{
		many<vec<L,T,Q>> out = many<vec<L,T,Q>>(u.size());
		normalize(u, out);
		return out;
	}
	template<length_t L, typename T, qualifier Q>
	inline many<T> length(const many<vec<L,T,Q>>& u) 
	{
		many<T> out = many<T>(u.size());
		length(u, out);
		return out;
	}
} // end namespace