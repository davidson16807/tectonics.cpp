#pragma once

// C libraries
#include <cmath>

// in-house libraries
#include <models/compound/published/published.hpp>
#include <models/compound/correlation/published.hpp>
#include <models/compound/correlation/speculative.hpp>
#include <models/compound/correlation/reflectance.hpp>
#include <models/compound/correlation/elasticity.hpp>
#include <models/compound/relation/gas/GasDensityStateRelation.hpp>
#include <models/compound/table/FullTable.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound{
namespace estimated{

    using LatentHeatTemperatureRelation = published::LatentHeatTemperatureRelation;

    using GasRefractiveIndexWavenumberRelation = published::GasRefractiveIndexWavenumberRelation;
    // using GasExtinctionCoefficientWavenumberRelation = published::GasExtinctionCoefficientWavenumberRelation;
    // using GasAbsorptionCoefficientWavenumberRelation = published::GasAbsorptionCoefficientWavenumberRelation;

    using GasDensityStateRelation = relation::GasDensityStateRelation;
    using GasDynamicViscosityStateRelation = published::GasDynamicViscosityStateRelation;
    using GasThermalConductivityStateRelation = published::GasThermalConductivityStateRelation;
    using GasHeatCapacityStateRelation = published::GasHeatCapacityStateRelation;

    using LiquidRefractiveIndexWavenumberRelation = published::LiquidRefractiveIndexWavenumberRelation;
    // using LiquidExtinctionCoefficientWavenumberRelation = published::LiquidExtinctionCoefficientWavenumberRelation;
    // using LiquidAbsorptionCoefficientWavenumberRelation = published::LiquidAbsorptionCoefficientWavenumberRelation;

    using LiquidDensityTemperatureRelation = published::LiquidDensityTemperatureRelation;
    using LiquidDynamicViscosityTemperatureRelation = published::LiquidDynamicViscosityTemperatureRelation;
    using LiquidVaporPressureTemperatureRelation = published::LiquidVaporPressureTemperatureRelation;
    using LiquidThermalConductivityTemperatureRelation = published::LiquidThermalConductivityTemperatureRelation;
    using LiquidHeatCapacityTemperatureRelation = published::LiquidHeatCapacityTemperatureRelation;

    using SolidRefractiveIndexWavenumberRelation = published::SolidRefractiveIndexWavenumberRelation;
    using SolidExtinctionCoefficientWavenumberRelation = published::SolidExtinctionCoefficientWavenumberRelation;
    using SolidAbsorptionCoefficientWavenumberRelation = published::SolidAbsorptionCoefficientWavenumberRelation;

    using SolidDensityTemperatureRelation = published::SolidDensityTemperatureRelation;
    using SolidVaporPressureTemperatureRelation = published::SolidVaporPressureTemperatureRelation;
    using SolidDynamicViscosityTemperatureRelation = published::SolidDynamicViscosityTemperatureRelation;

    using SolidShearYieldStrengthTemperatureRelation = published::SolidShearYieldStrengthTemperatureRelation;
    using SolidTensileYieldStrengthTemperatureRelation = published::SolidTensileYieldStrengthTemperatureRelation;
    using SolidCompressiveYieldStrengthTemperatureRelation = published::SolidCompressiveYieldStrengthTemperatureRelation;

    using SolidShearFractureStrengthTemperatureRelation = published::SolidShearFractureStrengthTemperatureRelation;
    using SolidTensileFractureStrengthTemperatureRelation = published::SolidTensileFractureStrengthTemperatureRelation;
    using SolidCompressiveFractureStrengthTemperatureRelation = published::SolidCompressiveFractureStrengthTemperatureRelation;

    using SolidBulkModulusTemperatureRelation = published::SolidBulkModulusTemperatureRelation;
    using SolidTensileModulusTemperatureRelation = published::SolidTensileModulusTemperatureRelation;
    using SolidLameParameterTemperatureRelation = published::SolidLameParameterTemperatureRelation;
    using SolidShearModulusTemperatureRelation = published::SolidShearModulusTemperatureRelation;
    using SolidPoissonRatioTemperatureRelation = published::SolidPoissonRatioTemperatureRelation;
    using SolidPwaveModulusTemperatureRelation = published::SolidPwaveModulusTemperatureRelation;

