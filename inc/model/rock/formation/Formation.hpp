#pragma once

// C libraries
#include <cstddef> // std::size_t

// std libraries
#include <vector>  // std::vector

// 3rd party libraries

// in-house libraries
#include <model/rock/stratum/StratumStore.hpp>

namespace rock
{

    template<std::size_t mineral_count>
    using Formation = std::vector<StratumStore<mineral_count>>;

}

