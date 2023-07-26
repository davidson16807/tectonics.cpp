
// 3rd party libraries
#include <glm/geometric.hpp>

// in-house libraries
#include "Voronoi.hpp"

namespace collignon 
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
    template<typename id, typename scalar>
	class Grid{
        using ivec2 = glm::vec<2,id,glm::defaultp>;\
        using vec2 = glm::vec<2,scalar,glm::defaultp>;\
        using vec3 = glm::vec<3,scalar,glm::defaultp>;\

		static constexpr scalar pi = 3.141592652653589793f;

		const Voronoi<id,scalar> voronoi;

	public:

		static constexpr id arrows_per_vertex = 4;

        inline constexpr explicit Grid(const scalar radius, const id vertex_count_per_half_meridian) : 
        	voronoi(radius, vertex_count_per_half_meridian)
    	{}

        inline constexpr Grid(const Grid& grid) : voronoi(grid.voronoi) {} // copy constructor

		constexpr id arrow_offset_memory_id(const ivec2 arrow_offset_grid_position) const
		{
			return 	((arrow_offset_grid_position.x + arrow_offset_grid_position.y < 0) << 1)
				+	(std::abs(arrow_offset_grid_position.y) > std::abs(arrow_offset_grid_position.x));
		}

		constexpr ivec2 arrow_offset_grid_position(const id arrow_offset_memory_id) const
		{
			return 	((arrow_offset_memory_id >> 1)? -1 : 1) 
				* 	((arrow_offset_memory_id & 1)? ivec2(0,1) : ivec2(1,0));
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

		inline constexpr id arrow_memory_id(const id source_id, const id offset_id) const
		{
			return arrows_per_vertex * source_id + offset_id;
		}

		inline constexpr ivec2 arrow_target_grid_id(const id source_id, const id offset_id) const
		{
			return voronoi.grid_id(source_id) + arrow_offset_grid_position(offset_id);
		}

		inline constexpr id arrow_target_memory_id(const id source_id, const id offset_id) const
		{
			return voronoi.memory_id(arrow_target_grid_id(source_id, offset_id));
		}

		// offset of the arrow
		inline constexpr vec3 arrow_offset(const id source_id, const id offset_id) const
		{
			return 	voronoi.sphere_position( arrow_target_memory_id(source_id, offset_id) )
				- 	voronoi.sphere_position( source_id );
		}

		// normal of the arrow
		inline constexpr vec3 arrow_normal(const id source_id, const id offset_id) const
		{
			return glm::normalize(arrow_offset(source_id, offset_id));
		}

		// normal of the arrow
		inline constexpr scalar arrow_length(const id source_id, const id offset_id) const
		{
			return glm::length(arrow_offset(source_id, offset_id));
		}

		// length of the arrow's dual
		inline constexpr scalar arrow_dual_length(const id source_id, const id offset_id) const
		{
			const vec2 midpointOB(vec2(voronoi.grid_id(source_id)) + scalar(0.5) * vec2(arrow_offset_grid_position(offset_id)));
			return glm::distance(
					voronoi.sphere_position( midpointOB + scalar(0.5)*vec2(arrow_offset_grid_position((offset_id+1)%arrows_per_vertex)) ),
				 	voronoi.sphere_position( midpointOB + scalar(0.5)*vec2(arrow_offset_grid_position((offset_id-1)%arrows_per_vertex)) )
				);
		}

		inline constexpr scalar vertex_dual_area(const id vertex_id) const 
		{
			const vec2 idO(voronoi.grid_id(vertex_id));
			const vec3 pointO(voronoi.sphere_position(idO));
			const vec3 offsetAB(voronoi.sphere_position(idO+vec2( 0.5, 0.5)) - pointO);
			const vec3 offsetBC(voronoi.sphere_position(idO+vec2( 0.5,-0.5)) - pointO);
			const vec3 offsetCD(voronoi.sphere_position(idO+vec2(-0.5,-0.5)) - pointO);
			const vec3 offsetDA(voronoi.sphere_position(idO+vec2(-0.5, 0.5)) - pointO);
			return 	glm::length(glm::cross(offsetAB, offsetBC))/2.0
				+ 	glm::length(glm::cross(offsetBC, offsetCD))/2.0
				+ 	glm::length(glm::cross(offsetCD, offsetDA))/2.0
				+ 	glm::length(glm::cross(offsetDA, offsetAB))/2.0;
		}

	};

}

