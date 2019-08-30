#pragma once

#include <iostream>

#include "../types.hpp"

namespace many
{
	template <glm::length_t L, class T, glm::qualifier Q>
	void get_x(const tmany<glm::vec<L,T,Q>>& a, tmany<T>& out )
	{
		transform(a, [](glm::vec<L,T,Q> ai){ return ai.x; }, out);
	}
	template <glm::length_t L, class T, glm::qualifier Q>
	void get_y(const tmany<glm::vec<L,T,Q>>& a, tmany<T>& out )
	{
		transform(a, [](glm::vec<L,T,Q> ai){ return ai.y; }, out);
	}
	template <glm::length_t L, class T, glm::qualifier Q>
	void get_z(const tmany<glm::vec<L,T,Q>>& a, tmany<T>& out )
	{
		transform(a, [](glm::vec<L,T,Q> ai){ return ai.z; }, out);
	}



	template <glm::length_t L, class T, glm::qualifier Q>
	void set_x(const tmany<glm::vec<L,T,Q>>& out, tmany<T>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i].x = a[i];
		}
	}
	template <glm::length_t L, class T, glm::qualifier Q>
	void set_y(const tmany<glm::vec<L,T,Q>>& out, tmany<T>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i].y = a[i];
		}
	}
	template <glm::length_t L, class T, glm::qualifier Q>
	void set_z(const tmany<glm::vec<L,T,Q>>& out, tmany<T>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i].z = a[i];
		}
	}

	template<typename T, glm::qualifier Q>
	void mult(const tmany<glm::vec<3,T,Q>>& a, const glm::mat<4,3,T,Q>& b, tmany<glm::vec<3,T,Q>>& out)
	{
		constexpr T one = T(1.);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = b * vec4(a[i], one);
		}
	}


	
	


	typedef tmany<glm::vec<1, bool, glm::defaultp>>	bvec1s;
	typedef tmany<glm::vec<2, bool, glm::defaultp>>	bvec2s;
	typedef tmany<glm::vec<3, bool, glm::defaultp>>	bvec3s;
	typedef tmany<glm::vec<4, bool, glm::defaultp>>	bvec4s;

	typedef tmany<glm::vec<1, int, glm::defaultp>> ivec1s;
	typedef tmany<glm::vec<2, int, glm::defaultp>> ivec2s;
	typedef tmany<glm::vec<3, int, glm::defaultp>> ivec3s;
	typedef tmany<glm::vec<4, int, glm::defaultp>> ivec4s;

	typedef tmany<glm::vec<1, unsigned int, glm::defaultp>> uvec1s;
	typedef tmany<glm::vec<2, unsigned int, glm::defaultp>> uvec2s;
	typedef tmany<glm::vec<3, unsigned int, glm::defaultp>> uvec3s;
	typedef tmany<glm::vec<4, unsigned int, glm::defaultp>> uvec4s;

	typedef tmany<glm::vec<1, double, glm::defaultp>> dvec1s;
	typedef tmany<glm::vec<2, double, glm::defaultp>> dvec2s;
	typedef tmany<glm::vec<3, double, glm::defaultp>> dvec3s;
	typedef tmany<glm::vec<4, double, glm::defaultp>> dvec4s;

	typedef tmany<glm::vec<1, float, glm::defaultp>> vec1s;
	typedef tmany<glm::vec<2, float, glm::defaultp>> vec2s;
	typedef tmany<glm::vec<3, float, glm::defaultp>> vec3s;
	typedef tmany<glm::vec<4, float, glm::defaultp>> vec4s;
	
}
