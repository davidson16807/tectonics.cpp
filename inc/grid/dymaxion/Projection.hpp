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
	
    template<typename id, typename scalar, glm::qualifier Q=glm::defaultp>
	class Projection
	{

        using vec2  = glm::vec<2,scalar,Q>;
        using vec3  = glm::vec<3,scalar,Q>;
        using ivec2 = glm::vec<2,id,Q>;
        using bvec2 = glm::vec<2,bool,Q>;
        using mat3  = glm::mat<3,3,scalar,Q>;
        using Point = dymaxion::Point<id,scalar>;

		static constexpr vec2 I = vec2(1,0);
		static constexpr vec2 J = vec2(0,1);
		static constexpr vec2 mirror = vec2(-1,1);
		static constexpr vec2 flip = vec2(1,-1);
		static constexpr ivec2 imirror = ivec2(-1,1);
		static constexpr scalar s0 = 0;
		static constexpr scalar s1 = 1;
		static constexpr scalar s2 = 2;
		static constexpr scalar pi = 3.141592652653589793f;
		static constexpr scalar turn = s2*pi;
		static constexpr id square_count = 10;
		static constexpr id triangle_count = 20;
		static constexpr scalar half_subgrid_longitude_arc_length = 2*pi/square_count;
		static constexpr scalar half = 0.5;
		static constexpr id i0 = 0;
		static constexpr id i1 = 1;
		static constexpr id i2 = 2;

		std::array<mat3,triangle_count> B;
		std::array<mat3,triangle_count> invB_NO;
		std::array<vec3,triangle_count> N;
		std::array<vec3,square_count> west_halfspace_normals;
		std::array<vec3,square_count> polar_halfspace_normals;

		const Triangles<id,scalar,Q> triangles;
		const Squares<id,scalar,Q> squares;

	public:

		explicit Projection():
			B(),
			invB_NO(),
			N(),
			west_halfspace_normals(),
			polar_halfspace_normals()
		{
			for (id i = 0; i < square_count; ++i)
			{
				for (id j = 0; j < 2; ++j)
				{
					bool is_polar(j==1);
					id triangle_id(triangles.triangle_id(i,is_polar));
					mat3 basis_(basis(i,is_polar));
					B[triangle_id]       = basis_;
					N[triangle_id]       = glm::normalize(glm::cross(basis_[1], basis_[0]));
					invB_NO[triangle_id] = glm::inverse(basis_) * glm::dot(N[triangle_id], origin(i,is_polar));
					// V3 (N⋅O)/(N⋅V3) projects onto the plane, and 
					// multiplying the result by B⁻¹ gives the coordinates, so we cache B⁻¹(N⋅O)
				}
				polar_halfspace_normals[i] = polar_halfspace_normal(i);
				west_halfspace_normals[i] = west_halfspace_normal(i);
			}
		}

		constexpr vec3 west_halfspace_normal(const id EWid) const 
		{
			id     Nid (i2*std::round((EWid+half)/s2));
			id     Sid (i2*std::round(((EWid+half)-s1)/s2)+i1);
			return glm::cross(squares.westmost(Sid), squares.westmost(Nid)); // |S×N|
		}

		constexpr vec3 polar_halfspace_normal(const id i) const 
		{
			return glm::normalize(glm::cross(squares.westmost(i), squares.eastmost(i))); // W×E
		}

		constexpr vec3 origin(const id i, const bool is_polar) const 
		{
			id     Oid (i+i1);
			scalar square_polarity(squares.polarity(i));
			return triangles.origin(Oid, square_polarity, is_polar);
		}

		constexpr mat3 basis(const id i, const bool is_polar) const 
		{
			vec3   W   (squares.westmost(i));    // W: westernmost triangle vertex
			vec3   E   (squares.eastmost(i)); // E: easternmost triangle vertex
			vec3   O   (origin(i,is_polar));     // O: northernmost or southernmost triangle vertex that serves as origin
			bool   is_inverted    (triangles.is_inverted_square_id   (i, is_polar));
			return triangles.basis(is_inverted,W,E,O);
		}

		constexpr Point standardize(const Point grid_id) const 
		{
			// return grid_id;
			id    i  ((grid_id.square_id+square_count) % square_count);
			vec2  V2 (grid_id.square_position);
			vec2  U2 (V2-half);
			bvec2 are_nonlocal   (glm::greaterThan(glm::abs(U2), vec2(half)));
			ivec2 nonlocal_sign  (glm::sign(U2) * vec2(are_nonlocal));
			bvec2 are_polar      (glm::equal(nonlocal_sign, imirror * id(std::pow(-i1,i))));
			bvec2 are_nonpolar   (glm::notEqual(nonlocal_sign, imirror * id(std::pow(-i1,i))));
			bool  is_polar       (are_polar.x || are_polar.y);
			bool  is_pole        (are_polar.x && are_polar.y);
			bool  is_corner      (are_nonlocal.x && are_nonlocal.y);
			vec2  modded         (V2-vec2(nonlocal_sign));
			vec2  inverted       (!is_polar? modded : vec2(are_nonpolar)*(s1-modded) + vec2(are_polar)*(modded));
			vec2  flipped        (is_corner? modded : is_polar? inverted.yx() : inverted);
			id    di             (math::compMax2((i1+ivec2(are_polar)) * nonlocal_sign));
			/* NOTE: there is more than one possible solution if `is_pole`, 
		    and these solutions do not represent the same point in space.
		    However the case where `is_pole` is still valid and must be supported.
		    Therefore, we declare that standardize() is identity if `is_pole`.
		    This has advantages in spatial transport and 3d rendering 
		    since triangles and edges naturally degenerate if `is_pole` is true for any vertex.*/
			Point standardized   (is_pole? grid_id : Point((i+di+square_count) % square_count, flipped));
			return standardized;
		}

		/*
		Performance observations on gcc:
		std::pow(-1,i) is faster than 1-2*(i%2) and even 1-2*(i&1)
		% is faster than math::modulus
		*/

		constexpr Point grid_id(const vec3 sphere_position) const 
		{
			vec3   V3(sphere_position);
			id     EWid(id((std::atan2(V3.y,V3.x)+turn)/half_subgrid_longitude_arc_length) % square_count);
			// V3⋅(N×S)>0 indicates whether V3 is in the easternmost of two squares that are possible for the longitude
			id     i ((EWid - (glm::dot(V3,west_halfspace_normals[EWid])>=s0) + square_count) % square_count);
			// V3⋅(W×E)>0 indicates whether V3 occupies a polar triangle
			bool   is_polar     (std::pow(-i1,i) * glm::dot(V3, polar_halfspace_normals[i]) >= s0);
			id     triangle_id  ((i%square_count) + is_polar*square_count);
			vec2   triangle_position((
				invB_NO[triangle_id] * V3 / glm::dot(N[triangle_id],V3)
			).yx());
			return Point(i,
					is_polar == (i&1)? 
						I+mirror*triangle_position : 
						J+flip*triangle_position
				);
		}

		constexpr vec3 sphere_position(const Point grid_id) const 
		{
			id   i  (grid_id.square_id);
			vec2 V2 (grid_id.square_position.yx());
			bool is_inverted (V2.y > V2.x);
			bool is_polar    (is_inverted == (i&1));
			vec3 triangle_position (
				is_inverted? 
					(V2-J)/flip : 
					(V2-I)/mirror,
				s1);
			return glm::normalize(B[(i%square_count) + is_polar*square_count] * triangle_position);
		}

	};

}
