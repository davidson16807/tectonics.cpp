#pragma once

#include <array>   // array

namespace sea
{
    /*
    "LiquidMassPools" is a data structure that holds the mass pools of all gases found on or within a world.
    */
    template<int M>
    using LiquidMassPools = std::array<float, M>  mass_pools;
}