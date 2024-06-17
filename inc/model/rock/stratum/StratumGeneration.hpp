#pragma once

#include "StratumStore.hpp"
#include "Stratum.hpp"

namespace rock {

    /* 
    `StratumGeneration` maps: id ⟶ stratum

    It accomplishes this by traversing the following diagram:

                              ↗ felsic_area_density 
    id ⟶ position ⟶ elevation → age                 
       ↘ area                 ↘ mafic_area_density  

    where arrows are provided by constructor parameters
    */
    template<
        typename Grid,
        typename ElevationForPosition,
        typename AgeForElevation,
        typename AreaDensityForElevation,
        typename ExtrusiveFractionForElevation>
    class StratumGeneration {

        using id     = typename Grid::size_type;
        using scalar = typename Grid::value_type;

        const Grid                          grid;
        const ElevationForPosition          elevation_for_position;
        const AgeForElevation               age_for_elevation;
        const AreaDensityForElevation       mafic_area_density_for_elevation;
        const AreaDensityForElevation       felsic_area_density_for_elevation;
        const ExtrusiveFractionForElevation mafic_extrusive_fraction;
        const ExtrusiveFractionForElevation felsic_extrusive_fraction;

    public:

        constexpr explicit StratumGeneration(
            const Grid&                           grid,
            const ElevationForPosition           elevation_for_position, // scalars, may be any unit but must agree with other parameter functions
            const AgeForElevation&               age_for_elevation,
            const AreaDensityForElevation&       mafic_area_density_for_elevation,
            const AreaDensityForElevation&       felsic_area_density_for_elevation,
            const ExtrusiveFractionForElevation& felsic_extrusive_fraction,
            const ExtrusiveFractionForElevation& mafic_extrusive_fraction
        ):
            grid(grid),
            elevation_for_position(elevation_for_position),
            age_for_elevation(age_for_elevation),
            mafic_area_density_for_elevation  (mafic_area_density_for_elevation),
            felsic_area_density_for_elevation (felsic_area_density_for_elevation),
            mafic_extrusive_fraction (mafic_extrusive_fraction),
            felsic_extrusive_fraction(felsic_extrusive_fraction)
        {}

        using size_type = std::size_t; 
        using value_type = StratumStore<2>; 

        constexpr inline size_type size() const { return grid.vertex_count(); } 
        constexpr value_type operator[] (const id vertex_id) const
        {
            auto area = grid.vertex_dual_area(vertex_id) * si::meter2;
            auto elevation = elevation_for_position(grid.vertex_position(vertex_id)) * si::meter;
            auto age = age_for_elevation(elevation);
            auto mafic_extrusive_fraction_ = mafic_extrusive_fraction (elevation);
            auto felsic_extrusive_fraction_ = felsic_extrusive_fraction (elevation);
            Mineral mafic (mafic_area_density_for_elevation (elevation) * area);
            Mineral felsic(felsic_area_density_for_elevation(elevation) * area);
            mafic .grain_type_relative_volume[int(GrainType::unweathered_extrusive)] =              mafic_extrusive_fraction_;
            felsic.grain_type_relative_volume[int(GrainType::unweathered_extrusive)] =             felsic_extrusive_fraction_;
            mafic .grain_type_relative_volume[int(GrainType::unweathered_intrusive)] = scalar(1) -  mafic_extrusive_fraction_;
            felsic.grain_type_relative_volume[int(GrainType::unweathered_intrusive)] = scalar(1) - felsic_extrusive_fraction_;
            return StratumStore<2>(
                Stratum<2>(age, age, std::vector<Mineral>{mafic, felsic})
            );
        }

    };

}

