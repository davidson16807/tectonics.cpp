#pragma once

#include <cmath>

#include <glm/vec3.hpp>               // *vec3

namespace rock {

	/*

	`FormationPredicates` (together with `CrustSimulation`) 
	is basically the most important part of the geological model.
	They contains everything that's needed to simulate the geology of a planet.
	*/

	template<int M>
    class FormationPredicates
    {

    	using mass = si::mass<float>;

    	using bools = std::vector<bool>;

    	static constexpr mass threshold = mass(si::kilogram);

    public:
        FormationPredicates()
        {}

        void empty(
			const Formation<M>& formation,
			bools& out
    	) const {
    		for (std::size_t i = 0; i < out.size(); ++i)
    		{
    			out[i] = formation[i].mass() <= threshold;
    		}
    	}

        void exists(
			const Formation<M>& formation,
			bools& out
    	) const {
    		for (std::size_t i = 0; i < out.size(); ++i)
    		{
    			out[i] = formation[i].mass() > threshold;
    		}
    	}

    };

}

