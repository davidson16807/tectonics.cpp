#pragma once

// C libraries
#include <cmath>

// 3rd party libraries
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>

// in-house libraries
#include <academics/units.hpp>
#include <academics/chemistry/phases/metamorphic_facies.hpp>

#include "Stratum.hpp"
#include "StratumTypes.hpp"
#include "StratumMassPoolTypes.hpp"

/*
Contains a large set of enums and functions for classifying individual rock layers
We place them outside the Strata class to avoid polluting that class namespace.
*/
namespace strata
{

    template<int M>
    void get_particle_size_bin_fractional_volume(
        const Stratum<M>& stratum, 
        std::array<float, int(ParticleSizeBins::count)>& output
    ){
        float chemically_susceptible_mass_fraction = 
            (stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::calcite)].mass +
             stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::organics)].mass) / stratum.mass();

        output.fill(0);

        for (std::size_t i=0; i<M; i++)
        {
            const std::array<float, int(GrainType::count)>& grains = stratum.mass_pools[i].grain_type_relative_volume;
            output[int(ParticleSizeBins::boulder)] += grains[int(GrainType::unweathered_extrusive)];
            output[int(ParticleSizeBins::boulder)] += grains[int(GrainType::unweathered_intrusive)];
            output[int(ParticleSizeBins::pebble)]  += grains[int(GrainType::mechanically_weathered_extrusive)];
            output[int(ParticleSizeBins::sand)]    += grains[int(GrainType::mechanically_weathered_intrusive)];
            output[int(ParticleSizeBins::clay)]    += grains[int(GrainType::chemically_weathered_extrusive)] * chemically_susceptible_mass_fraction;
            output[int(ParticleSizeBins::clay)]    += grains[int(GrainType::chemically_weathered_intrusive)] * chemically_susceptible_mass_fraction;
            output[int(ParticleSizeBins::silt)]    += grains[int(GrainType::chemically_weathered_extrusive)] * (1.0f - chemically_susceptible_mass_fraction);
            output[int(ParticleSizeBins::sand)]    += grains[int(GrainType::chemically_weathered_intrusive)] * (1.0f - chemically_susceptible_mass_fraction);
        }

        float total_relative_volume(0);
        for (std::size_t i=0; i<int(ParticleSizeBins::count); i++)
        {
            total_relative_volume += output[i];
        }
        for (std::size_t i=0; i<int(ParticleSizeBins::count); i++)
        {
            output[i] /= total_relative_volume;
        }
    }

    // NOTE: based off https://upload.wikimedia.org/wikipedia/commons/6/65/SoilTextureTriangle.jpg
    SoilTypes get_soil_type(const std::array<float, int(ParticleSizeBins::count)>& particle_size_bin_fractional_volume)
    {
        if (particle_size_bin_fractional_volume[int(ParticleSizeBins::clay)] > 0.4) 
        {
            if (particle_size_bin_fractional_volume[int(ParticleSizeBins::sand)] > 0.5)
            {
                return SoilTypes::sand_clay;
            }
            else if (particle_size_bin_fractional_volume[int(ParticleSizeBins::silt)] > 0.5)
            {
                return SoilTypes::silt_clay;
            }
            else
            {
                return SoilTypes::clay;
            }
        }
        else if (particle_size_bin_fractional_volume[int(ParticleSizeBins::sand)] > 0.8)
        {
            return SoilTypes::sand;
        } 
        else if (particle_size_bin_fractional_volume[int(ParticleSizeBins::silt)] > 0.8) 
        {
            return SoilTypes::silt;
        }
        else if (particle_size_bin_fractional_volume[int(ParticleSizeBins::sand)] > 0.5) 
        {
            return SoilTypes::sand_loam;
        }
        else if (particle_size_bin_fractional_volume[int(ParticleSizeBins::silt)] > 0.5) 
        {
            return SoilTypes::silt_loam;
        }
        else if (particle_size_bin_fractional_volume[int(ParticleSizeBins::clay)] > 0.3) 
        {
            return SoilTypes::clay_loam;
        }
        else 
        {
            return SoilTypes::loam;
        }
    }

    ParticleSizeBins get_dominant_particle_size_bin(
        const std::array<float, int(ParticleSizeBins::count)>& particle_size_bin_fractional_volume
    ){
        float v_max = 0;
        int i_max = 0;
        for (int i = 0; i < int(ParticleSizeBins::count); ++i)
        {
            if (particle_size_bin_fractional_volume[i] > v_max)
            {
                v_max = particle_size_bin_fractional_volume[i];
                i_max = i;
            }
        }
        return (ParticleSizeBins)i_max;
    }







    // NOTE: based off https://upload.wikimedia.org/wikipedia/commons/f/f4/Mineralogy_igneous_rocks_EN.svg
    template<int M>
    IgneousCompositionTypes get_igneous_composition_types(const Stratum<M>& stratum)
    {
        float total_mass = stratum.mass();
        if (stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::olivine)].mass / total_mass > 0.3)
        {
            return IgneousCompositionTypes::ultramafic;
        }
        else if (stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::pyroxene)].mass / total_mass > 0.3)
        {
            return IgneousCompositionTypes::mafic;
        }
        else if (stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::plagioclase)].mass / total_mass > 0.3)
        {
            return IgneousCompositionTypes::intermediate;
        }
        else if ((stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::orthoclase)].mass + 
                  stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::quartz)].mass) / total_mass > 0.3)
        {
            return IgneousCompositionTypes::felsic;
        }
        else 
        {
            return IgneousCompositionTypes::other;
        }
    }
    template<int M>
    void get_igneous_formation_type_fractional_volume(
        const Stratum<M>& stratum, 
        std::array<float, int(IgneousFormationTypes::count)>& output
    ){
        output.fill(0);
        for (std::size_t i=0; i<M; i++)
        {
            const std::array<float, int(GrainType::count)>& grains = stratum.mass_pools[i].grain_type_relative_volume;
            output[int(IgneousFormationTypes::extrusive)] += grains[int(GrainType::unweathered_extrusive)];
            output[int(IgneousFormationTypes::intrusive)] += grains[int(GrainType::unweathered_intrusive)];
            output[int(IgneousFormationTypes::extrusive)] += grains[int(GrainType::mechanically_weathered_extrusive)];
            output[int(IgneousFormationTypes::intrusive)] += grains[int(GrainType::mechanically_weathered_intrusive)];
            output[int(IgneousFormationTypes::extrusive)] += grains[int(GrainType::chemically_weathered_extrusive)];
            output[int(IgneousFormationTypes::intrusive)] += grains[int(GrainType::chemically_weathered_intrusive)];
        }
        float total_relative_volume(0);
        for (std::size_t i=0; i<int(ParticleSizeBins::count); i++)
        {
            total_relative_volume += output[i];
        }
        for (std::size_t i=0; i<int(ParticleSizeBins::count); i++)
        {
            output[i] /= total_relative_volume;
        }
    }

    IgneousFormationTypes get_igneous_formation_types(
        std::array<float, int(IgneousFormationTypes::count)>& igneous_formation_fractional_volume
    ){
        if (igneous_formation_fractional_volume[int(IgneousFormationTypes::intrusive)] > 
            igneous_formation_fractional_volume[int(IgneousFormationTypes::extrusive)]
        ){
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
    template<int M>
    phase::MetamorphicFacies get_metamorphic_facies(const Stratum<M>& stratum)
    {
        float p = 0.0; // the lowest max pressure shared by all mass pools
        float t = 0.0; // the lowest max temperature shared by all mass pools
        p = stratum.max_pressure_received;
        t = stratum.max_temperature_received;
        return phase::get_metamorphic_facies(p,t);
    }
    
    template<int M>
    MetamorphicGrades get_metamorphic_grades(const Stratum<M>& stratum)
    {
        float p = 0.0; // the lowest max pressure shared by all mass pools
        float t = 0.0; // the lowest max temperature shared by all mass pools
        p = stratum.max_pressure_received;
        t = stratum.max_temperature_received;
        glm::vec2 PT(p, std::max(t-units::standard_temperature, 0.0f));
        glm::vec2 scale(2e8*units::pascal, 100*units::kelvin);
        float distance = glm::length(PT/scale);
        /*
        NOTE: 
        2.2e6 Pascals is the pressure equivalent of 500ft of sediment @ 1500kg/m^3 density
        500ft from http://wiki.aapg.org/Sandstone_diagenetic_processes
        */
        if ( p < 2.2e6 && distance < 1.5f )
        {
            return MetamorphicGrades::igneous_or_sediment;
        }
        /*
        NOTE: 
        3e8 Pascals is the pressure equivalent of 11km of sedimentary rock @ 2700kg/m^3 density
        300 MPa from https://www.tulane.edu/~sanelson/eens212/typesmetamorph.htm
        also from tectonics-research/chemistry/phase-diagrams/facies2.gif
        */
        else if ( distance < 1.5f )
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

    /*
    `get_rock_composition_types` returns a `RockCompositionTypes` 
    indicating composition criteria used by `get_rock_type()`.
    We use `get_rock_composition_types()` to simplify code and improve performance.
    */
    template<int M>
    RockCompositionTypes get_rock_composition_types(const Stratum<M>& stratum)
    {
        RockCompositionTypes out;
        float total_mass = stratum.mass();
        out.partly_calcareous =  stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::calcite    )].mass  / total_mass > 0.5;
        out.calcareous        =  stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::calcite    )].mass  / total_mass > 0.75;
        out.silicaceous       =  stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::quartz     )].mass  / total_mass > 0.9;
        out.feldspathic       = (stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::plagioclase)].mass +
                                 stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::orthoclase )].mass) / total_mass > 0.25;
        out.volcanic          = (stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::olivine    )].mass +
                                 stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::pyroxene   )].mass +
                                 stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::plagioclase)].mass +
                                 stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::quartz     )].mass +
                                 stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::orthoclase )].mass) / total_mass > 0.9;
        out.ferrous           = (stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::hematite   )].mass +
                                 stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::pyrite     )].mass)/ total_mass > 0.15;
        out.organic           =  stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::organics   )].mass / total_mass > 0.9;
        return out;
    }

    template<int M>
    StratumTypes get_stratum_types(const Stratum<M>& stratum)
    {
        std::array<float, int(ParticleSizeBins::count)> particle_size_bin_fractional_volume;
        get_particle_size_bin_fractional_volume(stratum, particle_size_bin_fractional_volume);
        
        std::array<float, int(IgneousFormationTypes::count)> igneous_formation_fractional_volume;
        get_igneous_formation_type_fractional_volume(stratum, igneous_formation_fractional_volume);
        
        return StratumTypes(
            get_metamorphic_grades(stratum),
            get_igneous_composition_types(stratum),
            get_igneous_formation_types(igneous_formation_fractional_volume),
            get_dominant_particle_size_bin(particle_size_bin_fractional_volume),
            get_rock_composition_types(stratum)
        );
    }
}