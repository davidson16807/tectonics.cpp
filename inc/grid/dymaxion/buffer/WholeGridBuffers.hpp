#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries

// in-house libraries
#include "SquareBuffers.hpp"          // dymaxion::Square
#include "PoleBuffers.hpp"            // dymaxion::Pole

namespace dymaxion {

	/*
	`dymaxion::WholeGridBuffers` contains methods 
	that store representations of a `dymaxion::Grid` to a graphics library buffer.
	`dymaxion::WholeGridBuffers` coordinates `dymaxion::SquareBuffers` and `dymaxion::PoleBuffers`
	to cover each portion of the sphere in triangle faces without overlaps.
	See README.md for general discussion on design of `*Buffers` classes.
	*/

	template<typename id, typename id2, typename scalar, glm::qualifier Q=glm::defaultp>
	class WholeGridBuffers
	{

		static constexpr id2 square_count = 10;
		static constexpr id2 pole_count = 2;

        const SquareBuffers<id,id2,scalar,Q> squares;
        const PoleBuffers<id,id2,scalar,Q> poles;

	public:
		constexpr inline explicit WholeGridBuffers(const id2 vertices_per_square_side):
			squares(vertices_per_square_side),
			poles(vertices_per_square_side)
		{}

		template<typename Series>
		constexpr inline id2 triangles_size(const Series& input) const
		{
			return (
				square_count * squares.triangles_size(input) + 
				pole_count * poles.triangles_size(input)
			);
		}

		template<typename Series, typename Buffer>
		constexpr id2 storeTriangles(const Series& input, Buffer& output, const id2 buffer_start_id=0) const 
		{
			id2 buffer_id = buffer_start_id;
			buffer_id = poles.storeTriangles(0, input, output, buffer_id);
			buffer_id = poles.storeTriangles(1, input, output, buffer_id);
			for (int i=0; i<square_count; i++) { 
				buffer_id = squares.storeTriangles(i, input, output, buffer_id); 
			}
			return buffer_id;
		}

		template<typename Series>
		constexpr inline id2 triangle_strips_size(const Series& input) const
		{
			return (
				square_count * squares.triangle_strips_size(input) + 
				pole_count * poles.triangle_strips_size(input)
			);
		}

		template<typename Series, typename Buffer>
		constexpr id2 storeTriangleStrips(const Series& input, Buffer& output, const id2 buffer_start_id=0) const 
		{
			id2 buffer_id = buffer_start_id;
			buffer_id = poles.storeTriangleStrips(0, input, output, buffer_id);
			buffer_id = poles.storeTriangleStrips(1, input, output, buffer_id);
			for (int i=0; i<square_count; i++) { 
				buffer_id = squares.storeTriangleStrips(i, input, output, buffer_id); 
			}
			return buffer_id;
		}

	};

}

