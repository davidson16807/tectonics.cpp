#pragma once

#include <vector>

#include <glm/common.hpp>
#include <glm/geometric.hpp>

#include <grid/cartesian/OrthantIndexing.hpp>

#include "Monopole.hpp"

namespace field
{

	/*
	`BarnesHutMultipole` represents a mathematical field of arbitrary dimensionality
	where the value at each point is the sum of monopole fields.
	This is to say that the value at each point is the weighted sum of the inverse square of distances to particles.
	`BarnesHutMultipole` is an implementation of the Barnes-Hut algorithm.
	`BarnesHutMultipole` can be used to efficiently implement inverse distance weighting, 
	gravity simulations, charged particle simulations, light propagation, boids, etc.
	In inverse distance weighting, the weight associated with a sample is the measurement taken for the sample.
	In gravity simulation, the weight associated with a body is its mass.
	In charged particle simulation, the weight associated with a particle is its charge.
	In light propagation, the weight associated with a light source is its luminosity.
	In boid simulation, multiple `BarnesHutMultipole`s are used, and weights associated with a boid are either uniform or represent heading.
	*/
	template<int dimension_count, typename id, typename scalar, glm::qualifier quality = glm::defaultp>
	class BarnesHutMultipole
	{

		using vector = glm::vec<dimension_count, scalar, quality>;
		using ivector = glm::vec<dimension_count, id, quality>;

		static constexpr scalar half = 0.5;

		const cartesian::OrthantIndexing<dimension_count, id, quality> orthants;
		const vector grid_center;
		const scalar grid_width;
		const id level_count;
		const id orthant_count;
		id cell_count;
		std::vector<id> first_id_for_level;
		std::vector<Monopole<scalar,vector>> orthtree;

	public:

		BarnesHutMultipole(
			const vector grid_center,
			const scalar grid_width,
			const scalar min_cell_width
		):
			orthants(),
			grid_center(grid_center),
			grid_width(grid_width),
			level_count(std::ceil(std::log2(grid_width/min_cell_width))), 
			orthant_count(std::pow(id(2),dimension_count)), 
			first_id_for_level(level_count)
		{
			cell_count = 0;
			for (id level = id(0); level < level_count; ++level)
			{
				first_id_for_level[level] = cell_count;
				cell_count += std::pow(orthant_count, level);
			}
			orthtree.resize(cell_count, Monopole<scalar,vector>());
		}

		void clear()
		{
			std::fill(orthtree.begin(), orthtree.end(), Monopole<scalar,vector>());
		}

		/*
		`add` modifies the field to also consider a particle of given `position` and `weight`
		*/
		void add(const vector position, const scalar weight)
		{
			id nesting_id(0);
			id neighbor_id, orthant_id;
			vector cell_center(grid_center);
			scalar cell_width(grid_width);
			ivector orthant;
			for (id level = id(1); level < level_count; ++level)
			{
				orthant = ivector(glm::greaterThan(position-cell_center,vector(0)));
				orthant_id = orthants.memory_id(orthant);
				// add contributions to each orthant of the current level that do not contain the particle
				for (neighbor_id = 0; neighbor_id < orthant_count; ++neighbor_id)
				{
					if (neighbor_id != orthant_id)
					{
						orthtree[first_id_for_level[level] + orthant_count * nesting_id + neighbor_id] 
							+= Monopole<scalar,vector>(position, weight);
					}
				}
				nesting_id = orthant_count * nesting_id + orthant_id;
				cell_center += cell_width * (vector(orthant)-half);
				cell_width *= half;
			}
		}

		/*
		`sample` returns the value acting at a given `position` in the field
		*/
	    [[nodiscard]]
		constexpr inline scalar operator()(const vector& position ) const
		{
			id nesting_id(0);
			id orthant_id;
			Monopole<scalar,vector> monopole;
			vector cell_center(grid_center);
			scalar cell_width(grid_width);
			scalar value(0);
			ivector orthant;
			for (id level = id(1); level < level_count; ++level)
			{
				orthant = ivector(glm::greaterThan(position-cell_center,vector(0)));
				orthant_id = orthants.memory_id(orthant);
				value += orthtree[first_id_for_level[level] + nesting_id](position);
				nesting_id = orthant_count * nesting_id + orthant_id;
				cell_center += cell_width * (vector(orthant)-half);
				cell_width *= half;
			}
			return value;
		}

	};

}
