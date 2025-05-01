#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries

namespace dymaxion
{

	/*
	GridSeries.hpp contains classes of indexible objects that wrap the methods of a dymaxion::Grid.
	with the intent of participating in functions that require out-of-order memory access
	(such as those under vector_calculus.hpp or morphology.hpp),
	while still leveraging the procedural functionality within dymaxion::Grid
	*/

	#define DYMAXION_SERIES(TYPE, SIZE, TITLE, LOWER, METHOD) \
	template<typename id, typename scalar> \
	struct TITLE \
	{ \
		Grid<id,scalar> grid; \
		constexpr inline explicit TITLE(const Grid<id,scalar> grid): grid(grid) {} \
	    using ivec3 = glm::vec<3,id,glm::defaultp>; \
	    using vec3 = glm::vec<3,scalar,glm::defaultp>; \
	    using mat3 = glm::mat<3,3,scalar,glm::defaultp>; \
        using ipoint = Point<id,id>;\
        using point = Point<id,scalar>;\
	    using size_type = std::size_t; \
		using value_type = TYPE; \
		constexpr inline size_type size() const { return SIZE(); } \
		constexpr inline value_type operator()(const size_type memory_id ) const { return METHOD(memory_id); } \
		constexpr inline value_type operator[](const size_type memory_id ) const { return METHOD(memory_id); } \
	};\
	template<typename id, typename scalar>\
	constexpr inline TITLE<id,scalar> LOWER(const Grid<id,scalar> grid)\
	{\
		return TITLE<id,scalar>(grid);\
	}

	DYMAXION_SERIES(id,     grid.vertex_count,  SquareId,           square_ids,           grid.square_id)
	DYMAXION_SERIES(vec3,   grid.vertex_count,  VertexPositions,    vertex_positions,     grid.vertex_position)
	DYMAXION_SERIES(vec3,   grid.vertex_count,  VertexNormals,      vertex_normals,       grid.vertex_normal)
	DYMAXION_SERIES(vec3,   grid.vertex_count,  VertexEast,         vertex_east,          grid.vertex_east)
	DYMAXION_SERIES(vec3,   grid.vertex_count,  VertexNorth,        vertex_north,         grid.vertex_north)
	DYMAXION_SERIES(mat3,   grid.vertex_count,  VertexFrame,        vertex_frame,         grid.vertex_frame)
	DYMAXION_SERIES(scalar, grid.vertex_count,  VertexDualAreas,    vertex_dual_areas,    grid.vertex_dual_area)
	DYMAXION_SERIES(ipoint, grid.vertex_count,  VertexGridIds,      vertex_grid_ids,      grid.voronoi.grid_id)
	#undef DYMAXION_SERIES

    /*
    `dymaxion::GridSeries` is a wrapper for `dymaxion::Grid` that is polymorphic to `dymaxion::GridCache`.
    Whereas `GridCache` provides options between cached std::vector attributes and procedural methods 
    `GridSeries` forces usage of procedural methods by providing indexible attributes that are wrappers to procedural methods.
	This can be used either to quickly test performance using procedural methods,
	or to force the use of procedural methods on functions that otherwise prefer cached std::vectors when the developer thinks they know better.
	To illustrate, some functions might prefer cached std::vectors because they use an in-order traversal and want to optimize for that,
	but a developer might need to coopt those functions to run in a parallel scenario where shared memory resources are discouraged.
	They would pass that function a `dymaxion::GridSeries`.
    */
    template<typename id, typename scalar, glm::qualifier Q=glm::defaultp>
	class GridSeries{

        using ivec2 = glm::vec<2,id,Q>;
        using vec3 = glm::vec<3,scalar,Q>;

        using mat3 = glm::mat<3,3,scalar,Q>;

	public:

		const Grid<id,scalar,Q> grid;
		VertexDualAreas<id,scalar> vertex_dual_areas;
		VertexPositions<id,scalar> vertex_positions;
		VertexNormals<id,scalar> vertex_normals;

		using size_type = id;
		using value_type = scalar;

		static constexpr id arrows_per_vertex = 4;