    // Refer to property/published.png to see the category that we are traversing here.
    // Critical point properties and molecular diameter are guaranteed to be inferrable, so start there.
    // After critical point properties, we want to get to acentric factor ("omega") as soon as possible, 
    // since that lets us calculate so many other things.
    // We want to make sure we have at least some route to estimate acentric factor,
    // So we first try to estimate all the things that correlate with acentric factor but are derived by other means.
    // This includes latent_heat_of_vaporization, vapor_pressure_as_liquid, 
    // dynamic_viscosity_as_liquid, and molar_volume_as_liquid.
    // Some of these properties can be found using triple point properties, 
    // Some of these properties can be found using the pressure/temperature at which measurements were collected,
    // and one property ("molar_volume_as_liquid") can be found using liquid or solid density.
    // 
    // So to summarize, the order we use to derive is: 
    //   properties that are already table::completed by published values
    //   critical point properties
    //   pressure/temperature samples
    //   triple_point_temperature and densities
    //   latent_heat_of_vaporization, vapor_pressure_as_liquid, dynamic_viscosity_as_liquid, molar_volume_as_liquid
    //   acentric factor
    //   properties derived by acentric factor
    //   miscellaneous properties


    auto name = published::name;
    auto formula = published::formula;
    
    auto atoms_per_molecule = published::atoms_per_molecule;
    auto freezing_sample_point = published::freezing_sample_point;
    auto boiling_sample_point = published::boiling_sample_point;
    auto molar_mass = published::molar_mass;

    // via Klincewicz
    table::FullTable<si::pressure<double>> critical_point_pressure = 
        table::complete(published::critical_point_pressure, 
            table::derive<si::pressure<double>>(correlation::estimate_critical_pressure_from_klincewicz, molar_mass, atoms_per_molecule));

    // via Klincewicz
    table::FullTable<si::molar_volume<double>> critical_point_volume = 
        table::complete(published::critical_point_volume, 
            table::derive<si::molar_volume<double>>(correlation::estimate_critical_molar_volume_from_klincewicz, molar_mass, atoms_per_molecule));

    // via Klincewicz ⟶ Ihmels
    table::FullTable<si::temperature<double>> critical_point_temperature = 
        table::complete(published::critical_point_temperature, 
            table::derive<si::temperature<double>>(correlation::estimate_critical_temperature_from_ihmels, critical_point_pressure, critical_point_volume));

    // via Klincewicz ⟶ Ihmels ⟶ definition of compressibility
    table::FullTable<double> critical_point_compressibility = 
        table::complete(published::critical_point_compressibility, 
            table::derive<double>(correlation::get_critical_compressibility, critical_point_pressure, critical_point_temperature, critical_point_volume));

    // via Klincewicz ⟶ Ihmels ⟶ Tee-Gotoh-Steward
    table::FullTable<si::length<double>> molecular_diameter = 
        table::complete(published::molecular_diameter, 
            table::derive<si::length<double>>(correlation::estimate_molecular_diameter_from_tee_gotoh_steward, critical_point_temperature, critical_point_pressure));

    point<double> standard = point<double>(si::standard_pressure, si::standard_temperature);

    table::FullTable<point<double>> critical_point = 
        table::derive<point<double>>(
            [](si::pressure<double> p, si::temperature<double> T){ return point<double>(p,T); }, 
            critical_point_pressure, critical_point_temperature);

    table::FullTable<point<double>> solid_sample_point = 
        table::derive<point<double>>(
            [](point<double> freezing){ return  standard.temperature < freezing.temperature? standard : freezing; }, 
            freezing_sample_point);

