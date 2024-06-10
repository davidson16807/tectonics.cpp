#pragma once

#include <array>   // array

namespace air
{
    /*
    "Air" is a data structure that holds the mass pools of all gases found on or within a world.
    */
    template<int M>
    using Air = std::array<float, M>;
}