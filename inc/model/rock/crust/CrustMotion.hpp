#pragma once

// C libraries
#include <cmath>
#include <cstddef>

// std libraries
#include <vector>

// 3rd-party libraries
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

// in-house libraries
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
	template<std::size_t M, typename stored, typename handled, typename VectorCalculus, glm::qualifier quality=glm::defaultp>
    class CrustMotion
    {

    	using vec3 = glm::vec<3,stored,quality>;

    	using length            = si::length<handled>;
    	using volume            = si::volume<handled>;
    	using area              = si::area<handled>;
    	using acceleration      = si::acceleration<handled>;
    	using density           = si::density<handled>;
    	using dynamic_viscosity = si::dynamic_viscosity<handled>;
    	using force             = si::force<handled>;
    	using pressure          = si::pressure<handled>;
    	using torque            = si::torque<handled>;
    	using angular_momentum  = si::angular_momentum<handled>;

    	using bools = std::vector<bool>;
    	using vec3s = std::vector<glm::vec<3,stored,quality>>;

    	using forces = std::vector<si::force<stored>>;
    	using pressures = std::vector<si::pressure<stored>>;

    	static constexpr int lambda_sample_count = 1000;
    	static constexpr handled pi = 3.1415926535897932384626433832795028841971;

        const VectorCalculus calculus;
		const length world_radius;
		const acceleration gravity;
		const density mantle_density;
		const dynamic_viscosity mantle_viscosity;
		const length radius_units;

    public:
        CrustMotion(
			const VectorCalculus& calculus, 
			const length world_radius,
			const acceleration gravity,
			const density mantle_density,
			const dynamic_viscosity mantle_viscosity,
			const length radius_units
		):
            calculus(calculus),
            world_radius(world_radius),
            gravity(gravity),
            mantle_density(mantle_density),
            mantle_viscosity(mantle_viscosity),
            radius_units(radius_units)
        {}


		void buoyancy(
			const FormationSummary& summary,
			pressures& buoyancy_pressure
		) const {
		    for (std::size_t i = 0; i < summary.size(); ++i)
		    {
	            auto density_difference = si::max(
	                0.0*si::kilogram/si::meter3, 
	                density(summary[i].density() - mantle_density)
	            );
	            buoyancy_pressure[i] = pressure(
	                gravity * density_difference   // Δρ density difference
	              * length(summary[i].thickness()) // V  volume
	            );
		    }
		}

		/* 
		`slab_pull` calculates the "slab pull" force, b=Δρgn̂V, for each cell in a `summary`.
		Slab pull is treated as equal and perpendicular to the buoyancy force.
		See CrustMotion.txt for more details.

		TODO: See if it's possible to reuse this code for both 
		spin angular velocity and orbital angular velocity.
		*/
		template<typename Grid>
		void slab_pull(
			const Grid& grid,
			const pressures& buoyancy_pressure,
			const bools& exists,
			const force  slab_pull_units,
			vec3s& slab_pull
		) const {
		    calculus.gradient(grid, exists, slab_pull);
		    for (std::size_t i = 0; i < slab_pull.size(); ++i)
		    {
		    	slab_pull[i] =
		    		!exists[i]? vec3(0) :
		    		glm::length(slab_pull[i]) == 0.0? vec3(0) :
			    		stored(force(buoyancy_pressure[i] * grid.vertex_dual_area(i) * si::area<stored>(radius_units * radius_units))
			    			/slab_pull_units) * glm::normalize(-slab_pull[i]); // n̂  boundary normal
		    }
		}

		void is_slab(
			const vec3s& slab_pull,
			bools& is_slab
		) const {
		    for (std::size_t i = 0; i < slab_pull.size(); ++i)
		    {
		    	is_slab[i] = glm::length(slab_pull[i]) > 0.0f;
		    }
		}

		template<typename Grid>
		volume slab_volume(
			const Grid& grid,
			const FormationSummary& summary,
			const bools& is_slab
		) const {
			volume slab_volume(0.0);
			for (std::size_t i = 0; i < summary.size(); ++i)
			{
				if(is_slab[i])
				{
					slab_volume += length(summary[i].thickness()) *
						grid.vertex_dual_area(i) * radius_units * radius_units; 
				}
			}
			return slab_volume;
		}

		template<typename Grid>
		area slab_area(
			const Grid& grid,
			const FormationSummary& summary,
			const bools& is_slab
		) const {
			area slab_area(0.0);
			for (std::size_t i = 0; i < summary.size(); ++i)
			{
				if(is_slab[i])
				{
					slab_area += grid.vertex_dual_area(i) * radius_units * radius_units;
				}
			}
			return slab_area;
		}

		int slab_cell_count(
			const bools& is_slab
		) const {
		    int result(0);
		    for (std::size_t i = 0; i < is_slab.size(); ++i)
		    {
		    	result += is_slab[i];
		    }
		    return result;
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

		/* 
		`rigid_body_torque` calculates torque, τ=Σᴺr⃗×f⃗, acting on a crust 
		assuming that crust is a rigid body submitted to `forces`.
		See CrustMotion.txt for more details.
		*/
		template<typename Grid>
		glm::dvec3 rigid_body_torque(
			const Grid& grid,
			const vec3s& forces,
			const force  force_units,
			const torque torque_units
		) const {
			glm::dvec3 result(0);
		    for (std::size_t i = 0; i < forces.size(); ++i)
		    {
		    	result += glm::cross(grid.vertex_normal(i), forces[i]);
		    }
		    return result * (world_radius * force_units / torque_units);
		}

		angular_momentum drag_per_angular_velocity(
			const length slab_length,
			const length slab_thickness,
			const length slab_width
		) const {
			/*
			ς, shape_factor:
			       ⌠∞  2L²+λ            1
			ς = ³/₈⎮  ⎻⎻⎻⎻⎻⎻⎻ ⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻ dλ
			       ⌡0   L²+λ  √((L²+λ)(T²+λ)(W²+λ))
			*/
			length max_dimension(si::max(10*si::kilometer, si::max(si::max(slab_length, slab_thickness), slab_width)));
			area max_lambda(max_dimension*max_dimension); // clamp to a big number
			area dlambda(max_lambda/handled(lambda_sample_count));
			area lambda(0.0);
			si::spatial_frequency<handled> shape_factor;
			for (std::size_t i = 0; i < lambda_sample_count; ++i)
			{
				lambda = max_lambda * handled(i)/handled(lambda_sample_count);
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
    };

    template<
    	int M, typename stored, typename handled, 
    	typename VectorCalculus>
    CrustMotion<M, stored, handled, VectorCalculus> crust_motion(
		const VectorCalculus& calculus, 
		const si::length<handled> world_radius,
		const si::acceleration<handled> gravity,
		const si::density<handled> mantle_density,
		const si::dynamic_viscosity<handled> mantle_viscosity,
		const si::length<handled> radius_units
    ) {
    	return CrustMotion<M, stored, handled, VectorCalculus>(
    		calculus, world_radius, gravity, mantle_density, mantle_viscosity, radius_units
		);
    }

}

