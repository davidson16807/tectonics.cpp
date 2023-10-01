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
#include <glm/gtx/string_cast.hpp>  // to_string

// in-house libraries
#include <math/special.hpp>

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
	
    template<typename id=int, typename scalar=double, glm::qualifier Q=glm::defaultp>
	class Projection
	{

        using vec2  = glm::vec<2,scalar,Q>;
        using vec3  = glm::vec<3,scalar,Q>;
        using bvec2 = glm::vec<2,bool,Q>;
        using mat3  = glm::mat<3,3,scalar,Q>;
        using Point = dymaxion::Point<id,scalar>;

		static constexpr scalar pi = 3.141592652653589793f;
		static constexpr id square_count = 10;
		static constexpr scalar half_subgrid_longitude_arc_length = 2*pi/square_count;
		static constexpr scalar s0 = 0;
		static constexpr scalar s1 = 1;
		static constexpr scalar s2 = 2;
		static constexpr id i0 = 0;
		static constexpr id i1 = 1;
		static constexpr id i2 = 2;

		const Triangles<id,scalar,Q> triangles;
		const Squares<id,scalar,Q> squares;

	public:

		explicit Projection()
		{
		}

		constexpr Point standardize(const Point grid_id) const 
		{
			id    i  (grid_id.square_id);
			vec2  V2 (grid_id.square_position);
			vec2  clampedV2        (glm::clamp(V2,s0,s1));
			bvec2 are_local        (glm::equal(V2,clampedV2));
			bvec2 are_nonlocal     (glm::notEqual(V2,clampedV2));
			vec2  deviation_sign   (glm::sign(V2-clampedV2));
			bvec2 are_flipped      (glm::equal(deviation_sign, vec2(squares.polarity(i))));
			bool  is_nonlocal      (are_nonlocal.x || are_nonlocal.y);
			bool  is_flipped       (are_flipped.x || are_flipped.y);
			vec2  inverted_nonlocal(
				is_flipped? vec2(are_local)*V2 + vec2(are_nonlocal)*(s1-V2) : V2);
			vec2  modded           (math::mod(inverted_nonlocal, vec2(s1)));
			vec2  flipped          (is_flipped? modded.yx() : modded);
			id    deviation_id     (glm::dot(vec2(s1,-s1), deviation_sign));
			id    di(i2*deviation_id*is_flipped + deviation_id*(is_nonlocal && !is_flipped));
			return Point(math::mod(i+di,square_count), flipped);
		}

		constexpr Point grid_id(const vec3 sphere_position) const 
		{
			vec3   V3        (sphere_position);
			scalar longitude (std::atan2(V3.y,V3.x));
			scalar EWid(longitude/half_subgrid_longitude_arc_length);
			id     Nid (i2*std::round(EWid/s2));
			id     Sid (i2*std::round((EWid-s1)/s2)+i1);
			vec3   N   (squares.westmost(Nid)); // wrong
			vec3   S   (squares.westmost(Sid));
			id     i   (math::mod(
						std::min(Nid,Sid)-i1 + id(triangles.is_eastern_sphere_position(V3,N,S)),  
						square_count));
			id     Wid (i);
			id     Oid (i+i1);
			id     Eid (i+i2);
			vec3   W   (squares.westmost(Wid)); // W: westernmost triangle vertex
			vec3   E   (squares.westmost(Eid)); // E: easternmost triangle vertex
			scalar square_polarity(squares.polarity(i));
			bool   is_polar       (triangles.is_polar_sphere_position(square_polarity, V3, W,E));
			bool   is_inverted    (triangles.is_inverted_square_id   (i, is_polar));
			vec3   O     (triangles.origin(Oid, square_polarity, is_polar));
			mat3   basis (triangles.basis(is_inverted,W,E,O));
			vec3   Nhat  (glm::normalize(glm::cross(basis[0], basis[1])));
			vec3   triangle_position(glm::inverse(basis) * triangles.plane_project(V3,Nhat,O));
			vec2   V2    (is_inverted? triangle_position.xy() : vec2(s1)-triangle_position.xy());
			return Point(i,glm::clamp(V2,s0,s1));
		}

		constexpr vec3 sphere_position(const Point grid_id) const 
		{
			Point iV2(grid_id);
			// Point iV2(standardize(grid_id));
			id   i  (iV2.square_id);
			vec2 V2 (iV2.square_position);
			bool is_inverted (triangles.is_inverted_grid_position(V2));
			bool is_polar    (triangles.is_polar_square_id(i, is_inverted));
			vec2 triangle_position (is_inverted? V2:vec2(s1)-V2);
			id Wid (i     ); // west   longitude id
			id Oid (i + i1); // origin longitude id
			id Eid (i + i2); // east   longitude id
			vec3 W (squares.westmost(Wid));
			vec3 E (squares.westmost(Eid));
			vec3 O (triangles.origin(Oid, squares.polarity(i), is_polar));
			mat3 basis (triangles.basis(is_inverted, W,E,O));
			return triangles.sphere_project(basis * vec3(triangle_position, s1));
		}

	};

}
