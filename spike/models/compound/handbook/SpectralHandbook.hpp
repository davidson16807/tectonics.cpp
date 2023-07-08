#pragma once

// in-house libraries
#include <models/compound/correlation/reflectance.hpp>
#include <models/compound/table/FullTable.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound{
namespace estimated{

    using GasRefractiveIndexWavenumberRelation = published::GasRefractiveIndexWavenumberRelation;
    // using GasExtinctionCoefficientWavenumberRelation = published::GasExtinctionCoefficientWavenumberRelation;
    // using GasAbsorptionCoefficientWavenumberRelation = published::GasAbsorptionCoefficientWavenumberRelation;

    using LiquidRefractiveIndexWavenumberRelation = published::LiquidRefractiveIndexWavenumberRelation;
    // using LiquidExtinctionCoefficientWavenumberRelation = published::LiquidExtinctionCoefficientWavenumberRelation;
    // using LiquidAbsorptionCoefficientWavenumberRelation = published::LiquidAbsorptionCoefficientWavenumberRelation;

    using SolidRefractiveIndexWavenumberRelation = published::SolidRefractiveIndexWavenumberRelation;
    using SolidExtinctionCoefficientWavenumberRelation = published::SolidExtinctionCoefficientWavenumberRelation;
    using SolidAbsorptionCoefficientWavenumberRelation = published::SolidAbsorptionCoefficientWavenumberRelation;

    struct SpectralHandbook
    {
        table::FullTable<SolidRefractiveIndexWavenumberRelation> refractive_index;
        table::FullTable<SolidExtinctionCoefficientWavenumberRelation> extinction_coefficient;
        table::FullTable<SolidAbsorptionCoefficientWavenumberRelation> absorption_coefficient;

        SpectralHandbook(
            const table::PartialTable<SolidRefractiveIndexWavenumberRelation> n,
            const table::PartialTable<SolidExtinctionCoefficientWavenumberRelation> k,
            const table::PartialTable<SolidAbsorptionCoefficientWavenumberRelation> alpha,
            const table::FullTable<int> similarity,
            const int fallback_id
        ){

            using Absorption = SolidAbsorptionCoefficientWavenumberRelation;
            using Index = SolidRefractiveIndexWavenumberRelation;

            auto alpha2 = table::first<Absorption>({ alpha,
                    table::gather<Absorption>(
                        []( Index n,
                            Index k){
                            return Absorption(
                                [=](si::wavenumber<double> w){return correlation::get_absorption_coefficient_from_refractive_index(n(w), k(w), 1.0/w);});
                            },
                        n, k ) 
                    });

            auto n2 = table::first<Index>({ n,
                    table::gather<Index>(
                        []( Index k,
                            Absorption alpha){
                            return Index(
                                [=](si::wavenumber<double> w){return correlation::get_refractive_index_from_absorption_coefficient(alpha(w), k(w), 1.0/w);});
                            },
                        k, alpha ) 
                    });

            auto k2 = table::first<Index>({ n,
                    table::gather<Index>(
                        []( Index n,
                            Absorption alpha){
                            return Index(
                                [=](si::wavenumber<double> w){return correlation::get_extinction_coefficient_from_absorption_coefficient(alpha(w), n(w), 1.0/w);});
                            },
                        n, alpha ) 
                    });

            refractive_index       = table::complete(table::imitate(n2,     similarity), n2     [fallback_id], similarity.size());
            extinction_coefficient = table::complete(table::imitate(k2,     similarity), k2     [fallback_id], similarity.size());
            absorption_coefficient = table::complete(table::imitate(alpha2, similarity), alpha2 [fallback_id], similarity.size());

        }
    };

}}

