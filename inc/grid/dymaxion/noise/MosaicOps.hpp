#pragma once

// C libraries

// std libraries

// 3rd-party libraries
#include <glm/vector_relational.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/noise.hpp>

#include <grid/cartesian/UnboundedIndexing.hpp>

namespace dymaxion
{

	/*
	`MosaicOps` is analogous to `VectorMosaicOps`
	and permits noise such as `MosaicNoise`, `ValueNoise`, and `PerlinNoise`
	to be generated on dymaxion::Grids
	It is analogous to other possible classes that can be 
	used to extend these noise functions to operate on other domains.
	As such, it can be considered an implementation of the adapter pattern for vectors.
	*/
	template<typename id, typename scalar, glm::qualifier precision=glm::defaultp>
	struct MosaicOps
	{
		const Voronoi<id,scalar> voronoi;

        constexpr MosaicOps(const Voronoi<id,scalar>& voronoi) : 
            voronoi(voronoi)
        {
        }

        using vec3 = glm::vec<3,scalar,precision>;
        using vec2 = glm::vec<2,scalar,precision>;
        using ivec2 = glm::vec<2,id,precision>;

        using point = Point<id,scalar,precision>;
        using ipoint = Point<id,id,precision>;

		inline ipoint floor(const point grid_position) const
		{
			return ipoint(grid_position.square_id, ivec2(grid_position.square_position));
		}

		inline vec2 fract(const point grid_position) const
		{
			return glm::fract(grid_position.square_position);
		}

		inline ipoint floor(const ipoint grid_id) const
		{
			return grid_id;
		}

		inline vec2 fract(const ipoint grid_id) const
		{
			return vec2(0);
		}

		inline point add(const ipoint grid_id, vec2 offset) const
		{
			return point(grid_id.square_id, vec2(grid_id.square_position)+offset);
		}

		inline ipoint add(const ipoint grid_id, ivec2 offset) const
		{
			return ipoint(grid_id.square_id, grid_id.square_position+offset);
		}

		inline scalar distance(const point grid_id1, point grid_id2) const
		{
			return voronoi.radius * 
				std::acos(
					math::similarity(
						voronoi.unit_sphere_position(grid_id1), 
						voronoi.unit_sphere_position(grid_id2)
					)
				);
		}

		template<typename tpoint>
		inline scalar area(const tpoint position, const ipoint origin, ivec2 offset) const
		{
			// vec2 F = fract(position);
			// vec2 F01 = glm::smoothstep(vec2(0), vec2(1), F);
			// return math::prod(glm::mix(vec2(1)-F01, F01, vec2(offset)));
			bool is_diagonal = offset.y == offset.x;
			auto fract = glm::fract(position.square_position);
			point W(origin);
			point E(add(origin, ivec2(1,1)));
			point O(add(origin, fract.y > fract.x? ivec2(0,1) : ivec2(1,0)));
			vec3 A = voronoi.unit_sphere_position(!is_diagonal? W : offset.x == 0? E : W);
			vec3 B = voronoi.unit_sphere_position(!is_diagonal? E : O);
			vec3 V = voronoi.unit_sphere_position(position);
			// return glm::distance(A,V);
			return !is_diagonal && ((fract.y >= fract.x) != (offset.y >= offset.x))? 
				scalar(0)
			: 
			    // std::acos(math::similarity(A-V,B-V)) + 
			    // std::acos(math::similarity(V-A,B-A)) + 
			    // std::acos(math::similarity(V-B,A-B)) - 3.1415926535
			  	glm::length(glm::cross(A-V, B-V)) / scalar(2)
			;
		}

	};

	template<typename id, typename scalar, glm::qualifier precision=glm::defaultp>
	constexpr inline auto mosaic_ops(const Voronoi<id,scalar> voronoi)
	{
		return MosaicOps<id,scalar,precision>(voronoi);
	}

}

