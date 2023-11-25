#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries

// in-house libraries
#include "SquareBuffers.hpp"          // dymaxion::Square
#include "PoleBuffers.hpp"            // dymaxion::Pole

namespace dymaxion {

	/*
	`dymaxion::buffer::Square` contains methods 
	that store representations of a `dymaxion::Square` to a graphics library buffer.
	Decomposing the `dymaxion::Grid` in this way is done to 
	simplify testing, debugging, and implementation,
	as well as to permit partial renders of the sphere at high LOD,
	such as when on the surface of a planet.
	See README.md for general discussion on design.
	*/

	template<typename id, glm::qualifier Q=glm::defaultp>
	class WholeGridBuffers
	{

		static constexpr id square_count = 10;
		static constexpr id pole_count = 2;

        const SquareBuffers<id,Q> squares;
        const PoleBuffers<id,Q> poles;

	public:
		constexpr inline explicit WholeGridBuffers(const id vertices_per_square_side):
			squares(vertices_per_square_side),
			poles(vertices_per_square_side)
		{}

		template<typename Series>
		constexpr inline id triangles_size(const Series& input) const
		{
			return (
				square_count * squares.triangles_size(input) + 
				pole_count * poles.triangles_size(input)
			);
		}

		template<typename Series, typename Buffer>
		constexpr id storeTriangles(const Series& input, Buffer& output, const id buffer_start_id=0) const 
		{
			id buffer_id = buffer_start_id;
			buffer_id = poles.storeTriangles(0, input, output, buffer_id);
			buffer_id = poles.storeTriangles(1, input, output, buffer_id);
			for (int i=0; i<square_count; i++) { 
				buffer_id = squares.storeTriangles(i, input, output, buffer_id); 
			}
			return buffer_id;
		}

		template<typename Series>
		constexpr inline id triangle_strips_size(const Series& input) const
		{
			return (
				square_count * squares.triangle_strips_size(input) + 
				pole_count * poles.triangle_strips_size(input)
			);
		}

		template<typename Series, typename Buffer>
		constexpr id storeTriangleStrips(const Series& input, Buffer& output, const id buffer_start_id=0) const 
		{
			id buffer_id = buffer_start_id;
			buffer_id = poles.storeTriangleStrips(0, input, output, buffer_id);
			buffer_id = poles.storeTriangleStrips(1, input, output, buffer_id);
			for (int i=0; i<square_count; i++) { 
				buffer_id = squares.storeTriangleStrips(i, input, output, buffer_id); 
			}
			return buffer_id;
		}

	};

}

