#pragma once

// in house libraries

namespace rock{

    template <int mineral_count,int formation_count>
    using Lithosphere = std::vector<Crust<mineral_count,formation_count>>;

}