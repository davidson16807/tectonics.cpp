#pragma once

// in house libraries
#include <model/mix/rules.hpp>
#include <model/rock/mineral/MineralProperties.hpp>

namespace rock{

    auto density                        (const auto& table)                           { return density_as_solid                        <float>(table, MineralMass());}
    auto specific_heat_capacity         (const auto& table)                           { return specific_heat_capacity_as_solid         <float>(table, MineralMass());}
    auto dynamic_viscosity              (const auto& table)                           { return dynamic_viscosity_as_solid              <float>(table, MineralMass());}
    auto molar_heat_capacity            (const auto& table, const auto& molar_masses) { return molar_heat_capacity_as_solid            <float>(table, MineralMoles(molar_masses));}
    auto vapor_pressure                 (const auto& table, const auto& molar_masses) { return vapor_pressure_as_solid                 <float>(table, MineralMoles(molar_masses));}
    auto molar_density                  (const auto& table, const auto& molar_masses) { return molar_density_as_solid                  <float>(table, MineralMoles(molar_masses));}
    auto tensile_modulus                (const auto& table, const auto& densities)    { return tensile_modulus_as_solid                <float>(table, MineralVolume(densities));}
    auto shear_modulus                  (const auto& table, const auto& densities)    { return shear_modulus_as_solid                  <float>(table, MineralVolume(densities));}
    auto bulk_modulus                   (const auto& table, const auto& densities)    { return bulk_modulus_as_solid                   <float>(table, MineralVolume(densities));}
    auto pwave_modulus                  (const auto& table, const auto& densities)    { return pwave_modulus_as_solid                  <float>(table, MineralVolume(densities));}
    auto lame_parameter                 (const auto& table, const auto& densities)    { return lame_parameter_as_solid                 <float>(table, MineralVolume(densities));}
    auto poisson_ratio                  (const auto& table, const auto& densities)    { return poisson_ratio_as_solid                  <float>(table, MineralVolume(densities));}
    auto tensile_yield_strength         (const auto& table, const auto& densities)    { return tensile_yield_strength_as_solid         <float>(table, MineralVolume(densities));}
    auto shear_yield_strength           (const auto& table, const auto& densities)    { return shear_yield_strength_as_solid           <float>(table, MineralVolume(densities));}
    auto compressive_yield_strength     (const auto& table, const auto& densities)    { return compressive_yield_strength_as_solid     <float>(table, MineralVolume(densities));}
    auto tensile_fracture_strength      (const auto& table, const auto& densities)    { return tensile_fracture_strength_as_solid      <float>(table, MineralVolume(densities));}
    auto shear_fracture_strength        (const auto& table, const auto& densities)    { return shear_fracture_strength_as_solid        <float>(table, MineralVolume(densities));}
    auto compressive_fracture_strength  (const auto& table, const auto& densities)    { return compressive_fracture_strength_as_solid  <float>(table, MineralVolume(densities));}
    auto thermal_conductivity           (const auto& table, const auto& densities)    { return thermal_conductivity_as_solid           <float>(table, MineralVolume(densities));}

    /*
    `AgedStratumDensity` serves as an alternative to `rock::density` 
    for circumstances where the density of minerals will vary with age.
    This functionality is critical in order to correctly model plate tectonics.
    The cause for the age mediated density change is never represented -
    densities for each mineral are specified in a container as strict functions of age.
    Since the majority of a world's crust will be deposited upon rifting,
    the oldest age of rock within a stratum will be used to calculate density.
    */
    template<typename scalar, typename Table>
    class AgedStratumDensity
    {
        const Table density_for_age;
        const si::time<float> age_of_world;
    public:
        MineralDensity(const Table& density_for_age, const si::time<float> age_of_world):
            density_for_age(density_for_age),
            age_of_world(age_of_world)
        {}
        template<int M>
        auto operator() (const StratumStore<M>& stratum) const 
        {
            auto sum = constituents[0];
            for (std::size_t i=1; i<constituents.size(); i++)
            {
                sum += constituents[i];
            }
            si::specific_volume<float> result(0.0f);
            for (std::size_t i=0; i<constituents.size(); i++)
            {
                result += (constituents[i]/sum) / density_for_age[i](stratum.age_of_world_when_first_deposited()-age_of_world);
            }
            return 1.0f / result;
        }
    };
    template<typename scalar, typename Table>
    auto aged_stratum_density(const Table& table, const si::time<float> age_of_world)
    {
        return MineralDensity<scalar,Table>(table, get, check);
    }

}

