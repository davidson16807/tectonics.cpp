#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/geometric.hpp>

// in-house libraries
#include <math/special.hpp>

#include "Voronoi.hpp"
#include "Indexing.hpp"

namespace dymaxion 
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
    `Grid` builds upon `Voronoi` and `Indexing` to introduce concepts 
    that are necessary for spatially aware operations such as those in vector calculus or binary morphology.
    These concepts include vertex neighbors, faces, edges, and arrows, and the duals of such concepts.
    */
    template<typename id, typename id2, typename scalar, glm::qualifier precision=glm::defaultp>
	class Grid{

        using ivec2 = glm::vec<2,id,precision>;
        using vec2 = glm::vec<2,scalar,precision>;
        using vec3 = glm::vec<3,scalar,precision>;

        using mat3 = glm::mat<3,3,scalar,precision>;

		static constexpr scalar pi = scalar(3.141592653589793238462643383279502884L);

		static constexpr id i0 = (0);
		static constexpr id i1 = (1);
		static constexpr id i2 = (2);

	public:

		const Voronoi<id,id2,scalar> voronoi;
		const Indexing<id,id2,scalar> memory;

		using size_type = id2;
		using value_type = scalar;

		static constexpr id2 arrows_per_vertex = 4;

        inline constexpr explicit Grid(const scalar radius, const id2 vertices_per_square_side) : 
        	voronoi(radius, vertices_per_square_side),
        	memory (vertices_per_square_side)
    	{}

		inline constexpr scalar radius() const noexcept
		{
			return voronoi.radius;
		}

		inline constexpr id2 square_id(const id2 vertex_id) const noexcept
		{
			return memory.grid_id(vertex_id).square_id;
		}

		inline constexpr id2 arrow_offset_id(const id2 arrow_id) const noexcept
		{
			return math::floormod(arrow_id, arrows_per_vertex);
		}

		constexpr id2 arrow_offset_id(const ivec2 arrow_offset_grid_position) const noexcept
		{
			return 2*(arrow_offset_grid_position.x + arrow_offset_grid_position.y < 0) + 
				   (std::abs(arrow_offset_grid_position.y) > std::abs(arrow_offset_grid_position.x));
		}

		inline constexpr id2 arrow_source_id(const id2 arrow_id) const noexcept
		{
			return id2(arrow_id / arrows_per_vertex);
		}

		inline constexpr id2 arrow_target_id(const id2 source_id, const id2 offset_id) const noexcept
		{
			return memory.memory_id(memory.grid_id(source_id) + arrow_offset_grid_position(offset_id));
		}

		inline constexpr id2 arrow_target_id(const id2 arrow_id) const noexcept
		{
			return arrow_id % arrows_per_vertex;
		}

		inline constexpr id2 arrow_id(const id2 source_id, const id2 arrow_offset_id) const noexcept
		{
			return source_id * arrows_per_vertex + arrow_offset_id;
		}

		constexpr ivec2 arrow_offset_grid_position(const id2 arrow_offset_id) const noexcept
		{
			return 	id(-i2*((arrow_offset_id & i2) > i0)+i1)
				* 	ivec2(id((arrow_offset_id & i1)== i0),
						  id(arrow_offset_id & i1));
		}

		inline constexpr scalar total_radius() const  noexcept
		{
			return voronoi.radius;
		}

		inline constexpr scalar total_diameter() const  noexcept
		{
			return 2.0*voronoi.radius;
		}

		inline constexpr scalar total_area() const  noexcept
		{
			return 4.0 * pi * voronoi.radius * voronoi.radius;
		}

		inline constexpr scalar total_volume() const  noexcept
		{
			return 4.0/3.0 * pi * voronoi.radius * voronoi.radius * voronoi.radius;
		}

		inline constexpr scalar total_circumference() const  noexcept
		{
			return 2.0 * pi * voronoi.radius;
		}

		inline constexpr id2 vertices_per_square_side() const noexcept
		{
			return voronoi.vertices_per_square_side;
		}

		inline constexpr id2 vertices_per_meridian() const noexcept
		{
			return voronoi.vertices_per_meridian;
		}

		inline constexpr id2 vertex_count() const  noexcept
		{
			return voronoi.vertex_count;
		}

		inline constexpr id2 arrow_count() const  noexcept
		{
			// each vertex is guaranteed to have 4 arrows, however some arrows that cross seams will be of 0 dual area
			return arrows_per_vertex * voronoi.vertex_count;
		}

		// offset of the arrow
		inline constexpr vec3 arrow_offset(const id2 source_id, const id2 offset_id) const noexcept
		{
			return 	voronoi.sphere_position( memory.grid_id(source_id) + arrow_offset_grid_position(offset_id) )
				- 	voronoi.sphere_position( memory.grid_id(source_id) );
		}

		// normal of the arrow
		inline constexpr vec3 arrow_normal(const id2 source_id, const id2 offset_id) const noexcept
		{
			return glm::normalize(arrow_offset(source_id, offset_id));
		}

		// length of the arrow
		inline constexpr scalar arrow_length(const id2 source_id, const id2 offset_id) const noexcept
		{
			return glm::length(arrow_offset(source_id, offset_id));
		}

		// length of the arrow's dual
		constexpr scalar arrow_dual_length(const id2 source_id, const id2 offset_id) const noexcept
		{
			const auto Oid(memory.grid_id(source_id));
			const auto A(voronoi.sphere_position( Oid + arrow_offset_grid_position(math::floormod(offset_id+1, arrows_per_vertex)) ));
			const auto B(voronoi.sphere_position( Oid + arrow_offset_grid_position(math::floormod(offset_id,   arrows_per_vertex)) ));
			const auto C(voronoi.sphere_position( Oid + arrow_offset_grid_position(math::floormod(offset_id-1, arrows_per_vertex)) ));
			const auto AB(glm::normalize(A+B));
			const auto BC(glm::normalize(B+C));
			return glm::distance(AB,BC);
		}

		// `vertex_representative()` returns the memory id of a vertex
		// whose associated arrows should not cause artifacts during certain sensitive operations
		// (like gradient, divergence, laplacian) while also being physically near the vertex of the specified `vertex_id`,
		// thereby providing an adequate representation for the vertex with irregular edges.
		inline constexpr id2 vertex_representative(const id2 vertex_id) const  noexcept
		{
			return memory.memory_id(clamp(memory.grid_id(vertex_id), id2(1), voronoi.vertices_per_square_side-id2(2)));
		}

		inline constexpr vec3 vertex_position(const id2 vertex_id) const  noexcept
		{
			return voronoi.sphere_position(memory.grid_id(vertex_id));
		}

		inline constexpr vec3 vertex_normal(const id2 vertex_id) const  noexcept
		{
			return voronoi.sphere_normal(memory.grid_id(vertex_id));
		}

		inline constexpr vec3 vertex_east(const vec3 vertex_normal, const vec3 north_pole) const  noexcept
		{
			return glm::normalize(glm::cross(vertex_normal, north_pole));
		}

		inline constexpr vec3 vertex_north(const vec3 vertex_east, const vec3 vertex_normal) const  noexcept
		{
			return glm::normalize(glm::cross(vertex_east, vertex_normal));
		}

		inline constexpr mat3 vertex_frame(const id2 vertex_id, const vec3 north_pole) const  noexcept
		{
			const vec3 up(vertex_normal(vertex_id));
			const vec3 east(vertex_east(up, north_pole));
			const vec3 north(vertex_north(east, up));
			return glm::transpose(mat3(east, north, up));
		}

		constexpr scalar vertex_dual_area(const id2 vertex_id) const  noexcept
		{
			const scalar half(0.5);
			const auto Oid(memory.grid_id(vertex_id));
			const vec3 O(voronoi.sphere_position(Oid));
			const auto A(voronoi.sphere_position( Oid + arrow_offset_grid_position(0) ));
			const auto B(voronoi.sphere_position( Oid + arrow_offset_grid_position(1) ));
			const auto C(voronoi.sphere_position( Oid + arrow_offset_grid_position(2) ));
			const auto D(voronoi.sphere_position( Oid + arrow_offset_grid_position(3) ));
			const auto AB(glm::normalize(A+B)*voronoi.radius-O);
			const auto BC(glm::normalize(B+C)*voronoi.radius-O);
			const auto CD(glm::normalize(C+D)*voronoi.radius-O);
			const auto DA(glm::normalize(D+A)*voronoi.radius-O);
			auto area = 
			       (glm::length(glm::cross(AB, BC))
				+ 	glm::length(glm::cross(BC, CD))
				+ 	glm::length(glm::cross(CD, DA))
				+ 	glm::length(glm::cross(DA, AB)))*half;
			return area;
		}

		constexpr id2 nearest_vertex_id(const vec3 vertex_position) const noexcept
		{
			return memory.memory_id_when_standard(voronoi.grid_id(vertex_position));
		}

	};

}

