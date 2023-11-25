#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries

// in-house libraries

namespace dymaxion {
namespace buffer {

	/*
	`dymaxion::buffer::Pole` contains methods 
	that store representations of faces near the pole 
	of a `dymaxion::Grid` to a graphics library buffer.
	Decomposing the `dymaxion::Grid` in this way is done to 
	simplify testing, debugging, and implementation,
	and to avoid duplicated faces that are prone to z-fighting.
	See README.md for general discussion on design.
	*/

	template<typename id, glm::qualifier Q=glm::defaultp>
	class Pole
	{

        using ivec2 = glm::vec<2,id,Q>;
        using IdPoint = Point<id,id>;

		static constexpr id vertices_per_triangle = 3;
		static constexpr id triangle_count = 3;

	public:
        const Indexing<id> vertices;
        const int vertices_per_side;
		constexpr inline explicit Pole(const id vertices_per_side): 
			vertices(vertices_per_side),
			vertices_per_side(vertices_per_side)
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

		template<typename Series>
		constexpr inline id triangles_size(const Series& input) const
		{
			return vertices_per_triangle * triangle_count * point_size(input[0]);
		}

		template<typename Series, typename Buffer>
		constexpr id triangles(const id square_id, const Series& input, Buffer& output, const id buffer_start_id=0) const 
		{
			using element = typename Series::value_type;
			id buffer_id = buffer_start_id;
			ivec2 grid_id(square_id%2? 0 : vertices_per_side-1);
			element W2 = input[vertices.memory_id(IdPoint(square_id-4, grid_id))];
			element W1 = input[vertices.memory_id(IdPoint(square_id-2, grid_id))];
			element O  = input[vertices.memory_id(IdPoint(square_id+0, grid_id))];
			element E1 = input[vertices.memory_id(IdPoint(square_id+2, grid_id))];
			element E2 = input[vertices.memory_id(IdPoint(square_id+4, grid_id))];
			buffer_id = point(O,  output, buffer_id);
			buffer_id = point(W2, output, buffer_id);
			buffer_id = point(W1, output, buffer_id);
			buffer_id = point(O,  output, buffer_id);
			buffer_id = point(E1, output, buffer_id);
			buffer_id = point(E2, output, buffer_id);
			buffer_id = point(O,  output, buffer_id);
			buffer_id = point(E2, output, buffer_id);
			buffer_id = point(W2, output, buffer_id);
			return buffer_id;
		}

	};

}}

