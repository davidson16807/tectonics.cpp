#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries

namespace rock
{

    template<std::size_t mineral_count, std::size_t formation_count>
    using Column = std::array<StratumStore<mineral_count>,formation_count>;

}

