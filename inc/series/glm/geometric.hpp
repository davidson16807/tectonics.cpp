#pragma once

#include <cmath>    	    // isnan

#include <glm/geometric.hpp>// all the GLSL geometry functions: dot, cross, reflect, etc.

#include "../types.hpp"

namespace series
{

	template<glm::length_t L, typename T, glm::qualifier Q>
	inline float dot(const glm::vec<L,T,Q> a, const glm::vec<L,T,Q> b)
	{
		return glm::dot(a,b);
	}
	template<typename T, glm::qualifier Q>
	inline float cross(const glm::vec<2,T,Q> a, const glm::vec<2,T,Q> b)
	{
		return a.x*b.y - b.x*a.y;
	}
	template<typename T, glm::qualifier Q>
	inline glm::vec<3,T,Q> cross(const glm::vec<3,T,Q> a, const glm::vec<3,T,Q> b)
	{
		return glm::cross(a,b);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline T distance(const glm::vec<L,T,Q> a, const glm::vec<L,T,Q> b)
	{
		return glm::distance(a,b);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline T length(const glm::vec<L,T,Q> a)
	{
		return glm::length(a);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline glm::vec<L,T,Q> normalize(const glm::vec<L,T,Q> a)
	{
		T a_length = glm::length(a);
		return a / (a_length < T(1e-5)? T(1) : a_length);
	}

	template<typename T, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value, int> = 0>
	void dot (const T& u, const typename T::value_type v, Tout& out) {
		typedef typename T::value_type Ti;
		out.store([](const Ti ui, const Ti vi){ return series::dot(ui,vi); }, u, v);
	}
	template<typename T, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value, int> = 0>
	void cross (const T& u, const typename T::value_type v, Tout& out) 
	{
		typedef typename T::value_type Ti;
		out.store([](const Ti ui, const Ti vi){ return series::cross(ui,vi); }, u, v);
	}
	template<typename T, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value, int> = 0>
	void distance(const T& u, const typename T::value_type v, Tout& out) 
	{
		typedef typename T::value_type Ti;
		out.store([](const Ti ui, const Ti vi){ return series::distance(ui,vi); }, u, v);
	}

	template<typename T, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value, int> = 0>
	void dot (const typename T::value_type& u, const T&  v, Tout& out) {
		typedef typename T::value_type Ti;
		out.store([](const Ti ui, const Ti vi){ return series::dot(ui,vi); }, u, v);
	}
	template<typename T, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value, int> = 0>
	void cross (const typename T::value_type& u, const T&  v, Tout& out) 
	{
		typedef typename T::value_type Ti;
		out.store([](const Ti ui, const Ti vi){ return series::cross(ui,vi); }, u, v);
	}
	template<typename T, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T>::value, int> = 0>
	void distance(const typename T::value_type& u, const T&  v, Tout& out) 
	{
		typedef typename T::value_type Ti;
		out.store([](const Ti ui, const Ti vi){ return series::distance(ui,vi); }, u, v);
	}

	template<typename T1, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T1>::value && std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void dot (const T1& u, const T2&  v, Tout& out) {
		typedef typename T1::value_type T1i;
		typedef typename T2::value_type T2i;
		out.store([](const T1i ui, const T2i vi){ return series::dot(ui,vi); }, u, v);
	}
	template<typename T1, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T1>::value && std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void cross (const T1& u, const T2&  v, Tout& out) 
	{
		typedef typename T1::value_type T1i;
		typedef typename T2::value_type T2i;
		out.store([](const T1i ui, const T2i vi){ return series::cross(ui,vi); }, u, v);
	}
	template<typename T1, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T1>::value && std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	void distance(const T1& u, const T2&  v, Tout& out) 
	{
		typedef typename T1::value_type T1i;
		typedef typename T2::value_type T2i;
		out.store([](const T1i ui, const T2i vi){ return series::distance(ui,vi); }, u, v);
	}


	template<typename T, typename Tout>
	void normalize(const T& u, Tout& out) 
	{
		typedef typename T::value_type Ti;
		out.store([](const Ti ui){ return series::normalize(ui); }, u);
	}
	template<typename T, typename Tout>
	void length(const T& u, Tout& out) 
	{
		typedef typename T::value_type Ti;
		out.store([](const Ti ui){ return series::length(ui); }, u);
	}

}
