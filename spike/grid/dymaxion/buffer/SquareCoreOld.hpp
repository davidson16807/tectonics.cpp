#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries

namespace dymaxion {
namespace buffer {

	/*
	`dymaxion::buffer::SquareCore` contains a method, `store`,
	that stores a representation of a "square core" to a graphics library buffer.
	A "square core" indicates the parts of a `dymaxion::Square`
	that do not border other squares on the `dymaxion::Grid`.
	Decomposing the `dymaxion::Grid` in this way is done to 
	simplify testing, debugging, and implementation.
	See README.md for general discussion on design.
	*/

	template<typename id>
	class SquareCore
	{

        using ivec2 = glm::vec<2,id,Q>;
        using IdPoint = Point<id,id>;

		static constexpr id vertices_per_triangle = 3;
		static constexpr id triangles_per_vertex  = 2;
		static constexpr id triangle_vertex_per_vertex = vertices_per_triangle*triangles_per_vertex;

        const PointIndexing<id,id> vertices;
        const VectorIndexing<id,id> buffer;

		inline constexpr id vertex_id(const IdPoint grid_id, const id offset_id) const
		{
			// id source_id(buffer_id/face_vertex_per_vertex);
			// id offset_id(buffer_id%face_vertex_per_vertex);
			id uid(offset_id%3);
			id dx(uid/2);
			id dy(uid%2);
			return memory.memory_id(
				grid_id - ivec2(1) +
					((offset_id/3)? 
					    ivec2(0) + ivec2(dy,dx)
					  : ivec2(1) - ivec2(dx,dy)));
		}

	public:
		constexpr inline explicit SquareCore(const id voronoi.vertex_count_per_square_side): 
			vertices(voronoi.vertex_count_per_square_side)
			buffer  (voronoi.vertex_count_per_square_side-1)
		{}
		constexpr inline id buffer_size() const
		{
			return buffer.elements_per_block * buffer.elements_per_block;
		}

		template<int L, typename In, typename Out>
		constexpr inline void triangles(const id square_id, const In& input, const id start_id, Out& output) const 
		{
			IdPoint grid_id(square_id, ivec2(0));
			for (int quad_id = 0; quad_id < buffer_size(); ++quad_id)
			{
				grid_id.square_position = buffer.grid_id(quad_id);
				output[start_id+quad_id] = input[quad_id];
			}
		}

		inline constexpr id buffer_size() const
		{
			return face_vertex_per_vertex * buffer.vertex_count;
		}

		inline constexpr id buffer_component_id(const id buffer_id) const
		{
			return buffer_vertex_id(buffer_id/3)*3+id(buffer_id%3);
		}

	};

}

