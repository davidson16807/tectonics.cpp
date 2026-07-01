#pragma once

// C libraries
#include <cmath>    // std::abs

// std libraries
#include <limits>   // std::numeric_limits
#include <vector>   // std::vector
#include <algorithm>// std::min/max
#include <utility>  // std::pair

// 3rd-party libraries
#include <glm/vec3.hpp>          // *vec3

// in-house libraries
#include <math/special.hpp>           // math::roundfract
#include <model/orbit/Properties.hpp> // orbit::Properties
#include <model/orbit/Universals.hpp> // orbit::Universals
#include <model/orbit/UniversalPropagator.hpp> // orbit::UniversalPropagator

#include "EntityComponents.hpp"
#include "Resonance.hpp"
#include "DenseContiguousComponents.hpp"

namespace orrery
{

    template<typename id, typename scalar, glm::qualifier precision = glm::defaultp>
    class OrbitSystem{

        static constexpr scalar oo = std::numeric_limits<scalar>::infinity();
        static constexpr scalar s1 = scalar(1);
        static constexpr scalar i0 = id(0);
        static constexpr scalar i1 = id(1);
        static constexpr scalar i2 = id(2);

        using duration = scalar;
        using vec3 = glm::vec<3,scalar,precision>;

        using Orbits = DenseContiguousComponents<id, orbit::Universals<scalar>>;
        using TrackPositions = EntityComponents<id,vec3>;
        using Periods = EntityComponents<id,duration>;
        using Resonances = EntityComponents<id,Resonance<id,duration>>;

        using ids = std::vector<id>;
        using vec3s = std::vector<vec3>;
        using bools = std::vector<bool>;
        using scalars = std::vector<scalar>;

        const orbit::UniversalPropagator<scalar> propagator;
        const orbit::Properties<scalar> elliptics;

        id closest_resonance_index(const scalar fraction, const id max_index) const
        {
            id best_p(1);
            auto best_error = std::abs(math::roundfract(scalar(best_p) * fraction));

            for (id p = i2; p <= max_index; ++p)
            {
                auto error = std::abs(math::roundfract(scalar(p) * fraction));
                if (error < best_error)
                {
                    best_error = error;
                    best_p = p;
                }
            }
            return best_p;
        }

    public:

        OrbitSystem(
            const orbit::UniversalPropagator<scalar> propagator,
            const orbit::Properties<scalar> elliptics
        ) : 
            propagator(propagator),
            elliptics(elliptics)
        {}

        // Oⁿ → Nᵐ
        // partitions orbits among imperceptible cyclic and perceptible/noncyclic groups
        // `Spins` must be a `*Components` object that has the ability to get entity ids, i.e. `entity_for_index`
        void group(
            const Orbits& orbits, 
            const scalar min_perceivable_period,
            const scalar max_perceivable_period,
            Periods& imperceptible, 
            bools& perceptible
        ) const {
            perceptible.resize(orbits.entity_count());
            imperceptible.clear();
            imperceptible.reserve(orbits.entity_count());
            orbit::Universals<scalar> orbit;
            scalar inverse_semi_major_axis;
            for (std::size_t entity = 0; entity < orbits.entity_count(); ++entity) {
                if (orbits.has(entity)) {
                    orbit = orbits.component_for_entity(entity);
                    inverse_semi_major_axis = propagator.inverse_semi_major_axis(orbit);
                    if (!propagator.is_elliptic(inverse_semi_major_axis)) {
                        perceptible[entity] = true;
                    } else {
                        scalar period = elliptics.period_from_semi_major_axis(s1/inverse_semi_major_axis, orbit.combined_mass);
                        if (min_perceivable_period <= period && period <= max_perceivable_period) {
                            perceptible[entity] = true;
                        } else {
                            perceptible[entity] = false;
                            imperceptible.emplace_back(id(entity), period);
                        }
                    }
                }
            }
        }

        // // `Spins` must be a `*Components` object that has the ability to get entity ids, i.e. `.entity_for_index()`
        // void group(
        //     const Spins& spins, 
        //     const scalar min_perceivable_period,
        //     const scalar max_perceivable_period,
        //     Periods& imperceptible, 
        //     bools& perceptible
        // ) const {
        //     perceptible.resize(orbits.entity_count());
        //     imperceptible.clear();
        //     imperceptible.reserve(orbits.entity_count());
        //     Spin spin;
        //     for (std::size_t entity = 0; entity < orbits.entity_count(); ++entity) {
        //         if (spins.has(entity)) {
        //             spin = spins.component_for_entity(entity);
        //             scalar period = spin.period();
        //             if (min_perceivable_period <= period && period <= max_perceivable_period) {
        //                 perceptible[entity] = true;
        //             } else {
        //                 perceptible[entity] = false;
        //                 imperceptible.emplace_back(id(entity), period);
        //             }
        //         }
        //     }
        // }
        // // lists of imperceptible Periods for orbits and spins will need to be concatenated after invocation in intended use case

