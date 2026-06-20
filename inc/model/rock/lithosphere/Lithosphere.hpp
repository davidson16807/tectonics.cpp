#pragma once

// C libraries
#include <cstddef> // std::size_t

// std libraries
#include <vector>  // std::vector

// in-house libraries
#include <model/rock/crust/Crust.hpp>

namespace rock{

    template <std::size_t mineral_count,std::size_t formation_count>
    using Lithosphere = std::vector<Crust<mineral_count,formation_count>>;

}