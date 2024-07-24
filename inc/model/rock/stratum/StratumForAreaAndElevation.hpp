#pragma once

#include <model/rock/stratum/StratumStore.hpp>
#include <model/rock/stratum/Stratum.hpp>

namespace rock {

    /* 
    `StratumForAreaAndElevation` maps: age × elevation ⟶ stratum

    It accomplishes this with the following maps:

              ↗ felsic_area_density 
    elevation → age                 
              ↘ mafic_area_density  

    where arrows are provided by constructor parameters
    */
    template<
        typename AgeForElevation,
        typename AreaDensityForElevation,
        typename ExtrusiveFractionForElevation>
    class StratumForAreaAndElevation {

        static const int M = 2; // number of stored minerals

        const AgeForElevation               age_for_elevation;
        const AreaDensityForElevation       mafic_area_density_for_elevation;
        const AreaDensityForElevation       felsic_area_density_for_elevation;
        const ExtrusiveFractionForElevation mafic_extrusive_fraction;
        const ExtrusiveFractionForElevation felsic_extrusive_fraction;

    public:

        constexpr explicit StratumForAreaAndElevation(
            const AgeForElevation&               age_for_elevation,
            const AreaDensityForElevation&       mafic_area_density_for_elevation,
            const AreaDensityForElevation&       felsic_area_density_for_elevation,
            const ExtrusiveFractionForElevation& mafic_extrusive_fraction,
            const ExtrusiveFractionForElevation& felsic_extrusive_fraction
        ):
            age_for_elevation(age_for_elevation),
            mafic_area_density_for_elevation  (mafic_area_density_for_elevation),
            felsic_area_density_for_elevation (felsic_area_density_for_elevation),
            mafic_extrusive_fraction (mafic_extrusive_fraction),
            felsic_extrusive_fraction(felsic_extrusive_fraction)
        {}

        using size_type = std::size_t; 
        using value_type = StratumStore<M>; 

        auto operator() (const si::area<double> area, const si::length<double> elevation) const
        {
            auto age = age_for_elevation(elevation);
            auto mafic_extrusive_fraction_ = mafic_extrusive_fraction (elevation);
            auto felsic_extrusive_fraction_ = felsic_extrusive_fraction (elevation);
            Mineral mafic (mafic_area_density_for_elevation (elevation) * area);
            Mineral felsic(felsic_area_density_for_elevation(elevation) * area);
            mafic .grain_type_relative_volume[int(GrainType::unweathered_extrusive)] =              mafic_extrusive_fraction_;
            felsic.grain_type_relative_volume[int(GrainType::unweathered_extrusive)] =             felsic_extrusive_fraction_;
            mafic .grain_type_relative_volume[int(GrainType::unweathered_intrusive)] = double(1) -  mafic_extrusive_fraction_;
            felsic.grain_type_relative_volume[int(GrainType::unweathered_intrusive)] = double(1) - felsic_extrusive_fraction_;
            Stratum<M> stratum(age, age, std::array<Mineral,M>{mafic, felsic});
            return StratumStore<M>(stratum);
        }

    };

}