        inline constexpr explicit GridSeries(const Grid<id,scalar,Q> grid):
        	grid(grid),
        	vertex_dual_areas(grid),
        	vertex_positions(grid),
        	vertex_normals(grid)
    	{}

		inline constexpr id radius() const
		{
			return grid.radius();
		}

		inline constexpr id square_id(const id vertex_id) const
		{
			return grid.square_id(vertex_id);
		}

		inline constexpr id arrow_offset_id(const id arrow_id) const
		{
			return grid.arrow_offset_id(arrow_id);
		}

		inline constexpr id arrow_offset_id(const ivec2 arrow_offset_grid_position) const
		{
			return grid.arrow_offset_id(arrow_offset_grid_position);
		}

		inline constexpr id arrow_source_id(const id arrow_id) const
		{
			return grid.arrow_source_id(arrow_id);
		}

		inline constexpr id arrow_target_id(const id source_id, const id offset_id) const
		{
			return grid.arrow_target_id(source_id, offset_id);
		}

		inline constexpr id arrow_target_id(const id arrow_id) const
		{
			return grid.arrow_target_id(arrow_id);
		}

		inline constexpr id arrow_id(const id source_id, const id arrow_offset_id) const
		{
			return grid.arrow_id(source_id, arrow_offset_id);
		}

		inline constexpr ivec2 arrow_offset_grid_position(const id arrow_offset_id) const
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

		inline constexpr id vertices_per_square_side() const
		{
			return grid.vertices_per_square_side();
		}

		inline constexpr id vertices_per_meridian() const
		{
			return grid.vertices_per_meridian();
		}

		inline constexpr id vertex_count() const 
		{
			return grid.vertex_count();
		}

		inline constexpr id arrow_count() const 
		{
			return grid.arrow_count();
		}

		// offset of the arrow
		inline constexpr vec3 arrow_offset(const id source_id, const id offset_id) const
		{
			return grid.arrow_offset(source_id, offset_id);
		}

		// normal of the arrow
		inline constexpr vec3 arrow_normal(const id source_id, const id offset_id) const
		{
			return grid.arrow_normal(source_id, offset_id);
		}

		// length of the arrow
		inline constexpr scalar arrow_length(const id source_id, const id offset_id) const
		{
			return grid.arrow_length(source_id, offset_id);
		}

		// length of the arrow's dual
		inline constexpr scalar arrow_dual_length(const id source_id, const id offset_id) const
		{
			return grid.arrow_dual_length(source_id, offset_id);
		}

		// `vertex_representative()` returns the memory id of a vertex
		// whose associated arrows should not cause artifacts during certain sensitive operations
		// (like gradient, divergence, laplacian) while also being physically near the vertex of the specified `vertex_id`,
		// thereby providing an adequate representation for the vertex with irregular edges.
		inline constexpr id vertex_representative(const id vertex_id) const 
		{
			return grid.vertex_representative(vertex_id);
		}

		inline constexpr vec3 vertex_position(const id vertex_id) const 
		{
			return grid.vertex_position(vertex_id);
		}

		inline constexpr vec3 vertex_normal(const id vertex_id) const 
		{
			return grid.vertex_normal(vertex_id);
		}

		inline constexpr vec3 vertex_east(const vec3 vertex_normal, const vec3 north_pole) const 
		{
			return grid.vertex_east(vertex_normal, north_pole);
		}

		inline constexpr vec3 vertex_north(const vec3 vertex_east, const vec3 vertex_normal) const 
		{
			return grid.vertex_north(vertex_east, vertex_normal);
		}

		inline constexpr mat3 vertex_frame(const id vertex_id, const vec3 north_pole) const 
		{
			return grid.vertex_frame(vertex_id, north_pole);
		}

		inline constexpr scalar vertex_dual_area(const id vertex_id) const 
		{
			return grid.vertex_dual_area(vertex_id);
		}

		inline constexpr id nearest_vertex_id(const vec3 vertex_position) const
		{
			return grid.nearest_vertex_id(vertex_position);
		}

	};

}

