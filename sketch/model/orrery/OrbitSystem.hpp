#pragma once

// std libraries
#include <vector> // std::vector

// in-house libraries
#include "Cycle.hpp"
#include "CycleSample.hpp"
#include "TrackPosition.hpp"
#include "Resonance.hpp"
#include "Components.hpp"

template<typename id, typename scalar>
class OrbitSystem{

    using Orbits = Components<orbit::Universals<scalar>>;
    using Cycles = std::vector<Cycle<id,scalar>>;
    using CycleSamples = std::vector<CycleSample<id,scalar>>;
    using TrackPositions = std::vector<TrackPosition<scalar>>
    using Resonances = std::vector<Resonance<id,scalar>>

    const orbit::UniversalPropagator<scalar> propagator;
    const orbit::Properties<scalar> elliptics;

    bool is_resonant(scalar period1, scalar period2) const {

    }

public:

    OrbitSystem(
        const orbit::UniversalPropagator<scalar> propagator,
        const orbit::Properties<scalar> elliptics
    ) : 
        propagator(propagator),
        elliptics(elliptics)
    {}

    void group(
        const Orbits& orbits, 
        const scalar min_perceivable_period,
        const scalar max_perceivable_period,
        Cycles& imperceptible, 
        ids& perceptible
    ) const {
        perceptible.reserve(orbits.size());
        perceptible.clear();
        imperceptible.reserve(orbits.size());
        imperceptible.clear();
        orbit::Universals<scalar> orbit;
        for (std::size_t i = 0; i < orbits.size(); ++i) {
            orbit = orbits[i];
            if (!propagator.is_elliptic(orbit)) {
                perceptible.emplace_back(i);
            } else {
                scalar period = elliptics.period_from_semi_major_axis(semi_major_axis, orbit.combined_mass);
                if (min_perceivable_period <= period && period <= max_perceivable_period) {
                    imperceptible.emplace_back(i, period);
                } else {
                    perceptible.emplace_back(i);
                }
            }
        }
    }

    void resonances(
        const Cycles& cycles,
        Resonances& resonances
    ) const {
        for (std::size_t i = 0; i < cycles.size(); ++i) {
            for (std::size_t j = 0; j < i; ++j) {
                if (is_resonant(cycles[i].period, cycles[j].period)){
                    resonances.emplace_back(cycles[i].node, resonance);
                }
            }
        }
    }

    void sample(
        const Cycles& cycles, 
        const id samples_per_cycle, 
        const id sample, 
        CycleSamples& samples
    ) const {
        samples.reserve(orbits.size());
        samples.clear();
        for (std::size_t i = 0; i < samples.size(); ++i) {
            samples.emplace_back(
                cycles[i], 
                id(sample/std::pow(samples_per_cycle,i))%samples_per_cycle,
                samples_per_cycle
            );
        }
    }

    void positions(
        const Orbits& orbits,
        const CycleSamples& samples,
        TrackPositions& results
    ) const {
        results.reserve(samples.size());
        results.clear();
        CycleSample sample;
        for (std::size_t i = 0; i < samples.size(); ++i) {
            sample = samples[i];
            results.emplace_back(
                sample.node, 
                propagator.advance(
                    orbits.get(sample.node), 
                    sample.time()
                )); // TODO: add logic to consider time offsets in the orbit itself
        }
    }

    void positions(
        const Orbits& orbits,
        const ids& filtered,
        const scalar time_offset,
        TrackPositions& results
    ) const {
        results.reserve(filtered.size());
        results.clear();
        for (std::size_t i = 0; i < filtered.size(); ++i) {
            results.emplace_back(
                filtered[i],
                propagator.advance(
                    orbits.get(filtered[i]),
                    time_offset
                )); // TODO: add logic to consider time offsets in the orbit itself
        }
    }

    void update(
        const TrackPositions& updates,
        const vec3s& tree,
        vec3s& updated
    ) const {
        updated.reserve(subsamples.size());
        updated.clear();
        if(&updated != &tree) {
            for (std::size_t i = 0; i < tree.size(); ++i) {
                updated[i] = tree[i];
            }
        }
        for (std::size_t i = 0; i < updates.size(); ++i) {
            updated[i] = updates[i].position;
        }
    }

    void update(
        const Resonances& resonances,
        const CycleSamples& samples,
        const CycleSamples& subsamples,
        CycleSamples& updated
    ) const {
        if(&updated != &subsamples) {
            updated.reserve(subsamples.size());
            updated.clear();
            for (std::size_t i = 0; i < subsamples.size(); ++i) {
                updated[i] = subsamples[i];
            }
        }
        for (std::size_t i = 0; i < samples.size(); ++i) {
            period = samples[resonances[i].resonance].period;
            subperiod = updated[resonances[i].node].period;
            updated[resonances[i].node] = 
                updated[resonances[i].node].with_time(samples[resonances[i].resonance].time());
        }
    }

}

/*
SⁿPᵐ→ℝ⁴⁴   return rotation of a spin for trajectories at phases
BⁿNᵐt→(Nℝ³)ᵐ    update positions in scene tree for trajectories at time

phase 2:
Pⁿ → Rᵐ       identify resonances by indices with sample counts
*/

