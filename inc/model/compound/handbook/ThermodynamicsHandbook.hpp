#pragma once

// in-house libraries
#include <model/compound/correlation/published.hpp>
#include <model/compound/correlation/speculative.hpp>
#include <model/compound/table/FullTable.hpp>
#include <model/compound/table/PartialTable.hpp>
#include <model/compound/relation/gas/GasDensityStateRelation.hpp>
#include <model/compound/published/published.hpp>

namespace compound{
namespace estimated{

    using LatentHeatTemperatureRelation = published::LatentHeatTemperatureRelation;

    using GasDensityStateRelation = relation::GasDensityStateRelation;
    using GasDynamicViscosityStateRelation = published::GasDynamicViscosityStateRelation;
    using GasThermalConductivityStateRelation = published::GasThermalConductivityStateRelation;
    using GasHeatCapacityStateRelation = published::GasHeatCapacityStateRelation;

    using LiquidDensityTemperatureRelation = published::LiquidDensityTemperatureRelation;
    using LiquidDynamicViscosityTemperatureRelation = published::LiquidDynamicViscosityTemperatureRelation;
    using LiquidVaporPressureTemperatureRelation = published::LiquidVaporPressureTemperatureRelation;
    using LiquidThermalConductivityTemperatureRelation = published::LiquidThermalConductivityTemperatureRelation;
    using LiquidHeatCapacityTemperatureRelation = published::LiquidHeatCapacityTemperatureRelation;

    using SolidDensityTemperatureRelation = published::SolidDensityTemperatureRelation;
    using SolidVaporPressureTemperatureRelation = published::SolidVaporPressureTemperatureRelation;
    using SolidDynamicViscosityTemperatureRelation = published::SolidDynamicViscosityTemperatureRelation;
    using SolidHeatCapacityTemperatureRelation = published::SolidHeatCapacityTemperatureRelation;
    using SolidThermalConductivityTemperatureRelation = published::SolidThermalConductivityTemperatureRelation;

    /*
    `ThermodynamicsHandbook` serves the role of a chemical handbook that contain 
    phase based properties and other properties that are relevant to thermodynamic behavior
    */
    struct ThermodynamicsHandbook
    {
        table::FullTable<LiquidDensityTemperatureRelation> density_as_liquid;
        table::FullTable<GasDensityStateRelation> density_as_gas;
        table::FullTable<SolidDensityTemperatureRelation> density_as_solid;

        table::FullTable<si::temperature<double>> triple_point_temperature;
        table::FullTable<si::pressure<double>> triple_point_pressure;
        table::FullTable<point<double>> triple_point;

        table::FullTable<LatentHeatTemperatureRelation> latent_heat_of_vaporization;
        table::FullTable<LatentHeatTemperatureRelation> latent_heat_of_sublimation;
        table::FullTable<LatentHeatTemperatureRelation> latent_heat_of_fusion;

        table::FullTable<SolidHeatCapacityTemperatureRelation> isobaric_specific_heat_capacity_as_solid;
        table::FullTable<LiquidHeatCapacityTemperatureRelation> isobaric_specific_heat_capacity_as_liquid;
        table::FullTable<GasHeatCapacityStateRelation> isobaric_specific_heat_capacity_as_gas;

        table::FullTable<SolidDynamicViscosityTemperatureRelation> dynamic_viscosity_as_solid;
        table::FullTable<LiquidDynamicViscosityTemperatureRelation> dynamic_viscosity_as_liquid;
        table::FullTable<GasDynamicViscosityStateRelation> dynamic_viscosity_as_gas;

        table::FullTable<SolidVaporPressureTemperatureRelation> vapor_pressure_as_solid;
        table::FullTable<LiquidVaporPressureTemperatureRelation> vapor_pressure_as_liquid;

        table::FullTable<SolidThermalConductivityTemperatureRelation> thermal_conductivity_as_solid;
        table::FullTable<LiquidThermalConductivityTemperatureRelation> thermal_conductivity_as_liquid;
        table::FullTable<GasThermalConductivityStateRelation> thermal_conductivity_as_gas;

        table::FullTable<published::LiquidSurfaceTensionTemperatureRelation> surface_tension_as_liquid;

