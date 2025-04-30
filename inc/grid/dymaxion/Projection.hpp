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
        using mat2  = glm::mat<2,2,scalar,Q>;
        using mat2x3= glm::mat<2,3,scalar,Q>;
        using mat3x2= glm::mat<3,2,scalar,Q>;
        using Point = dymaxion::Point<id,scalar>;

		static constexpr vec2 I = vec2(1,0);
		static constexpr vec2 J = vec2(0,1);
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
		static constexpr id i1 = 1;
		static constexpr id i2 = 2;

		std::array<mat2x3,triangle_count> B;
		std::array<mat3x2,triangle_count> Binv_NO;
		std::array<vec3,triangle_count> N;
		std::array<vec3,triangle_count> O;
		std::array<vec3,square_count> west_halfspace_normals;
		std::array<vec3,square_count> polar_halfspace_normals;

		const Triangles<id,scalar,Q> triangles;
		const Squares<id,scalar,Q> squares;

	public:

		explicit Projection():
			B(),
			Binv_NO(),
			N(),
			O(),
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
					vec3 origin_(origin(i,is_polar));
					B[triangle_id]       = mat2x3(basis_);
					N[triangle_id]       = glm::normalize(glm::cross(basis_[1], basis_[0]));
					O[triangle_id]       = origin_;
					Binv_NO[triangle_id] = mat3x2(glm::inverse(basis_)) * glm::dot(N[triangle_id], origin_);
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
			return scalar(std::pow(-i1,i)) * glm::normalize(glm::cross(squares.westmost(i), squares.eastmost(i))); // W×E
		}

		constexpr vec3 origin(const id i, const bool is_polar) const 
		{
			return triangles.origin(i+i1, squares.polarity(i), is_polar);
		}

		constexpr mat3 basis(const id i, const bool is_polar) const 
		{
			bool is_inverted(triangles.is_inverted_square_id(i, is_polar));
			return triangles.basis(
				is_inverted,
				squares.westmost(i), // westmost vertex of square
				squares.eastmost(i), // westmost vertex of square
				origin(i,is_polar)   // northernmost or southernmost triangle vertex that serves as origin
			);
		}

		constexpr Point standardize(const Point grid_id) const 
		{
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
			return Point(is_pole? grid_id : Point((i+di+square_count) % square_count, flipped));
		}

		/*
		Performance observations on gcc:
		std::pow(-1,i) is faster than 1-2*(i%2) and even 1-2*(i&1)
		% is faster than math::modulus
		*/

		constexpr Point grid_id(const vec3 sphere_position) const 
		{
			vec3 V3(sphere_position);
			id   EWid(id((std::atan2(V3.y,V3.x)+turn)/half_subgrid_longitude_arc_length) % square_count);
			id   i ((EWid - (glm::dot(V3,west_halfspace_normals[EWid])>=s0) + square_count) % square_count);
			bool is_polar    (glm::dot(V3,polar_halfspace_normals[i]) >= s0);
			bool is_inverted (is_polar == (i&i1));
			id   triangle_id (i + is_polar*square_count);
			vec2 U2(Binv_NO[triangle_id] * (V3 / glm::dot(N[triangle_id],V3)));
			// U3 = V3 (N⋅O)/(N⋅V3) projects onto the plane, and 
			// and U2 = B⁻¹ U3 = B⁻¹ V3 (N⋅O)/(N⋅V3), 
			// so we cache B⁻¹(N⋅O) and N since they are the largest groupings of constants
			return Point(i, U2.yx() + (is_inverted?I:J));
		}

		constexpr vec3 sphere_position(const Point grid_id) const 
		{
			id   i  (grid_id.square_id);
			vec2 U2 (grid_id.square_position.yx());
			bool is_inverted (U2.y > U2.x);
			bool is_polar    (is_inverted == (i&i1));
			vec2 V2 (U2 - (is_inverted?J:I));
			id   triangle_id ((i%square_count) + is_polar*square_count);
			return glm::normalize(B[triangle_id] * V2 + O[triangle_id]);
		}

	};

}
