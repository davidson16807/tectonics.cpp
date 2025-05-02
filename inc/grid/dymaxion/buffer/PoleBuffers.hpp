#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries

// in-house libraries
#include <buffer/PrimitiveBuffers.hpp>
#include "../Indexing.hpp"

namespace dymaxion {

	/*
	`dymaxion::PoleBuffers` contains methods 
	that store representations of faces near the pole 
	of a `dymaxion::Grid` to a graphics library buffer.
	Decomposing the `dymaxion::Grid` in this way is done to 
	simplify testing, debugging, and implementation,
	and to avoid duplicated faces that are prone to z-fighting.
	See README.md for general discussion on design.
	*/

	template<typename id, typename id2, typename scalar, glm::qualifier Q=glm::defaultp>
	class PoleBuffers
	{

        using ivec2 = glm::vec<2,id,Q>;
        using ipoint = Point<id,id>;

		static constexpr id vertices_per_triangle = 3;
		static constexpr id triangle_count = 3;

	public:
        const Indexing<id,id2,scalar> vertices;
        const int vertices_per_side;
        const buffer::PrimitiveBuffers<id2,Q> primitives;

		constexpr inline explicit PoleBuffers(const id vertices_per_side): 
			vertices(vertices_per_side),
			vertices_per_side(vertices_per_side),
			primitives()
		{}

		template<typename Series>
		constexpr inline id2 triangles_size(const Series& input) const
		{
			return vertices_per_triangle * triangle_count * primitives.point_size(input[0]);
		}

		template<typename Series, typename Buffer>
		constexpr id2 storeTriangles(const id2 square_id, const Series& input, Buffer& output, const id2 buffer_start_id=0) const 
		{
			using element = typename Series::value_type;
			id2 buffer_id(buffer_start_id);
			bool is_southern(square_id%2);
			ivec2 grid_id = is_southern? ivec2(vertices_per_side-1,0) : ivec2(0,vertices_per_side-1);
			element W2 = input[vertices.memory_id(ipoint(square_id-4, grid_id))];
			element W1 = input[vertices.memory_id(ipoint(square_id-2, grid_id))];
			element O  = input[vertices.memory_id(ipoint(square_id+0, grid_id))];
			element E1 = input[vertices.memory_id(ipoint(square_id+2, grid_id))];
			element E2 = input[vertices.memory_id(ipoint(square_id+4, grid_id))];
			if (is_southern)
			{
				buffer_id = primitives.storeTriangle(O, W1, W2, output, buffer_id);
				buffer_id = primitives.storeTriangle(O, E2, E1, output, buffer_id);
				buffer_id = primitives.storeTriangle(O, W2, E2, output, buffer_id);
			} else {
				buffer_id = primitives.storeTriangle(O, W2, W1, output, buffer_id);
				buffer_id = primitives.storeTriangle(O, E1, E2, output, buffer_id);
				buffer_id = primitives.storeTriangle(O, E2, W2, output, buffer_id);
			}
			return buffer_id;
		}

		template<typename Series>
		constexpr inline id2 triangle_strips_size(const Series& input) const
		{
			return 7 * primitives.point_size(input[0]);
		}

		template<typename Series, typename Buffer>
		constexpr id2 storeTriangleStrips(const id2 square_id, const Series& input, Buffer& output, const id2 buffer_start_id=0) const 
		{
			using element = typename Series::value_type;
			id2 buffer_id(buffer_start_id);
			bool is_southern(square_id%2);
			ivec2 grid_id = is_southern? ivec2(vertices_per_side-1,0) : ivec2(0,vertices_per_side-1);
			element W2 = input[vertices.memory_id(ipoint(square_id-4, grid_id))];
			element W1 = input[vertices.memory_id(ipoint(square_id-2, grid_id))];
			element O  = input[vertices.memory_id(ipoint(square_id+0, grid_id))];
			element E1 = input[vertices.memory_id(ipoint(square_id+2, grid_id))];
			element E2 = input[vertices.memory_id(ipoint(square_id+4, grid_id))];
			buffer_id = primitives.storePoint(W1,  output, buffer_id);
			buffer_id = primitives.storePoint(W1,  output, buffer_id);
			buffer_id = primitives.storePoint(W2,  output, buffer_id);
			buffer_id = primitives.storePoint(O,   output, buffer_id);
			buffer_id = primitives.storePoint(E2,  output, buffer_id);
			buffer_id = primitives.storePoint(E1,  output, buffer_id);
			buffer_id = primitives.storePoint(E1,  output, buffer_id);
			return buffer_id;
		}

	};

}