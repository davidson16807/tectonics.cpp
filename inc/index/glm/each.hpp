#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vector_relational.hpp>

// in-house libraries
#include <math/glm/special.hpp>

#include "../each.hpp"

namespace each {

	#define EACH_UNARY_FUNCTION(NAME) \
	template <typename In1, typename Out>\
	void NAME(const In1& a, Out& out)\
	{\
		assert(each::compatible(a,out));\
		auto size = out.size();\
		for (std::size_t i = 0; i < size; ++i)\
		{\
			out[i] = NAME(a[i]);\
		}\
	}

	EACH_UNARY_FUNCTION(length)
	EACH_UNARY_FUNCTION(normalize)

	#define EACH_BINARY_FUNCTION(NAME) \
	template <typename In1, typename In2, typename Out>\
	void NAME(const In1& a, const In2& b, Out& out)\
	{\
		assert(each::compatible(a,b,out));\
		auto size = out.size();\
		for (std::size_t i = 0; i < size; ++i)\
		{\
			out[i] = NAME(a[i], b[i]);\
		}\
	}

	EACH_BINARY_FUNCTION(dot)
	EACH_BINARY_FUNCTION(cross)
	EACH_BINARY_FUNCTION(reflect)
	EACH_BINARY_FUNCTION(refract)

	template <glm::length_t L, typename T1, glm::qualifier Q>
	inline auto x(const glm::vec<L,T1,Q> a){ return a.x; }
	template <glm::length_t L, typename T1, glm::qualifier Q>
	inline auto y(const glm::vec<L,T1,Q> a){ return a.y; }
	template <glm::length_t L, typename T1, glm::qualifier Q>
	inline auto z(const glm::vec<L,T1,Q> a){ return a.z; }

	EACH_UNARY_FUNCTION(x)
	EACH_UNARY_FUNCTION(y)
	EACH_UNARY_FUNCTION(z)

	template <glm::length_t L, typename T1, glm::qualifier Q>
	inline auto x(const glm::vec<L,T1,Q> a, const T1 value){ auto out=a; out.x=value; return out; }
	template <glm::length_t L, typename T1, glm::qualifier Q>
	inline auto y(const glm::vec<L,T1,Q> a, const T1 value){ auto out=a; out.y=value; return out; }
	template <glm::length_t L, typename T1, glm::qualifier Q>
	inline auto z(const glm::vec<L,T1,Q> a, const T1 value){ auto out=a; out.z=value; return out; }

	EACH_BINARY_FUNCTION(x)
	EACH_BINARY_FUNCTION(y)
	EACH_BINARY_FUNCTION(z)

	#undef EACH_UNARY_FUNCTION
	#undef EACH_BINARY_FUNCTION


	template <int L, typename A, typename Out>
	void vector_deinterleave(const A& a, Out& out)
	{
		assert(compatible(a,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i/L][i%L];
		}
	}

	template <int L, typename A, typename Out>
	void vector_interleave(const A& a, Out& out)
	{
		// assert(compatible(a,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i/L][i%L] = a[i];
		}
	}

	template <typename Out, typename X, typename Y>
	void vector2_zip(const X x, const Y y, Out& out)
	{
		assert(compatible(x,out));
		assert(compatible(y,out));
		auto size = out.size();
		using value_type = typename Out::value_type;
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = value_type(x[i],y[i]);
		}
	}

	template <typename Out, typename X, typename Y, typename Z>
	void vector3_zip(const X x, const Y y, const Z z, Out& out)
	{
		assert(compatible(x,out));
		assert(compatible(y,out));
		assert(compatible(z,out));
		auto size = out.size();
		using value_type = typename Out::value_type;
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = value_type(x[i],y[i],z[i]);
		}
	}

	template <typename Out, typename X, typename Y, typename Z, typename W>
	void vector4_zip(const X& x, const Y& y, const Z& z, const W& w, Out& out)
	{
		assert(compatible(x,out));
		assert(compatible(y,out));
		assert(compatible(z,out));
		assert(compatible(w,out));
		auto size = out.size();
		using value_type = typename Out::value_type;
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = value_type(x[i],y[i],z[i],w[i]);
		}
	}

}

