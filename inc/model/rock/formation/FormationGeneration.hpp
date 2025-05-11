#pragma once

#include <model/rock/stratum/StratumStore.hpp>
#include <model/rock/stratum/Stratum.hpp>
#include <model/rock/stratum/StratumOps.hpp>

namespace rock {

    /* 
    `FormationGeneration` maps: id ⟶ stratum

    It accomplishes this by traversing the following diagram:

    id ⟶ position ⟶ elevation 
       ↘            ↑
         area     ← Π → stratum

    */
    template<typename Grid, typename StratumPerAreaForPosition>
    class FormationGeneration {

        using id     = typename Grid::size_type;
        using scalar = typename Grid::value_type;

        static const int M = 2; // number of stored minerals

        const Grid grid;
        const StratumPerAreaForPosition  stratum_per_area_for_position;
        const StratumOps<M> ops;

    public:

        constexpr explicit FormationGeneration(
            const Grid& grid,
            const StratumPerAreaForPosition& stratum_per_area_for_position
        ):
            grid(grid),
            stratum_per_area_for_position(stratum_per_area_for_position),
            ops()
        {}

        using size_type = std::size_t; 
        using value_type = StratumStore<M>; 

        constexpr inline size_type size() const { return grid.vertex_count(); } 
        auto operator[] (const id vertex_id) const
        {
            auto stratum = stratum_per_area_for_position(grid.vertex_position(vertex_id));
            ops.scale(stratum, grid.vertex_dual_area(vertex_id), stratum);
            return stratum;
        }

    };

}

