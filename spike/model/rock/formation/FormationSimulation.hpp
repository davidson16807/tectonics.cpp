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

	template<int M>
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
    	using points    = std::vector<compound::point<float>>;

    	using masses    = std::vector<mass>;
    	using lengths   = std::vector<length>;
    	using densities = std::vector<density>;
    	using speeds    = std::vector<speeds>;
    	using forces    = std::vector<force>;
    	using pressures = std::vector<pressure>;
    	using area_densities = std::vector<area_density>;

        const acceleration gravity;

    public:
        CrustSimulation(
			const acceleration gravity
		):
            calculus(calculus),
            world_radius(world_radius),
            gravity(gravity)
        {}

        template <typename pressure_series>
        void overburden(
			const pressure_series& top_overburden, 
			const Formation<M>& formation,
			pressures& out
    	) const {
    		for (int i = 0; i < formation.size(); ++i)
    		{
    			out[i] = gravity * formation.area_density();
    		}
    	}

    	/*
		`overburdened_mass` estimates the amount of mass that is overburdened within a formation.
		It assumes that overburden pressure is a linear function of depth over the entire formation.
    	*/
		void overburdened_mass(
			const pressure overburden_pressure,
			const pressures& top_overburden,    
			const pressures& bottom_overburden,
			const Formation<M>& formation,
			Formation<M>& out
		) const {
			float overburdened_fraction;
			Polynomial<T,0,1> overburden;
    		for (int i = 0; i < formation.size(); ++i)
    		{
    			overburden = analytic::linear_spline(1.0f, 0.0f, top_overburden[i], bottom_overburden[i]);
    			overburdened_fraction = analytic::inverse(overburden)(overburden_pressure);
    			out[i] = strata.scale(formation[i], overburdened_fraction);
    		}
		}

    	/*
		`overburdened_mass` estimates the amount of mass that is overburdened within a formation.
		It assumes that overburden pressure is a linear function of depth over the entire formation.
    	*/
		void overburdened_mass(
			const LinearPhaseBoundary& phase_boundary,
			const compound::point<float>& top_point,    
			const compound::point<float>& bottom_point,
			const Formation<M>& formation,
			Formation<M>& out
		) const {
			float overburdened_fraction;
			Polynomial<T,0,1> overburden;
    		for (int i = 0; i < formation.size(); ++i)
    		{
    			overburden = analytic::linear_spline(1.0f, 0.0f, top_overburden[i], bottom_overburden[i]);
    			overburdened_fraction = analytic::inverse(overburden)(overburden_pressure);
    			out[i] = strata.scale(formation[i], overburdened_fraction);
    		}
		}

		void metamorphosis(
			const std::vector<LinearPhaseBoundary>& phase_boundaries,
			const points& top_condition,    
			const points& bottom_condition,
			const Formation<M>& source_formation,
			const Formation<M>& target_formation,
			Formation<M>& source_delta,
			Formation<M>& target_delta,
		) const {
    		for (int i = 0; i < source_formation.size(); ++i)
    		{
	    		for (int j = 0; j < phase_boundaries.size(); ++j)
	    		{
	    			int source_id = phase_boundaries[j].source_mineral_id;
	    			int target_id = phase_boundaries[j].target_mineral_id;
	    			fraction = phase_boundaries[j].transition_fraction(top_condition[i], bottom_condition[i])
	    			source_delta[i][source_id] = minerals.combine(source_formation[i][source_id], minerals.scale(source_formation[i][source_id], -fraction));
	    			target_delta[i][target_id] = minerals.combine(target_formation[i][target_id], minerals.scale(source_formation[i][source_id],  fraction));
	    		}
    		}
		}

    };

}

