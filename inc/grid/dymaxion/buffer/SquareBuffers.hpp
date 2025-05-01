#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries

// in-house libraries
#include <buffer/PrimitiveBuffers.hpp>
#include "../Indexing.hpp"

namespace dymaxion {

	/*
	`dymaxion::SquareBuffers` contains methods 
	that store representations of a `dymaxion::Square` to a graphics library buffer.
	Decomposing the `dymaxion::Grid` in this way is done to 
	simplify testing, debugging, and implementation,
	as well as to permit partial renders of the sphere at high LOD,
	such as when on the surface of a planet.
	See README.md for general discussion on design.
	*/

	template<typename id, typename scalar, glm::qualifier Q=glm::defaultp>
	class SquareBuffers
	{

        using ivec2 = glm::vec<2,std::int8_t,Q>;
        using ipoint = Point<id,std::int8_t>;

		static constexpr id vertices_per_triangle = 3;
		static constexpr id triangles_per_quad  = 2;
		static constexpr id vertices_per_quad = triangles_per_quad * vertices_per_triangle;
		static constexpr id vertices_per_strip_quad = 4;

        const Indexing<id,scalar> vertices;
        const id vertices_per_square_side;
        const buffer::PrimitiveBuffers<id,Q> primitives;

	public:
		constexpr inline explicit SquareBuffers(const id vertices_per_square_side): 
			vertices(vertices_per_square_side),
			vertices_per_square_side(vertices_per_square_side),
			primitives()
		{}

		constexpr inline id quad_count() const
		{
			return vertices_per_square_side * vertices_per_square_side;
		}

		template<typename Series>
		constexpr inline id triangles_size(const Series& input) const
		{
			return quad_count() * vertices_per_quad * primitives.point_size(input[0]) +
				vertices_per_square_side * vertices_per_quad * primitives.point_size(input[0]);
		}

		template<typename Series, typename Buffer>
		constexpr id storeTriangles(const id square_id, const Series& input, Buffer& output, const id buffer_start_id=0) const 
		{
			using element = typename Series::value_type;
			element N,S,E,W;
			id buffer_id = buffer_start_id;
			ivec2 grid_id;
			for (int j = 0; j < vertices_per_square_side; ++j) {
				for (int i = 0; i < vertices_per_square_side; ++i) {
					grid_id = ivec2(i,j);
					W = input[vertices.memory_id(ipoint(square_id, grid_id             ))];
					S = input[vertices.memory_id(ipoint(square_id, grid_id + ivec2(1,0)))];
					N = input[vertices.memory_id(ipoint(square_id, grid_id + ivec2(0,1)))];
					E = input[vertices.memory_id(ipoint(square_id, grid_id + ivec2(1,1)))];
					buffer_id = primitives.storeTriangle(S,E,W, output, buffer_id);
					buffer_id = primitives.storeTriangle(W,E,N, output, buffer_id);
				}
			}
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
			return (vertices_per_square_side+2) * vertices_per_square_side * vertices_per_strip_quad * primitives.point_size(input[0]);
		}

		template<typename Series, typename Buffer>
		constexpr id storeTriangleStrips(const id square_id, const Series& input, Buffer& output, const id buffer_start_id=0) const 
		{
			/*
			NOTE:
			The first and the last entries in each strip are always repeated so that multiple strips can share the same buffer.
			For illustration, see https://stackoverflow.com/questions/5775105/problem-with-degenerate-triangles-and-gl-triangle-strip
			*/
			id buffer_id = buffer_start_id;
			ivec2 grid_id;
			for (int i = 0; i < vertices_per_square_side; ++i) {
				grid_id = ivec2(i,0);
				buffer_id = primitives.storePoint(input[vertices.memory_id(ipoint(square_id, grid_id))], output, buffer_id); // S
				for (int j = 0; j < vertices_per_square_side; ++j) {
					grid_id = ivec2(i,j);
					buffer_id = primitives.storePoint(input[vertices.memory_id(ipoint(square_id, grid_id + ivec2(1,0)))], output, buffer_id); // S
					buffer_id = primitives.storePoint(input[vertices.memory_id(ipoint(square_id, grid_id + ivec2(1,1)))], output, buffer_id); // E
					buffer_id = primitives.storePoint(input[vertices.memory_id(ipoint(square_id, grid_id             ))], output, buffer_id); // W
					buffer_id = primitives.storePoint(input[vertices.memory_id(ipoint(square_id, grid_id + ivec2(0,1)))], output, buffer_id); // N
				}
				buffer_id = primitives.storePoint(input[vertices.memory_id(ipoint(square_id, grid_id + ivec2(1,1)))], output, buffer_id);
			}
			return buffer_id;
		}

	};

}

