#pragma once

#include <cmath>

#include "../types.hpp"
#include "../convenience.hpp"
#include "geometric.hpp"

/*
"convenience.hpp" contains functions that pass output as return values rather than using output parameters
This provides convenience at the expense of performance, since now we have to call constructors for the new objects. 
See https://codeyarns.com/2010/10/21/c-return-value-versus-output-parameter/ for more info.
It is important to keep these functions separate from the rest of the library for two reasons:
 1.) It encourages good practice, since you have to explicitly opt-in to less performant convenience functions.
 2.) It provides a nice itemization of functions that will have to be created if you subclass Series<T> (as we do within the rasters library)
*/

namespace series
{
	// NOTE: Here we have convenience functions that are stand-ins for operators
	//  we do this because there are no operators that can express them succinctly

	// NOTE: all operators and convenience functions are marked inline,
	//  because they are thin wrappers of static functions



	template <glm::length_t L, typename T, glm::qualifier Q>
	inline Series<T> get_x(const Series<glm::vec<L,T,Q>>& a)
	{
		Series<T> out(a.size());
		out.store([](glm::vec<L,T,Q> ai){ return ai.x; }, a);
		return out;
	}

	template <glm::length_t L, typename T, glm::qualifier Q>
	inline Series<T> get_y(const Series<glm::vec<L,T,Q>>& a)
	{
		Series<T> out(a.size());
		out.store([](glm::vec<L,T,Q> ai){ return ai.y; }, a);
		return out;
	}

	template <glm::length_t L, typename T, glm::qualifier Q>
	inline Series<T> get_z(const Series<glm::vec<L,T,Q>>& a)
	{
		Series<T> out(a.size());
		out.store([](glm::vec<L,T,Q> ai){ return ai.z; }, a);
		return out;
	}

	
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline Series<T> dot (const Series<glm::vec<L,T,Q>>& u, const glm::vec<L,T,Q> v ) 
	{
		Series<T> out(u.size());
		series::dot(u, v, out);
		return out;
	}
	template<typename T, glm::qualifier Q>
	inline Series<glm::vec<3,T,Q>> cross (const Series<glm::vec<3,T,Q>>& u, const glm::vec<3,T,Q> v ) 
	{
		Series<glm::vec<3,T,Q>> out(u.size());
		series::cross(u, v, out);
		return out;
	}
	template<typename T, glm::qualifier Q>
	inline Series<float> cross (const Series<glm::vec<2,T,Q>>& u, const glm::vec<2,T,Q> v ) 
	{
		Series<float> out(u.size());
		series::cross(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline Series<T> distance(const Series<glm::vec<L,T,Q>>& u, const glm::vec<L,T,Q> v ) 
	{
		Series<T> out(u.size());
		series::distance(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline Series<T> dot (const Series<glm::vec<L,T,Q>>& u, const Series<glm::vec<L,T,Q>>& v ) 
	{
		Series<T> out(u.size());
		series::dot(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline Series<glm::vec<L,T,Q>> cross (const Series<glm::vec<L,T,Q>>& u, const Series<glm::vec<L,T,Q>>& v ) 
	{
		Series<glm::vec<L,T,Q>> out(u.size());
		series::cross(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline Series<T> distance(const Series<glm::vec<L,T,Q>>& u, const Series<glm::vec<L,T,Q>>& v ) 
	{
		Series<T> out(u.size());
		series::distance(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline Series<glm::vec<L,T,Q>> normalize(const Series<glm::vec<L,T,Q>>& u) 
	{
		Series<glm::vec<L,T,Q>> out(u.size());
		series::normalize(u, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline Series<T> length(const Series<glm::vec<L,T,Q>>& u) 
	{
		Series<T> out(u.size());
		series::length(u, out);
		return out;
	}

} // end namespace