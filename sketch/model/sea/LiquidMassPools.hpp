#pragma once

#include <array>   // array

namespace sea
{
    /*
    "LiquidMassPools" is a data structure that holds the mass pools of all gases found on or within a world.
    */
    template<std::size_t M>
    using LiquidMassPools = std::array<float, M>  mass_pools;
}