        ThermodynamicsHandbook(
            const table::PartialTable<point<double>>& triple_point0,
            const table::PartialTable<published::LatentHeatTemperatureRelation>& latent_heat_of_sublimation0,
            const table::PartialTable<published::LatentHeatTemperatureRelation>& latent_heat_of_fusion0,
            const table::PartialTable<published::LatentHeatTemperatureRelation>& latent_heat_of_vaporization0,
            const table::PartialTable<published::SolidDensityTemperatureRelation>&  density_as_solid0,
            const table::PartialTable<published::LiquidDensityTemperatureRelation>&  density_as_liquid0,
            const table::PartialTable<published::SolidHeatCapacityTemperatureRelation>& isobaric_specific_heat_capacity_as_solid0,
            const table::PartialTable<published::LiquidHeatCapacityTemperatureRelation>& isobaric_specific_heat_capacity_as_liquid0,
            const table::PartialTable<published::GasHeatCapacityStateRelation>& isobaric_specific_heat_capacity_as_gas0,
            const table::PartialTable<published::SolidDynamicViscosityTemperatureRelation>& dynamic_viscosity_as_solid0,
            const table::PartialTable<published::LiquidDynamicViscosityTemperatureRelation>& dynamic_viscosity_as_liquid0,
            const table::PartialTable<published::GasDynamicViscosityStateRelation>& dynamic_viscosity_as_gas0,
            const table::PartialTable<published::SolidVaporPressureTemperatureRelation>& vapor_pressure_as_solid0,
            const table::PartialTable<published::LiquidVaporPressureTemperatureRelation>& vapor_pressure_as_liquid0,
            const table::PartialTable<published::SolidThermalConductivityTemperatureRelation>& thermal_conductivity_as_solid0,
            const table::PartialTable<published::LiquidThermalConductivityTemperatureRelation>& thermal_conductivity_as_liquid0,
            const table::PartialTable<published::GasThermalConductivityStateRelation>& thermal_conductivity_as_gas0,
            const table::PartialTable<published::LiquidSurfaceTensionTemperatureRelation>& surface_tension_as_liquid0,
            const MolecularPropertiesHandbook& molecular,
            const CriticalPropertiesHandbook& critical,
            const SamplePointHandbook& sample,
            const BasicHandbook& basic,
            const table::FullTable<int>& canonical_polymorph,
            const table::FullTable<int>& polymorph_compound,
            const table::FullTable<int>& compound_similarity,
            const table::FullTable<int>& polymorph_similarity,
            const table::FullTable<int>& polymorph_compound_similarity,
            const int fallback_id
        ){

            // via Klincewicz ⟶ Ihmels ⟶ Tee-Gotoh-Steward ⟶ Bird-Steward-Lightfoot2
            density_as_liquid = 
                table::complete(density_as_liquid0, 
                    table::derive<LiquidDensityTemperatureRelation>(
                        [](si::molar_mass<double> M, si::length<double> sigma){
                            return LiquidDensityTemperatureRelation(M / correlation::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma));}, 
                        basic.molar_mass, molecular.diameter));

            // via Klincewicz ⟶ Ihmels ⟶ definition of compressibility ⟶ definition of compressibility
            density_as_gas = 
                table::derive<GasDensityStateRelation>(
                    [](si::molar_mass<double> M, si::pressure<double> pc, si::temperature<double> Tc, double Zc){
                        return GasDensityStateRelation(M,pc,Tc,Zc);}, 
                    basic.molar_mass, 
                    critical.pressure, 
                    critical.temperature, 
                    critical.compressibility);

            // via Klincewicz ⟶ Ihmels ⟶ Tee-Gotoh-Steward ⟶ Bird-Steward-Lightfoot2 ⟶ Goodman (with speculative fallback)
            density_as_solid = 
                table::complete(
                    table::first<SolidDensityTemperatureRelation>({
                        table::imitate(density_as_solid0, polymorph_similarity), 
                        table::gather<SolidDensityTemperatureRelation>([](si::molar_mass<double> M, LiquidDensityTemperatureRelation rhoL, point<double> liquid, point<double> triple){
                                return SolidDensityTemperatureRelation(M / correlation::estimate_molar_volume_as_solid_from_goodman(M / rhoL(liquid.temperature), liquid.temperature, triple.temperature));}, 
                            table::partial(basic.molar_mass), 
                            table::partial(density_as_liquid), 
                            table::partial(sample.liquid_point), 
                            triple_point0),
                    }),
                    table::derive<SolidDensityTemperatureRelation>(
                        [](LiquidDensityTemperatureRelation rhoL, point<double> liquid){
                            return correlation::guess_density_as_solid_from_density_as_liquid(rhoL(liquid.temperature));}, 
                        density_as_liquid, 
                        sample.liquid_point) // WARNING: results are speculative
                )[polymorph_compound];

