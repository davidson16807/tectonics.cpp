#pragma once

#include <cmath>

#include <glm/vec3.hpp>               // *vec3

namespace rock {

	/*

	`CrustSimulation` is basically the most important part of the geological model.
	It contains everything that's needed to simulate the geology of a planet.

	This includes the following:
	* melting        the conversion of solids to liquids
	* sublimation    the conversion of solids to gases
	* metamorphosis  the conversion of solids from one crystal structure to another
	* lithification  the conversion of sediment to sedimentary rock
	* weathering     the conversion of rock to sediment
	* erosion        the horizontal transport of sediment
	* rifting        the creation of new rock columns when there are gaps in the plates
	* detaching      the destruction of rock columns when there are overlaps in the plates
	* plate motion   the motion of plates
	* plate segmentation the division of plates
	* hotspots       the addition of mass at a function of distance from a prescribed geocoordinate
	* astroblemes    the removal of mass as a function of distance from a prescribed geocoordinate

	*/

	template<int M, typename VectorCalculus, typename Morphology>
    class CrustSimulation
    {
    	using mass      = si::mass<float>;
    	using length    = si::length<float>;
    	using density   = si::density<float>;
    	using speed     = si::speed<float>;
    	using force     = si::force<float>;
    	using pressure  = si::pressure<float>;
    	using acceleration = si::acceleration<float>;
    	using area_density = si::area_density<float>;

    	using bools     = std::vector<bool>;
    	using vec3s     = std::vector<glm::vec3>;
    	using uint8s    = std::vector<std::uint8_t>;

    	using masses    = std::vector<mass>;
    	using lengths   = std::vector<length>;
    	using densities = std::vector<density>;
    	using speeds    = std::vector<speeds>;
    	using forces    = std::vector<force>;
    	using pressures = std::vector<pressure>;
    	using area_densities = std::vector<area_density>;

        const VectorCalculus calculus;
        const Morphology morphology;
        const length world_radius;
        const acceleration gravity;

    public:
        CrustSimulation(
			const VectorCalculus& calculus, 
			const Morphology& morphology,
			const length world_radius,
			const acceleration gravity
		):
            calculus(calculus),
            world_radius(world_radius),
            gravity(gravity)
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

        /*
		`alone` returns a boolean raster indicating where cells are occupied by one or fewer plate
        */
        void alone(
			const CrustSummary& crust,
			bools& out
    	) const {
    		for (int i = 0; i < crust.size(); ++i)
    		{
    			out[i] = crust[i].plate_count() <= 1;
    		}
    	}

        /*
		`exists` returns a boolean raster indicating where cells are occupied by a plate of given id
        */
        void exists(
        	const int plate_id,
			const CrustSummary& crust,
			bools& out
    	) const {
    		for (int i = 0; i < crust.size(); ++i)
    		{
    			out[i] = crust[i].includes(plate_id);
    		}
    	}

        /*
		`top` returns a boolean raster indicating cells where the topmost plate is of given id
        */
        void top(
        	const int plate_id,
			const CrustSummary& crust,
			bools& out
    	) const {
    		for (int i = 0; i < crust.size(); ++i)
    		{
    			out[i] = crust[i].is_top(plate_id);
    		}
    	}

        /*
		`foundering` returns a boolean raster indicating where an entire rock column is foundering
        */
        void foundering(
        	const density mantle_density,
			const CrustSummary& crust,
			bools& out
    	) const {
    		for (int i = 0; i < crust.size(); ++i)
    		{
    			out[i] = crust[i].density() > mantle_density;
    		}
    	}

        /*
		`buoyancy` returns a scalar raster indicating buoyancy forces
        */
        void buoyancy(
        	const force gravity,
			const CrustSummary& crust,
			forces& out
    	) const {
    		for (int i = 0; i < crust.size(); ++i)
    		{
    			out[i] = gravity * crust[i].area_density();
    		}
    	}

        /*
		`rifting` returns a scalar raster indicating where gaps in the plates should be filled by a given plate
        */
		void rifting(
			const bools& alone,
			const bools& top,
			const bools& exists,
			bools& out,
			bools& scratch
		) const {
			/*
			The implementation below is equivalent to the following set logic:
				rifting = erode(alone ∩ top) ∩ margin(exists)
			*/
			bools* riftable           = &out;
			bools* will_stay_riftable = &scratch;
			bools* just_outside       = &out;
			each::intersect    (alone, top,         riftable);
			morphology.erode   (riftable,           will_stay_riftable);
			morphology.outshell(exists,             just_outside);
			each::intersect    (will_stay_riftable, just_outside, out);
		}

        /*
		`detaching` returns a scalar raster indicating where cells of a plate should be destroyed
        */
		void detaching(
			const bools& alone,
			const bools& top,
			const bools& exists,
			const bools& foundering,
			bools& out,
			bools& scratch
		) const {
			/*
			The implementation below is equivalent to the following set logic:
				detaching = erode(!alone ∩ !top) ∩ padding(exists) ∩ foundering
			*/
			bools* not_alone            = &out;
			bools* not_top              = &scratch;
			bools* subducting           = &out;
			// bools* will_stay_subducting = &scratch;
			bools* just_inside          = &out;
			bools* detachable           = &out;
			each::negate      (alone,                not_alone);
			each::negate      (top,                  not_top);
			each::intersect   (not_alone, not_top,   subducting);
			// morphology.erode  (subducting, will_stay_subducting); // NOTE: this was in the js implementation but it is suspected to be wrong
			morphology.inshell(exists,               just_inside);
			each::intersect   (subducting, just_inside, detachable);
			each::intersect   (detachable, foundering, out);
		}

