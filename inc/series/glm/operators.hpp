#pragma once

#include <cmath>

#include "../types.hpp"
#include "../convenience.hpp"
#include "relational.hpp"

/*
"operators.hpp" contains functions that pass output as return values rather than using output parameters
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
	std::ostream &operator<<(std::ostream &os, const Series<glm::vec<L,T,Q>>& a) { 
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
	inline Series<glm::vec<L,T,Q>>& operator+=(Series<glm::vec<L,T,Q>>& a, const T b) 
	{
		series::add(a, b, a);
		return a;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline Series<glm::vec<L,T,Q>>& operator-=(Series<glm::vec<L,T,Q>>& a, const T b) 
	{
		series::sub(a, b, a);
		return a;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline Series<glm::vec<L,T,Q>>& operator*=(Series<glm::vec<L,T,Q>>& a, const T b) 
	{
		series::mult(a, b, a);
		return a;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline Series<glm::vec<L,T,Q>>& operator/=(Series<glm::vec<L,T,Q>>& a, const T b) 
	{
		series::div(a, b, a);
		return a;
	}
} // end namespace