    table::FullTable<point<double>> liquid_sample_point = 
        table::complete(
            table::gather<point<double>>(
                [](point<double> freezing, point<double> boiling){ 
                    return freezing.temperature < standard.temperature && standard.temperature < boiling.temperature? 
                        standard : (freezing + boiling)/2.0; }, 
                table::partial(freezing_sample_point), boiling_sample_point),
            freezing_sample_point
        );

    table::FullTable<point<double>> gas_sample_point = 
        table::complete(
            table::gather<point<double>>([](point<double> boiling){ return boiling.temperature < standard.temperature? standard : boiling; }, published::boiling_sample_point),
            critical_point
        );


    // NOTE: boiling point is not guaranteed to exist for all compounds, since some compounds decompose before the boiling point is reached

    table::PartialTable<si::temperature<double>> boiling_sample_point_temperature = 
        table::gather<si::temperature<double>>(
            [](point<double> pT){ return pT.temperature; }, 
            boiling_sample_point);

    table::PartialTable<si::pressure<double>> boiling_sample_point_pressure = 
        table::gather<si::pressure<double>>(
            [](point<double> pT){ return pT.pressure; }, 
            boiling_sample_point);


    table::FullTable<si::temperature<double>> freezing_sample_point_temperature = 
        table::derive<si::temperature<double>>(
            [](point<double> pT){ return pT.temperature; }, 
            freezing_sample_point);

    table::FullTable<si::pressure<double>> freezing_sample_point_pressure = 
        table::derive<si::pressure<double>>(
            [](point<double> pT){ return pT.pressure; }, 
            freezing_sample_point);

