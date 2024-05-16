#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries

namespace rock
{

    template<std::size_t M>
    using Crust = std::array<Formation<M>,FormationTypes::count>;

}

