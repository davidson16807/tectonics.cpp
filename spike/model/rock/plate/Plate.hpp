#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries

namespace rock
{

    template<std::size_t M>
    struct Plate
    {
        Crust<M> crust;
        glm::mat<3,3> local_to_global;
        glm::mat<3,3> global_to_local;
    };

}

