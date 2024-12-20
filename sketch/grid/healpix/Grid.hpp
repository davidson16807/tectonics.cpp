#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/geometric.hpp>

// in-house libraries
#include "Voronoi.hpp"

namespace healpix 
{

    /*
    properties used:
        metrics->arrow_dual_lengths
        metrics->arrow_dual_normals
        metrics->arrow_lengths
        metrics->vertex_dual_areas
        structure->arrow_vertex_ids
		structure->arrow_target_vertex_id
        structure->arrow_source_vertex_id
    */

    /*
    `Grid` is a wrapper around a `Voronoi` 
    that introduces the concepts of vertex neighbors, 
    and therefore the concepts of faces, edges, and arrows.
    */
    template<typename id, typename scalar, glm::qualifier Q=glm::defaultp>
	class Grid{
        using ivec2 = glm::vec<2,id,Q>;
        using vec2 = glm::vec<2,scalar,Q>;
        using vec3 = glm::vec<3,scalar,Q>;
        using mat3 = glm::mat<3,3,scalar,Q>;
        using IdPoint = Point<id,id>;
        using ScalarPoint = Point<id,scalar>;

		static constexpr scalar pi = 3.141592652653589793f;

		// arrow_target_grid_id must be private, 
		// since healpix::Grid and collignon::Grid are meant to share similar interfaces,
		// however healpix::Grid and collignon::Grid must differ in their representations of grid ids,
		// and for this reason we do not wish to expose grid ids to classes that are using *Grids
		inline constexpr IdPoint arrow_target_grid_id(const id source_id, const id offset_id) const
		{
			return voronoi.grid_id(source_id) + arrow_offset_grid_position(offset_id);
		}

	public:

		const Voronoi<id,scalar> voronoi;

		using size_type = id;
		using value_type = scalar;

		static constexpr id arrows_per_vertex = 4;

        inline constexpr explicit Grid(const scalar radius, const id vertices_per_meridian) : 
        	voronoi  (radius, vertices_per_meridian)
    	{}

    	// NOTE: this method is for debugging, only
		constexpr id square_id(const id vertex_id) const
		{
			return voronoi.grid_id(vertex_id).square_id;
		}

		constexpr id arrow_offset_id(const ivec2 arrow_offset_grid_position) const
		{
			return 	((arrow_offset_grid_position.x + arrow_offset_grid_position.y < 0) << 1)
				+	(std::abs(arrow_offset_grid_position.y) > std::abs(arrow_offset_grid_position.x));
		}

		constexpr ivec2 arrow_offset_grid_position(const id arrow_offset_id) const
		{
			return 	((arrow_offset_id >> 1)? -1 : 1) 
				* 	((arrow_offset_id & 1)? ivec2(0,1) : ivec2(1,0));
		}

		inline constexpr scalar total_radius() const 
		{
			return voronoi.radius;
		}

		inline constexpr scalar total_diameter() const 
		{
			return voronoi.radius;
		}

		inline constexpr scalar total_area() const 
		{
			return 4.0 * pi * voronoi.radius * voronoi.radius;
		}

		inline constexpr scalar total_circumference() const 
		{
			return 2.0 * pi * voronoi.radius;
		}

		inline constexpr id vertices_per_meridian() const
		{
			return voronoi.vertices_per_meridian;
		}

		inline constexpr id vertex_count() const 
		{
			return voronoi.vertex_count;
		}

		inline constexpr id arrow_count() const 
		{
			// each vertex is guaranteed to have 4 arrows, however some arrows that cross seams will be of 0 dual area
			return arrows_per_vertex * voronoi.vertex_count;
		}

		inline constexpr id arrow_target_id(const id source_id, const id offset_id) const
		{
			return voronoi.memory_id(arrow_target_grid_id(source_id, offset_id));
		}

		// offset of the arrow
		inline constexpr vec3 arrow_offset(const id source_id, const id offset_id) const
		{
			return 	voronoi.sphere_position( arrow_target_id(source_id, offset_id) )
				- 	voronoi.sphere_position( source_id );
		}

		// normal of the arrow
		inline constexpr vec3 arrow_normal(const id source_id, const id offset_id) const
		{
			return glm::normalize(arrow_offset(source_id, offset_id));
		}

		// length of the arrow
		inline constexpr scalar arrow_length(const id source_id, const id offset_id) const
		{
			return glm::length(arrow_offset(source_id, offset_id));
		}

		// length of the arrow's dual
		constexpr scalar arrow_dual_length(const id source_id, const id offset_id) const
		{
			const ScalarPoint midpointOB(ScalarPoint(voronoi.grid_id(source_id)) + vec2(0.5) * vec2(arrow_offset_grid_position(offset_id)));
			return glm::distance(
					voronoi.sphere_position( midpointOB + scalar(0.5)*vec2(arrow_offset_grid_position((offset_id+1)%arrows_per_vertex)) ),
				 	voronoi.sphere_position( midpointOB + scalar(0.5)*vec2(arrow_offset_grid_position((offset_id-1)%arrows_per_vertex)) )
				);
		}

		// `vertex_representative()` returns the memory id of a vertex
		// whose associated arrows should not cause artifacts during certain sensitive operations
		// (like divergence or laplacian) while also being physically near the vertex of the specified `vertex_id`,
		// thereby providing an adequate representation for the vertex with irregular edges.
		inline constexpr id vertex_representative(const id vertex_id) const 
		{
			// return vertex_id;
			return voronoi.memory_id(clamp(voronoi.grid_id(vertex_id), 1, voronoi.vertices_per_triangle_leg-2));
		}

		inline constexpr vec3 vertex_position(const id vertex_id) const 
		{
			return voronoi.sphere_position(vertex_id);
		}

		inline constexpr vec3 vertex_normal(const id vertex_id) const 
		{
			return voronoi.sphere_normal(vertex_id);
		}

		inline constexpr vec3 vertex_east(const vec3 vertex_normal, const vec3 north_pole) const 
		{
			return glm::normalize(glm::cross(vertex_normal, north_pole));
		}

		inline constexpr vec3 vertex_north(const vec3 vertex_east, const vec3 vertex_normal) const 
		{
			return glm::normalize(glm::cross(vertex_east, vertex_normal));
		}

		inline constexpr mat3 vertex_frame(const id vertex_id, const vec3 north_pole) const 
		{
			const vec3 up(vertex_normal(vertex_id));
			const vec3 east(vertex_east(up, north_pole));
			const vec3 north(vertex_north(east, up));
			return glm::transpose(mat3(east, north, up));
		}

		constexpr scalar vertex_dual_area(const id vertex_id) const 
		{
			const ScalarPoint idO(voronoi.grid_id(vertex_id));
			const vec3 pointO(voronoi.sphere_position(idO));
			const vec3 offsetAB(voronoi.sphere_position(idO+vec2( 0.5, 0.5)) - pointO);
			const vec3 offsetBC(voronoi.sphere_position(idO+vec2( 0.5,-0.5)) - pointO);
			const vec3 offsetCD(voronoi.sphere_position(idO+vec2(-0.5,-0.5)) - pointO);
			const vec3 offsetDA(voronoi.sphere_position(idO+vec2(-0.5, 0.5)) - pointO);
			return (glm::length(glm::cross(offsetAB, offsetBC))
				+ 	glm::length(glm::cross(offsetBC, offsetCD))
				+ 	glm::length(glm::cross(offsetCD, offsetDA))
				+ 	glm::length(glm::cross(offsetDA, offsetAB)))/2.0;
		}

	};

}

