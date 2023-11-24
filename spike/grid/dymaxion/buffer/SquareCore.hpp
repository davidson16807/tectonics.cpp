#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries

// in-house libraries
#include "../VectorIndexing.hpp"

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

	template<typename id, glm::qualifier Q=glm::defaultp>
	class SquareCore
	{

        using ivec2 = glm::vec<2,id,Q>;
        using IdPoint = Point<id,id>;

		static constexpr id vertices_per_triangle = 3;
		static constexpr id triangles_per_quad  = 2;
		static constexpr id vertices_per_triangle_quad = triangles_per_quad * vertices_per_triangle;
		static constexpr id vertices_per_triangle_strip_quad = 4;


	public:
        const PointIndexing<id> vertices;
        const Vector2Indexing<id> buffers;
		constexpr inline explicit SquareCore(const id vertex_count_per_square_side): 
			vertices(vertex_count_per_square_side),
			buffers (vertex_count_per_square_side)
		{}

		constexpr inline id point_size(const float value) const
		{
			return 1;
		}

		template<int L, typename T>
		constexpr inline id point_size(const glm::vec<L,T,Q> value) const
		{
			return L;
		}

		template<typename Buffer>
		constexpr inline id point(const float value, Buffer& output, const id buffer_start_id=0) const
		{
			output[buffer_start_id] = value;
			return buffer_start_id + 1;
		}

		template<int L, typename T, typename Buffer>
		constexpr id point(const glm::vec<L,T,Q> value, Buffer& output, const id buffer_start_id=0) const
		{
			id buffer_id = buffer_start_id;
			for (int i = 0; i < L; ++i)
			{
				output[buffer_id+i] = value[i];
			}
			return buffer_start_id + L;
		}

		constexpr inline id quad_count() const
		{
			return buffers.vertex_count;
		}

		template<typename Series>
		constexpr inline id triangles_size(const Series& input) const
		{
			return quad_count() * vertices_per_triangle_quad * point_size(input[0]) +
				buffers.vertex_count_per_side * vertices_per_triangle_quad * point_size(input[0]);
		}

		template<typename Series, typename Buffer>
		constexpr id triangles(const id square_id, const Series& input, Buffer& output, const id buffer_start_id=0) const 
		{
			using element = typename Series::value_type;
			element N,S,E,W;
			id buffer_id = buffer_start_id;
			ivec2 grid_id;
			for (id quad_id = 0; quad_id < quad_count(); ++quad_id)
			{
				grid_id = buffers.grid_id(quad_id)+ivec2(0,-1);
				S = input[vertices.memory_id(IdPoint(square_id, grid_id             ))];
				E = input[vertices.memory_id(IdPoint(square_id, grid_id + ivec2(1,0)))];
				W = input[vertices.memory_id(IdPoint(square_id, grid_id + ivec2(0,1)))];
				N = input[vertices.memory_id(IdPoint(square_id, grid_id + ivec2(1,1)))];
				buffer_id = point(S, output, buffer_id);
				buffer_id = point(E, output, buffer_id);
				buffer_id = point(W, output, buffer_id);
				buffer_id = point(W, output, buffer_id);
				buffer_id = point(E, output, buffer_id);
				buffer_id = point(N, output, buffer_id);
			}
			// eastern corner requires special handling
			grid_id = ivec2(buffers.vertex_count_per_side-1,0);
			W = input[vertices.memory_id(IdPoint(square_id, grid_id              ))];
			N = input[vertices.memory_id(IdPoint(square_id, grid_id + ivec2(1, 0)))];
			S = input[vertices.memory_id(IdPoint(square_id, grid_id + ivec2(0,-1)))];
			E = input[vertices.memory_id(IdPoint(square_id, grid_id + ivec2(1, 1)))];
			buffer_id = point(S, output, buffer_id);
			buffer_id = point(N, output, buffer_id);
			buffer_id = point(W, output, buffer_id);
			buffer_id = point(S, output, buffer_id);
			buffer_id = point(E, output, buffer_id);
			buffer_id = point(N, output, buffer_id);
			return buffer_id;
		}

		template<typename Series>
		constexpr inline id triangle_strips_size(const Series& input) const
		{
			/*
			NOTE:
			The first and the last entries in the output are always repeated so that multiple strips can share the same buffer.
			For illustration, see https://stackoverflow.com/questions/5775105/problem-with-degenerate-triangles-and-gl-triangle-strip
			*/
			return (buffers.vertex_count_per_side+2) * buffers.vertex_count_per_side * vertices_per_triangle_strip_quad * point_size(input[0]);
		}

		template<typename Series, typename Buffer>
		constexpr id triangle_strips(const id square_id, const Series& input, Buffer& output, const id buffer_start_id=0) const 
		{
			/*
			NOTE:
			The first and the last entries in each strip are always repeated so that multiple strips can share the same buffer.
			For illustration, see https://stackoverflow.com/questions/5775105/problem-with-degenerate-triangles-and-gl-triangle-strip
			*/
			id buffer_id = buffer_start_id;
			ivec2 grid_id;
			for (int j = 0; j < buffers.vertex_count_per_side; ++j) {
				grid_id = ivec2(j,0);
				buffer_id = point(input[vertices.memory_id(IdPoint(square_id, grid_id))], output, buffer_id); // S
				for (int i = 0; i < buffers.vertex_count_per_side; ++i) {
					grid_id = ivec2(j,i);
					buffer_id = point(input[vertices.memory_id(IdPoint(square_id, grid_id             ))], output, buffer_id); // S
					buffer_id = point(input[vertices.memory_id(IdPoint(square_id, grid_id + ivec2(1,0)))], output, buffer_id); // E
					buffer_id = point(input[vertices.memory_id(IdPoint(square_id, grid_id + ivec2(0,1)))], output, buffer_id); // W
					buffer_id = point(input[vertices.memory_id(IdPoint(square_id, grid_id + ivec2(1,1)))], output, buffer_id); // N
				}
				buffer_id = point(input[vertices.memory_id(IdPoint(square_id, grid_id + ivec2(1,1)))], output, buffer_id);
			}
			return buffer_id;
		}

	};

}}

