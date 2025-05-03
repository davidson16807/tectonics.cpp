#pragma once

// C libraries
#include <math.h>       // ceil, round 

// std libraries
#include <algorithm> 	// max, min

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL      // disable anonymous structs so we can build with ISO C++
#include <glm/vec2.hpp>       	 // *vec3
#include <glm/vec3.hpp>       	 // *vec3
#include <glm/mat3x3.hpp>        // *mat3

// in-house libraries
#include <math/special.hpp>
#include <math/glm/special.hpp>

#include "Point.hpp"
#include "Triangles.hpp"
#include "Squares.hpp"

namespace dymaxion
{

	/*
	GUIDE TO VARIABLE NAMES:
	* `is_*` prefix: booleans
	* lowercase: scalars
	* UPPERCASE: vectors
	* `N/S/E/W`: cardinal direction
	* `V3`: 3d position
	* `V2`: 2d position
	* `N`: normal
	* `O`: origin
	* `i`: subgrid id
	*/
	
    template<typename id, typename id2, typename scalar, glm::qualifier Q=glm::defaultp>
	class Projection
	{

        using vec2  = glm::vec<2,scalar,Q>;
        using vec3  = glm::vec<3,scalar,Q>;
        using ivec2 = glm::vec<2,id,Q>;
        using bvec2 = glm::vec<2,bool,Q>;
        using mat3  = glm::mat<3,3,scalar,Q>;
        using point = dymaxion::Point<id2,scalar>;

		static constexpr vec2 I = vec2(1,0);
		static constexpr vec2 J = vec2(0,1);
		static constexpr ivec2 imirror = ivec2(-1,1);
		static constexpr vec2 mirror = vec2(-1,1);
		static constexpr vec2 flip = vec2(1,-1);
		static constexpr scalar s0 = 0;
		static constexpr scalar s1 = 1;
		static constexpr scalar s2 = 2;
		static constexpr scalar pi = 3.141592652653589793f;
		static constexpr scalar turn = s2*pi;
		static constexpr id2 square_count = 10;
		static constexpr id2 triangle_count = 20;
		static constexpr scalar half_subgrid_fraction_of_turn = square_count/(2*pi);
        static constexpr vec2 half_cell = vec2(0.5);
		static constexpr scalar half = 0.5;
		static constexpr id2 i1 = 1;
		static constexpr id2 i2 = 2;
		static constexpr id2 i3 = 3;
		static constexpr id2 i4 = 4;
		static constexpr scalar ua = (0.0954915028125); // cos(π 2/5)²
		static constexpr scalar ub = (0.654508497187);  // cos(π 1/5)²
		static constexpr scalar du = (ub-ua);

		struct TriangleCache {
		    mat3   inverse_basis;
		    vec3   normal;
		    scalar normal_dot_origin;
		  	private:
		    vec3   padding;
		};

		std::array<TriangleCache,triangle_count> cache;
		std::array<vec3,square_count> west_halfspace_normals;
		std::array<vec3,square_count> polar_halfspace_normals;
		std::array<scalar,4> padding;
		std::array<mat3,triangle_count> bases;

		const Triangles<id2,scalar,Q> triangles;
		const Squares<id2,scalar,Q> squares;

		constexpr vec3 west_halfspace_normal(const id2 EWid) const 
		{
			id2     Nid (i2*std::round((EWid+half)/s2));
			id2     Sid (i2*std::round(((EWid+half)-s1)/s2)+i1);
			return glm::cross(squares.westmost(Sid), squares.westmost(Nid)); // |S×N|
		}

		constexpr vec3 polar_halfspace_normal(const id2 i) const 
		{
			return glm::normalize(glm::cross(squares.westmost(i), squares.eastmost(i))); // W×E
		}

		constexpr vec3 origin(const id2 i, const bool is_polar) const 
		{
			id2     Oid (i+i1);
			scalar square_polarity(squares.polarity(i));
			return triangles.origin(Oid, square_polarity, is_polar);
		}

		constexpr mat3 basis(const id2 i, const bool is_polar) const 
		{
			vec3   W   (squares.westmost(i));    // W: westernmost triangle vertex
			vec3   E   (squares.westmost(i+i2)); // E: easternmost triangle vertex
			vec3   O   (origin(i,is_polar));     // O: northernmost or southernmost triangle vertex that serves as origin
			bool   is_inverted    (triangles.is_inverted_square_id   (i, is_polar));
			return triangles.basis(is_inverted,W,E,O);
		}

