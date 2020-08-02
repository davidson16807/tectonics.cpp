#pragma once

// C libraries
#include <cmath>

// 3rd party libraries
#include <glm/vec2.hpp>

// in-house libraries
#include "Stratum.hpp"
#include "enums.hpp"

/*
Contains a large set of enums and functions for classifying individual rock layers
We place them outside the Strata class to avoid polluting that class namespace.
*/
namespace strata
{

    void get_particle_size_bin_fractional_volume(
        const Stratum& stratum, 
        const std::array<float, stratum_mass_pool_count>& mass_pool_chemical_susceptibility, 
        std::array<float, ParticleSizeBins::COUNT>& output
    ){
        std::array<float, ParticleSizeBins::COUNT> output;
        output.fill(0);
        output[ParticleSizeBins::boulder] += stratum.grain_type_bin_fractional_volume[GrainType::unweathered_extrusive];
        output[ParticleSizeBins::boulder] += stratum.grain_type_bin_fractional_volume[GrainType::unweathered_intrusive];
        output[ParticleSizeBins::gravel]  += stratum.grain_type_bin_fractional_volume[GrainType::mechanically_weathered_extrusive];
        output[ParticleSizeBins::sand]    += stratum.grain_type_bin_fractional_volume[GrainType::mechanically_weathered_intrusive];
        for (int i = 0; i < stratum_mass_pool_count; ++i)
        {
            output[ParticleSizeBins::silt] += mass_pool_chemical_susceptibility[i] * stratum.grain_type_bin_fractional_volume[GrainType::chemically_weathered_extrusive];
            output[ParticleSizeBins::sand] += mass_pool_chemical_susceptibility[i] * stratum.grain_type_bin_fractional_volume[GrainType::chemically_weathered_intrusive];

            output[ParticleSizeBins::clay] += (1.0f-mass_pool_chemical_susceptibility[i]) * stratum.grain_type_bin_fractional_volume[GrainType::chemically_weathered_extrusive];
            output[ParticleSizeBins::clay] += (1.0f-mass_pool_chemical_susceptibility[i]) * stratum.grain_type_bin_fractional_volume[GrainType::chemically_weathered_intrusive];
        }

        float total_relative_volume(0);
        for (std::size_t i=0; i<ParticleSizeBins::COUNT; i++)
        {
            total_relative_volume += output[i];
        }
        for (std::size_t i=0; i<ParticleSizeBins::COUNT; i++)
        {
            output[i] /= total_relative_volume;
        }
    }

    // NOTE: based off https://upload.wikimedia.org/wikipedia/commons/6/65/SoilTextureTriangle.jpg
    SoilTypes get_soil_type(const Stratum& stratum){
        float total_volume = stratum.particle_size_bin_relative_volume_sum();
        if (stratum.particle_size_bin_relative_volume[ParticleSizeBins::clay] / total_volume > 0.4) 
        {
            if (stratum.particle_size_bin_relative_volume[ParticleSizeBins::sand] / total_volume > 0.5)
            {
                return SoilTypes::sand_clay;
            }
            else if (stratum.particle_size_bin_relative_volume[ParticleSizeBins::silt] / total_volume > 0.5)
            {
                return SoilTypes::silt_clay;
            }
            else
            {
                return SoilTypes::clay;
            }
        }
        else if (stratum.particle_size_bin_relative_volume[ParticleSizeBins::sand] / total_volume > 0.8)
        {
            return SoilTypes::sand;
        } 
        else if (stratum.particle_size_bin_relative_volume[ParticleSizeBins::silt] / total_volume > 0.8) 
        {
            return SoilTypes::silt;
        }
        else if (stratum.particle_size_bin_relative_volume[ParticleSizeBins::sand] / total_volume > 0.5) 
        {
            return SoilTypes::sand_loam;
        }
        else if (stratum.particle_size_bin_relative_volume[ParticleSizeBins::silt] / total_volume > 0.5) 
        {
            return SoilTypes::silt_loam;
        }
        else if (stratum.particle_size_bin_relative_volume[ParticleSizeBins::clay] / total_volume > 0.3) 
        {
            return SoilTypes::clay_loam;
        }
        else 
        {
            return SoilTypes::loam;
        }
    }

