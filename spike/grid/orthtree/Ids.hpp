#pragma once

// C libraries
#include <cmath>

// 3rd party libraries
#define GLM_FORCE_PURE          // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL // disable anonymous structs so we can build with ISO C++
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/common.hpp>       // glm::round
#include <glm/geometric.hpp>    // glm::distance2

/*
The lower left corner of each cell is at a position equal to cell_id
The midpoint of each cell is at a position equal to cell_id
Therefore, the 2ᴺ neighborhood of cells that surround a particle can be found 
by rounding the particle position and negating by members of the cartesian product {0,1}×I,
where I is a set of basis vectors of size `N`. 

This 2ᴺ neighborhood can be broken down into a 4ᴺ neighborhood of higher resolution.
The 4ᴺ neighborhood has two kinds of cells, which are distinguished 
by whether they fall within the analogous 2ᴺ neighborhood for that higher resolution.
Values for cells that fall within the 2ᴺ neighborhood for the higher resolution
have already been stored at that higher resolution, 
so the values for those cells at the lower resolution should not be stored.

Cells within the higher resolution 2ᴺ neighborhood 
can be characterized by whether their midpoint is no more than 
½√N from the midpoint of the rounded particle position,
where N is the number of dimensions.
In other words: `distance2(round(particle), midpoint(cell)) < N/4` is true 
for cells fall within the higher resolution neighborhood
*/

namespace orthtree{

	template <int N, typename id, typename scalar, glm::qualifier Q=glm::highp>
	struct Ids
	{
		using id_type = id;
		using scalar_type = scalar;

		const int dimension_count = N;
		const glm::qualifier qualifier = Q;

		scalar grid_width;
		glm::vec<N,scalar,Q> lowest_cell_boundary;

		Ids(const scalar grid_width,
			const glm::vec<N,scalar,Q> lowest_cell_boundary):
			grid_width(grid_width),
			lowest_cell_boundary(lowest_cell_boundary)
		{}

		constexpr id level_cell_count(const id level) const
		{
			return 1 << (N * level);
		}

		constexpr scalar level_cell_width(const id level) const
		{
			return grid_width / scalar(1 << level);
		}

		constexpr id level_dimension_cell_count(const id level) const
		{
			return 1 << level;
		}

		// NOTE: inclusive
		constexpr id level_start_id(const id level) const
		{
			return level > 1? 1 << (N * level) : 0;
		}

		// NOTE: inclusive
		constexpr id level_end_id(const id level) const
		{
			return 1 << (N * level + 1);
		}

		constexpr id level_from_memory_id(const id memory_id) const
		{
			return (log2(memory_id) - 1) / N;
		}

		constexpr glm::vec<N,scalar,Q> midpoint(const glm::vec<N,id,Q> cell_id, const scalar level_cell_width) const
		{
			return (glm::vec<N,scalar,Q>(cell_id) + 0.5) * level_cell_width + lowest_cell_boundary;
		}

		constexpr glm::vec<N,scalar,Q> level_position(const glm::vec<N,scalar,Q> position, const scalar level_cell_width) const
		{
			return (position - lowest_cell_boundary) / level_cell_width;
		}

		constexpr glm::vec<N,id,Q> cell_id(const glm::vec<N,scalar,Q> position, const scalar level_cell_width) const
		{
			return (position - lowest_cell_boundary) / level_cell_width;
		}

		constexpr glm::vec<N,id,Q> parent_cell_id_from_child_cell_id(const glm::vec<N,id,Q> child_cell_id) const
		{
			return child_cell_id / 2;
		}

		constexpr glm::vec<N,id,Q> lowest_child_id_from_parent_id(const glm::vec<N,id,Q> child_cell_id) const
		{
			return child_cell_id * 2;
		}

		constexpr glm::vec<N,id,Q> lowest_cell_id_of_neighborhood(const glm::vec<N,scalar,Q> level_position) const
		{
			return glm::vec<N,id,Q>(glm::round(level_position) - 1);
		}

		constexpr bool is_in_neighborhood(const glm::vec<N,id,Q> cell_id, const glm::vec<N,scalar,Q> level_position) const
		{
			return glm::distance(glm::vec<N,scalar,Q>(cell_id) + 0.5, glm::round(level_position)) < std::sqrt(N/4);
		}

	};

	template <typename Ids, typename Expression>
	class Storage
	{

		Ids ids;
		Expression expression;

	public:
		using vec = glm::vec<Ids::dimension_count, typename Ids::scalar_type, Ids::qualifier>;
		using id_type = typename Ids::id_type;
		using scalar_type = typename Ids::scalar_type;
		using qualifier = typename Ids::qualifier;

		Storage(const Ids& ids):
			ids(ids)
		{}

