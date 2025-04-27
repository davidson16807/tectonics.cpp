#pragma once

// C libraries
#include <math.h>       // ceil, round 

// std libraries
#include <algorithm> 	// max, min, clamp

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

		static constexpr scalar pi = 3.141592652653589793f;
		static constexpr id square_count = 10;
		static constexpr id triangle_count = 20;
		static constexpr scalar half_subgrid_longitude_arc_length = 2*pi/square_count;
		static constexpr scalar epsilon = 1e-7;
		static constexpr scalar s0 = 0;
		static constexpr scalar s1 = 1;
		static constexpr scalar s2 = 2;
		static constexpr id i0 = 0;
		static constexpr id i1 = 1;
		static constexpr id i2 = 2;

		std::array<vec3,triangle_count> origins;
		std::array<vec3,triangle_count> Nhats;
		std::array<mat3,triangle_count> bases;
		std::array<mat3,triangle_count> inverse_bases;

		const Triangles<id,scalar,Q> triangles;
		const Squares<id,scalar,Q> squares;

	public:

		explicit Projection():
			origins(),
			Nhats(),
			bases(),
			inverse_bases()
		{
			for (id i = 0; i < square_count; ++i)
			{
				for (id j = 0; j < 2; ++j)
				{
					bool is_polar(j==1);
					id triangle_id(triangles.triangle_id(i,is_polar));
					mat3 basis_(basis(i,is_polar));
					origins[triangle_id]       = origin(i,is_polar);
					bases[triangle_id]         = basis_;
					inverse_bases[triangle_id] = glm::inverse(basis_);
					Nhats[triangle_id]         = glm::normalize(glm::cross(basis_[1], basis_[0]));
				}
			}
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
			vec3   E   (squares.westmost(i+i2)); // E: easternmost triangle vertex
			vec3   O   (origin(i,is_polar));     // O: northernmost or southernmost triangle vertex that serves as origin
			bool   is_inverted    (triangles.is_inverted_square_id   (i, is_polar));
			return triangles.basis(is_inverted,W,E,O);
		}

		constexpr Point standardize(const Point grid_id) const 
		{
			// return grid_id;
			id    i  (math::modulus(grid_id.square_id, square_count));
			vec2  V2 (grid_id.square_position);
			vec2  U2 (V2-scalar(0.5));
			ivec2 square_polarity(squares.polarity(i));
			bvec2 are_nonlocal   (glm::greaterThan(glm::abs(U2), vec2(0.5)));
			ivec2 nonlocal_sign  (glm::sign(U2) * vec2(are_nonlocal));
			bvec2 are_polar      (glm::equal(nonlocal_sign, glm::ivec2(-1,1) * id(squares.polarity(i))));
			bvec2 are_nonpolar   (glm::notEqual(nonlocal_sign, glm::ivec2(-1,1) * id(squares.polarity(i))));
			bool  is_polar       (glm::any(are_polar));
			bool  is_pole        (glm::all(are_polar));
			bool  is_corner      (glm::all(are_nonlocal));
			vec2  modded         (V2-vec2(nonlocal_sign));
			vec2  inverted       (!is_polar? modded : vec2(are_nonpolar)*(s1-modded) + vec2(are_polar)*(modded));
			vec2  flipped        (is_corner? modded : is_polar? inverted.yx() : inverted);
			id    di             (math::compMaxAbs((i1+ivec2(are_polar)) * nonlocal_sign));
			/* NOTE: there is more than one possible solution if `is_pole`, 
		    and these solutions do not represent the same point in space.
		    However the case where `is_pole` is still valid and must be supported.
		    Therefore, we declare that standardize() is identity if `is_pole`.
		    This has advantages in spatial transport and 3d rendering 
		    since triangles and edges naturally degenerate if `is_pole` is true for any vertex.*/
			Point standardized   (is_pole? grid_id : Point(math::modulus(i+di, square_count), flipped));
			return standardized;
		}

		constexpr Point grid_id(const vec3 sphere_position) const 
		{
			vec3   V3        (sphere_position);
			scalar longitude (std::atan2(V3.y,V3.x));
			scalar EWid(longitude/half_subgrid_longitude_arc_length);
			id     Nid (i2*std::round(EWid/s2));
			id     Sid (i2*std::round((EWid-s1)/s2)+i1);
			vec3   N   (squares.westmost(Nid));
			vec3   S   (squares.westmost(Sid));
			id     i   (math::modulus(
						std::min(Nid,Sid)-i1 + id(triangles.is_eastern_sphere_position(V3,N,S)),  
						square_count));
			id     Wid (i);
			id     Eid (i+i2);
			vec3   W   (squares.westmost(Wid)); // W: westernmost triangle vertex
			vec3   E   (squares.westmost(Eid)); // E: easternmost triangle vertex
			scalar square_polarity(squares.polarity(i));
			bool   is_polar       (triangles.is_polar_sphere_position(square_polarity, V3, W,E));
			bool   is_inverted    (triangles.is_inverted_square_id   (i, is_polar));
			id     triangle_id    (triangles.triangle_id(i,is_polar));
			vec3   triangle_position(
				inverse_bases[triangle_id] * 
				triangles.plane_project(V3,Nhats[triangle_id],origins[triangle_id])
			);
			vec2   V2    (is_inverted? 
				vec2(0,1)+vec2(1,-1)*triangle_position.xy() : 
				vec2(1,0)+vec2(-1,1)*triangle_position.xy()
			);
			return Point(i,glm::clamp(V2.yx(),s0,s1));
		}

		constexpr vec3 sphere_position(const Point grid_id) const 
		{
			Point iV2(grid_id);
			id   i  (iV2.square_id);
			vec2 V2 (iV2.square_position.yx());
			bool is_inverted (triangles.is_inverted_grid_position(V2));
			bool is_polar    (triangles.is_polar_square_id(i, is_inverted));
			vec2 triangle_position ((is_inverted? 
				(V2-vec2(0,1))/vec2(1,-1) : 
				(V2-vec2(1,0))/vec2(-1,1)));
			return triangles.sphere_project(bases[triangles.triangle_id(i,is_polar)] * vec3(triangle_position, s1));
		}

	};

}
