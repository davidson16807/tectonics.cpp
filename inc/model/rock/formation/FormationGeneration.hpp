#pragma once

#include <model/rock/stratum/StratumStore.hpp>
#include <model/rock/stratum/Stratum.hpp>

namespace rock {

    /* 
    `FormationGeneration` maps: id ⟶ stratum

    It accomplishes this by traversing the following diagram:

    id ⟶ position ⟶ elevation 
       ↘            ↑
         area     ← Π → stratum

    */
    template<
        typename Grid,
        typename ElevationForPosition,
        typename StratumForAreaElevation,
        typename length>
    class FormationGeneration {

        using id     = typename Grid::size_type;
        using scalar = typename Grid::value_type;

        static const int M = 2; // number of stored minerals

        const Grid                     grid;
        const ElevationForPosition     elevation_for_position;
        const StratumForAreaElevation  stratum_for_area_elevation;
        const length radius_units;

    public:

        constexpr explicit FormationGeneration(
            const Grid&                    grid,
            const ElevationForPosition&    elevation_for_position, 
            const StratumForAreaElevation& stratum_for_area_elevation,
            const length world_radius
        ):
            grid(grid),
            elevation_for_position(elevation_for_position),
            stratum_for_area_elevation(stratum_for_area_elevation),
            radius_units(world_radius/grid.voronoi.radius)
        {}

        using size_type = std::size_t; 
        using value_type = StratumStore<M>; 

        constexpr inline size_type size() const { return grid.vertex_count(); } 
        auto operator[] (const id vertex_id) const
        {
            return stratum_for_area_elevation(
                grid.vertex_dual_area(vertex_id) * radius_units * radius_units,
                elevation_for_position(grid.vertex_position(vertex_id))
            );
        }

    };

}

