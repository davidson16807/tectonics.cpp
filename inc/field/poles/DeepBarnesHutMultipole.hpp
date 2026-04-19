#pragma once

#include <vector>
#include <unordered_map>

#include <glm/common.hpp>
#include <glm/geometric.hpp>

#include <grid/cartesian/OrthantIndexing.hpp>

#include "Monopole.hpp"

namespace field
{

	/*
	`DeepBarnesHutMultipole` represents a mathematical field of arbitrary dimensionality
	where the value at each point is the sum of monopole fields.
	This is to say that the value at each point is the weighted sum of the inverse square of distances to particles.
	`DeepBarnesHutMultipole` is an implementation of the Barnes-Hut algorithm.
	`DeepBarnesHutMultipole` can be used to efficiently implement inverse distance weighting, 
	gravity simulations, charged particle simulations, light propagation, boids, etc.
	In inverse distance weighting, the weight associated with a sample is the measurement taken for the sample.
	In gravity simulation, the weight associated with a body is its mass.
	In charged particle simulation, the weight associated with a particle is its charge.
	In light propagation, the weight associated with a light source is its luminosity.
	In boid simulation, multiple `DeepBarnesHutMultipole`s are used, and weights associated with a boid are either uniform or represent heading.

	`DeepBarnesHutMultipole` represents a field using an orthtree that is stored as a std::unordered_map.
	A `std::unordered_map` is able to represent orthtrees of arbitrary depth in a reasonable amount of memory,
	however it is not stored in contiguous memory, and it requires running additional hasing logic,
	which in some cases make `DeepBarnesHutMultipole` slower than `ShallowBarnesHutMultipole`.
	*/
	template<int dimension_count, typename id, typename scalar, glm::qualifier quality = glm::defaultp>
	class DeepBarnesHutMultipole
	{

		using vector = glm::vec<dimension_count, scalar, quality>;
		using ivector = glm::vec<dimension_count, id, quality>;

		struct CellKey {
			ivector cell;
		    std::uint32_t level;

		    CellKey(
		    	const std::uint32_t level,
		    	const ivector cell
		    ):
		    	cell(cell),
		    	level(level)
		    {}

		    bool operator==(const CellKey& other) const noexcept {
		        return level == other.level && cell == other.cell;
		    }

		};

		struct CellKeyHash {

	    	const id level_count;
	    	const id orthant_count;
		    std::vector<std::size_t> first_id_for_level;

		    CellKeyHash(
		    	const id level_count,
		    	const id orthant_count
		    ): 
				level_count(level_count), 
				orthant_count(orthant_count), 
		    	first_id_for_level(level_count)
		    {
				id cell_count(0);
				for (id level = id(0); level < level_count; ++level)
				{
					first_id_for_level[level] = cell_count;
					cell_count += std::pow(orthant_count, level);
				}
		    }

		    std::size_t operator()(const CellKey& key) const noexcept {
			    const std::size_t cells_per_axis = (std::size_t(1) << key.level);
			    std::size_t nesting_id(0);
			    for (std::uint32_t dimension = 0; dimension < dimension_count; ++dimension)
			    {
			    	nesting_id *= cells_per_axis;
			    	nesting_id += key.cell[dimension];
			    }
			    return first_id_for_level[key.level] + nesting_id;
		    }

		};	

		using Map = std::unordered_map<CellKey, Monopole<double, vector>, CellKeyHash>;

		static constexpr scalar half = 0.5;

		const vector grid_center;
		const scalar grid_width;
		const id level_count;
		const id orthant_count;
		const scalar min_cell_width;
		const cartesian::OrthantIndexing<dimension_count, id, quality> orthants;
		std::vector<id> first_id_for_level;
		Map orthtree;

	public:

		DeepBarnesHutMultipole(
			const vector grid_center,
			const scalar grid_width,
			const scalar min_cell_width
		):
			grid_center(grid_center),
			grid_width(grid_width),
			level_count(std::ceil(std::log2(grid_width/min_cell_width))), 
			orthant_count(std::pow(id(2),dimension_count)), 
			min_cell_width(min_cell_width),
			orthants(),
			first_id_for_level(level_count),
			orthtree(0, CellKeyHash(level_count, orthant_count))
		{
			// orthtree.resize(cell_count, Monopole<scalar,vector>()); // if orthtree is std::vector
		}

		void clear()
		{
			orthtree.clear();
			// std::fill(orthtree.begin(), orthtree.end(), Monopole<scalar,vector>()); // if orthtree is std::vector
		}

		/*
		`add` modifies the field to also consider a particle of given `position` and `weight`
		*/
		void add(const vector position, const scalar weight)
		{
			ivector orthant, neighbor;
			id neighbor_id, orthant_id;
			vector cell_center(grid_center);
			scalar cell_width(grid_width);
			ivector nesting(0);
			for (id level = id(1); level < level_count; ++level)
			{
				if (!glm::any(glm::greaterThan(glm::abs(position-grid_center), vector(half*grid_width))))
				{
					orthant = ivector(glm::greaterThan(position-cell_center,vector(0)));
					orthant_id = orthants.memory_id(orthant);
					// add contributions to each orthant of the current level that do not contain the particle
					for (neighbor_id = 0; neighbor_id < orthant_count; ++neighbor_id)
					{
						if (neighbor_id != orthant_id)
						{
							neighbor = orthants.grid_id(neighbor_id);
							orthtree.emplace(
									CellKey(level, id(2) * nesting + neighbor), 
									Monopole<scalar,vector>{}
								).first->second += Monopole<scalar,vector>(position, weight);
						}
					}
					nesting = id(2) * nesting + orthant;
					cell_center += cell_width * (vector(orthant)-half) * half;
					cell_width *= half;
				}
			}
		}

		/*
		`sample` returns the value acting at a given `position` in the field
		*/
		[[nodiscard]] inline vector operator()(const vector& position ) const
		{
			vector cell_center(grid_center);
			scalar cell_width(grid_width);
			vector value(0);
			ivector nesting(0);
			ivector orthant;
			for (id level = id(1); level < level_count; ++level)
			{
				orthant = ivector(glm::greaterThan(position-cell_center,vector(0)));
				nesting = id(2) * nesting + orthant;
				auto it = orthtree.find(CellKey(level, nesting));
				if (it != orthtree.end()) {
					const Monopole<scalar,vector>& monopole = it->second;
					vector offset = monopole.offset_for_position(position);
                	scalar distance2 = glm::dot(offset, offset);
                	if (distance2 > min_cell_width*min_cell_width) {
					    value += monopole.value_for_offset(offset);
                	}
				}
				cell_center += cell_width * (vector(orthant)-half) * half;
				cell_width *= half;
			}
			return value;
		}

	};

}
