#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries
#include <model/rock/stratum/StratumStore.hpp>

namespace rock
{

    template<std::size_t M>
    using Formation = std::vector<StratumStore<M>>;

}

