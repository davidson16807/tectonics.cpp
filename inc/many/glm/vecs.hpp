#pragma once

#include <iostream>

#include "../composite.hpp"

namespace many
{
	template <glm::length_t L, class T, glm::qualifier Q>
	void get_x(const composite<glm::vec<L,T,Q>>& a, composite<T>& out )
	{
		transform(a, [](glm::vec<L,T,Q> ai){ return ai.x; }, out);
	}

	template <glm::length_t L, class T, glm::qualifier Q>
	void get_y(const composite<glm::vec<L,T,Q>>& a, composite<T>& out )
	{
		transform(a, [](glm::vec<L,T,Q> ai){ return ai.y; }, out);
	}

	template <glm::length_t L, class T, glm::qualifier Q>
	void get_z(const composite<glm::vec<L,T,Q>>& a, composite<T>& out )
	{
		transform(a, [](glm::vec<L,T,Q> ai){ return ai.z; }, out);
	}

	template <glm::length_t L, class T, glm::qualifier Q>
	void set_x(const composite<glm::vec<L,T,Q>>& out, composite<T>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i].x = a[i];
		}
	}

	template <glm::length_t L, class T, glm::qualifier Q>
	void set_y(const composite<glm::vec<L,T,Q>>& out, composite<T>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i].y = a[i];
		}
	}

	template <glm::length_t L, class T, glm::qualifier Q>
	void set_z(const composite<glm::vec<L,T,Q>>& out, composite<T>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i].z = a[i];
		}
	}

	template<typename T, glm::qualifier Q>
	void mult(const composite<glm::vec<3,T,Q>>& a, const glm::mat<4,3,T,Q>& b, composite<glm::vec<3,T,Q>>& out)
	{
		constexpr T one = T(1.);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = b * vec4(a[i], one);
		}
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	bool equal(const composite<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b)
	{
		bool out(true);
		T threshold(many_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= distance(a[i], b) <= threshold;
		}
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	bool notEqual(const composite<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b)
	{
		bool out(false);
		T threshold(many_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= distance(a[i], b) > threshold;
		}
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	bool equal(const composite<glm::vec<L,T,Q>>& a, const composite<glm::vec<L,T,Q>>& b)
	{
		bool out(true);
		T threshold(many_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= distance(a[i], b[i]) <= threshold;
		}
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	bool notEqual(const composite<glm::vec<L,T,Q>>& a, const composite<glm::vec<L,T,Q>>& b)
	{
		bool out(false);
		T threshold(many_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= distance(a[i], b[i]) > threshold;
		}
		return out;
	}



	template<glm::length_t L, typename T, glm::qualifier Q>
	void equal(const composite<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, composite<bool>& out)
	{
		T threshold(many_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = distance(a[i], b) <= threshold;
		}
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void notEqual(const composite<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, composite<bool>& out)
	{
		T threshold(many_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = distance(a[i], b) > threshold;
		}
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void equal(const composite<glm::vec<L,T,Q>>& a, const composite<glm::vec<L,T,Q>>& b, composite<bool>& out)
	{
		T threshold(many_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = distance(a[i], b[i]) <= threshold;
		}
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void notEqual(const composite<glm::vec<L,T,Q>>& a, const composite<glm::vec<L,T,Q>>& b, composite<bool>& out)
	{
		T threshold(many_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = distance(a[i], b[i]) > threshold;
		}
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	std::ostream &operator<< (std::ostream &out, const glm::vec<L,T,Q> &vec) {
	    out << "[" 
	        << vec.x << "," << vec.y << ","<< vec.z 
	        << "]";

	    return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	std::ostream &operator<<(std::ostream &os, const composite<glm::vec<L,T,Q>>& a) { 
		os << "[";
		for (unsigned int i = 0; i < a.size(); ++i)
		{
		    os << a[i] << " ";
		}
		os << "]";
		return os;
	}





	template<glm::length_t L, typename T, glm::qualifier Q>
	inline composite<glm::vec<L,T,Q>>& operator+=(composite<glm::vec<L,T,Q>>& a, const T b) 
	{
		add(a, b, a);
		return a;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline composite<glm::vec<L,T,Q>>& operator-=(composite<glm::vec<L,T,Q>>& a, const T b) 
	{
		sub(a, b, a);
		return a;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline composite<glm::vec<L,T,Q>>& operator*=(composite<glm::vec<L,T,Q>>& a, const T b) 
	{
		mult(a, b, a);
		return a;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline composite<glm::vec<L,T,Q>>& operator/=(composite<glm::vec<L,T,Q>>& a, const T b) 
	{
		div(a, b, a);
		return a;
	}



	


	typedef composite<glm::vec<1, bool, glm::defaultp>>	bvec1s;
	typedef composite<glm::vec<2, bool, glm::defaultp>>	bvec2s;
	typedef composite<glm::vec<3, bool, glm::defaultp>>	bvec3s;
	typedef composite<glm::vec<4, bool, glm::defaultp>>	bvec4s;

	typedef composite<glm::vec<1, int, glm::defaultp>> ivec1s;
	typedef composite<glm::vec<2, int, glm::defaultp>> ivec2s;
	typedef composite<glm::vec<3, int, glm::defaultp>> ivec3s;
	typedef composite<glm::vec<4, int, glm::defaultp>> ivec4s;

	typedef composite<glm::vec<1, unsigned int, glm::defaultp>> uvec1s;
	typedef composite<glm::vec<2, unsigned int, glm::defaultp>> uvec2s;
	typedef composite<glm::vec<3, unsigned int, glm::defaultp>> uvec3s;
	typedef composite<glm::vec<4, unsigned int, glm::defaultp>> uvec4s;

	typedef composite<glm::vec<1, double, glm::defaultp>> dvec1s;
	typedef composite<glm::vec<2, double, glm::defaultp>> dvec2s;
	typedef composite<glm::vec<3, double, glm::defaultp>> dvec3s;
	typedef composite<glm::vec<4, double, glm::defaultp>> dvec4s;

	typedef composite<glm::vec<1, float, glm::defaultp>> vec1s;
	typedef composite<glm::vec<2, float, glm::defaultp>> vec2s;
	typedef composite<glm::vec<3, float, glm::defaultp>> vec3s;
	typedef composite<glm::vec<4, float, glm::defaultp>> vec4s;

}
