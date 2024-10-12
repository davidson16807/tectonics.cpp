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

	`CrustPhaseChange` (together with `FormationPhaseChange`)
	is basically *the most important part of the geological model*.
	They contain everything that's needed to simulate the geology of a planet.

	This may include the following:
	* melting        the conversion of solids to liquids
	* sublimation    the conversion of solids to gases
	* metamorphosis  the conversion of solids from one crystal structure to another
	* lithification  the conversion of sediment to sedimentary rock
	* weathering     the conversion of rock to sediment
	* hotspots       the addition of mass at a function of distance from a prescribed geocoordinate
	* astroblemes    the removal of mass as a function of distance from a prescribed geocoordinate

	*/

	// NOTE: `M` is mineral count, `F` is formation count
	template<int M, int F>
    class CrustPhaseChange
    {
    	using pressure  = si::pressure<float>;

    	using pressures = std::vector<pressure>;

        const FormationPhaseChange formations;

    public:
        CrustPhaseChange(
        	const FormationPhaseChange& formations
		): 
			formations(formations)
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


		void lithification(
			const int plate_id,
			const pressures& top_overburden, // NOTE: this is typically calculated as top_rock_overburden + fluid_overburden 
			const Crust<M,F>& crust,
			Formation<M>& delta,
			pressures& scratch
		) const {
		    // NOTE: 2.2e6 Pascals is the pressure equivalent of 500ft of sediment @ 1500kg/m^3 density
			// 500ft from http://wiki.aapg.org/Sandstone_diagenetic_processes
			// TODO: rephrase in terms of lithostatic pressure + geothermal gradient
			const pressure transition_pressure(2.2e6f*si::pascal);

			pressures* bottom_overburden    = &scratch;
			pressures* formation_overburden = &scratch;

			formations.overburden        (procedural::uniform(pressure(0.0f)), crust[formations::sediment], bottom_overburden);
			formations.overburdened_mass (transition_pressure, top_overburden, bottom_overburden, crust[formations::sediment], delta);
		}

		/*
		currently assumes:
		* gravity is constant throughout the crust
		* no geothermal gradient exists
		
		INTENDED FUTURE DESIGN:
		* mineral mass pools are each assumed to be characterized by:
		    * an origin phase
		    * a linear pressure/temperature phase boundary between the origin phase and the metamorphic phase
		* non-metamorphic minerals have origin phases that are equal to their own, thereby accomplishing no-op behavior
		* temperature rasters are added as parameters to `CrustPhaseChange::metamorphosis()`
		* an intersection is calculated between the phase boundary and the linear geothermal/geobaric gradient
		*/
		void metamorphosis(
			const std::vector<LinearPhaseBoundary>& phase_boundaries,
			const int plate_id,
			const pressures& topmost_overburden, // NOTE: this is typically calculated as top_rock_overburden + fluid_overburden 
			const Crust<M,F>& crust,
			Crust<M,F>& deltas,
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
			formations.overburden        (procedural::uniform(pressure(0.0f)), crust[formations::sediment], bottom_overburden);
			/* 
			NOTE: we do not move sediment directly to metamorphic, since lithification will also occur on that same mass,
			and we do not want metamorphosis and lithification fighting over or double counting mass.
			We therefore wait until the sediment has lithified, then metamorphose the sedimentary rock.
			*/

			for (int i = 0; i < crust.size(); i+=2)
			{
				nonmetamorphic = i;
				metamorphic    = i+1;

				top_overburden                 = &bottom_overburden;
				formation_overburden_footprint = &top_overburden;
				bottom_overburden              = &top_overburden;
				formations.overburden    (top_overburden, crust[i], bottom_overburden);
				formations.metamorphosis (phase_boundaries, 
					top_overburden, bottom_overburden,
					deltas[nonmetamorphic], deltas[metamorphic], 
					deltas[nonmetamorphic], deltas[metamorphic]);

				/*
				If further metamorphosis occurs in the metamorphic layer, then send the results to the same layer
				*/
				top_overburden                 = &bottom_overburden;
				formation_overburden_footprint = &top_overburden;
				bottom_overburden              = &top_overburden;
				formations.overburden    (top_overburden, crust[i], bottom_overburden);
				formations.metamorphosis (phase_boundaries, 
					top_overburden, bottom_overburden,
					deltas[metamorphic], deltas[metamorphic], 
					deltas[metamorphic], deltas[metamorphic]);
			}

		}

}

