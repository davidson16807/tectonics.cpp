#pragma once

#include <model/rock/stratum/Stratum.hpp>
#include <model/rock/stratum/StratumOps.hpp>

namespace rock {

    /* 
    `EmptyCrustField` populates the igneous formation of a crust with a `FormationField`
    */
    class EmptyCrustField {

        static const int M = 2; // number of stored minerals

    public:

        constexpr explicit EmptyCrustField()
        {}

        using size_type = std::size_t; 
        using value_type = Stratum<M>; 

        template<typename Grid, typename id>
        auto operator() (const Grid& grid, const std::size_t formation_id, const id vertex_id) const
        {
            return Stratum<M>{};
        }

    };

}

