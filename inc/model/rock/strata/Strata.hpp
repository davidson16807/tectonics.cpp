#pragma once

// C libraries
#include <cstddef>

// std libraries
#include <array>

// 3rd party libraries

// in-house libraries
#include <model/rock/stratum/StratumStore.hpp>

namespace rock
{

    template<std::size_t mineral_count, std::size_t formation_count>
    using Strata = std::array<StratumStore<mineral_count>,formation_count>;

}

