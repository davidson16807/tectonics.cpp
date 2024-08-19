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

	// NOTE: `M` is mineral count
	template<int M, typename VectorCalculus, typename Metric, typename Arithmetic, typename Grid>
    class CrustMotion
    {

    	using vec3 = glm::vec3;

    	using length            = si::length<double>;
    	using acceleration      = si::acceleration<double>;
    	using density           = si::density<double>;
    	using dynamic_viscosity = si::dynamic_viscosity<double>;
    	using torque            = si::torque<double>;

    	using bools = std::vector<bool>;
    	using vec3s = std::vector<vec3>;

    	static constexpr lambda_sample_count = 1000;

        const VectorCalculus calculus;
        const Metric metric;
        const Arithmetic arithmetic;
		const Grid& grid; 
		const length world_radius;
		const acceleration gravity;
		const density mantle_density;
		const dynamic_viscosity mantle_viscosity;

    public:
        CrustMotion(
			const VectorCalculus& calculus, 
	        const Metric metric,
	        const Arithmetic arithmetic,
			const Grid& grid, 
			const length world_radius,
			const acceleration gravity,
			const density mantle_density,
			const dynamic_viscosity mantle_viscosity
		):
            calculus(calculus),
            metric(metric),
            arithmetic(arithmetic),
            grid(grid),
            world_radius(world_radius),
            gravity(gravity),
            mantle_density(mantle_density),
            mantle_viscosity(mantle_viscosity)
        {}

        /*
		TODO: see if it's possible to reuse this code for both spin angular velocity and orbital angular velocity
        */

		inline void buoyancy_force(
			const FormationSummary<M>& summary,
			const bools& exists,
			const force  buoyancy_units,
			vec3s& buoyancy
		) const {
			// calculates Δρgn̂V
		    calculus.gradient  (grid, exists, buoyancy);
		    density density_difference;
		    for (int i = 0; i < buoyancy.size(); ++i)
		    {
		    	density_difference = std::max(0.0f*si::kilogram/si::meter3, summary.density() - mantle_density);
		    	buoyancy[i] = 
		    		glm::normalize(buoyancy[i])                // n̂  boundary normal
		    	  * gravity * density_difference              // Δρ density difference
		    	  * summary.thickness() * grid.vertex_area(i) // V  volume
		    	  / buoyancy_units; 
		    }
		}

		inline glm::dvec3 rigid_body_torque(
			const vec3s& force,
			const force  force_units,
			const torque& torque_units
		) const {
			// calculates Σᴺr⃗×b⃗
			glm::dvec3 result(0);
		    for (int i = 0; i < force.size(); ++i)
		    {
		    	result += glm::cross(grid.vertex_normal(i), force[i]);
		    }
		    return result * (world_radius * force_units / torque_units);
		}

		inline angular_momentum drag_torque_per_angular_velocity(
			const FormationSummary<M>& summary,
			const bools foundering,
			const areas& area
		) const {
			/*
			ς, shape_factor:
			       ⌠∞  2L²+λ            1
			ς = ³/₈⎮  ⎻⎻⎻⎻⎻⎻⎻ ⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻ dλ
			       ⌡0   L²+λ  √((L²+λ)(T²+λ)(W²+λ))
			*/
			length radius_units(world_radius/grid.voronoi.radius);
			int slab_cell_count(arithmetic.sum(foundering));
			volume slab_volume(0.0);
			area   slab_area(0.0);
			for (int i = 0; i < summary.size(); ++i)
			{
				slab_volume += grid.vertex_dual_area(i) * summary[i].thickness() * radius_units * radius_units * radius_units;
				slab_area += grid.vertex_dual_area(i) * radius_units * radius_units;
			}
			length slab_thickness(slab_volume/slab_area);
			// NOTE: the following assumes that subduction zone is always a single cell in width
			length slab_length(si::sqrt(slab_area/slab_cell_count)); 
			length slab_width(slab_area/slab_length);
			length max_dimension(std::max(std::max(length, thickness), width));
			area max_lambda(max_dimension*max_dimension);
			area dlambda(max_lambda/double(lambda_sample_count));
			si::units<-1,0,0,0,0,0,0,double> shape_factor(0.0);
			for (int i = 0; i < lambda_sample_count; ++i)
			{
				lambda = max_lambda * double(i)/double(lambda_sample_count);
				shape_factor += dlambda
					* (2.0 * slab_length*slab_length + lambda)
					/ (      slab_length*slab_length + lambda)
					/ si::sqrt(
						(slab_length   *slab_length    + lambda) * 
						(slab_width    *slab_width     + lambda) * 
						(slab_thickness*slab_thickness + lambda));
			}
			return shape_factor / 
				(24.0 * pi * mantle_viscosity * world_radius * world_radius);
		}

    };

}

