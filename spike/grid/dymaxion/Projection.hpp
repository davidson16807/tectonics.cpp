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
	
    template<typename id=int, typename scalar=double, glm::qualifier Q=glm::defaultp>
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
		static constexpr scalar half_subgrid_longitude_arc_length = 2*pi/square_count;
		static constexpr scalar epsilon = 1e-7;
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
			ivec2 square_polarity(squares.polarity(i));
			vec2  modded         (math::modulus(V2, vec2(s1)));
			bvec2 are_nonlocal   (glm::greaterThan(glm::abs(V2-modded), vec2(epsilon)));
			ivec2 nonlocal_sign  (glm::sign(V2-0.5) * vec2(are_nonlocal));
			bvec2 are_polar      (glm::equal(nonlocal_sign, square_polarity));
			bvec2 are_nonpolar   (glm::notEqual(nonlocal_sign, square_polarity));
			bool  is_polar       (glm::any(are_polar));
			bool  is_pole        (glm::all(are_polar));
			bool  is_corner      (glm::all(are_nonlocal));
			vec2  inverted       (vec2(are_nonpolar)*modded + vec2(are_polar)*(s1-modded));
			vec2  flipped        (is_corner? modded : is_polar? inverted.yx() : inverted);
			id    di (math::compMaxAbs(ivec2(i1,-i1) * (ivec2(are_nonlocal)+ivec2(are_polar)) * nonlocal_sign));
			/* NOTE: there is more than one possible solution if `is_pole`, 
		    and these solutions do not represent the same point in space.
		    However the case where `is_pole` is still valid and must be supported.
		    Therefore, we declare that standardize() is identity if both x≥1 and y≥1.
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
			vec3   N   (squares.westmost(Nid)); // wrong
			vec3   S   (squares.westmost(Sid));
			id     i   (math::modulus(
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
