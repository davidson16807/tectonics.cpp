#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>

namespace field
{

	/*
	Given a `Series`: ℕ→ℝ, a `VectorZip` maps: ℕ→ℝᴸ.
	*/
	template<typename X, typename Y, typename Z, typename W, glm::qualifier Q=glm::highp>
	struct Vector4Zip
	{
		using R = typename X::value_type;
	    using size_type = std::size_t;
		using value_type = glm::vec<4,R,Q>;

		X x;
		Y y;
		Z z;
		W w;

		constexpr inline explicit Vector4Zip(const X& x, const Y& y, const Z& z, const W& w) : 
			x(x),
			y(y),
			z(z),
			w(w)
		{}

		constexpr inline size_type size() const { return 1; }

		constexpr inline value_type operator()(const glm::vec<4,R,Q> V ) const
		{
			return glm::vec<4,R,Q>(x(V), y(V), z(V), w(V));
		}

	};

	/*
	Given a `Series`: ℕ→ℝ, a `VectorZip` maps: ℕ→ℝᴸ.
	*/
	template<typename X, typename Y, typename Z, glm::qualifier Q=glm::highp>
	struct Vector3Zip
	{
		using R = typename X::value_type;
	    using size_type = std::size_t;
		using value_type = glm::vec<3,R,Q>;

		X x;
		Y y;
		Z z;

		constexpr inline explicit Vector3Zip(const X& x, const Y& y, const Z& z) : 
			x(x),
			y(y),
			z(z)
		{}

		constexpr inline size_type size() const { return 1; }

		constexpr inline value_type operator()(const glm::vec<3,R,Q> V ) const
		{
			return glm::vec<3,R,Q>(x(V), y(V), z(V));
		}

	};

	/*
	Given a `Series`: ℕ→ℝ, a `VectorZip` maps: ℕ→ℝᴸ.
	*/
	template<typename X, typename Y, glm::qualifier Q=glm::highp>
	struct Vector2Zip
	{
		using R = typename X::value_type;
	    using size_type = std::size_t;
		using value_type = glm::vec<2,R,Q>;

		X x;
		Y y;

		constexpr inline explicit Vector2Zip(const X& x, const Y& y) : 
			x(x),
			y(y)
		{}

		constexpr inline size_type size() const { return 1; }

		constexpr inline value_type operator()(const glm::vec<2,R,Q> V ) const
		{
			return glm::vec<2,R,Q>(x(V), y(V));
		}

	};

	template<typename X, typename Y, typename Z, typename W, glm::qualifier Q=glm::highp>
	inline Vector4Zip<X,Y,Z,W,Q> vector4_zip(const X& x, const Y& y, const Z& z, const W& w)
	{
		return Vector4Zip<X,Y,Z,W,Q>(x,y,z,w);
	}

	template<typename X, typename Y, typename Z, glm::qualifier Q=glm::highp>
	inline Vector3Zip<X,Y,Z,Q> vector3_zip(const X& x, const Y& y, const Z& z)
	{
		return Vector3Zip<X,Y,Z,Q>(x,y,z);
	}

	template<typename X, typename Y, glm::qualifier Q=glm::highp>
	inline Vector2Zip<X,Y,Q> vector2_zip(const X& x, const Y& y)
	{
		return Vector2Zip<X,Y,Q>(x,y);
	}

}