	public:

		explicit Projection()
		{
			for (id2 i = 0; i < square_count; ++i)
			{
				for (id2 j = 0; j < 2; ++j)
				{
					bool is_polar(j==1);
					id2 triangle_id(triangles.triangle_id(i,is_polar));
					mat3 basis_(basis(i,is_polar));
					bases[triangle_id]               = basis_;
					cache[triangle_id].inverse_basis = glm::inverse(basis_);
					cache[triangle_id].normal        = glm::normalize(glm::cross(basis_[1], basis_[0]));
					cache[triangle_id].normal_dot_origin = glm::dot(cache[triangle_id].normal, origin(i,is_polar));
					// std::cout << std::to_string(normal_dot_origin[triangle_id]) << std::endl;
				}
				polar_halfspace_normals[i] = polar_halfspace_normal(i);
				west_halfspace_normals[i] = west_halfspace_normal(i);
			}
		}

		constexpr point standardize(const point& grid_id) const 
		{
			// return grid_id;
			id2    i  (grid_id.square_id);
			vec2  V2 (grid_id.square_position);
			vec2  U2 (V2-half);
			bvec2 are_nonlocal   (glm::greaterThan(glm::abs(U2), half_cell));
			ivec2 nonlocal_sign  (glm::sign(U2) * vec2(are_nonlocal));
			bvec2 are_polar      (glm::equal(nonlocal_sign, imirror * id(std::pow(-i1,i))));
			bvec2 are_nonpolar   (!are_polar.x, !are_polar.y);
			bool  is_polar       (are_polar.x || are_polar.y);
			bool  is_pole        (are_polar.x && are_polar.y);
			bool  is_corner      (are_nonlocal.x && are_nonlocal.y);
			vec2  modded         (V2-vec2(nonlocal_sign));
			vec2  inverted       (!is_polar? modded : vec2(are_nonpolar)*(s1-modded) + vec2(are_polar)*(modded));
			vec2  flipped        (is_corner? modded : is_polar? inverted.yx() : inverted);
			id2    di             (math::compMaxAbs((id(i1)+ivec2(are_polar)) * nonlocal_sign));
			/* NOTE: there is more than one possible solution if `is_pole`, 
		    and these solutions do not represent the same point in space.
		    However the case where `is_pole` is still valid and must be supported.
		    Therefore, we declare that standardize() is identity if `is_pole`.
		    This has advantages in spatial transport and 3d rendering 
		    since triangles and edges naturally degenerate if `is_pole` is true for any vertex.*/
			return is_pole? grid_id : point((i+di+square_count) % square_count, flipped);
		}

		/*
		Performance observations on gcc:
		std::pow(-1,i) is faster than 1-2*(i%2)
		% is faster than math::modulus
		*/

		inline constexpr id2 longitude_id(const scalar y, const scalar x) const {
			id2 f(std::floor((ua-x*x)/du));
			id2 g((x<s0? -f : f)+i3);
			return ((y<s0? -g : g-i1)+square_count) % square_count;
		}

		constexpr point grid_id(const vec3 V3 /*position on the sphere*/) const 
		{
			id2     EWid(longitude_id(V3.y, V3.x));
			id2     i ((EWid - id2(glm::dot(V3,west_halfspace_normals[EWid])>=s0) + square_count) % square_count);
			bool   is_polar     (std::pow(-i1, i) * glm::dot(V3, polar_halfspace_normals[i]) >= s0);
			TriangleCache triangle(cache[i + is_polar*square_count]);
			vec2   triangle_position((
				triangle.inverse_basis * 
				V3 * (triangle.normal_dot_origin/glm::dot(triangle.normal,V3))
				// V3 (N⋅O)/(N⋅V3) projects onto the plane
			).yx());
			return point(i,
					is_polar == (i&i1)? 
						I+mirror*triangle_position : 
						J+flip*triangle_position
				);
		}

		constexpr vec3 sphere_position(const point& grid_id) const 
		{
			id2   i  (grid_id.square_id);
			vec2 V2 (grid_id.square_position.yx());
			bool is_inverted (V2.y > V2.x);
			vec3 triangle_position (
				is_inverted? 
					(V2-J)*flip : 
					(V2-I)*mirror,
				s1);
			return glm::normalize(bases[(i%square_count) + (is_inverted == (i&i1))*square_count] * triangle_position);
		}

	};

}
