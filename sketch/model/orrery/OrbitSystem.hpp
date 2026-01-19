#pragma once

// C libraries
#include <cmath>    // std::abs

// std libraries
#include <limits>   // std::numeric_limits
#include <vector>   // std::vector
#include <algorithm>// std::min/max/fill
#include <utility>  // std::pair

// 3rd-party libraries
#include <glm/vec3.hpp>          // *vec3

// in-house libraries
#include <math/special.hpp> // math::roundfract

#include "Cycle.hpp"
#include "CycleSample.hpp"
#include "TrackPosition.hpp"
#include "Resonance.hpp"
#include "Components.hpp"

namespace orrery
{

    template<typename id, typename scalar, glm::qualifier precision = glm::defaultp>
    class OrbitSystem{

        static constexpr scalar oo = std::numeric_limits<scalar>::infinity();
        static constexpr scalar s1 = scalar(1);
        static constexpr scalar i0 = id(0);
        static constexpr scalar i1 = id(1);
        static constexpr scalar i2 = id(2);

        using vec3 = glm::vec<3,scalar,precision>;

        using Orbits = Components<orbit::Universals<scalar>>;
        using Cycles = std::vector<Cycle<id,scalar>>;
        using CycleSamples = std::vector<CycleSample<id,scalar>>;
        using TrackPositions = std::vector<TrackPosition<scalar>>;
        using Resonances = std::vector<Resonance<id,scalar>>;
        using ids = std::vector<id>;
        using vec3s = std::vector<vec3>;

        const orbit::UniversalPropagator<scalar> propagator;
        const orbit::Properties<scalar> elliptics;
        const id max_index;

        id closest_resonance_index(const scalar fraction) const
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
            const orbit::Properties<scalar> elliptics,
            const id max_index
        ) : 
            propagator(propagator),
            elliptics(elliptics),
            max_index(max_index)
        {}

        // Oⁿ → Nᵐ
        void group(
            const Orbits& orbits, 
            const scalar min_perceivable_period,
            const scalar max_perceivable_period,
            Cycles& imperceptible, 
            ids& perceptible
        ) const {
            perceptible.clear();
            perceptible.reserve(orbits.size());
            imperceptible.clear();
            imperceptible.reserve(orbits.size());
            orbit::Universals<scalar> orbit;
            scalar inverse_semi_major_axis;
            for (std::size_t i = 0; i < orbits.size(); ++i) {
                orbit = orbits.get(i);
                inverse_semi_major_axis = propagator.inverse_semi_major_axis(orbit);
                if (!propagator.is_elliptic(inverse_semi_major_axis)) {
                    perceptible.emplace_back(id(i));
                } else {
                    scalar period = elliptics.period_from_semi_major_axis(s1/inverse_semi_major_axis, orbit.combined_mass);
                    if (min_perceivable_period <= period && period <= max_perceivable_period) {
                        perceptible.emplace_back(id(i));
                    } else {
                        imperceptible.emplace_back(id(i), period);
                    }
                }
            }
        }

        // Tⁿ→RⁿI
        void resonances(
            const Cycles& cycles,
            ids& resonances,
            id& resonance_count
        ) const {
            resonances.assign(cycles.size(), -i1);
            resonance_count = i0;
            for (std::size_t j = 0; j < cycles.size(); ++j) {
                for (std::size_t i = 0; i < j; ++i) {
                    const scalar fraction = cycles[i].period / cycles[j].period;
                    const id p = closest_resonance_index(fraction);
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
        std::vector<scalar> periods(
            const Cycles& cycles,
            const std::vector<id>& resonances,
            const id resonance_count
        ) {

            std::vector<std::pair<scalar, scalar>> resonance_period_range(
                std::size_t(resonance_count),
                { oo, scalar(0) }
            );

            for (std::size_t node = 0; node < cycles.size(); ++node)
            {
                const id resonance = id(resonances[node]);
                if (0 <= resonance&&resonance < resonance_count) {
                    auto& pr = resonance_period_range[resonance];
                    pr.first  = std::min(pr.first,  cycles[node].period); // lo
                    pr.second = std::max(pr.second, cycles[node].period); // hi
                }
            }

            std::vector<scalar> resonance_periods;
            resonance_periods.reserve(resonance_count);
            for (const auto& [lo, hi] : resonance_period_range) {
                resonance_periods.push_back(closest_resonance_index(lo/hi) * lo);
            }

            return resonance_periods;

        }

        // (IT)ⁿII → (NPCT)ⁿ
        void sample(
            const Cycles& cycles, 
            const id samples_per_cycle, 
            const id sample, 
            CycleSamples& samples
        ) const {
            samples.clear();
            samples.reserve(cycles.size());
            for (std::size_t i = 0; i < cycles.size(); ++i) {
                samples.emplace_back(
                    cycles[i], 
                    id(sample/std::pow(samples_per_cycle,i))%samples_per_cycle,
                    samples_per_cycle
                );
            }
        }

        // Oⁿ(NPCT)ⁿ → (Nℝ³)ᵐ
        void positions(
            const Orbits& orbits,
            const CycleSamples& samples,
            TrackPositions& results
        ) const {
            results.clear();
            results.reserve(samples.size());
            CycleSample<id,scalar> sample;
            for (std::size_t i = 0; i < samples.size(); ++i) {
                sample = samples[i];
                results.emplace_back(
                    sample.node, 
                    propagator.advance(
                        orbits.get(i), 
                        sample.time()
                    )); // TODO: add logic to consider time offsets in the orbit itself
            }
        }

        // OⁿNᵐt→(Nℝ³)ᵐ
        void positions(
            const Orbits& orbits,
            const ids& filtered,
            const scalar time_offset,
            TrackPositions& results
        ) const {
            results.clear();
            results.reserve(filtered.size());
            for (std::size_t i = 0; i < filtered.size(); ++i) {
                results.emplace_back(
                    filtered[i],
                    propagator.advance(
                        orbits.get(filtered[i]),
                        time_offset
                    )); // TODO: add logic to consider time offsets in the orbit itself
            }
        }

        // (Nℝ³)ᵐ(ℝ³)ⁿ → (ℝ³)ⁿ
        void update(
            const TrackPositions& updates,
            const vec3s& tree,
            vec3s& updated
        ) const {
            if(&updated != &tree) {
                updated.clear();
                updated.reserve(tree.size());
                for (std::size_t i = 0; i < tree.size(); ++i) {
                    updated.emplace_back(tree[i]);
                }
            }
            for (std::size_t i = 0; i < updates.size(); ++i) {
                updated[updates[i].node] = updates[i].position;
            }
        }

        // (NPCT)ⁿ(RPCT)ᵐ → (NPCT)ⁿ
        void update(
            const Resonances& resonances,
            const CycleSamples& samples,
            const CycleSamples& subsamples,
            CycleSamples& updated
        ) const {
            if(&updated != &subsamples) {
                updated.clear();
                updated.reserve(subsamples.size());
                for (std::size_t i = 0; i < subsamples.size(); ++i) {
                    updated.emplace_back(subsamples[i]);
                }
            }
            CycleSample period;
            CycleSample subperiod;
            for (std::size_t i = 0; i < samples.size(); ++i) {
                period = samples[resonances[i].resonance];
                subperiod = updated[resonances[i].node];
                updated[resonances[i].node] = updated[subperiod].with_time(period.time());
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
