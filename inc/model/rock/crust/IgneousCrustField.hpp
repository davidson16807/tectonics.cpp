#pragma once

#include <model/rock/stratum/StratumStore.hpp>
#include <model/rock/stratum/Stratum.hpp>
#include <model/rock/stratum/StratumOps.hpp>

namespace rock {

    /* 
    `IgneousCrustField` populates the igneous formation of a crust with a `FormationField`
    */
    template<typename FormationField>
    class IgneousCrustField {

        static const int M = 2; // number of stored minerals

        const FormationField formation_field;
        const StratumOps<M> ops;

    public:

        constexpr explicit IgneousCrustField(
            const FormationField formation_field
        ):
            formation_field(formation_field),
            ops()
        {}

        using size_type = std::size_t; 
        using value_type = Stratum<M>; 

        template<typename Grid, typename id>
        auto operator() (const Grid& grid, const std::size_t formation_id, const id vertex_id) const
        {
            const Stratum<M> empty;
            return formation_id == formations::igneous? formation_field(grid,vertex_id) : empty;
        }

    };

}

