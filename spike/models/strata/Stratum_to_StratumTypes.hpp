#pragma once

// C libraries
#include <cmath>

// 3rd party libraries
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>

// in-house libraries
#include <academics/units.hpp>

#include "Stratum.hpp"
#include "StratumTypes.hpp"
#include "MassPoolTypes.hpp"

/*
Contains a large set of enums and functions for classifying individual rock layers
We place them outside the Strata class to avoid polluting that class namespace.
*/
namespace strata
{

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

    Fascies are represented as spherical regions on a PT diagram for both performance and ease of modeling.
    We check for intersection with them in a given order, and the first match determines the fascies.
    See research/lithosphere/facies.gif for the source diagram that we use derived these numbers.
    We also use this diagram as reason to set an equivalence of 900K = 20kb 
    for determining less well known forms of metamorphism and lithification.
    */
    MetamorphicFacies get_metamorphic_facies(float p, float t)
    {
        glm::vec2 pt_scaled = glm::vec2(p,t) / glm::vec2(2e8*units::pascal, 100*units::kelvin);

        if ( glm::length(pt_scaled-glm::vec2(0,2.65))  < 0.011 )
        {
            return MetamorphicFacies::igneous_or_sediment;
        }
        else if ( glm::length(pt_scaled-glm::vec2(0,2.65))  < 2.0 )
        {
            return MetamorphicFacies::sedimentary;
        }
        else if( glm::length(pt_scaled-glm::vec2(0,2.65))  < 3.0 )
        {
            return MetamorphicFacies::zeolite;
        }
        else if( glm::length(pt_scaled-glm::vec2(9.1,-4.8))  < 11.5 )
        {
            return MetamorphicFacies::blueschist;
        }
        else if( glm::length(pt_scaled-glm::vec2(13.5,7.1))  < 7.4 )
        {
            return MetamorphicFacies::eclogite;
        }
        else if( glm::length(pt_scaled-glm::vec2(-9.2,7.6))  < 10.5 )
        {
            return MetamorphicFacies::hornfels;
        }
        else if( glm::length(pt_scaled-glm::vec2(0.9,-9.2))  < 15.4 )
        {
            return MetamorphicFacies::prehnite_pumpellyte;
        }
        else if( glm::length(pt_scaled-glm::vec2(5.9,-8.15))  < 15.6 )
        {
            return MetamorphicFacies::greenschist;
        }
        else if( glm::length(pt_scaled-glm::vec2(6.6, -11.8))  < 19.8 )
        {
            return MetamorphicFacies::epidote_amphibiolite;
        }
        else if( glm::length(pt_scaled-glm::vec2(6.1,-0.75))  < 10.5 )
        {
            return MetamorphicFacies::amphibolite;
        }
        else
        {
            return MetamorphicFacies::granulite;
        }
    }

    template<int M>
    void get_particle_size_bin_fractional_volume(
        const Stratum<M>& stratum, 
        std::array<float, int(ParticleSizeBins::count)>& output
    ){
        float chemically_susceptible_mass_fraction = 
            (stratum.mass_pools[int(OxygenPlanetMassPoolTypes::calcite)].mass +
             stratum.mass_pools[int(OxygenPlanetMassPoolTypes::organics)].mass) / stratum.mass();

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
        if (stratum.mass_pools[int(OxygenPlanetMassPoolTypes::olivine)].mass / total_mass > 0.3)
        {
            return IgneousCompositionTypes::ultramafic;
        }
        else if (stratum.mass_pools[int(OxygenPlanetMassPoolTypes::pyroxene)].mass / total_mass > 0.3)
        {
            return IgneousCompositionTypes::mafic;
        }
        else if (stratum.mass_pools[int(OxygenPlanetMassPoolTypes::plagioclase)].mass / total_mass > 0.3)
        {
            return IgneousCompositionTypes::intermediate;
        }
        else if ((stratum.mass_pools[int(OxygenPlanetMassPoolTypes::orthoclase)].mass + 
                  stratum.mass_pools[int(OxygenPlanetMassPoolTypes::quartz)].mass) / total_mass > 0.3)
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
        out.partly_calcareous =  stratum.mass_pools[int(OxygenPlanetMassPoolTypes::calcite    )].mass  / total_mass > 0.5;
        out.calcareous        =  stratum.mass_pools[int(OxygenPlanetMassPoolTypes::calcite    )].mass  / total_mass > 0.75;
        out.silicaceous       =  stratum.mass_pools[int(OxygenPlanetMassPoolTypes::quartz     )].mass  / total_mass > 0.9;
        out.feldspathic       = (stratum.mass_pools[int(OxygenPlanetMassPoolTypes::plagioclase)].mass +
                                 stratum.mass_pools[int(OxygenPlanetMassPoolTypes::orthoclase )].mass) / total_mass > 0.25;
        out.volcanic          = (stratum.mass_pools[int(OxygenPlanetMassPoolTypes::olivine    )].mass +
                                 stratum.mass_pools[int(OxygenPlanetMassPoolTypes::pyroxene   )].mass +
                                 stratum.mass_pools[int(OxygenPlanetMassPoolTypes::plagioclase)].mass +
                                 stratum.mass_pools[int(OxygenPlanetMassPoolTypes::quartz     )].mass +
                                 stratum.mass_pools[int(OxygenPlanetMassPoolTypes::orthoclase )].mass) / total_mass > 0.9;
        out.ferrous           = (stratum.mass_pools[int(OxygenPlanetMassPoolTypes::hematite   )].mass +
                                 stratum.mass_pools[int(OxygenPlanetMassPoolTypes::pyrite     )].mass)/ total_mass > 0.15;
        out.organic           =  stratum.mass_pools[int(OxygenPlanetMassPoolTypes::organics   )].mass / total_mass > 0.9;
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
            get_metamorphic_facies(stratum.max_pressure_received, stratum.max_temperature_received),
            get_igneous_composition_types(stratum),
            get_igneous_formation_types(igneous_formation_fractional_volume),
            get_dominant_particle_size_bin(particle_size_bin_fractional_volume),
            get_soil_type(particle_size_bin_fractional_volume),
            get_rock_composition_types(stratum)
        );
    }
}