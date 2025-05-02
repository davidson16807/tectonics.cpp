#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/geometric.hpp>

// in-house libraries
#include "Voronoi.hpp"
#include "Indexing.hpp"

namespace dymaxion 
{

    /*
    `dymaxion::GridCache` is a wrapper for `dymaxion::Grid`
    that caches calculations in Grid to memory
    where doing so results in a performance gain on large grids.
    Care should be taken if using `GridCache` as a class attribute
    since its memory footprint is not trivial as with `Grid`.
    */
    template<typename id, typename id2, typename scalar, glm::qualifier Q=glm::defaultp>
	class GridCache{

        using ivec2 = glm::vec<2,id2,Q>;
        using vec3 = glm::vec<3,scalar,Q>;

        using mat3 = glm::mat<3,3,scalar,Q>;

	public:

		const Grid<id,id2,scalar,Q> grid;
		std::vector<scalar> vertex_dual_areas;
		std::vector<vec3> vertex_positions;
		std::vector<vec3> vertex_normals;

		using coordinate_type = id;
		using size_type = id2;
		using value_type = scalar;

		static constexpr id2 arrows_per_vertex = 4;

        inline constexpr explicit GridCache(const Grid<id,id2,scalar,Q> grid):
        	grid(grid),
        	vertex_dual_areas(grid.vertex_count()),
        	vertex_positions(grid.vertex_count()),
        	vertex_normals(grid.vertex_count())
    	{
    		for (int i = 0; i < grid.vertex_count(); ++i)
    		{
    			vertex_dual_areas[i] = grid.vertex_dual_area(i);
    		}
    		for (int i = 0; i < grid.vertex_count(); ++i)
    		{
    			vertex_normals[i] = grid.vertex_normal(i);
    		}
    		for (int i = 0; i < grid.vertex_count(); ++i)
    		{
    			vertex_positions[i] = grid.vertex_position(i);
    		}
    	}

		inline constexpr id2 radius() const
		{
			return grid.radius();
		}

		inline constexpr id2 square_id(const id2 vertex_id) const
		{
			return grid.square_id(vertex_id);
		}

		inline constexpr id2 arrow_offset_id(const id2 arrow_id) const
		{
			return grid.arrow_offset_id(arrow_id);
		}

		inline constexpr id2 arrow_offset_id(const ivec2 arrow_offset_grid_position) const
		{
			return grid.arrow_offset_id(arrow_offset_grid_position);
		}

		inline constexpr id2 arrow_source_id(const id2 arrow_id) const
		{
			return grid.arrow_source_id(arrow_id);
		}

		inline constexpr id2 arrow_target_id(const id2 source_id, const id2 offset_id) const
		{
			return grid.arrow_target_id(source_id, offset_id);
		}

		inline constexpr id2 arrow_target_id(const id2 arrow_id) const
		{
			return grid.arrow_target_id(arrow_id);
		}

		inline constexpr id2 arrow_id(const id2 source_id, const id2 arrow_offset_id) const
		{
			return grid.arrow_id(source_id, arrow_offset_id);
		}

		inline constexpr ivec2 arrow_offset_grid_position(const id2 arrow_offset_id) const
		{
			return grid.arrow_offset_grid_position(arrow_offset_id);
		}

		inline constexpr scalar total_radius() const 
		{
			return grid.total_radius();
		}

		inline constexpr scalar total_diameter() const 
		{
			return grid.total_diameter();
		}

		inline constexpr scalar total_area() const 
		{
			return grid.total_area();
		}

		inline constexpr scalar total_volume() const 
		{
			return grid.total_volume();
		}

		inline constexpr scalar total_circumference() const 
		{
			return grid.total_circumference();
		}

		inline constexpr id2 vertices_per_square_side() const
		{
			return grid.vertices_per_square_side();
		}

		inline constexpr id2 vertices_per_meridian() const
		{
			return grid.vertices_per_meridian();
		}

		inline constexpr id2 vertex_count() const 
		{
			return grid.vertex_count();
		}

		inline constexpr id2 arrow_count() const 
		{
			return grid.arrow_count();
		}

		// offset of the arrow
		inline constexpr vec3 arrow_offset(const id2 source_id, const id2 offset_id) const
		{
			return grid.arrow_offset(source_id, offset_id);
		}

		// normal of the arrow
		inline constexpr vec3 arrow_normal(const id2 source_id, const id2 offset_id) const
		{
			return grid.arrow_normal(source_id, offset_id);
		}

		// length of the arrow
		inline constexpr scalar arrow_length(const id2 source_id, const id2 offset_id) const
		{
			return grid.arrow_length(source_id, offset_id);
		}

		// length of the arrow's dual
		inline constexpr scalar arrow_dual_length(const id2 source_id, const id2 offset_id) const
		{
			return grid.arrow_dual_length(source_id, offset_id);
		}

		// `vertex_representative()` returns the memory id2 of a vertex
		// whose associated arrows should not cause artifacts during certain sensitive operations
		// (like gradient, divergence, laplacian) while also being physically near the vertex of the specified `vertex_id`,
		// thereby providing an adequate representation for the vertex with irregular edges.
		inline constexpr id2 vertex_representative(const id2 vertex_id) const 
		{
			return grid.vertex_representative(vertex_id);
		}

		inline constexpr vec3 vertex_position(const id2 vertex_id) const 
		{
			return grid.vertex_position(vertex_id);
			// return vertex_positions[vertex_id];
		}

		inline constexpr vec3 vertex_normal(const id2 vertex_id) const 
		{
			return grid.vertex_normal(vertex_id);
			// return vertex_normals[vertex_id];
		}

		inline constexpr vec3 vertex_east(const vec3 vertex_normal, const vec3 north_pole) const 
		{
			return grid.vertex_east(vertex_normal, north_pole);
		}

		inline constexpr vec3 vertex_north(const vec3 vertex_east, const vec3 vertex_normal) const 
		{
			return grid.vertex_north(vertex_east, vertex_normal);
		}

		inline constexpr mat3 vertex_frame(const id2 vertex_id, const vec3 north_pole) const 
		{
			return grid.vertex_frame(vertex_id, north_pole);
		}

		inline constexpr scalar vertex_dual_area(const id2 vertex_id) const 
		{
			// return grid.vertex_dual_area(vertex_id);
			return vertex_dual_areas[vertex_id];
		}

		inline constexpr id2 nearest_vertex_id(const vec3 vertex_position) const
		{
			return grid.nearest_vertex_id(vertex_position);
		}

	};

}

