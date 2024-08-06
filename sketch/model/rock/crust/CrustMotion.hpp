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

    	using bools     = std::vector<bool>;
    	using vec3s     = std::vector<glm::vec3>;

    	using masses    = std::vector<mass>;
    	using forces    = std::vector<force>;

        const VectorCalculus calculus;
        const Segmentation segmentation;
        const length world_radius;

		inline glm::vec3 rotation_matrix(
			const vec3s& linear_velocity,
			const auto& position,
			const glm::vec3 axis,
			const si::time<double> seconds,
			vec3s& scratch1,
			floats& scratch2,
		) const {
			for (int i = 0; i < linear_velocity.size(); ++i)
			{
				vec3 offset = position[i] - axis[i];
				rotation_vector += glm::cross(linear_velocity[i], offset) / glm::dot(offset, offset);
			}
			rotation_vector /= linear_velocity.size();
			rotation_vector *= seconds;
    		// TODO: negation shouldn't theoretically be needed! find out where the discrepancy lies and fix it the proper way! 
			glm::mat4 rotation_matrix = glm::rotate(glm::length(rotation_vector), -glm::normalize(rotation_vector));
			rotation_matrix = std::is_nan(rotation_matrix)? glm::mat4(1.0f) : rotation_matrix;
			return rotation_matrix;
		}

    public:
        CrustMotion(
			const VectorCalculus& calculus, 
			const Metric& metric, 
			const Arithmetic& arithmetic, 
			const Segmentation& segmentation,
			const length world_radius
		):
            calculus(calculus),
            metric(metric),
            arithmetic(arithmetic),
            world_radius(world_radius)
        {}

        /*
		NOTE: We are guaranteed to generate CrustSummary at least once every iteration,
		This is done in order to performantly calculate plate gaps and collisions.
		Unless we need information that is only contained in StratumStore, it is always more performant to operate on CrustSummary,
		since StratumStore has a bigger footprint in memory, and operating on StratumStore requires iterating through MineralStores.
		To simplify design, encourage performant usage, and encourage cache reuse in the CPU,
		all operations will be implemented exclusively on CrustSummary, unless required for out-of-order traversal or otherwise impossible.
		Exceptions will be noted in the comments. 
        */

		inline glm::vec3 plate_center_of_mass(
			const vec3s& position,
			const masses& mass
		) const {
			return whole::weighted_average(position, mass);
		}

		/*
		NOTE:
		invocation looks like this:
		glm::vec3 center_of_world(0,0,0);
		plate_rotation_matrix(center_of_plate_angular_velocity, center_of_x);
		*/

		glm::mat4 plate_rotation_matrix(
			const vec3s& plate_velocity, 
			const vec3&  postiion,
			const vec3& center_of_plate,
			const vec3& center_of_world,
			const si::time<double> seconds
		) const {
			/*
		    We assume that plates move as rigid bodies. 
		    When not constained to a sphere, rigid bodies are subjected to two kinds of motion:
		       * angular motion, which rotates the body around its center of mass
		       * linear motion, which translates the body
			Both can be represented as transformations of a local coordinate system that's defined around the body's center of mass.
		    Angular acceleration can be represented as a rotation of this local coordinate system using an inertial matrix.
		    Linear acceleration can be represented as a translation of this local coordinate system.

		    In our case, the motion of our rigid bodies are constrained to a sphere, so we must refine the above statement.

		    On a sphere, angular motion can still be represented as rotation around a center of mass, 
		    however motion is confined to a 2d manifold, so for sufficiently small plates all rotation is 2d 
		    This rotation must then occur around an axis that runs through the center of mass in the same direction as the surface normal.
		    For the sake of simplicity we will assume all plates are of sufficient size for this to apply.

		    When constrained to a sphere, linear motion must also occur as a rotation.
			This rotation rotates the local coordinate system around the euler pole for that plate.

			Therefore, in our simplified model all motion is described using two rotations:
			one that occurs around an euler pole, and another that occurs around the plate's center of mass.
		    */
		    return
				rotation_matrix(plate_velocity, position, center_of_plate, seconds)  // linear motion
			  * rotation_matrix(plate_velocity, position, center_of_world, seconds); // angular motion
		}

        /*
		NOTE: `guess_plate_velocity` calculates the gradient of `exists`, which is an out-of-order traversal.
		Since `exists` has a smaller footprint than a CrustSummary, we operate on `exists` and not CrustSummary as we usually do.
        */
		void guess_plate_velocity(
			const bools&  exists,
			const forces& buoyancy,
			const density mantle_viscosity, 
			vec3s& result
		) const {

			/*
		    NOTE: 
		    Here, we calculate plate velocity as the terminal velocity of a subducting slab as it falls through the mantle.
		    
		    Imagine a cloth with lead weights strapped to one side as it slides down into a vat of honey - it's kind of like that.
		    
		    WARNING:
		    most of this is wrong! Here, we try calculating terminal velocity for each grid cell in isolation,
		     then try to average them to approximate the velocity of the rigid body. 
		    
		    If we wanted to do it the correct way, this is how we'd do it:
		     * find drag force, subtract it from slab pull force, and use that to update velocity
		     * repeat simulation for several iterations every time step 
		    the reason we don't do it the correct way is 1.) it's slow, 2.) it's complicated, and 3.) it's not super important

		    buoyancy
		    
		    from Schellart 2010
		    particularly http://rockdef.wustl.edu/seminar/Schellart%20et%20al%20(2010)%20Supp%20Material.pdf
		    TODO: modify to linearize the width parameter ("W") 
		    originally: 
		            v = S F (WLT)^2/3 /18 cμ
		    modify to:
		            v = S F W(LT)^1/2 /18 cμ
		    
		    TODO: commit Schellart 2010 to the research folder!
		    TODO: REMOVE HARDCODED CONSTANTS!
		    TODO: make width dependant on the size of subducting region!
			*/
		    float width = 300e3f; // meters 
		    float length = 600e3f; // meters
		    float thickness = 100e3f; // meters
		    float effective_area = pow(thickness * length * width, 2.f/3.f); // m^2
		    float shape_parameter = 0.725f; // unitless
		    float slab_dip_angle_constant = 4.025f; // unitless

		    float lateral_speed_per_unit_force  = 1.f;
		    lateral_speed_per_unit_force *= effective_area;					// start with m^2
		    lateral_speed_per_unit_force /= mantle_viscosity;				// convert to m/s per Newton
		    lateral_speed_per_unit_force /= 18.f;							// apply various unitless constants
		    lateral_speed_per_unit_force *= shape_parameter;                     
		    lateral_speed_per_unit_force /= slab_dip_angle_constant;             
		    // lateral_speed_per_unit_force *= Units.MEGAYEAR;				// convert to m/My per Newton
		    lateral_speed_per_unit_force /= world_radius; 					// convert to radians/My per Newton

		    // find boundary normal, store it in result
		    // NOTE: result does double duty for performance reasons
		    calculus.gradient (grid, plate_mask, result);
		    metric.normalize   (result,           result);

		    // buoyancy * lateral_speed_per_unit_force yields lateral velocity yields lateral speed
		    // boundary normal times lateral speed yields lateral velocity
		    // we store all this in result for performance reasons
		    arithmetic.mult(result, buoyancy,                                      result); 
		    arithmetic.mult(result, series::uniform(lateral_speed_per_unit_force), result); // radians/My

		}

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

