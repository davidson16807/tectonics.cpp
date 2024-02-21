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

	};

	template<typename id, typename scalar, glm::qualifier precision=glm::defaultp>
	constexpr inline auto mosaic_ops(const Voronoi<id,scalar> voronoi)
	{
		return MosaicOps<id,scalar,precision>(voronoi);
	}

}

