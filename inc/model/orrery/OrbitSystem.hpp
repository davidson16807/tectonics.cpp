#pragma once

// C libraries
#include <cmath>    // std::abs

// std libraries
#include <limits>   // std::numeric_limits
#include <vector>   // std::vector

// 3rd-party libraries
#include <glm/vec3.hpp>          // *vec3

// in-house libraries
#include <model/orbit/Properties.hpp> // orbit::Properties
#include <model/orbit/Universals.hpp> // orbit::Universals
#include <model/orbit/UniversalPropagator.hpp> // orbit::UniversalPropagator

#include "EntityComponents.hpp"
#include "DenseContiguousComponents.hpp"

namespace orrery
{

    template<typename id, typename scalar, glm::qualifier precision = glm::defaultp>
    class OrbitSystem{

        static constexpr scalar s1 = scalar(1);

        using duration = scalar;
        using vec3 = glm::vec<3,scalar,precision>;

        using bools = std::vector<bool>;

        using Orbit = orbit::Universals<scalar>;

        using Orbits = DenseContiguousComponents<id, Orbit>;
        using TrackPositions = EntityComponents<id,vec3>;
        using Periods = EntityComponents<id,duration>;

        const orbit::UniversalPropagator<scalar> propagator;
        const orbit::Properties<scalar> elliptics;

    public:

        OrbitSystem(
            const orbit::UniversalPropagator<scalar> propagator,
            const orbit::Properties<scalar> elliptics
        ) : 
            propagator(propagator),
            elliptics(elliptics)
        {}

        // partitions orbits among periodic an nonperiodic
        void periods(
            const Orbits& orbits,
            Periods& periods,
            bools& aperiodic
        ) const {
            periods.clear();
            periods.reserve(orbits.entity_count());
            Orbit orbit;
            scalar inverse_semi_major_axis;
            for (std::size_t entity = 0; entity < orbits.entity_count(); ++entity) {
                if (!orbits.has(entity)) {
                    aperiodic[entity] = true;
                } else {
                    orbit = orbits.component_for_entity(entity);
                    inverse_semi_major_axis = propagator.inverse_semi_major_axis(orbit);
                    if (!propagator.is_elliptic(inverse_semi_major_axis)) {
                        aperiodic[entity] = true;
                    } else {
                        periods.add(id(entity), 
                            elliptics.period_from_semi_major_axis(s1/inverse_semi_major_axis, orbit.combined_mass));
                    }
                }
            }
        }

        // Oⁿ… → (Nℝ³)ᵐ
        // generates barycentric offsets for each cyclic orbit and configuration
        // this is motivated by the need to quickly track imperceptible elliptic orbits
        void offsets(
            const Orbits& orbits,
            const Periods& periods,
            const std::vector<scalar>& fractions,
            TrackPositions& results
        ) const {
            results.clear();
            results.reserve(periods.size());
            for (std::size_t entity = 0; entity < periods.size(); ++entity) {
                auto cycle = periods.entity_for_index(entity);
                auto period = periods.component_for_index(entity);
                auto fraction = fractions[entity];
                results.add(
                    cycle, 
                    propagator.state(
                        orbits.component_for_entity(cycle),
                        fraction * period
                    ).position); // TODO: add logic to consider time offsets in the orbit itself
            }
        }

        // Oⁿ… → (Nℝ³)ᵐ
        // generates barycentric offsets for each cyclic orbit and configuration
        // this is motivated by the need to quickly track imperceptible elliptic orbits
        void offsets(
            const Orbits& orbits,
            const Periods& periods,
            const scalar time_offset,
            TrackPositions& results
        ) const {
            results.clear();
            results.reserve(orbits.entity_count());
            for (std::size_t entity = 0; entity < orbits.entity_count(); ++entity) {
                if (orbits.has(entity)){
                    results.add(
                        entity,
                        propagator.state(
                            orbits.component_for_entity(entity),
                            time_offset
                        ).position); // TODO: add logic to consider time offsets in the orbit itself
                }
            }
        }

        // OⁿNᵐt→(Nℝ³)ᵐ
        // generates barycentric offsets at a given time offset for each orbit
        // this is motivated by the need to track hyperbolic, parabolic, and perceptible elliptic orbits
        void offsets(
            const Orbits& orbits,
            const bools& filter,
            const scalar time_offset,
            TrackPositions& results
        ) const {
            results.clear();
            results.reserve(orbits.entity_count());
            for (std::size_t entity = 0; entity < orbits.entity_count(); ++entity) {
                if (orbits.has(entity) && filter[entity]){
                    results.add(
                        entity,
                        propagator.state(
                            orbits.component_for_entity(entity),
                            time_offset
                        ).position); // TODO: add logic to consider time offsets in the orbit itself
                }
            }
        }

        // OⁿNᵐt→(Nℝ³)ᵐ
        // generates barycentric offsets at a given time offset for each orbit
        // this is motivated by the need to track hyperbolic, parabolic, and perceptible elliptic orbits
        void offsets(
            const Orbits& orbits,
            const scalar time_offset,
            TrackPositions& results
        ) const {
            results.clear();
            results.reserve(orbits.entity_count());
            for (std::size_t entity = 0; entity < orbits.entity_count(); ++entity) {
                if (orbits.has(entity)){
                    results.add(
                        entity,
                        propagator.state(
                            orbits.component_for_entity(entity),
                            time_offset
                        ).position); // TODO: add logic to consider time offsets in the orbit itself
                }
            }
        }

    };

    /*
    SⁿPᵐ→ℝ⁴⁴      return rotation of a spin for trajectories at phases
    BⁿNᵐt→(Nℝ³)ᵐ  update positions in scene tree for trajectories at time

    phase 2:
    Pⁿ → Rᵐ       identify resonances by indices with sample counts

    tests:
    * determinism
    * imperceptible and perceptible returned from group must be disjoint
    * reordering `orbits` does not change the *set* of outputs
    * periods are positive and finite
    * 

    */

}