    // via Klincewicz ⟶ Ihmels ⟶ Tee-Gotoh-Steward ⟶ Bird-Steward-Lightfoot2
    table::FullTable<LiquidDensityTemperatureRelation> density_as_liquid = 
        table::complete(published::density_as_liquid, 
            table::derive<LiquidDensityTemperatureRelation>(
                [](si::molar_mass<double> M, si::length<double> sigma){
                    return LiquidDensityTemperatureRelation(M / correlation::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma));}, 
                molar_mass, molecular_diameter));

    // via Klincewicz ⟶ Ihmels ⟶ definition of compressibility ⟶ definition of compressibility
    table::FullTable<GasDensityStateRelation> density_as_gas = 
        table::derive<GasDensityStateRelation>(
            [](si::molar_mass<double> M, si::pressure<double> pc, si::temperature<double> Tc, double Zc){
                return GasDensityStateRelation(M,pc,Tc,Zc);}, 
            molar_mass, critical_point_pressure, critical_point_temperature, critical_point_compressibility);

    // via Klincewicz ⟶ Ihmels ⟶ Tee-Gotoh-Steward ⟶ Bird-Steward-Lightfoot2 ⟶ Goodman (with speculative fallback)
    table::FullTable<SolidDensityTemperatureRelation> density_as_solid = 
        table::complete(
            table::first<SolidDensityTemperatureRelation>({
                published::density_as_solid, 
                table::gather<SolidDensityTemperatureRelation>([](si::molar_mass<double> M, LiquidDensityTemperatureRelation rhoL, point<double> liquid, point<double> triple){
                        return SolidDensityTemperatureRelation(M / correlation::estimate_molar_volume_as_solid_from_goodman(M / rhoL(liquid.temperature), liquid.temperature, triple.temperature));}, 
                    table::partial(molar_mass), 
                    table::partial(density_as_liquid), 
                    table::partial(liquid_sample_point), 
                    published::triple_point),
            }),
            table::derive<SolidDensityTemperatureRelation>(
                [](LiquidDensityTemperatureRelation rhoL, point<double> liquid){
                    return correlation::guess_density_as_solid_from_density_as_liquid(rhoL(liquid.temperature));}, 
                density_as_liquid, liquid_sample_point) // WARNING: results are speculative
        );



    // // CALCULATE POSSIBLE ROUTES TO ACENTRIC FACTOR

    table::PartialTable<SolidVaporPressureTemperatureRelation> vapor_pressure_as_solid = 
        table::first<SolidVaporPressureTemperatureRelation>({
            published::vapor_pressure_as_solid,
            table::gather<SolidVaporPressureTemperatureRelation>(
                []( LatentHeatTemperatureRelation Hs, 
                    si::molar_mass<double> M, 
                    point<double> triple){
                        auto T = analytic::Identity<float>();
                        auto R = si::universal_gas_constant;
                        float T3 = triple.temperature / si::kelvin;
                        float P3 = triple.pressure / si::pascal;
                        float k = (Hs(triple.temperature)*M/R)/si::kelvin;
                        using Polynomial = analytic::Polynomial<float,-1,1>;
                        using Clamped = analytic::Clamped<float,Polynomial>;
                        using Railyard = analytic::Railyard<float,Polynomial>;
                        return P3 * SolidVaporPressureTemperatureRelation(
                            Railyard(Clamped(0.0f, T3, Polynomial(-k*(1.0f/T - 1.0f/T3)))),
                            si::kelvin, si::pascal);
                        // NOTE: the above is translated from property/published.hpp:
                        // return triple_point_pressure * exp(-((latent_heat_of_sublimation*molar_mass / si::universal_gas_constant) * (1.0/temperature - 1.0/triple_point_temperature)));
                }, 
                published::latent_heat_of_sublimation, // NOTE: this should be latent heat of sublimation at the triple point, update this line if ever we convert latent_heat_of_fusion to a relation
                table::partial(molar_mass), 
                published::triple_point)
        });

    // CALCULATE ACENTRIC FACTOR
    table::PartialTable<double> acentric_factor = 
        table::first<double>({
            published::acentric_factor,
            table::gather<double>(
                []( point<double> liquid,
                    point<double> critical,
                    si::molar_mass<double> M,
                    LatentHeatTemperatureRelation Hv){
                    auto T = si::clamp(liquid.temperature, si::absolute_zero, critical.temperature);
                    auto Tc = critical.temperature;
                    return std::clamp(correlation::estimate_accentric_factor_from_pitzer(Hv(T), M, T, Tc), -1.0, 1.0);
                },
                table::partial(liquid_sample_point),
                table::partial(critical_point),
                table::partial(molar_mass),
                published::latent_heat_of_vaporization
            ),
            table::gather<double>(
                []( point<double> liquid,
                    point<double> critical,
                    si::molar_mass<double> M,
                    LiquidDynamicViscosityTemperatureRelation nuL){
                    auto T = liquid.temperature;
                    auto Tc = critical.temperature;
                    auto pc = critical.pressure;
                    return std::clamp(correlation::estimate_acentric_factor_from_letsou_stiel(nuL(T), M, T, Tc, pc), -1.0, 1.0);
                },
                table::partial(liquid_sample_point),
                table::partial(critical_point),
                table::partial(molar_mass),
                published::dynamic_viscosity_as_liquid
            ),
            table::gather<double>(
                []( point<double> liquid,
                    point<double> critical,
                    LiquidVaporPressureTemperatureRelation Pv){
                    auto T = liquid.temperature;
                    auto Tc = critical.temperature;
                    auto pc = critical.pressure;
                    return std::clamp(correlation::estimate_accentric_factor_from_lee_kesler(Pv(T), T, Tc, pc), -1.0, 1.0);
                },
                table::partial(liquid_sample_point),
                table::partial(critical_point),
                published::vapor_pressure_as_liquid
            ),
        });

    // CALCULATE PROPERTIES THAT CAN BE DERIVED FROM ACENTRIC FACTOR

    table::FullTable<double> molecular_degrees_of_freedom = 
        table::complete(
            table::first<double>({
                published::molecular_degrees_of_freedom,
                table::gather<double>(
                    correlation::guess_molecular_degrees_of_freedom,
                    acentric_factor,
                    table::partial(atoms_per_molecule)
                )
            }),
            6.0,
            atoms_per_molecule.size()
        );

    /* 
    TODO: adapt correlation::estimate_isobaric_heat_capacity_as_gas_from_rowlinson_poling
     for use with GasPropertyStateRelations using Dippr119Terms, then invoke results here
    */
    table::FullTable<GasHeatCapacityStateRelation> isobaric_specific_heat_capacity_as_gas = 
        table::complete(
            published::isobaric_specific_heat_capacity_as_gas,
            table::derive<GasHeatCapacityStateRelation>(
                correlation::get_isobaric_heat_capacity_as_ideal_gas,
                molar_mass,
                molecular_degrees_of_freedom
            )
        );

    table::PartialTable<LatentHeatTemperatureRelation> latent_heat_of_vaporization = 
        table::first<LatentHeatTemperatureRelation>({
            published::latent_heat_of_vaporization,
            table::gather<LatentHeatTemperatureRelation>(
                relation::get_pitzer_latent_heat_of_vaporization_temperature_relation,
                table::partial(molar_mass), 
                table::partial(critical_point_temperature), 
                acentric_factor
            ),
        });

    table::PartialTable<LatentHeatTemperatureRelation> latent_heat_of_sublimation = 
        table::first<LatentHeatTemperatureRelation>({
            published::latent_heat_of_sublimation,
            latent_heat_of_vaporization + published::latent_heat_of_fusion,
        });

    table::PartialTable<LatentHeatTemperatureRelation> latent_heat_of_fusion = 
        table::first<LatentHeatTemperatureRelation>({
            published::latent_heat_of_fusion,
            published::latent_heat_of_sublimation - latent_heat_of_vaporization,
        });

    table::PartialTable<LiquidDynamicViscosityTemperatureRelation> dynamic_viscosity_as_liquid = 
        table::first<LiquidDynamicViscosityTemperatureRelation>({
            published::dynamic_viscosity_as_liquid,
            table::gather<LiquidDynamicViscosityTemperatureRelation>(
                relation::estimate_viscosity_as_liquid_from_letsou_stiel,
                acentric_factor,
                table::partial(molar_mass),
                table::partial(critical_point_temperature),
                table::partial(critical_point_pressure)
            ),
        });

    table::PartialTable<LiquidVaporPressureTemperatureRelation> vapor_pressure_as_liquid = 
        table::first<LiquidVaporPressureTemperatureRelation>({
            published::vapor_pressure_as_liquid,
            // table::gather<LiquidVaporPressureTemperatureRelation>(
            //     relation::estimate_vapor_pressure_as_liquid_from_lee_kesler,
            //     acentric_factor,
            //     table::partial(critical_point_temperature),
            //     table::partial(critical_point_pressure)
            // ),
        });

    // CALCULATE MISCELLANEOUS PROPERTIES
    table::FullTable<LiquidThermalConductivityTemperatureRelation> thermal_conductivity_as_liquid = 
        table::complete(
            published::thermal_conductivity_as_liquid,
            table::derive<LiquidThermalConductivityTemperatureRelation>(
                relation::estimate_thermal_conductivity_as_liquid_from_sheffy_johnson,
                molar_mass, 
                freezing_sample_point_temperature,
                critical_point_temperature
            )
        );

    auto isobaric_specific_heat_capacity_as_liquid = published::isobaric_specific_heat_capacity_as_liquid;

    auto dynamic_viscosity_as_gas = published::dynamic_viscosity_as_gas;

    auto thermal_conductivity_as_gas = published::thermal_conductivity_as_gas;

    table::PartialTable<SolidDynamicViscosityTemperatureRelation> dynamic_viscosity_as_solid = 
        table::first<SolidDynamicViscosityTemperatureRelation>({
            published::dynamic_viscosity_as_solid,
            table::gather<SolidDynamicViscosityTemperatureRelation>(
                []( LiquidDynamicViscosityTemperatureRelation nuL,
                    point<double> liquid){
                    return SolidDynamicViscosityTemperatureRelation(
                        correlation::guess_viscosity_as_solid_from_viscosity_as_liquid(nuL(liquid.temperature)));
                    // WARNING: results above are speculative, 
                    // see property/speculative.hpp for reasoning and justification
                },
                dynamic_viscosity_as_liquid,
                table::partial(liquid_sample_point)
            )
        });


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





    struct ElasticitiesHandbook
    {
        table::FullTable<SolidBulkModulusTemperatureRelation> bulk_modulus;
        table::FullTable<SolidTensileModulusTemperatureRelation> tensile_modulus;
        table::FullTable<SolidLameParameterTemperatureRelation> lame_parameter;
        table::FullTable<SolidShearModulusTemperatureRelation> shear_modulus;
        table::FullTable<SolidPoissonRatioTemperatureRelation> poisson_ratio;
        table::FullTable<SolidPwaveModulusTemperatureRelation> pwave_modulus;

        ElasticitiesHandbook(
            const table::PartialTable<SolidBulkModulusTemperatureRelation> K,
            const table::PartialTable<SolidTensileModulusTemperatureRelation> E,
            const table::PartialTable<SolidLameParameterTemperatureRelation> l,
            const table::PartialTable<SolidShearModulusTemperatureRelation> G,
            const table::PartialTable<SolidPoissonRatioTemperatureRelation> nu,
            const table::PartialTable<SolidPwaveModulusTemperatureRelation> M,
            const table::FullTable<int> similarity,
            const int fallback_id
        ){

            // Standardize on bulk and tensile modulus since they seem to be reported most often,
            // then use those two to calculate the remaining modulii.

            using Modulus = relation::GenericRelation<si::temperature<double>,si::pressure<double>>;
            using Poisson = relation::GenericRelation<si::temperature<double>,double>;

            auto E2 = table::first<Modulus>({ E,
                table::gather<Modulus>([](auto K,  auto G ) { return Modulus([=](auto T){ return correlation::get_tensile_from_bulk_and_shear   (K(T), G(T)); }); }, K, G ),
                table::gather<Modulus>([](auto K,  auto nu) { return Modulus([=](auto T){ return correlation::get_tensile_from_bulk_and_poisson (K(T), nu(T));}); }, K, nu),
                table::gather<Modulus>([](auto K,  auto M ) { return Modulus([=](auto T){ return correlation::get_tensile_from_bulk_and_pwave   (K(T), M(T)); }); }, K, M ),
                table::gather<Modulus>([](auto l,  auto G ) { return Modulus([=](auto T){ return correlation::get_tensile_from_lame_and_shear   (l(T), G(T)); }); }, l, G ),
                table::gather<Modulus>([](auto l,  auto nu) { return Modulus([=](auto T){ return correlation::get_tensile_from_lame_and_poisson (l(T), nu(T));}); }, l, nu),
                table::gather<Modulus>([](auto l,  auto M ) { return Modulus([=](auto T){ return correlation::get_tensile_from_lame_and_pwave   (l(T), M(T)); }); }, l, M ),
                table::gather<Modulus>([](auto G,  auto nu) { return Modulus([=](auto T){ return correlation::get_tensile_from_shear_and_poisson(G(T), nu(T));}); }, G, nu),
                table::gather<Modulus>([](auto G,  auto M ) { return Modulus([=](auto T){ return correlation::get_tensile_from_shear_and_pwave  (G(T), M(T)); }); }, G, M ),
                table::gather<Modulus>([](auto nu, auto M ) { return Modulus([=](auto T){ return correlation::get_tensile_from_poisson_and_pwave(nu(T),M(T)); }); }, nu,M ),
            });

            auto K2 = table::first<Modulus>({ K,
                table::gather<Modulus>([](auto E,  auto l ) { return Modulus([=](auto T){ return correlation::get_bulk_from_tensile_and_lame   (E(T), l(T) ); }); }, E,  l ),
                table::gather<Modulus>([](auto E,  auto G ) { return Modulus([=](auto T){ return correlation::get_bulk_from_tensile_and_shear  (E(T), G(T) ); }); }, E,  G ),
                table::gather<Modulus>([](auto E,  auto nu) { return Modulus([=](auto T){ return correlation::get_bulk_from_tensile_and_poisson(E(T), nu(T)); }); }, E,  nu),
                table::gather<Modulus>([](auto l,  auto G ) { return Modulus([=](auto T){ return correlation::get_bulk_from_lame_and_shear     (l(T), G(T) ); }); }, l,  G ),
                table::gather<Modulus>([](auto l,  auto nu) { return Modulus([=](auto T){ return correlation::get_bulk_from_lame_and_poisson   (l(T), nu(T)); }); }, l,  nu),
                table::gather<Modulus>([](auto l,  auto M ) { return Modulus([=](auto T){ return correlation::get_bulk_from_lame_and_pwave     (l(T), M(T) ); }); }, l,  M ),
                table::gather<Modulus>([](auto G,  auto nu) { return Modulus([=](auto T){ return correlation::get_bulk_from_shear_and_poisson  (G(T), nu(T)); }); }, G,  nu),
                table::gather<Modulus>([](auto G,  auto M ) { return Modulus([=](auto T){ return correlation::get_bulk_from_shear_and_pwave    (G(T), M(T) ); }); }, G,  M ),
                table::gather<Modulus>([](auto nu, auto M ) { return Modulus([=](auto T){ return correlation::get_bulk_from_poisson_and_pwave  (nu(T), M(T)); }); }, nu, M ),
            });

            auto l2  = table::first<Modulus>({ l,  table::gather<Modulus>([](auto K, auto E) { return Modulus([=](auto T){ return correlation::get_lame_from_bulk_and_tensile    (K(T),E(T)); }); }, K2, E2), });
            auto G2  = table::first<Modulus>({ G,  table::gather<Modulus>([](auto K, auto E) { return Modulus([=](auto T){ return correlation::get_shear_from_bulk_and_tensile   (K(T),E(T)); }); }, K2, E2), });
            auto nu2 = table::first<Poisson>({ nu, table::gather<Poisson>([](auto K, auto E) { return Poisson([=](auto T){ return correlation::get_poisson_from_bulk_and_tensile (K(T),E(T)); }); }, K2, E2), });
            auto M2  = table::first<Modulus>({ M,  table::gather<Modulus>([](auto K, auto E) { return Modulus([=](auto T){ return correlation::get_pwave_from_bulk_and_tensile   (K(T),E(T)); }); }, K2, E2), });

            bulk_modulus    = table::complete(table::imitate(K2,  similarity), K2 [fallback_id], similarity.size());
            tensile_modulus = table::complete(table::imitate(E2,  similarity), E2 [fallback_id], similarity.size());
            lame_parameter  = table::complete(table::imitate(l2,  similarity), l2 [fallback_id], similarity.size());
            shear_modulus   = table::complete(table::imitate(G2,  similarity), G2 [fallback_id], similarity.size());
            poisson_ratio   = table::complete(table::imitate(nu2, similarity), nu2[fallback_id], similarity.size());
            pwave_modulus   = table::complete(table::imitate(M2,  similarity), M2 [fallback_id], similarity.size());

        }
    };

    SpectralHandbook spectrums(
        published::refractive_index_as_solid,
        published::extinction_coefficient_as_solid,
        published::absorption_coefficient_as_solid,
        published::similarity,
        ids::water
    );

    StrengthsHandbook strengths(
        published::shear_yield_strength_as_solid,
        published::tensile_yield_strength_as_solid,
        published::compressive_yield_strength_as_solid,
        published::shear_fracture_strength_as_solid,
        published::tensile_fracture_strength_as_solid,
        published::compressive_fracture_strength_as_solid,
        published::similarity,
        ids::water
    );

    ElasticitiesHandbook elasticities(
        published::bulk_modulus_as_solid,
        published::tensile_modulus_as_solid,
        published::lame_parameter_as_solid,
        published::shear_modulus_as_solid,
        published::poisson_ratio_as_solid,
        published::pwave_modulus_as_solid,
        published::similarity,
        ids::water
    );

}}