    	/*
		`top_rock_overburden` estimates the amount of overburden at the top of the crust that occurs due to rock
		It currently assumes that gravity is a constant value throughout the crust.
    	*/
        void top_rock_overburden(
        	const force gravity,
			const CrustSummary& crust,
			forces& out
    	) const {
    		for (int i = 0; i < crust.size(); ++i)
    		{
    			out[i] = gravity * (context.is_top(plate_id)? area_density(0.0f) : context[i].top.area_density());
    		}
    	}

		void lithification(
			const int plate_id,
			const pressures& top_overburden, // NOTE: this is typically calculated as top_rock_overburden + fluid_overburden 
			const CrustSummary& context,
			const Crust<M>& crust,
			Formation<M>& delta,
			pressures& scratch
		) const {
		    // NOTE: 2.2e6 Pascals is the pressure equivalent of 500ft of sediment @ 1500kg/m^3 density
			// 500ft from http://wiki.aapg.org/Sandstone_diagenetic_processes
			// TODO: rephrase in terms of lithostatic pressure + geothermal gradient
			const pressure transition_pressure(2.2e6f*si::pascal);

			pressures* bottom_overburden    = &scratch;
			pressures* formation_overburden = &scratch;

			formations.overburden      (series::uniform(pressure(0.0f)), crust.sediment, bottom_overburden);
			formations.overburden_mass (transition_pressure, top_overburden, bottom_overburden, crust.sediment, delta);
		}

		/*
		currently assumes:
		* gravity is constant throughout the crust
		* no geothermal gradient exists
		
		INTENDED FUTURE DESIGN:
		* mineral mass pools are each assumed to be characterized by:
		    * an origin phase
		    * a linear pT phase boundary between the origin phase and the metamorphic phase
		* temperature rasters are added as parameters to `metamorphosis()`
		* an intersection is calculated between the phase boundary and the linear geothermal/geobaric gradient
		*/
		void metamorphosis(
			const int plate_id,
			const pressures& topmost_overburden, // NOTE: this is typically calculated as top_rock_overburden + fluid_overburden 
			const CrustSummary& context,
			const Crust<M>& crust,
			Crust<M>& deltas,
			pressures& scratch1,
			pressures& scratch2
		) const {
			// NOTE: 300e6 Pascals is the pressure equivalent of 11km of sedimentary rock @ 2700kg/m^3 density
			// 300 MPa from https://www.tulane.edu/~sanelson/eens212/typesmetamorph.htm
			// TODO: rephrase in terms of lithostatic pressure + geothermal gradient
			const pressure transition_pressure(300e6f*si::pascal);

			pressures* top_overburden       = &scratch1;
			pressures* bottom_overburden    = &scratch2;

			each::copy(topmost_overburden, top_overburden);
			formations.overburden(series::uniform(pressure(0.0f)), crust.sediment, bottom_overburden);
			formations.overburden_mass (transition_pressure, top_overburden, bottom_overburden, crust.sediment, deltas.sediment);
			formations.metamorphosis   (deltas.sediment, deltas.sediment,    deltas.sediment);

			top_overburden                 = &bottom_overburden;
			formation_overburden_footprint = &top_overburden;
			bottom_overburden              = &top_overburden;
			formations.overburden(top_overburden, crust.sedimentary, bottom_overburden);
			formations.overburden_mass (transition_pressure, top_overburden, bottom_overburden, crust.sedimentary, deltas.sedimentary);
			formations.metamorphosis   (deltas.sediment, deltas.sedimentary, deltas.sedimentary);

			top_overburden                 = &bottom_overburden;
			formation_overburden_footprint = &top_overburden;
			bottom_overburden              = &top_overburden;
			formations.overburden(top_overburden, crust.bedrock, bottom_overburden);
			formations.overburden_mass (transition_pressure, top_overburden, bottom_overburden, crust.bedrock, deltas.bedrock);
			formations.metamorphosis   (deltas.sediment, deltas.bedrock,     deltas.bedrock);

		}

		void weathering(
			const speeds& precipitation,
			const pressures& top_overburden, // NOTE: this is typically calculated as top_rock_overburden + fluid_overburden 
			const Crust<M>& crust,
			Crust<M>& deltas,
		) const {
		}

		void erosion(
			const speeds& precipitation,
			const lengths& displacements,
			const Formation<M>& formation,
			Formation<M>& deltas,
		) const {
		}

		void guess_plate_map(
			const vec3s& velocities,
			const std::size_t segment_num, 
			const std::size_t min_segment_size, 
			std::vector<std::uint8_t>& plate_ids
		) const {
		}

		glm::vec3 guess_plate_center_of_mass(
			const masses& mass
		) const {
		}

		glm::mat3 guess_plate_rotation_matrix(
			const vec3s& plate_velocity,
			glm::vec3 center_of_plate,
			si::time<double> seconds
		) const {
			/*
		    plates are rigid bodies
		    as with any rigid body, there are two ways that forces can manifest themselves:
		       1.) linear acceleration     translates a body 
		       2.) angular acceleration     rotates a body around its center of mass (CoM)
		    but on a sphere, linear acceleration just winds up rotating a plate around the world's center
		    this contrasts with angular acceleration, which rotates a plate around its center of mass.
		    Here, we track both kinds of motion.
		    */
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

    };

}
