#pragma once

#include <cmath>

#include "../composite.hpp"

/*
"convenience.hpp" contains functions that pass output as return values rather than using output parameters
This provides convenience at the expense of performance, since now we have to call constructors for the new objects. 
See https://codeyarns.com/2010/10/21/c-return-value-versus-output-parameter/ for more info.
It is important to keep these functions separate from the rest of the library for two reasons:
 1.) It encourages good practice, since you have to explicitly opt-in to less performant convenience functions.
 2.) It provides a nice itemization of functions that will have to be created if you subclass composite<T> (as we do within the rasters library)
*/

namespace many
{
	// NOTE: Here we have convenience functions that are stand-ins for operators
	//  we do this because there are no operators that can express them succinctly

	// NOTE: all operators and convenience functions are marked inline,
	//  because they are thin wrappers of static functions



	template <glm::length_t L, class T, glm::qualifier Q>
	inline composite<T> get_x(const composite<glm::vec<L,T,Q>>& a)
	{
		return transform(a, [](glm::vec<L,T,Q> ai){ return ai.x; });
	}

	template <glm::length_t L, class T, glm::qualifier Q>
	inline composite<T> get_y(const composite<glm::vec<L,T,Q>>& a)
	{
		return transform(a, [](glm::vec<L,T,Q> ai){ return ai.y; });
	}

	template <glm::length_t L, class T, glm::qualifier Q>
	inline composite<T> get_z(const composite<glm::vec<L,T,Q>>& a)
	{
		return transform(a, [](glm::vec<L,T,Q> ai){ return ai.z; });
	}

	
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline composite<T> dot (const composite<glm::vec<L,T,Q>>& u, const glm::vec<L,T,Q> v ) 
	{
		glm::vec<L,T,Q> out = glm::vec<L,T,Q>(u.size());
		dot(u, v, out);
		return out;
	}
	template<typename T, glm::qualifier Q>
	inline composite<glm::vec<3,T,Q>> cross (const composite<glm::vec<3,T,Q>>& u, const glm::vec<3,T,Q> v ) 
	{
		composite<glm::vec<3,T,Q>> out = composite<glm::vec<3,T,Q>>(u.size());
		cross(u, v, out);
		return out;
	}
	template<typename T, glm::qualifier Q>
	inline composite<float> cross (const composite<glm::vec<2,T,Q>>& u, const glm::vec<2,T,Q> v ) 
	{
		composite<float> out = composite<float>(u.size());
		cross(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline composite<T> distance(const composite<glm::vec<L,T,Q>>& u, const glm::vec<L,T,Q> v ) 
	{
		composite<T> out = composite<T>(u.size());
		distance(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline composite<T> dot (const composite<glm::vec<L,T,Q>>& u, const composite<glm::vec<L,T,Q>>& v ) 
	{
		glm::vec<L,T,Q> out = glm::vec<L,T,Q>(u.size());
		dot(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline composite<glm::vec<L,T,Q>> cross (const composite<glm::vec<L,T,Q>>& u, const composite<glm::vec<L,T,Q>>& v ) 
	{
		composite<glm::vec<L,T,Q>> out = composite<glm::vec<L,T,Q>>(u.size());
		cross(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline composite<T> distance(const composite<glm::vec<L,T,Q>>& u, const composite<glm::vec<L,T,Q>>& v ) 
	{
		composite<T> out = composite<T>(u.size());
		distance(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline composite<glm::vec<L,T,Q>> normalize(const composite<glm::vec<L,T,Q>>& u) 
	{
		composite<glm::vec<L,T,Q>> out = composite<glm::vec<L,T,Q>>(u.size());
		normalize(u, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline composite<T> length(const composite<glm::vec<L,T,Q>>& u) 
	{
		composite<T> out = composite<T>(u.size());
		length(u, out);
		return out;
	}
} // end namespace