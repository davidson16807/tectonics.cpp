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
	template<int M, typename VectorCalculus, typename Grid>
    class CrustMotion
    {

    	using vec3 = glm::vec3;

    	using length            = si::length<double>;
    	using volume            = si::volume<double>;
    	using area              = si::area<double>;
    	using acceleration      = si::acceleration<double>;
    	using density           = si::density<double>;
    	using dynamic_viscosity = si::dynamic_viscosity<double>;
    	using force             = si::force<double>;
    	using torque            = si::torque<double>;

    	using bools = std::vector<bool>;
    	using vec3s = std::vector<vec3>;

    	static constexpr lambda_sample_count = 1000;

        const VectorCalculus calculus;
		const Grid& grid; 
		const length world_radius;
		const acceleration gravity;
		const density mantle_density;
		const dynamic_viscosity mantle_viscosity;
		const length radius_units;

    public:
        CrustMotion(
			const VectorCalculus& calculus, 
			const Grid& grid, 
			const length world_radius,
			const acceleration gravity,
			const density mantle_density,
			const dynamic_viscosity mantle_viscosity
		):
            calculus(calculus),
            grid(grid),
            world_radius(world_radius),
            gravity(gravity),
            mantle_density(mantle_density),
            mantle_viscosity(mantle_viscosity),
            radius_units(world_radius/grid.voronoi.radius)
        {}

        /*
        */

		/* 
		`buoyancy_forces` calculates the buoyancy force, 
		b=Δρgn̂V, for each cell in a `summary`.
		See CrustMotion.txt for more details.

		TODO: See if it's possible to reuse this code for both 
		spin angular velocity and orbital angular velocity.
		*/
		inline void buoyancy_forces(
			const FormationSummary<M>& summary,
			const bools& exists,
			const force  buoyancy_units,
			vec3s& buoyancy
		) const {
		    calculus.gradient(grid, exists, buoyancy);
		    density density_difference;
		    for (int i = 0; i < buoyancy.size(); ++i)
		    {
		    	density_difference = std::max(0.0f*si::kilogram/si::meter3, summary.density() - mantle_density);
		    	buoyancy[i] = 
			    	exists[i]? vec3(0)
			    	  : glm::normalize(-buoyancy[i])              // n̂  boundary normal
			    	  * gravity * density_difference              // Δρ density difference
			    	  * summary.thickness() * grid.vertex_area(i) // V  volume
			    	  / buoyancy_units; 
		    }
		}

		/* 
		`rigid_body_torque` calculates torque, τ=Σᴺr⃗×f⃗, acting on a crust 
		assuming that crust is a rigid body submitted to `forces`.
		See CrustMotion.txt for more details.
		*/
		inline glm::dvec3 rigid_body_torque(
			const vec3s& forces,
			const force  force_units,
			const torque& torque_units
		) const {
			glm::dvec3 result(0);
		    for (int i = 0; i < forces.size(); ++i)
		    {
		    	result += glm::cross(grid.vertex_normal(i), forces[i]);
		    }
		    return result * (world_radius * force_units / torque_units);
		}

		inline int slab_cell_count(
			const vec3s& buoyancy,
			const force  buoyancy_units,
			const force  threshold
		) const {
		    int slab_cell_count;
		    for (int i = 0; i < summary.size(); ++i)
		    {
		    	slab_cell_count += buoyancy[i].length() > threshold/buoyancy_units;
		    }
		}

		inline volume slab_volume(
			const FormationSummary<M>& summary
		) const {
			volume slab_volume(0.0);
			for (int i = 0; i < summary.size(); ++i)
			{
				if(foundering[i])
				{
					slab_volume += grid.vertex_dual_area(i) * summary[i].thickness() * radius_units * radius_units * radius_units;
				}
			}
			return slab_volume;
		}

		inline area slab_area(
			const FormationSummary<M>& summary
		) const {
			area slab_area(0.0);
			for (int i = 0; i < summary.size(); ++i)
			{
				if(foundering[i])
				{
					slab_area += grid.vertex_dual_area(i) * radius_units * radius_units;
				}
			}
			return slab_area;
		}

		inline length slab_thickness(
			const volume slab_volume,
			const area slab_area
		) const {
			return slab_volume/slab_area;
		}

		inline length slab_length(
			const area slab_area,
			const int slab_cell_count
		) const {
			return si::sqrt(slab_area/slab_cell_count);
		}

		inline length slab_width(
			const area slab_area,
			const length slab_length
		) const {
			return slab_area/slab_length;
		}

		inline angular_momentum drag_torque_per_angular_velocity(
			const length slab_thickness,
			const length slab_length,
			const length slab_width
		) const {
			/*
			ς, shape_factor:
			       ⌠∞  2L²+λ            1
			ς = ³/₈⎮  ⎻⎻⎻⎻⎻⎻⎻ ⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻ dλ
			       ⌡0   L²+λ  √((L²+λ)(T²+λ)(W²+λ))
			*/
			length max_dimension(std::max(std::max(slab_length, slab_thickness), slab_width));
			area max_lambda(max_dimension*max_dimension);
			area dlambda(max_lambda/double(lambda_sample_count));
			area lambda(0.0);
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

