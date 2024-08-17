#pragma once

#include <cmath>

#include <vector>

#include <glm/vec3.hpp>               // *vec3

#include <unit/si.hpp>

#include <model/rock/formation/Formation.hpp>
#include <model/compound/phase/boundary/LinearPhaseBoundary.hpp>

#include "Crust.hpp"

/*
needs:
FormationSummarization
FormationSummaryOps
*/

namespace rock {

	/*

	`CrustMotion` (together with `FormationSimulation`)
	is basically *the most important part of the geological model*.
	They contain everything that's needed to simulate the geology of a planet.

	This may include the following:
	* plate motion   the motion of plates
	* plate segmentation the division of plates

	*/

	// NOTE: `M` is mineral count, `F` is formation count
	template<int M, int F, typename VectorCalculus, typename Morphology, typename Segmentation, typename Metric, typename Arithmetic>
    class CrustMotion
    {
    	using mass      = si::mass<float>;
    	using length    = si::length<float>;
    	using density   = si::density<float>;
    	using force     = si::force<float>;
        using torque = glm::vec<3,si::torque<double>,glm::defaultp>;
        using angular_velocity = glm::vec<3,si::frequency<double>,glm::defaultp>;

    	using bools     = std::vector<bool>;

        const VectorCalculus calculus;
		const Grid& grid; 
		const length world_radius;
		const acceleration gravity;
		const dynamic_viscosity mantle_viscosity;

    public:
        CrustMotion(
			const VectorCalculus& calculus, 
			const Grid& grid, 
			const length world_radius,
			const acceleration gravity,
			const dynamic_viscosity mantle_viscosity
		):
            calculus(calculus),
            grid(grid),
            world_radius(world_radius),
            gravity(gravity),
            mantle_viscosity(mantle_viscosity)
        {}

		void guess_plate_map(
			const vec3s& velocities,
			const std::size_t segment_num, 
			const std::size_t min_segment_size, 
			std::vector<std::uint8_t>& plate_ids
		) const {
			segmentation.segment(
			  grid, vertex_gradient, 7, 10, 
			  vertex_colored_scalars, scratch, mask1, mask2, mask3
			);
		}


    };

}

