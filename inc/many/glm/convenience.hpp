#pragma once

#include <cmath>

#include "../types.hpp"
#include "../convenience.hpp"
#include "relational.hpp"

/*
"convenience.hpp" contains functions that pass output as return values rather than using output parameters
This provides convenience at the expense of performance, since now we have to call constructors for the new objects. 
See https://codeyarns.com/2010/10/21/c-return-value-versus-output-parameter/ for more info.
It is important to keep these functions separate from the rest of the library for two reasons:
 1.) It encourages good practice, since you have to explicitly opt-in to less performant convenience functions.
 2.) It provides a nice itemization of functions that will have to be created if you subclass series<T> (as we do within the rasters library)
*/

namespace many
{
	// NOTE: Here we have convenience functions that are stand-ins for operators
	//  we do this because there are no operators that can express them succinctly

	// NOTE: all operators and convenience functions are marked inline,
	//  because they are thin wrappers of static functions



	template <glm::length_t L, class T, glm::qualifier Q>
	inline series<T> get_x(const series<glm::vec<L,T,Q>>& a)
	{
		series<T> out = series<T>(a.size());
		transform(a, [](glm::vec<L,T,Q> ai){ return ai.x; }, out);
		return out;
	}

	template <glm::length_t L, class T, glm::qualifier Q>
	inline series<T> get_y(const series<glm::vec<L,T,Q>>& a)
	{
		series<T> out = series<T>(a.size());
		transform(a, [](glm::vec<L,T,Q> ai){ return ai.y; }, out);
		return out;
	}

	template <glm::length_t L, class T, glm::qualifier Q>
	inline series<T> get_z(const series<glm::vec<L,T,Q>>& a)
	{
		series<T> out = series<T>(a.size());
		transform(a, [](glm::vec<L,T,Q> ai){ return ai.z; }, out);
		return out;
	}

	
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline series<T> dot (const series<glm::vec<L,T,Q>>& u, const glm::vec<L,T,Q> v ) 
	{
		series<T> out = series<T>(u.size());
		dot(u, v, out);
		return out;
	}
	template<typename T, glm::qualifier Q>
	inline series<glm::vec<3,T,Q>> cross (const series<glm::vec<3,T,Q>>& u, const glm::vec<3,T,Q> v ) 
	{
		series<glm::vec<3,T,Q>> out = series<glm::vec<3,T,Q>>(u.size());
		cross(u, v, out);
		return out;
	}
	template<typename T, glm::qualifier Q>
	inline series<float> cross (const series<glm::vec<2,T,Q>>& u, const glm::vec<2,T,Q> v ) 
	{
		series<float> out = series<float>(u.size());
		cross(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline series<T> distance(const series<glm::vec<L,T,Q>>& u, const glm::vec<L,T,Q> v ) 
	{
		series<T> out = series<T>(u.size());
		distance(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline series<T> dot (const series<glm::vec<L,T,Q>>& u, const series<glm::vec<L,T,Q>>& v ) 
	{
		series<T> out = series<T>(u.size());
		dot(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline series<glm::vec<L,T,Q>> cross (const series<glm::vec<L,T,Q>>& u, const series<glm::vec<L,T,Q>>& v ) 
	{
		series<glm::vec<L,T,Q>> out = series<glm::vec<L,T,Q>>(u.size());
		cross(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline series<T> distance(const series<glm::vec<L,T,Q>>& u, const series<glm::vec<L,T,Q>>& v ) 
	{
		series<T> out = series<T>(u.size());
		distance(u, v, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline series<glm::vec<L,T,Q>> normalize(const series<glm::vec<L,T,Q>>& u) 
	{
		series<glm::vec<L,T,Q>> out = series<glm::vec<L,T,Q>>(u.size());
		normalize(u, out);
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline series<T> length(const series<glm::vec<L,T,Q>>& u) 
	{
		series<T> out = series<T>(u.size());
		length(u, out);
		return out;
	}



	template<typename T, glm::qualifier Q>
	std::ostream &operator<< (std::ostream &out, const glm::vec<3,T,Q> &vec) {
	    out << "[" 
	        << vec.x << "," << vec.y << ","<< vec.z 
	        << "]";

	    return out;
	}
	template<typename T, glm::qualifier Q>
	std::ostream &operator<< (std::ostream &out, const glm::vec<2,T,Q> &vec) {
	    out << "[" 
	        << vec.x << "," << vec.y 
	        << "]";

	    return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	std::ostream &operator<<(std::ostream &os, const series<glm::vec<L,T,Q>>& a) { 
		os << "[";
		for (unsigned int i = 0; i < a.size(); ++i)
		{
		    os << a[i] << " ";
		}
		os << "]";
		return os;
	}





	// NOTE: all operators are suggested to be inline because they are thin wrappers of functions
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline series<glm::vec<L,T,Q>>& operator+=(series<glm::vec<L,T,Q>>& a, const T b) 
	{
		add(a, b, a);
		return a;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline series<glm::vec<L,T,Q>>& operator-=(series<glm::vec<L,T,Q>>& a, const T b) 
	{
		sub(a, b, a);
		return a;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline series<glm::vec<L,T,Q>>& operator*=(series<glm::vec<L,T,Q>>& a, const T b) 
	{
		mult(a, b, a);
		return a;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline series<glm::vec<L,T,Q>>& operator/=(series<glm::vec<L,T,Q>>& a, const T b) 
	{
		div(a, b, a);
		return a;
	}
} // end namespace