		template<typename Raster>
		constexpr void add_multipole(const vec origin, const Raster& raster, Raster& out) const
		{
			auto neighborhood_size = ids.level_cell_count(2);
			if (&raster != &out)
			{
				std::copy(raster.begin(), raster.end(), out.begin());
			}
			auto max_level = ids.level_from_memory_id(raster.size()-1);
			for (int level = 2; level <= max_level; ++level)
			{
				auto level_cell_width = ids.level_cell_width(level);
				auto origin_cell_id = ids.cell_id(origin, level_cell_width);
				auto origin_memory_id = ids.memory_id_from_cell_id(origin_cell_id, level);
				auto origin_level_position = ids.level_position(origin, level_cell_width);
				auto lowest_cell_id_of_neighborhood =  
					ids.lowest_child_id_from_parent_id(
						ids.lowest_cell_id_of_neighborhood(
							ids.level_position(origin, ids.level_cell_width(level+1))));
				for (int i = 0; i < neighborhood_size; ++i)
				{
					auto neighbor_id = lowest_cell_id_of_neighborhood + ids.cell_id_from_level_memory_id(i, 2);
					if ((!ids.is_in_neighborhood(neighbor_id, origin_level_position) || level == max_level) &&
						  ids.memory_id_from_cell_id(neighbor_id, level) != origin_cell_id)
					{
						out[ids.memory_id_from_cell_id(neighbor_id, level)] += expression(ids.midpoint(neighbor_id, level_cell_width) - origin);
					}
				}
			}
		}

		template<typename Raster>
		constexpr auto sample(const vec position, const Raster& raster) const
		{
			typename Raster::value_type out(0);
			auto max_level = ids.level_from_memory_id(raster.size()-1);
			for (int level = 2; level <= max_level; ++level)
			{
				out += raster[ids.memory_id_from_cell_id(
								ids.level_position(position, ids.level_cell_width(level)))];
			}
			return out;
		}

	};

	template <typename id, typename scalar, glm::qualifier Q=glm::highp>
	struct Ids1 : Ids<1,id,scalar,Q>
	{

		Ids1(const scalar grid_width,
			 const scalar lowest_cell_boundary):
			Ids<1,id,scalar,Q>(grid_width, lowest_cell_boundary)
		{}

		constexpr id memory_id_from_cell_id(const id cell_id, const id level) const 
		{
			return this->level_start_id(level) + cell_id;
		}

		constexpr id cell_id_from_memory_id(const id memory_id, const id level) const
		{
			return this->cell_id_from_level_memory_id(memory_id - this->level_start_id(level), level);
		}

		constexpr id cell_id_from_level_memory_id(const id level_memory_id, const id level) const
		{
			return level_memory_id;
		}

	};

	template <typename id, typename scalar, glm::qualifier Q=glm::highp>
	struct Ids2 : Ids<2,id,scalar,Q>
	{

		Ids2(const scalar grid_width,
			 const glm::vec<2,scalar,Q> lowest_cell_boundary):
			Ids<2,id,scalar,Q>(grid_width, lowest_cell_boundary)
		{}

		constexpr id memory_id_from_cell_id(const glm::vec<2,id,Q> cell_id, const id level) const 
		{
			return this->level_start_id(level) + this->level_dimension_cell_count(level) * cell_id.x + cell_id.y;
		}

		constexpr glm::vec<2,id,Q> cell_id_from_level_memory_id(const id level_memory_id, const id level) const
		{
			const id level_dimension_cell_count = this->level_dimension_cell_count(level);
			return glm::vec<2,id,Q>(
				level_memory_id / level_dimension_cell_count, 
				level_memory_id % level_dimension_cell_count);
		}

		constexpr glm::vec<2,id,Q> cell_id_from_memory_id(const id memory_id, const id level) const
		{
			return cell_id_from_level_memory_id(memory_id - this->level_start_id(level), level);
		}

	};

	template <typename id, typename scalar, glm::qualifier Q=glm::highp>
	struct Ids3 : Ids<3,id,scalar,Q>
	{

		Ids3(const scalar grid_width,
			 const glm::vec<3,scalar,Q> lowest_cell_boundary):
			Ids<3,id,scalar,Q>(grid_width, lowest_cell_boundary)
		{}

		constexpr id memory_id_from_cell_id(const glm::vec<3,id,Q> cell_id, const id level) const 
		{
			auto level_dimension_cell_count = this->level_dimension_cell_count(level);
			return level_start_id(level) + 
				cell_id.x * level_dimension_cell_count * level_dimension_cell_count + 
				cell_id.y * level_dimension_cell_count +
				cell_id.z;
		}

		constexpr glm::vec<3,id,Q> cell_id_from_level_memory_id(const id level_memory_id, const id level) const
		{
			const id level_dimension_cell_count = this->level_dimension_cell_count(level);
			return glm::vec<3,id,Q>(
				level_memory_id / level_dimension_cell_count / level_dimension_cell_count, 
				(level_memory_id % level_dimension_cell_count) / level_dimension_cell_count,
				level_memory_id % (level_dimension_cell_count * level_dimension_cell_count));
		}

		constexpr glm::vec<3,id,Q> cell_id_from_memory_id(const id memory_id, const id level) const
		{
			return cell_id_from_level_memory_id(memory_id - level_start_id(level), level);
		}

	};

}

