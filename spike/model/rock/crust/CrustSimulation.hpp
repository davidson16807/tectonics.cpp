#pragma once

#include <cmath>

#include <glm/vec3.hpp>               // *vec3

namespace rock {

    class CrustSimulation
    {
        const VectorCalculus calculus;
    public:
        CrustSimulation(const VectorCalculus& calculus, const si::length<float> world_radius):
            calculus(calculus)
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

        void plate_mask(
			const CrustSummary& crust,
			std::vector<bool>& out
    	) const {
    		for (int i = 0; i < crust.size(); ++i)
    		{
    			out[i] = crust[i].exists();
    		}
    	}

        void buoyancy(
        	const si::force<float> gravity,
			const std::vector<si::density<float>>& area_density,
			std::vector<si::force<float>>& out
    	) const {
    		for (int i = 0; i < crust.size(); ++i)
    		{
    			out[i] = gravity * crust[i].area_density();
    		}
    	}

		void guess_plate_map(
			const std::vector<glm::vec3>& velocities,
			const std::size_t segment_num, 
			const std::size_t min_segment_size, 
			std::vector<std::uint8_t>& plate_ids
		) const {
		}

		glm::vec3 guess_plate_center_of_mass(
			const std::vector<si::mass<float>>& mass
		) const {
		}

		glm::mat3 guess_plate_rotation_matrix(
			const std::vector<glm::vec3>& plate_velocity,
			glm::vec3 center_of_plate,
			si::time<double> seconds
		) const {
		}

        /*
		NOTE: `guess_plate_velocity` calculates the gradient of plate_mask, which is an out-of-order traversal.
		Since plate_mask has a smaller footprint than a CrustSummary, we operate on plate_mask and not CrustSummary as we usually do.
        */
		void guess_plate_velocity(
			const std::vector<bool>&  plate_mask,
			const std::vector<float>& buoyancy,
			const float mantle_viscosity, 
			std::vector<glm::vec3>& result
		) const {

		    // NOTE: 
		    // Here, we calculate plate velocity as the terminal velocity of a subducting slab as it falls through the mantle.
		    // 
		    // Imagine a cloth with lead weights strapped to one side as it slides down into a vat of honey - it's kind of like that.
		    // 
		    // WARNING:
		    // most of this is wrong! Here, we try calculating terminal velocity for each grid cell in isolation,
		    //  then try to average them to approximate the velocity of the rigid body. 
		    // 
		    // If we wanted to do it the correct way, this is how we'd do it:
		    //  * find drag force, subtract it from slab pull force, and use that to update velocity via netwonian integration
		    //  * repeat simulation for several iterations every time step 
		    // the reason we don't do it the correct way is 1.) it's slow, 2.) it's complicated, and 3.) it's not super important
		    // 
		    // from Schellart 2010
		    // particularly http://rockdef.wustl.edu/seminar/Schellart%20et%20al%20(2010)%20Supp%20Material.pdf
		    // TODO: modify to linearize the width parameter ("W") 
		    // originally: 
		    //         v = S F (WLT)^2/3 /18 cμ
		    // modify to:
		    //         v = S F W(LT)^1/2 /18 cμ
		    // 
		    // TODO: commit Schellart 2010 to the research folder!
		    // TODO: REMOVE HARDCODED CONSTANTS!
		    // TODO: make width dependant on the size of subducting region!
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
		    each::normalize   (result,           result);

		    // buoyancy * lateral_speed_per_unit_force yields lateral velocity yields lateral speed
		    // boundary normal times lateral speed yields lateral velocity
		    // we store all this in result for performance reasons
		    each::mult(result, buoyancy,                                      result); 
		    each::mult(result, series::uniform(lateral_speed_per_unit_force), result); // radians/My

		}

		void rift_ids(
			const std::vector<bool>&  plate_mask,
			std::vector<int>& ids
		) const {
		}

		void subduct_ids(
			const std::vector<bool>&  plate_mask,
			std::vector<int>& ids
		) const {
		}

		void weathering(
			const std::vector<si::pressure<float>>& overburden,
			const std::vector<si::speed<float>>& precipitation,
			const Crust<M>& crust,
			Crust<M>& deltas,
		) const {
		}

		void lithification(
			const std::vector<si::pressure<float>>& overburden,
			const Crust<M>& crust,
			Crust<M>& deltas,
		) const {
		}

		void metamorphosis(
			const std::vector<si::pressure<float>>& overburden,
			const Crust<M>& crust,
			Crust<M>& deltas,
		) const {
		}

		void erosion(
			const std::vector<si::length<float>>& displacements,
			const Formation<M>& formation,
			Formation<M>& deltas,
		) const {
		}

    };

}




