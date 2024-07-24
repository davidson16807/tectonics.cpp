#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries

namespace rock
{

    template<std::size_t mineral_count, std::size_t formation_count>
    using Crust = std::array<Formation<mineral_count>,formation_count>;

}

