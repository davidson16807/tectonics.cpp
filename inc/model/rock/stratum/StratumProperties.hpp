#pragma once

// in house libraries
#include <unit/si.hpp>
#include <model/mix/rules.hpp>
#include <model/rock/mineral/MineralProperties.hpp>
#include <model/rock/stratum/StratumStore.hpp>

namespace rock{

    auto StratumDensity                      (const auto& table)                           { return mix::density_as_solid                        <float>(table, MineralMass());}
    auto StratumSpecificHeatCapacity         (const auto& table)                           { return mix::specific_heat_capacity_as_solid         <float>(table, MineralMass());}
    auto StratumDynamicViscosity             (const auto& table)                           { return mix::dynamic_viscosity_as_solid              <float>(table, MineralMass());}
    auto StratumMolarHeatCapacity            (const auto& table, const auto& molar_masses) { return mix::molar_heat_capacity_as_solid            <float>(table, MineralMoles(molar_masses));}
    auto StratumVaporPressure                (const auto& table, const auto& molar_masses) { return mix::vapor_pressure_as_solid                 <float>(table, MineralMoles(molar_masses));}
    auto StratumMolarDensity                 (const auto& table, const auto& molar_masses) { return mix::molar_density_as_solid                  <float>(table, MineralMoles(molar_masses));}
    auto StratumTensileModulus               (const auto& table, const auto& densities)    { return mix::tensile_modulus_as_solid                <float>(table, MineralVolume(densities));}
    auto StratumShearModulus                 (const auto& table, const auto& densities)    { return mix::shear_modulus_as_solid                  <float>(table, MineralVolume(densities));}
    auto StratumBulkModulus                  (const auto& table, const auto& densities)    { return mix::bulk_modulus_as_solid                   <float>(table, MineralVolume(densities));}
    auto StratumPwaveModulus                 (const auto& table, const auto& densities)    { return mix::pwave_modulus_as_solid                  <float>(table, MineralVolume(densities));}
    auto StratumLameParameter                (const auto& table, const auto& densities)    { return mix::lame_parameter_as_solid                 <float>(table, MineralVolume(densities));}
    auto StratumPoissonRatio                 (const auto& table, const auto& densities)    { return mix::poisson_ratio_as_solid                  <float>(table, MineralVolume(densities));}
    auto StratumTensileYieldStrength         (const auto& table, const auto& densities)    { return mix::tensile_yield_strength_as_solid         <float>(table, MineralVolume(densities));}
    auto StratumShearYieldStrength           (const auto& table, const auto& densities)    { return mix::shear_yield_strength_as_solid           <float>(table, MineralVolume(densities));}
    auto StratumCompressiveYieldStrength     (const auto& table, const auto& densities)    { return mix::compressive_yield_strength_as_solid     <float>(table, MineralVolume(densities));}
    auto StratumTensileFractureStrength      (const auto& table, const auto& densities)    { return mix::tensile_fracture_strength_as_solid      <float>(table, MineralVolume(densities));}
    auto StratumShearFractureStrength        (const auto& table, const auto& densities)    { return mix::shear_fracture_strength_as_solid        <float>(table, MineralVolume(densities));}
    auto StratumCompressiveFractureStrength  (const auto& table, const auto& densities)    { return mix::compressive_fracture_strength_as_solid  <float>(table, MineralVolume(densities));}
    auto StratumThermalConductivity          (const auto& table, const auto& densities)    { return mix::thermal_conductivity_as_solid           <float>(table, MineralVolume(densities));}

    /*
    `AgedStratumDensity` serves as an alternative to `rock::density` 
    for circumstances where the density of minerals will vary with age.
    This functionality is critical in order to correctly model plate tectonics.
    The cause for the age mediated density change is never represented -
    densities for each mineral are specified in a container as strict functions of age.
    Since the majority of a world's crust will be deposited upon rifting,
    the oldest age of rock within a stratum will be used to calculate density.
    */
    template<typename Table>
    class AgedStratumDensity
    {
        using density = si::density<float>;

        const Table density_for_age;
        const si::time<float> age_of_world;
    public:
        AgedStratumDensity(const Table& density_for_age, const si::time<float> age_of_world):
            density_for_age(density_for_age),
            age_of_world(age_of_world)
        {}
        template<std::size_t M>
        auto operator() (const StratumStore<M>& stratum) const 
        {
            // return si::density<float>(1.0f);
            si::mass<float> mass(0.0f);
            si::mass<float> total_mass(0.0f);
            si::volume<float> total_volume(0.0f);
            for (std::size_t i=0; i<M; i++)
            {
                mass = stratum[i].mass();
                total_mass += mass;
                // volume = specific volume * mass
                // sum up to get total volume
                total_volume += mass / 
                    density(density_for_age[i](stratum.age_of_world_when_first_deposited()-age_of_world));
            }
            // volume = total volume * mass
            return total_mass / total_volume;
        }
    };

    struct StratumMass
    {
        StratumMass(){}
        template<std::size_t M>
        auto operator() (const StratumStore<M>& stratum) const { return stratum.mass ();}
    };

}

