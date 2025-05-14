#pragma once

#include <model/rock/stratum/StratumStore.hpp>
#include <model/rock/stratum/Stratum.hpp>
#include <model/rock/stratum/StratumOps.hpp>

namespace rock {

    /* 
    `FormationField` curries the map: (vec3 → stratum) → (grid×id ⟶ stratum)

    It gets the position for a vertex_id, 
    maps the position to a stratum using the vec3→stratum field that was passed to the constructor,
    then and scales the stratum based on the vertex_dual_area for that vertex_id
    */
    template<typename StratumPerAreaForPosition>
    class FormationField {

        static const int M = 2; // number of stored minerals

        const StratumPerAreaForPosition stratum_per_area_for_position;
        const StratumOps<M> ops;

    public:

        constexpr explicit FormationField(
            const StratumPerAreaForPosition& stratum_per_area_for_position
        ):
            stratum_per_area_for_position(stratum_per_area_for_position),
            ops()
        {}

        using size_type = std::size_t; 
        using value_type = Stratum<M>; 

        template<typename Grid, typename id>
        auto operator() (const Grid& grid, const id vertex_id) const
        {
            auto stratum = stratum_per_area_for_position(grid.vertex_position(vertex_id));
            ops.scale(stratum, grid.vertex_dual_area(vertex_id), stratum);
            return stratum;
        }

    };

}

