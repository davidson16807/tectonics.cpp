#pragma once

// std libraries
#include <vector> // std::vector

#include "CycleSample.hpp"

template<typename id, typename duration>
struct Cycle {

    const duration period; // period of the cycle for this node
    const id cycle;        // node or resonance of the cycle

    Cycle(
        const id cycle,
        const duration period
    ): 
        period(period),
        cycle(cycle)
    {}

};

/*
BⁿNᵐt→(Nℝ³)ᵐ   update positions in scene tree for trajectories at time
SⁿPᵐ→ℝ⁴⁴   return rotation of a spin for trajectories at phases

PⁿPⁿ → Pⁿ    overwrite sample phases with another where available

phase 2:
Pⁿ → Rᵐ       identify resonances by indices with sample counts
PⁿPᵐ → Pⁿ    update sample phases to represent resonances
*/

