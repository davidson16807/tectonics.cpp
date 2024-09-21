#pragma once

#include <cmath>

#include <vector>

#include <unit/si.hpp>

#include <model/rock/formation/FormationSummary.hpp>

/*
needs:
FormationSummarization
FormationSummaryOps
*/

namespace rock {

	/*
	`CrustMotion` (together with `FormationSimulation`)
	contains everything needed to simulate plate motion
	starting from the `*Summaries` of model state.

	This includes the following:
	* plate motion   the motion of plates
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
    	using angular_momentum  = si::angular_momentum<double>;

    	using bools = std::vector<bool>;
    	using vec3s = std::vector<vec3>;

    	static constexpr int lambda_sample_count = 1000;
    	static constexpr double pi = 3.1415926535897932384626433832795028841971;

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
		`buoyancy_forces` calculates the buoyancy force, 
		b=Δρgn̂V, for each cell in a `summary`.
		See CrustMotion.txt for more details.

		TODO: See if it's possible to reuse this code for both 
		spin angular velocity and orbital angular velocity.
		*/
		void buoyancy_forces(
			const FormationSummary& summary,
			const bools& exists,
			const force  buoyancy_units,
			vec3s& buoyancies
		) const {
		    calculus.gradient(grid, exists, buoyancies);
		    density density_difference;
		    for (int i = 0; i < buoyancies.size(); ++i)
		    {
		    	density_difference = si::max(
		    		0.0*si::kilogram/si::meter3, 
		    		density(summary[i].density() - mantle_density)
	    		);
		    	float magnitude = 
		    		gravity * density_difference                 // Δρ density difference
			      * length(summary[i].thickness()) * grid.vertex_area(i) // V  volume
			      / buoyancy_units;
		    	buoyancies[i] = 
			    	!exists[i]? vec3(0) : 
			    		magnitude * glm::normalize(-buoyancies[i]); // n̂  boundary normal
		    }
		}

		/* 
		`rigid_body_torque` calculates torque, τ=Σᴺr⃗×f⃗, acting on a crust 
		assuming that crust is a rigid body submitted to `forces`.
		See CrustMotion.txt for more details.
		*/
		glm::dvec3 rigid_body_torque(
			const vec3s& forces,
			const force  force_units,
			const torque torque_units
		) const {
			glm::dvec3 result(0);
		    for (int i = 0; i < forces.size(); ++i)
		    {
		    	result += glm::cross(grid.vertex_normal(i), forces[i]);
		    }
		    return result * (world_radius * force_units / torque_units);
		}

		void is_slab(
			const vec3s& buoyancies,
			const bools& is_slab
		) const {
		    for (int i = 0; i < buoyancies.size(); ++i)
		    {
		    	is_slab[i] = buoyancies[i].length() > 0.0f;
		    }
		}

		int slab_cell_count(
			const bools& is_slab
		) const {
		    int slab_cell_count;
		    for (int i = 0; i < is_slab.size(); ++i)
		    {
		    	slab_cell_count += is_slab[i];
		    }
		    return slab_cell_count;
		}

		volume slab_volume(
			const FormationSummary& summary,
			const bools& is_slab
		) const {
			volume slab_volume(0.0);
			for (int i = 0; i < summary.size(); ++i)
			{
				if(is_slab[i])
				{
					slab_volume += grid.vertex_dual_area(i) * summary[i].thickness() 
						* radius_units * radius_units * radius_units;
				}
			}
			return slab_volume;
		}

		area slab_area(
			const FormationSummary& summary,
			const bools& is_slab
		) const {
			area slab_area(0.0);
			for (int i = 0; i < summary.size(); ++i)
			{
				if(is_slab[i])
				{
					slab_area += grid.vertex_dual_area(i) 
						* radius_units * radius_units;
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

		angular_momentum drag_per_angular_velocity(
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
			si::spatial_frequency<double> shape_factor;
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
			return std::isnan(shape_factor*si::meter)? angular_momentum(0.0) : 
				(24.0 * pi * mantle_viscosity * world_radius * world_radius) / shape_factor;
		}

		/*
		TESTS:
		* `drag_per_angular_velocity` is scale invariant
		* `drag_per_angular_velocity` is commutative wrt thickness and width
		* `drag_per_angular_velocity` has diminishing returns wrt thickness, length, and width
		* `drag_per_angular_velocity` reproduces results from Schellart 2010 when combined with appropriate torque
		* `rigid_body_torque` is linear with respect to force magnitudes
		* `buoyancy_forces ⋅ surface normal == 0` 
		* `slab_thickness * slab_width * slab_length` must reproduce `slab_volume`
		* `slab_thickness * slab_area` must reproduce `slab_volume`
		* `slab_width * slab_length` must reproduce `slab_area`
		* `slab_width ≥ slab_length`
		* domains:
			* `drag_per_angular_velocity > 0`
			* `slab_volume > 0`
			* `slab_cell_count > 0`
			* `slab_area > 0`
			* `slab_thickness > 0`
			* `slab_length > 0`
			* `slab_width > 0`
		* monotonic: 
			* `drag_per_angular_velocity` increases wrt length, width, and thickness
			* `slab_thickness` increases wrt volume
			* `slab_length` increases wrt area
			* `slab_width` increases wrt area
		* rotationally invariant:
			* `is_slab`
			* `slab_cell_count`
			* `slab_volume`
			* `slab_area`
			* `rigid_body_torque`
			* `buoyancy_forces`

		* `buoyancy_forces` in combination with `drag_per_angular_velocity` and `CrustFracture`
			must produce velocities on the same order as velocities seen on earth
			when given earthlike `FormationSummary`s
		* nontrivial output in combination with `CrustFracture` when given earthlike `FormationSummary`s:
			* 0 ≤ slab_cell_count < N
			* 0 ≤ slab_volume < total_volume
			* 0 ≤ slab_area < total_area
		*/

    };

    template<int M, typename VectorCalculus, typename Grid>
    CrustMotion<M, VectorCalculus, Grid> crust_motion(
		const VectorCalculus& calculus, 
		const Grid& grid, 
		const si::length<double> world_radius,
		const si::acceleration<double> gravity,
		const si::density<double> mantle_density,
		const si::dynamic_viscosity<double> mantle_viscosity
    ) {
    	return CrustMotion<M, VectorCalculus, Grid>(
    		calculus, grid, world_radius, 
    		gravity, mantle_density, mantle_viscosity
		);
    }

}

