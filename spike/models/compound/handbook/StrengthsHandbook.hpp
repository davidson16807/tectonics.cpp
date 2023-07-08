#pragma once

// in-house libraries
#include <models/compound/correlation/strength.hpp>
#include <models/compound/table/FullTable.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound{
namespace estimated{

    using SolidShearYieldStrengthTemperatureRelation = published::SolidShearYieldStrengthTemperatureRelation;
    using SolidTensileYieldStrengthTemperatureRelation = published::SolidTensileYieldStrengthTemperatureRelation;
    using SolidCompressiveYieldStrengthTemperatureRelation = published::SolidCompressiveYieldStrengthTemperatureRelation;

    using SolidShearFractureStrengthTemperatureRelation = published::SolidShearFractureStrengthTemperatureRelation;
    using SolidTensileFractureStrengthTemperatureRelation = published::SolidTensileFractureStrengthTemperatureRelation;
    using SolidCompressiveFractureStrengthTemperatureRelation = published::SolidCompressiveFractureStrengthTemperatureRelation;

    struct StrengthsHandbook
    {
        table::FullTable<SolidShearYieldStrengthTemperatureRelation> shear_yield;
        table::FullTable<SolidTensileYieldStrengthTemperatureRelation> tensile_yield;
        table::FullTable<SolidCompressiveYieldStrengthTemperatureRelation> compressive_yield;
        table::FullTable<SolidShearFractureStrengthTemperatureRelation> shear_fracture;
        table::FullTable<SolidTensileFractureStrengthTemperatureRelation> tensile_fracture;
        table::FullTable<SolidCompressiveFractureStrengthTemperatureRelation> compressive_fracture;

        StrengthsHandbook(
            const table::PartialTable<SolidShearYieldStrengthTemperatureRelation> Gyield,
            const table::PartialTable<SolidTensileYieldStrengthTemperatureRelation> Eyield,
            const table::PartialTable<SolidCompressiveYieldStrengthTemperatureRelation> Cyield,
            const table::PartialTable<SolidShearFractureStrengthTemperatureRelation> Gfracture,
            const table::PartialTable<SolidTensileFractureStrengthTemperatureRelation> Efracture,
            const table::PartialTable<SolidCompressiveFractureStrengthTemperatureRelation> Cfracture,
            const table::FullTable<int> similarity,
            const int fallback_id
        ){

            // We can correlate tensile and shear yield strengths using the Von Misces Theorem.
            // If none of the yield strengths are known and cannot be derived, the most common explanation 
            // is that the material is too brittle for a distinct yield strength to be measured,
            // If this is the case then yield strength is equal to the fracture strength equivalent,
            // so this is what we assume as our fallback value.
            // Even if this assumption does not hold, the fracture strength 
            // is still the most plausible estimate given no other information

            // We reuse abbreviations for modulii to represent yield strengths

            using Strength = relation::PolynomialRailyardRelation<si::temperature<double>, si::pressure<double>, 0,1>;

            auto Gyield2 = table::first<Strength>({
                Gyield,
                table::gather<Strength>(
                    []( Strength E){ return E / sqrt(3.0); },
                    Eyield
                ),
                Gfracture
            });

            auto Eyield2 = table::first<Strength>({
                Eyield,
                table::gather<Strength>(
                    []( Strength G){ return G * sqrt(3.0); },
                    Gyield
                ),
                Efracture
            });

            auto Cyield2 = table::first<Strength>({
                Cyield, 
                Cfracture
            });

            shear_yield          = table::complete(table::imitate(Gyield2,   similarity), Gyield2   [fallback_id], similarity.size());
            tensile_yield        = table::complete(table::imitate(Eyield2,   similarity), Eyield2   [fallback_id], similarity.size());
            compressive_yield    = table::complete(table::imitate(Cyield2,   similarity), Cyield2   [fallback_id], similarity.size());
            shear_fracture       = table::complete(table::imitate(Gfracture, similarity), Gfracture [fallback_id], similarity.size());
            tensile_fracture     = table::complete(table::imitate(Efracture, similarity), Efracture [fallback_id], similarity.size());
            compressive_fracture = table::complete(table::imitate(Cfracture, similarity), Cfracture [fallback_id], similarity.size());

        }
    };

}}

