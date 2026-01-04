#pragma once

// C libraries
#include <math.h>       // ceil, round 

// std libraries
#include <algorithm> 	// max, min, clamp

// 3rd party libraries
#include <glm/vec2.hpp>       	 // *vec3
#include <glm/vec3.hpp>       	 // *vec3
#include <glm/mat2x2.hpp>        // *mat2
#include <glm/gtx/string_cast.hpp>  // to_string

// in-house libraries
#include <math/special.hpp>

#include "Point.hpp"
#include "Triangles.hpp"
#include "Squares.hpp"

namespace healpix
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
	struct IProjection
	{
		// Empty virtual destructor for proper cleanup
        using vec2 = glm::vec<2,scalar,Q>;
        using vec3 = glm::vec<3,scalar,Q>;
		virtual ~IProjection() {}
		virtual vec2 grid_id(const vec3 sphere_position) const = 0;
		virtual vec3 sphere_position(const vec2 grid_id) const = 0;
	};

    template<typename id=int, typename scalar=double, glm::qualifier Q=glm::defaultp>
	struct Collignon : public IProjection<id,scalar,Q>
	{

        using vec2  = glm::vec<2,scalar,Q>;
        using vec3  = glm::vec<3,scalar,Q>;

		static constexpr scalar pi = math::pi;
		static constexpr scalar turn = 2*math::pi;
		static constexpr scalar epsilon = 1e-7;
		static constexpr scalar s0 = 0.0;
		static constexpr scalar s1 = 1.0;
		static constexpr scalar s2 = 2.0;

        const scalar leglength;
        const scalar longitude0;
        const bool debug;

        explicit Collignon(const scalar longitude0, const scalar triangle_area, const bool debug=false):
        	leglength(std::sqrt(s2*triangle_area)),
        	longitude0(longitude0),
        	debug(debug)
        {
        }

		constexpr vec2 grid_id(const vec3 sphere_position) const 
		{
			vec3 V3(sphere_position);
			scalar longitude(std::atan2(V3.y,V3.x));
			scalar dlongitude(math::roundmod(turn+longitude-longitude0, turn));
			scalar legscale(std::sqrt(s1-abs(V3.z)));
			vec2 V2(glm::vec2(
				legscale*s2*dlongitude/leglength,
				math::bitsign(V3.z)*(s1-legscale)*leglength));
			return V2;
		}

		constexpr vec3 sphere_position(const vec2 grid_id) const 
		{
			scalar x(grid_id.x);
			scalar y(grid_id.y);
			scalar legscale(s1-std::abs(y/leglength));
			scalar z(math::bitsign(y) * (s1-legscale*legscale));
			scalar dlongitude(x*leglength/(s2*legscale));
			scalar longitude(longitude0+dlongitude);
			scalar r(std::sqrt(s1-z*z));
			vec3 V3(
				r*std::cos(longitude),
				r*std::sin(longitude),
				z);
			// if (debug)
			// {
			// 	raise(SIGTRAP);
			// }
			return V3;
		}

	};

    template<typename id=int, typename scalar=double, glm::qualifier Q=glm::defaultp>
	struct Lambert : public IProjection<id,scalar,Q>
	{

        using vec2  = glm::vec<2,scalar,Q>;
        using vec3  = glm::vec<3,scalar,Q>;

		static constexpr scalar pi = math::pi;
		static constexpr scalar turn = 2*math::pi;
		static constexpr scalar epsilon = 1e-7;
		static constexpr scalar s0 = 0.0;
		static constexpr scalar s1 = 1.0;

        const scalar longitude0;
        const bool debug;

        explicit Lambert(const scalar longitude0, const bool debug=false):
        	longitude0(longitude0),
        	debug(debug)
        {
        }

		constexpr vec2 grid_id(const vec3 sphere_position) const 
		{
			vec3 V3(sphere_position);
			scalar longitude(std::atan2(V3.y,V3.x));
			return vec2(
				math::roundmod(turn+longitude-longitude0, turn),
				V3.z);
		}

		constexpr vec3 sphere_position(const vec2 grid_id) const 
		{
			scalar dlongitude(grid_id.x);
			scalar longitude(longitude0 + dlongitude);
			scalar z(grid_id.y);
			scalar r(std::sqrt(s1-z*z));
			vec3 V3(
				r*std::cos(longitude),
				r*std::sin(longitude),
				z);
			// if (debug)
			// {
			// 	raise(SIGTRAP);
			// }
			return V3;
		}

	};

    template<typename id=int, typename scalar=double, glm::qualifier Q=glm::defaultp>
	class Projection
	{

        using vec2  = glm::vec<2,scalar,Q>;
        using vec3  = glm::vec<3,scalar,Q>;
        using bvec2 = glm::vec<2,bool,Q>;
        using mat2  = glm::mat<2,2,scalar,Q>;
        using Point = healpix::Point<id,scalar>;

		static constexpr scalar turn = 2*math::pi;
		static constexpr scalar total_area = 4*math::pi;
		static constexpr id triangle_count = 20;
		static constexpr id square_count = 10;
		static constexpr scalar area_per_square = total_area/square_count;
		static constexpr scalar area_per_triangle = total_area/triangle_count;
		static constexpr scalar half_subgrid_longitude_arc_length = turn/square_count;
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
			// id    i  (0);
			vec2  V2 (grid_id.square_position);
			// vec2  V2 (0,0);
			vec2  square_polarity (squares.polarity(i));
			vec2  modded        (math::modulus(V2, vec2(s1)));
			// std::cout << glm::to_string(glm::abs(V2-modded)) << std::endl;
			bvec2 are_nonlocal  (glm::greaterThan(glm::abs(V2-modded), vec2(epsilon)));
			// std::cout << glm::to_string(are_nonlocal) << std::endl;
			vec2  nonlocal_sign (glm::sign(V2-0.5) * vec2(are_nonlocal));
			bvec2 are_polar     (glm::equal(nonlocal_sign, square_polarity));
			bvec2 are_nonpolar  (glm::notEqual(nonlocal_sign, square_polarity));
			bool  is_nonlocal   (glm::any(are_nonlocal));
			bool  is_corner     (glm::all(are_nonlocal));
			bool  is_polar      (glm::any(are_polar));
			vec2  inverted      (vec2(are_nonpolar)*modded + vec2(are_polar)*(s1-modded));
			vec2  flipped       (is_polar? inverted.yx() : inverted);
			id    nonlocal_id   (glm::dot(vec2(s1,-s1), nonlocal_sign));
			id    di (i2*nonlocal_id*is_polar + nonlocal_id*(is_nonlocal && !is_polar));
			/* NOTE: there is more than one possible solution if both x>1 and y>1, 
		    and these solutions do not represent the same point in space.
		    However the case where x=1 and y=1 is still valid and must be supported.
		    Therefore, we declare that standardize() is identity if both x≥1 and y≥1.*/
			Point standardized  (is_corner? grid_id : Point(math::modulus(i+di,square_count), flipped));
			// raise(SIGTRAP);
			return standardized;
		}

		constexpr Point grid_id(const vec3 sphere_position) const 
		{
			vec3   V3        (sphere_position);
			scalar idlength  (half_subgrid_longitude_arc_length);
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
			id     Oid (i+i1);
			id     Eid (i+i2);
			vec3   W   (squares.westmost(Wid)); // W: westernmost triangle vertex
			vec3   E   (squares.westmost(Eid)); // E: easternmost triangle vertex
			bool   is_polar       (triangles.is_polar_sphere_position(squares.polarity(i), V3, W,E));
			bool   is_inverted    (triangles.is_inverted_square_id   (i, is_polar));
			vec3   O     (triangles.origin(Oid, squares.polarity(i), is_polar));
			IProjection<id,scalar>* projection;
			// bool debug = V3.x < -0.666 && V3.y< -0.666 && V3.z < -0.333;
			if (is_polar)
			{
				projection = new Collignon(Oid*idlength, area_per_triangle);
			}
			else
			{
				projection = new Lambert(Oid*idlength);
			}
			auto Wgrid (projection->grid_id(W));
			auto Egrid (projection->grid_id(E));
			auto Ogrid (projection->grid_id(O));
			mat2   basis (triangles.basis(
				is_inverted,
				Wgrid,
				Egrid,
				Ogrid
			));
			// W2: position on a triangle in projected coordinates
			vec2   W2    (projection->grid_id(V3) - projection->grid_id(O));
			// U2: position on a triangle in stored coordinates
			vec2   U2    (glm::inverse(basis) * W2);
			// V2: position on a square
			vec2   V2    (is_inverted? U2.xy() : vec2(s1)-U2.xy());
			auto iV2 = standardize(Point(i,glm::clamp(V2,s0,s1)));
			// if (debug)
			// {
			// 	raise(SIGTRAP);
			// }
			return iV2;
		}

		constexpr vec3 sphere_position(const Point grid_id) const 
		{
			scalar idlength  (half_subgrid_longitude_arc_length);
			id   i  (grid_id.square_id);
			vec2 V2 (grid_id.square_position);
			// V2: position on a square
			bool is_inverted (triangles.is_inverted_grid_position(V2));
			bool is_polar    (triangles.is_polar_square_id(i, is_inverted));
			vec2 U2 (is_inverted? V2:vec2(s1)-V2);
			// U2: position on a triangle in stored coordinates
			id Wid (i     ); // west   longitude id
			id Oid (i + i1); // origin longitude id
			id Eid (i + i2); // east   longitude id
			vec3 W (squares.westmost(Wid));
			vec3 E (squares.westmost(Eid));
			vec3 O (triangles.origin(Oid, squares.polarity(i), is_polar));
			bool debug(glm::distance(V2,vec2(0.708,0.458)) < 0.03 and i==5);
			IProjection<id,scalar>* projection;
			if (is_polar)
			{
				projection = new Collignon(Oid*idlength, area_per_triangle);
			}
			else
			{
				projection = new Lambert(Oid*idlength, debug);
			}
			auto Wgrid (projection->grid_id(W));
			auto Egrid (projection->grid_id(E));
			auto Ogrid (projection->grid_id(O));
			mat2   basis (triangles.basis(
				is_inverted,
				Wgrid,
				Egrid,
				Ogrid
			));
			// W2: position on a triangle in projected coordinates
			vec2 W2 (basis*U2);
			vec3 V3 (projection->sphere_position(W2+projection->grid_id(O)));
			// if (debug)
			// {
			// 	raise(SIGTRAP);
			// }
			return V3;
		}

	};

}

