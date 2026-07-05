#pragma once

template<typename id, typename duration>
struct Resonance {

    const id resonance;    // resonance id
    const id sample_count; // how many samples are in this resonance
    const duration period; // period of the cycle for this node

    Resonance(
        const id resonance,
        const id sample_count,
        const duration period
    ): 
        resonance(resonance), 
        sample_count(sample_count),
        period(period)
    {}

};
