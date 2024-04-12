#pragma once

#include "StratumStore.hpp"
#include "Stratum.hpp"

namespace rock {

    /* 
    `StratumGenerator`
    */
    template<
        typename scalar, 
        typename MaficMass,
        typename FelsicMass,
        typename MaficExtrusiveFraction,
        typename FelsicExtrusiveFraction>
    struct StratumGenerator {

        MaficMass               mafic_mass;
        FelsicMass              felsic_mass;
        MaficExtrusiveFraction  felsic_extrusive_fraction;
        FelsicExtrusiveFraction mafic_extrusive_fraction;

        constexpr explicit StratumGenerator(
            const MaficMass&               mafic_mass,
            const FelsicMass&              felsic_mass,
            const MaficExtrusiveFraction&  felsic_extrusive_fraction,
            const FelsicExtrusiveFraction& mafic_extrusive_fraction
        ):
            mafic_mass  (mafic_mass),
            felsic_mass (felsic_mass),
            felsic_extrusive_fraction(felsic_extrusive_fraction),
            mafic_extrusive_fraction (mafic_extrusive_fraction)
        {}
        constexpr StratumStore<2> operator()(const scalar elevation_in_meters) const
        {
            Mineral mafic (mafic_mass (elevation_in_meters));
            Mineral felsic(felsic_mass(elevation_in_meters));
            mafic_mass .grain_type_relative_volume[int(GrainType::nonmetamorphic_unweathered_extrusive)] =             mafic_extrusive_fraction (elevation_in_meters);
            felsic_mass.grain_type_relative_volume[int(GrainType::nonmetamorphic_unweathered_extrusive)] =             felsic_extrusive_fraction(elevation_in_meters);
            mafic_mass .grain_type_relative_volume[int(GrainType::nonmetamorphic_unweathered_intrusive)] = scalar(1) - mafic_extrusive_fraction (elevation_in_meters);
            felsic_mass.grain_type_relative_volume[int(GrainType::nonmetamorphic_unweathered_intrusive)] = scalar(1) - felsic_extrusive_fraction(elevation_in_meters);
            return StratumStore<2>(
                Stratum<2>(
                    0.0*si::kelvin, 0.0*si::pascal,
                    0.0*si::year,   0.0*si::year,
                    std::vector<Mineral>{mafic, felsic}
                ));
        }
    };

}