        // Tⁿ→RⁿI
        // detects resonant systems between Periods
        void resonances(
            const Periods& periods,
            ids& resonances,
            id& resonance_count
        ) const {
            resonances.assign(periods.size(), -i1);
            resonance_count = i0;
            for (std::size_t j = 0; j < periods.size(); ++j) {
                for (std::size_t i = 0; i < j; ++i) {
                    const auto period1 = periods.component_for_index(i);
                    const auto period2 = periods.component_for_index(j);
                    const scalar fraction = period1 / period2;
                    const id p = closest_resonance_index(fraction, id(periods.size()));
                    if (std::abs(math::roundfract(scalar(p) * fraction)) < scalar(1e-3)) {
                        id resonance_id;
                        if (resonances[i] >= i0) {
                            resonance_id = resonances[i];
                        } else {
                            resonance_id = resonance_count;
                            ++resonance_count;
                        }
                        resonances[i] = resonance_id;
                        resonances[j] = resonance_id;
                    }
                }
            }
        }

        // TⁿRⁿI→Tʳ
         // calculates the periods of resonant systems
        std::vector<scalar> periods(
            const Periods& periods,
            const ids& resonances,
            const id resonance_count
        ) {

            std::vector<std::pair<scalar, scalar>> resonance_period_range(
                std::size_t(resonance_count),
                { oo, scalar(0) }
            );

            for (std::size_t node = 0; node < periods.size(); ++node)
            {
                const id resonance = id(resonances[node]);
                if (0 <= resonance&&resonance < resonance_count) {
                    const duration period = periods.component_for_index(node);
                    auto& pr = resonance_period_range[resonance];
                    pr.first  = std::min(pr.first,  period); // lo
                    pr.second = std::max(pr.second, period); // hi
                }
            }

            std::vector<scalar> resonance_periods;
            resonance_periods.reserve(resonance_count);
            for (const auto& [lo, hi] : resonance_period_range) {
                resonance_periods.push_back(closest_resonance_index(lo/hi, resonance_count) * lo);
            }

            return resonance_periods;

        }

        // (IT)ⁿII → (NPCT)ⁿ
        // for a given sample id, this generates that sample's cycle configuration
        void sample(
            const Periods& periods, 
            const ids& generations, 
            const id samples_per_cycle, 
            const id sample, 
            scalars& fractions
        ) const {
            fractions.clear();
            fractions.reserve(periods.size());
            for (std::size_t i = 0; i < periods.size(); ++i) {
                fractions.push_back(
                    ((scalar(sample)/scalar(std::pow(samples_per_cycle,generations[i])))%scalar(samples_per_cycle)) / 
                    scalar(samples_per_cycle));
            }
        }

        // Oⁿ(NPCT)ⁿ → (Nℝ³)ᵐ
        // generates barycentric offsets for each cyclic orbit and configuration
        // this is motivated by the need to quickly track imperceptible elliptic orbits
        template<typename Fractions>
        void offsets(
            const Orbits& orbits,
            const Periods& periods,
            const Fractions& fractions,
            TrackPositions& results
        ) const {
            results.clear();
            results.reserve(periods.size());
            for (std::size_t i = 0; i < periods.size(); ++i) {
                auto cycle = periods.entity_for_index(i);
                auto period = periods.component_for_index(i);
                auto fraction = fractions[i];
                results.add(
                    cycle, 
                    propagator.state(
                        orbits.component_for_entity(cycle),
                        fraction * period
                    ).position); // TODO: add logic to consider time offsets in the orbit itself
            }
        }

        /*
        TODO: 
        sample(sample, samples_per_cycle) → phases
        offset(orbits, time_offsets) → offsets

        this will allow:
        noise(seed) → phases
        periods * phases → time_offsets
        */

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
            for (std::size_t i = 0; i < orbits.entity_count(); ++i) {
                if (filter[i]){
                    results.add(
                        i,
                        propagator.state(
                            orbits.component_for_entity(i),
                            time_offset
                        ).position); // TODO: add logic to consider time offsets in the orbit itself
                }
            }
        }

        // (NPCT)ⁿ(RPCT)ᵐ → (NPCT)ⁿ
        // updates the given sample configuration of Periods, `subsamples`, 
        // to reflect a given sample configuration for resonance systems, `samples`
        // void update(
        //     const Resonances& resonances,
        //     const PeriodSamples& samples,
        //     const PeriodSamples& subsamples,
        //     PeriodSamples& updated
        // ) const {
        //     if(&updated != &subsamples) {
        //         updated.clear();
        //         updated.reserve(subsamples.size());
        //         for (std::size_t i = 0; i < subsamples.size(); ++i) {
        //             updated.emplace_back(subsamples[i]);
        //         }
        //     }
        //     PeriodSample<scalar,scalar> sample;
        //     PeriodSample<scalar,scalar> subsample;
        //     for (std::size_t i = 0; i < samples.size(); ++i) {
        //         sample = samples.component_for_index(resonances.component_for_entity(i).resonance);
        //         subsample = updated[resonances.entity_for_index(i)];
        //         updated[resonances.entity_for_index(i)] = updated[subsample].with_time(sample.time());
        //     }
        // }

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
