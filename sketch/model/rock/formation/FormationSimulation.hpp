#pragma once

#include <cmath>

#include <glm/vec3.hpp>               // *vec3

namespace rock {

	/*

	`FormationSimulation` (together with `CrustSimulation`) 
	is basically the most important part of the geological model.
	They contains everything that's needed to simulate the geology of a planet.
	*/

	template<int M>
    class FormationSimulation
    {
    	using pressure  = si::pressure<float>;
    	using acceleration = si::acceleration<float>;

    	using points    = std::vector<compound::point<float>>;
    	using pressures = std::vector<pressure>;

        const acceleration gravity;

    public:
        FormationSimulation(
			const acceleration gravity
		):
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
    			out[i] = gravity * formation[i].area_density();
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