            triple_point_temperature = 
                table::complete(
                    table::gather<si::temperature<double>>([](point<double> triple_point){ return triple_point.temperature; },
                        triple_point0),
                    table::derive<si::temperature<double>>([](
                            si::molar_mass<double> M, 
                            LiquidDensityTemperatureRelation rhoL, 
                            SolidDensityTemperatureRelation rhoS, 
                            point<double> liquid
                        ){
                            si::temperature<double> T = liquid.temperature;
                            return si::temperature<double>(correlation::estimate_triple_point_temperature_from_goodman(M / rhoL(T), M / rhoS(T), T));
                        },
                        basic.molar_mass,
                        density_as_liquid,
                        density_as_solid[canonical_polymorph],
                        sample.liquid_point)
                );

            auto triple_point_pressure1 = 
                table::first<si::pressure<double>>({
                        table::gather<si::pressure<double>>([](point<double> triple_point){ return triple_point.pressure; },
                            triple_point0),
                        table::gather<si::pressure<double>>([](
                                published::LatentHeatTemperatureRelation Hs, 
                                si::molar_mass<double> M, 
                                point<double> solid, 
                                si::temperature<double> Tt,
                                SolidVaporPressureTemperatureRelation PvS
                            ){
                                si::temperature<double> Ts = solid.temperature;
                                return correlation::estimate_triple_point_pressure_from_clapeyron(Hs(Ts), M, Ts, Tt, PvS(Ts));
                            },
                            latent_heat_of_sublimation0,
                            table::partial(basic.molar_mass),
                            table::partial(sample.solid_point),
                            table::partial(triple_point_temperature),
                            vapor_pressure_as_solid0[canonical_polymorph])
                    });

            auto triple_point1 =
                table::gather<point<double>>([](
                        si::pressure<double> Pt,
                        si::temperature<double> Tt
                    ){
                        return point<double>(Pt, Tt);
                    },
                    triple_point_pressure1,
                    table::partial(triple_point_temperature)
                );

            // CALCULATE PROPERTIES THAT CAN BE DERIVED FROM ACENTRIC FACTOR

            isobaric_specific_heat_capacity_as_gas = 
                table::complete(
                    isobaric_specific_heat_capacity_as_gas0,
                    table::derive<GasHeatCapacityStateRelation>(
                        correlation::get_isobaric_heat_capacity_as_gas,
                        basic.molar_mass,
                        molecular.degrees_of_freedom
                    )
                );

            /* 
            TODO: adapt correlation::estimate_isobaric_heat_capacity_as_gas_from_rowlinson_poling
             for use with GasPropertyStateRelations using Dippr119Terms, then invoke results here
            */

            latent_heat_of_vaporization = 
                table::complete(
                    latent_heat_of_vaporization0,
                    table::derive<LatentHeatTemperatureRelation>(
                        relation::get_pitzer_latent_heat_of_vaporization_temperature_relation,
                        basic.molar_mass, 
                        critical.temperature, 
                        molecular.acentric_factor
                    )
                );

            auto latent_heat_of_sublimation1 = 
                table::first<LatentHeatTemperatureRelation>({
                    latent_heat_of_sublimation0,
                    table::partial(latent_heat_of_vaporization) + latent_heat_of_fusion0
                });

            dynamic_viscosity_as_liquid = 
                table::complete(
                    dynamic_viscosity_as_liquid0,
                    table::derive<LiquidDynamicViscosityTemperatureRelation>(
                        relation::estimate_viscosity_as_liquid_from_letsou_stiel,
                        molecular.acentric_factor,
                        basic.molar_mass,
                        critical.temperature,
                        critical.pressure
                    )
                );

            auto vapor_pressure_as_liquid1 = 
                table::first<LiquidVaporPressureTemperatureRelation>({
                    vapor_pressure_as_liquid0,
                    // table::gather<LiquidVaporPressureTemperatureRelation>(
                    //     relation::estimate_vapor_pressure_as_liquid_from_lee_kesler,
                    //     table::partial(molecular.acentric_factor),
                    //     table::partial(critical.temperature),
                    //     table::partial(critical.pressure)
                    // ),
                });

