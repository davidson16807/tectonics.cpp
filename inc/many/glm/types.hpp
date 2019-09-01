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
	void set_x(tmany<glm::vec<L,T,Q>>& out, const tmany<T>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i].x = a[i];
		}
	}
	template <glm::length_t L, class T, glm::qualifier Q>
	void set_y(tmany<glm::vec<L,T,Q>>& out, const tmany<T>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i].y = a[i];
		}
	}
	template <glm::length_t L, class T, glm::qualifier Q>
	void set_z(tmany<glm::vec<L,T,Q>>& out, const tmany<T>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i].z = a[i];
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



	
	

	typedef tmany<glm::mat<2,2, double, glm::defaultp>> dmat2s;
	typedef tmany<glm::mat<2,2, double, glm::defaultp>> dmat2x2s;
	typedef tmany<glm::mat<2,3, double, glm::defaultp>> dmat2x3s;
	typedef tmany<glm::mat<2,4, double, glm::defaultp>> dmat2x4s;

	typedef tmany<glm::mat<3,3, double, glm::defaultp>> dmat3s;
	typedef tmany<glm::mat<3,2, double, glm::defaultp>> dmat3x2s;
	typedef tmany<glm::mat<3,3, double, glm::defaultp>> dmat3x3s;
	typedef tmany<glm::mat<3,4, double, glm::defaultp>> dmat3x4s;

	typedef tmany<glm::mat<4,4, double, glm::defaultp>> dmat4s;
	typedef tmany<glm::mat<4,2, double, glm::defaultp>> dmat4x2s;
	typedef tmany<glm::mat<4,3, double, glm::defaultp>> dmat4x3s;
	typedef tmany<glm::mat<4,4, double, glm::defaultp>> dmat4x4s;
	
	

	typedef tmany<glm::mat<2,2, float, glm::defaultp>> mat2s;
	typedef tmany<glm::mat<2,2, float, glm::defaultp>> mat2x2s;
	typedef tmany<glm::mat<2,3, float, glm::defaultp>> mat2x3s;
	typedef tmany<glm::mat<2,4, float, glm::defaultp>> mat2x4s;

	typedef tmany<glm::mat<3,3, float, glm::defaultp>> mat3s;
	typedef tmany<glm::mat<3,2, float, glm::defaultp>> mat3x2s;
	typedef tmany<glm::mat<3,3, float, glm::defaultp>> mat3x3s;
	typedef tmany<glm::mat<3,4, float, glm::defaultp>> mat3x4s;

	typedef tmany<glm::mat<4,4, float, glm::defaultp>> mat4s;
	typedef tmany<glm::mat<4,2, float, glm::defaultp>> mat4x2s;
	typedef tmany<glm::mat<4,3, float, glm::defaultp>> mat4x3s;
	typedef tmany<glm::mat<4,4, float, glm::defaultp>> mat4x4s;
	
}
