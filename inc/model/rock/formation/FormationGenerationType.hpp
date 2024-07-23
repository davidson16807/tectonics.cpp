#pragma once

#include "FormationGeneration.hpp"  // FormationGeneration

namespace rock {

    /* 
    `FormationGenerationType` represents types of `Formation` that can be generated procedurally 
    and therefore can be represented via `FormationGeneration`.

    `FormationGenerationType` maps the following:
             ElevationForPosition  ⟶  FormationGeneration
            (position ⟶ elevation) ⟶    (id ⟶ stratum)

    It can be thought of as a curried form of the `FormationGeneration` constructor
    */
    template<
        typename Grid,
        typename AgeForElevation,
        typename AreaDensityForElevation,
        typename ExtrusiveFractionForElevation>
    class FormationGenerationType {

        using id     = typename Grid::size_type;
        using scalar = typename Grid::value_type;

        static const int M = 2; // number of stored minerals

        const Grid                          grid;
        const AgeForElevation               age_for_elevation;
        const AreaDensityForElevation       mafic_area_density_for_elevation;
        const AreaDensityForElevation       felsic_area_density_for_elevation;
        const ExtrusiveFractionForElevation mafic_extrusive_fraction;
        const ExtrusiveFractionForElevation felsic_extrusive_fraction;

    public:

        constexpr explicit FormationGenerationType(
            const Grid&                          grid,
            const AgeForElevation&               age_for_elevation,
            const AreaDensityForElevation&       mafic_area_density_for_elevation,
            const AreaDensityForElevation&       felsic_area_density_for_elevation,
            const ExtrusiveFractionForElevation& felsic_extrusive_fraction,
            const ExtrusiveFractionForElevation& mafic_extrusive_fraction
        ):
            grid(grid),
            age_for_elevation(age_for_elevation),
            mafic_area_density_for_elevation  (mafic_area_density_for_elevation),
            felsic_area_density_for_elevation (felsic_area_density_for_elevation),
            mafic_extrusive_fraction (mafic_extrusive_fraction),
            felsic_extrusive_fraction(felsic_extrusive_fraction)
        {}

        template<typename ElevationForPosition>
        auto operator() (const ElevationForPosition& elevation_for_position) const
        {
            return rock::FormationGeneration(
                grid,
                elevation_for_position,
                age_for_elevation,
                mafic_area_density_for_elevation,
                felsic_area_density_for_elevation,
                felsic_extrusive_fraction,
                mafic_extrusive_fraction
            );
        }

    };

}

