#pragma once

#include "StratumStore.hpp"
#include "Stratum.hpp"

namespace rock {

    /* 
    `StratumGenerator`
    */
    template<
        typename scalar, 
        typename AgeForElevation,
        typename MassForElevation,
        typename ExtrusiveFractionForElevation>
    struct StratumGenerator {

        const AgeForElevation               age;
        const MassForElevation              mafic_mass;
        const MassForElevation              felsic_mass;
        const ExtrusiveFractionForElevation felsic_extrusive_fraction;
        const ExtrusiveFractionForElevation mafic_extrusive_fraction;

        constexpr explicit StratumGenerator(
            const AgeForElevation&               age,
            const MassForElevation&              mafic_mass,
            const MassForElevation&              felsic_mass,
            const ExtrusiveFractionForElevation& mafic_extrusive_fraction,
            const ExtrusiveFractionForElevation& felsic_extrusive_fraction
        ):
            age(age),
            mafic_mass  (mafic_mass),
            felsic_mass (felsic_mass),
            felsic_extrusive_fraction(felsic_extrusive_fraction),
            mafic_extrusive_fraction (mafic_extrusive_fraction)
        {}
        constexpr StratumStore<2> operator()(const scalar elevation_in_meters) const
        {
            Mineral mafic (mafic_mass (elevation_in_meters));
            Mineral felsic(felsic_mass(elevation_in_meters));
            mafic_mass .grain_type_relative_volume[int(GrainType::unweathered_extrusive)] =             mafic_extrusive_fraction (elevation_in_meters);
            felsic_mass.grain_type_relative_volume[int(GrainType::unweathered_extrusive)] =             felsic_extrusive_fraction(elevation_in_meters);
            mafic_mass .grain_type_relative_volume[int(GrainType::unweathered_intrusive)] = scalar(1) - mafic_extrusive_fraction (elevation_in_meters);
            felsic_mass.grain_type_relative_volume[int(GrainType::unweathered_intrusive)] = scalar(1) - felsic_extrusive_fraction(elevation_in_meters);
            return StratumStore<2>(
                Stratum<2>(
                    0.0*si::kelvin, 0.0*si::pascal,
                    age(elevation_in_meters), age(elevation_in_meters),
                    std::vector<Mineral>{mafic, felsic}
                ));
        }
    };

}