            auto vapor_pressure_as_solid1 = 
                table::first<SolidVaporPressureTemperatureRelation>({
                    table::imitate(vapor_pressure_as_solid0, polymorph_similarity),
                    table::gather<SolidVaporPressureTemperatureRelation>(
                        []( LatentHeatTemperatureRelation Hs, 
                            si::molar_mass<double> M, 
                            point<double> triple){
                                auto T = math::Identity<float>();
                                auto R = si::universal_gas_constant;
                                float T3 = triple.temperature / si::kelvin;
                                float P3 = triple.pressure / si::pascal;
                                float k = (Hs(triple.temperature)*M/R)/si::kelvin;
                                using Polynomial = math::Polynomial<float,-1,1>;
                                using Clamped = math::Clamped<float,Polynomial>;
                                using Railyard = math::Railyard<float,Polynomial>;
                                return P3 * SolidVaporPressureTemperatureRelation(
                                    Railyard(Clamped(0.0f, T3, Polynomial(-k*(1.0f/T - 1.0f/T3)))),
                                    si::kelvin, si::pascal);
                                // NOTE: the above is translated from property/published.hpp:
                                // return triple_point_pressure * exp(-((latent_heat_of_sublimation*molar_mass / si::universal_gas_constant) * (1.0/temperature - 1.0/triple_point_temperature)));
                        },
                        latent_heat_of_sublimation1, // NOTE: this should be latent heat of sublimation at the triple point, update this line if ever we convert latent_heat_of_fusion to a relation
                        table::partial(basic.molar_mass), 
                        triple_point1)[polymorph_compound]
                });

            // CALCULATE MISCELLANEOUS PROPERTIES
            thermal_conductivity_as_liquid = 
                table::complete(
                    thermal_conductivity_as_liquid0,
                    table::derive<LiquidThermalConductivityTemperatureRelation>(
                        relation::estimate_thermal_conductivity_as_liquid_from_sheffy_johnson,
                        basic.molar_mass, 
                        sample.freezing_point_temperature,
                        critical.temperature
                    )
                );

            dynamic_viscosity_as_solid = 
                table::complete(
                    table::imitate(dynamic_viscosity_as_solid0, polymorph_similarity),
                    table::derive<SolidDynamicViscosityTemperatureRelation>(
                        []( LiquidDynamicViscosityTemperatureRelation nuL,
                            point<double> liquid){
                            return SolidDynamicViscosityTemperatureRelation(
                                correlation::guess_viscosity_as_solid_from_viscosity_as_liquid(nuL(liquid.temperature)));
                            // WARNING: results above are speculative, 
                            // see property/speculative.hpp for reasoning and justification
                        },
                        dynamic_viscosity_as_liquid,
                        sample.liquid_point
                    )[polymorph_compound]
                );

            surface_tension_as_liquid                 = table::complete(table::imitate(surface_tension_as_liquid0,                  compound_similarity), surface_tension_as_liquid0                 [fallback_id],  compound_similarity.size());
            triple_point_pressure                     = table::complete(table::imitate(triple_point_pressure1,                      compound_similarity), triple_point_pressure1                     [fallback_id],  compound_similarity.size());
            triple_point                              = table::complete(table::imitate(triple_point1,                               compound_similarity), triple_point1                              [fallback_id],  compound_similarity.size());
            latent_heat_of_sublimation                = table::complete(table::imitate(latent_heat_of_sublimation1,                 compound_similarity), latent_heat_of_sublimation1                [fallback_id],  compound_similarity.size());
            latent_heat_of_fusion                     = table::complete(table::imitate(latent_heat_of_fusion0,                      compound_similarity), latent_heat_of_fusion0                     [fallback_id],  compound_similarity.size());
            isobaric_specific_heat_capacity_as_liquid = table::complete(table::imitate(isobaric_specific_heat_capacity_as_liquid0,  compound_similarity), isobaric_specific_heat_capacity_as_liquid0 [fallback_id],  compound_similarity.size());
            dynamic_viscosity_as_gas                  = table::complete(table::imitate(dynamic_viscosity_as_gas0,                   compound_similarity), dynamic_viscosity_as_gas0                  [fallback_id],  compound_similarity.size());
            vapor_pressure_as_liquid                  = table::complete(table::imitate(vapor_pressure_as_liquid1,                   compound_similarity), vapor_pressure_as_liquid1                  [fallback_id],  compound_similarity.size());
            thermal_conductivity_as_gas               = table::complete(table::imitate(thermal_conductivity_as_gas0,                compound_similarity), thermal_conductivity_as_gas0               [fallback_id],  compound_similarity.size());

            thermal_conductivity_as_solid             = table::complete(table::imitate(thermal_conductivity_as_solid0,             polymorph_compound_similarity), thermal_conductivity_as_solid0             [fallback_id], polymorph_compound_similarity.size());
            isobaric_specific_heat_capacity_as_solid  = table::complete(table::imitate(isobaric_specific_heat_capacity_as_solid0,  polymorph_compound_similarity), isobaric_specific_heat_capacity_as_solid0  [fallback_id], polymorph_compound_similarity.size());
            vapor_pressure_as_solid                   = table::complete(table::imitate(vapor_pressure_as_solid1,                   polymorph_compound_similarity), vapor_pressure_as_solid1                   [fallback_id], polymorph_compound_similarity.size());

        }
    };

}}