    // NOTE: based off https://upload.wikimedia.org/wikipedia/commons/f/f4/Mineralogy_igneous_rocks_EN.svg
    IgneousCompositionTypes get_igneous_composition_types(const Stratum& stratum)
    {
        float total_mass = stratum.mass();
        if (stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::olivine].mass / total_mass > 0.3)
        {
            return IgneousCompositionTypes::ultramafic;
        }
        else if (stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::pyroxene].mass / total_mass > 0.3)
        {
            return IgneousCompositionTypes::mafic;
        }
        else if (stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::plagioclase].mass / total_mass > 0.3)
        {
            return IgneousCompositionTypes::intermediate;
        }
        else if ((stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::orthoclase].mass + 
                  stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::quartz].mass) / total_mass > 0.3)
        {
            return IgneousCompositionTypes::felsic;
        }
        else 
        {
            return IgneousCompositionTypes::other;
        }
    }


    IgneousCompositionTypes get_igneous_formation_types(const Stratum& stratum)
    {
        float extrusive_grain_bin_relative_volume = 0.0;
        float intrusive_grain_bin_relative_volume = 0.0;
        for (int i = 0; i < stratum.mass_pools.size(); ++i)
        {
            intrusive_grain_bin_relative_volume += stratum.mass_pools[i].intrusive_grain_bin_relative_volume;
            extrusive_grain_bin_relative_volume += stratum.mass_pools[i].extrusive_grain_bin_relative_volume;
        }
        if (intrusive_grain_bin_relative_volume > extrusive_grain_bin_relative_volume)
        {
            return IgneousFormationTypes::intrusive;
        }
        else
        {
            return IgneousFormationTypes::extrusive;
        }
    }

    /*
    `get_metamorphic_facies` returns the facies of a rock that has been submitted to a given temperature and pressure.
    Metamorphic facies are like phases of matter for rocks.
    Rocks are complex assortments of mineral grains. 
    Each mineral has its own phase diagram, 
    but there are many kinds of minerals and their phase diagrams are poorly documented. 
    However the phase diagrams for minerals share some consistencies,
    and these consistencies allow us to make predictions about what an earth-like rock will look like
    when submitted to a given pressure and temperature.
    This will apply to any rock that is composed of minerals commonly found on earth.
    */
    MetamorphicFacies get_metamorphic_facies(const Stratum& stratum)
    {
        float p = 0.0; // the lowest max pressure shared by all mass pools
        float t = 0.0; // the lowest max temperature shared by all mass pools
        for (int i = OxygenPlanetStratumMassPoolTypes::olivine; i < OxygenPlanetStratumMassPoolTypes::orthoclase; ++i)
        {
            p = min(p, stratum.mass_pools[i].max_pressure);
            t = min(t, stratum.mass_pools[i].max_temperature);
        }
        return phases::get_metamorphic_facies(p,t);
    }
    
    MetamorphicGrades get_metamorphic_grades(const Stratum& stratum)
    {
        float P = 0.0; // the lowest max pressure shared by all mass pools
        float T = 0.0; // the lowest max temperature shared by all mass pools
        for (int i = 0; i < stratum.mass_pools.size(); ++i)
        {
            P = min(P, stratum.mass_pools[i].max_pressure);
            T = min(T, stratum.mass_pools[i].max_temperature);
        }
        glm::vec2 PT(P,T);
        glm::vec2 scale(2e8*units::pascal, 100*units::kelvin);
        float distance = glm::length(PT/scale);
        if ( distance < 0.011f )
        {
            return MetamorphicGrades::igneous_or_sediment;
        }
        else if ( distance < 2.0f )
        {
            return MetamorphicGrades::sedimentary;
        }
        else if( distance < 6.0f )
        {
            return MetamorphicGrades::low;
        }
        else if( distance < 10.0f )
        {
            return MetamorphicGrades::intermediate;
        }
        else
        {
            return MetamorphicGrades::high;
        }
    }
    ParticleSizeBins get_dominant_particle_size_bin(const Stratum& stratum)
    {
        float v_max = 0;
        int i_max = 0;
        for (int i = 0; i < stratum.particle_size_bin_relative_volume.size(); ++i)
        {
            if (stratum.particle_size_bin_relative_volume[i] > v_max)
            {
                v_max = stratum.particle_size_bin_relative_volume[i];
                i_max = i;
            }
        }
        return (ParticleSizeBins)i_max;
    }


    /*
    `get_rock_composition_types` returns a `RockCompositionTypes` 
    indicating composition criteria used by `get_rock_type()`.
    We use `get_rock_composition_types()` to simplify code and improve performance.
    */
    RockCompositionTypes get_rock_composition_types(const Stratum& stratum)
    {
        RockCompositionTypes out;
        float total_mass = stratum.mass();
        out.partly_calcarous =  stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::calcite]     / total_mass > 0.5;
        out.calcarous        =  stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::calcite]     / total_mass > 0.9;
        out.silicaceous      =  stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::quartz]      / total_mass > 0.9;
        out.feldspathic      = (stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::plagioclase] +
                                stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::orthoclase]) / total_mass > 0.25;
        out.volcanic         = (stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::olivine]     +
                                stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::pyroxene]    +
                                stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::plagioclase] +
                                stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::quartz]      +
                                stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::orthoclase]) / total_mass > 0.9;
        out.ferrous          = (stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::hematite]    +
                                stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::pyrite])     / total_mass > 0.15;
        out.organic          =  stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::organics]    / total_mass > 0.9;
        return out;
    }


    /*
    "get_rock_type_hash" returns a bitfield integer representing all aspects that might distinguish a rock stratum.
    The set of values it represents is a superset of those represented by RockTypes. 
    The function is designed to easily and efficiently determine whether two rock layers should be merged together.
    */
    int get_rock_type_hash(const Stratum& stratum)
    {
        float total_mass = stratum.mass();
        float total_volume = stratum.particle_size_bin_relative_volume_sum();
        RockCompositionTypes rock_composition_types = get_rock_composition_types(stratum);
        return 0
            * MetamorphicGrades::COUNT       + get_metamorphic_grades(stratum)
            * IgneousCompositionTypes::COUNT + get_igneous_composition_types(stratum)
            * IgneousFormationTypes::COUNT   + get_igneous_formation_types(stratum)
            * ParticleSizeBins::COUNT        + get_dominant_particle_size_bin(stratum)
            * 2 + rock_composition_types.partly_calcarous
            * 2 + rock_composition_types.calcareous
            * 2 + rock_composition_types.silicaceous
            * 2 + rock_composition_types.feldspathic
            * 2 + rock_composition_types.volcanic
            * 2 + rock_composition_types.ferrous
            * 2 + rock_composition_types.organic;
        ;
    }


}