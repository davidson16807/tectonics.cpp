#pragma once

// in-house libraries
#include <models/compound/correlation/published.hpp>
#include <models/compound/correlation/speculative.hpp>
#include <models/compound/table/FullTable.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/published/published.hpp>

namespace compound{
namespace estimated{

    /*
    `MolecularPropertiesHandbook` serves the role of a chemical handbook that contain 
    phase based properties and other properties that are relevant to thermodynamic behavior
    */
    struct MolecularPropertiesHandbook
    {
        table::FullTable<si::length<double>> diameter;
        table::FullTable<double> degrees_of_freedom;
        table::FullTable<double> acentric_factor;

        MolecularPropertiesHandbook(
            const table::PartialTable<si::length<double>>& diameter0,
            const table::PartialTable<double>& degrees_of_freedom0,
            const table::PartialTable<double>& acentric_factor0,
            const table::PartialTable<published::LiquidDynamicViscosityTemperatureRelation>& dynamic_viscosity_as_liquid0,
            const table::PartialTable<published::LatentHeatTemperatureRelation>& latent_heat_of_vaporization0,
            const table::PartialTable<published::LiquidVaporPressureTemperatureRelation>& vapor_pressure_as_liquid0,
            const CriticalPropertiesHandbook& critical,
            const SamplePointHandbook& sample,
            const BasicHandbook& basic
        ){

            // via Klincewicz ⟶ Ihmels ⟶ Tee-Gotoh-Steward
            diameter = 
                table::complete(diameter0,
                    table::derive<si::length<double>>(
                        [](si::temperature<double> T, si::pressure<double> P){ return correlation::estimate_molecular_diameter_from_tee_gotoh_steward(T,P); }, 
                        critical.temperature, 
                        critical.pressure));

            // CALCULATE ACENTRIC FACTOR

            acentric_factor = 
                table::complete(
                    table::first<double>({
                        acentric_factor0,
                        table::gather<double>(
                            []( point<double> critical,
                                published::LiquidVaporPressureTemperatureRelation Pv){
                                auto Tc = critical.temperature;
                                auto Pc = critical.pressure;
                                return correlation::get_acentric_factor(Pv(0.7*Tc), Pc);
                            },
                            table::partial(critical.point_),
                            vapor_pressure_as_liquid0
                        ),
                        table::gather<double>(
                            []( point<double> liquid,
                                point<double> critical,
                                si::molar_mass<double> M,
                                published::LatentHeatTemperatureRelation Hv){
                                auto T = si::clamp(liquid.temperature, si::absolute_zero, critical.temperature);
                                auto Tc = critical.temperature;
                                return std::clamp(correlation::estimate_acentric_factor_from_pitzer(Hv(T), M, T, Tc), -1.0, 1.0);
                            },
                            table::partial(sample.liquid_point),
                            table::partial(critical.point_),
                            table::partial(basic.molar_mass),
                            latent_heat_of_vaporization0
                        ),
                        table::gather<double>(
                            []( point<double> liquid,
                                point<double> critical,
                                si::molar_mass<double> M,
                                published::LiquidDynamicViscosityTemperatureRelation nuL){
                                auto T = liquid.temperature;
                                auto Tc = critical.temperature;
                                auto pc = critical.pressure;
                                return std::clamp(correlation::estimate_acentric_factor_from_letsou_stiel(nuL(T), M, T, Tc, pc), -1.0, 1.0);
                            },
                            table::partial(sample.liquid_point),
                            table::partial(critical.point_),
                            table::partial(basic.molar_mass),
                            dynamic_viscosity_as_liquid0
                        ),
                    }),
                    table::derive<double>(
                        correlation::estimate_acentric_factor_from_edmister,
                        sample.boiling_point_temperature,
                        critical.temperature,
                        critical.pressure
                    )
                );

            // CALCULATE PROPERTIES THAT CAN BE DERIVED FROM ACENTRIC FACTOR

            degrees_of_freedom = 
                table::complete(
                    degrees_of_freedom0,
                    table::derive<double>(
                        correlation::guess_molecular_degrees_of_freedom,
                        acentric_factor,
                        basic.atoms_per_molecule
                    )
                );

        }
    };

}}

