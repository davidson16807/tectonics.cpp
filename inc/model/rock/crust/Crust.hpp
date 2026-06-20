#pragma once

// C libraries
#include <cstddef> // std::size_t

// std libraries
#include <array>   // std::array

// 3rd party libraries

// in-house libraries
#include <model/rock/formation/Formation.hpp>

namespace rock
{

    template<std::size_t mineral_count, std::size_t formation_count>
    using Crust = std::array<Formation<mineral_count>,formation_count>;

}

