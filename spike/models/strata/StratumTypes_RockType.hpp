#pragma once

// in-house libraries
#include "enums.hpp"
#include "RockType.hpp"

/*
Contains a large set of enums and functions for classifying individual rock layers
*/
namespace strata
{
    /*
    "get_rock_type" returns the best description possible for rock within a stratum.
    Its output is never used in model behavior,
    and its usage is limited to providing descriptions to the user one stratum at a time,
    so we make the strategic decision to ignore just how absurdly slow or complex it is.
    Its output only describes the fraction of rock that is composed of minerals found on earth.
    For instance, if a rock on an icy moon is a conglomerate of silicates and water ice, 
    then the output will only describe the silicate fraction of the rock,
    and the user interface will describe the rock as being a mixture of that rock type plus water ice.
    */
    RockType get_rock_type(const StratumType& stratum_type) {

        // TODO: metamorphic (overrides igneous and sedimentary)

        // defined by composition
        // NOTE: rocks commonly don't experience the peak pressure at the same time as the peak temperature
        // peak pressure commonly comes before peak temperature
        // We will ignore this both to simplify the model and to reduce the memory footprint of rock strata
        if (metamorphic_grade >= MetamorphicGrades::low)
        {
            // metamorphic rocks

            // special compositions and grades
            // silica,   medium grade or highern f                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
            if (rock_composition_types.silicaceous && metamorphic_grade == MetamorphicGrades::medium)
            {
                return RockType::quartzite; 
            }
            // calcite,  low grade or higher
            else if (rock_composition_types.partly_calcarous && metamorphic_grade == MetamorphicGrades::low)
            {
                return RockType::marble;    
            }
            // organics, low grade or higher
            else if (rock_composition_types.organic && metamorphic_grade == MetamorphicGrades::low)
            {
                return RockType::anthracite;
            }

            // ultramafic facies
            else if(composition_type == IgneousCompositionTypes::ultramafic)
            {
                // ultramafic, low grade
                if (metamorphic_grade == MetamorphicGrades::low)
                {
                    return RockType::serpentinite;
                }
                // ultramafic, medium grade 
                else if (metamorphic_grade == MetamorphicGrades::intermediate)
                {
                    return RockType::soapstone;
                }
                // ultramafic, high grade 
                else if (metamorphic_grade == MetamorphicGrades::high)
                {
                    return RockType::jadeite;
                }
            }

            // sedimentary facies
            // silt or smaller, low grade
            else if (particle_size_bin <= ParticleSizeBins::silt)
            {
                // silt or smaller, low grade
                if (metamorphic_grade == MetamorphicGrades::low)
                {
                    return RockType::slate;
                }
                // silt or smaller, low/medium grade
                else if (metamorphic_grade == MetamorphicGrades::intermediate)
                {
                    return RockType::phyllite;
                }
                // silt or smaller, medium grade
                else if (metamorphic_grade == MetamorphicGrades::high)
                {
                    return RockType::schist;
                }
            }
            // any igneous or sedimentary, high grade
            else if (particle_size_bin <= ParticleSizeBins::sand && metamorphic_grade == MetamorphicGrades::high)
            {
                return RockType::gneiss;
            }
            // granule or larger
            else if (particle_size_bin >= ParticleSizeBins::granule)
            {
                return RockType::metaconglomerate;
            }

            // generic
            else if(metamorphic_facies == MetamorphicFacies::zeolite)
            {
                return RockType::zeolite;
            }
            else if(metamorphic_facies == MetamorphicFacies::hornfels)
            {
                return RockType::hornfels;
            }
            else if(metamorphic_facies == MetamorphicFacies::greenschist)
            {
                return RockType::greenschist;
            }
            else if(metamorphic_facies == MetamorphicFacies::blueschist)
            {
                return RockType::blueschist;
            }
            else if(metamorphic_facies == MetamorphicFacies::sanidinite)
            {
                return RockType::sanidinite;
            }
            else if(metamorphic_facies == MetamorphicFacies::amphibolite)
            {
                return RockType::amphibolite;
            }
            else if(metamorphic_facies == MetamorphicFacies::granulite)
            {
                return RockType::granulite;
            }
            else if(metamorphic_facies == MetamorphicFacies::eclogite)
            {
                return RockType::eclogite;
            }
            else
            {
                return RockType::metamorphic;
            }
        }
        if (metamorphic_grade == MetamorphicGrades::sedimentary)
        {
            //  sedimentary rocks defined by composition and particle size
            if (particle_size_bin <= ParticleSizeBins::silt)
            {
                // quartz,   silt or smaller
                if (rock_composition_types.silicaceous)
                {
                    return RockType::chert;
                }
                // organics, silt or smaller
                else if (rock_composition_types.organic)
                {
                    return RockType::asphalt;
                }
            }
            else if (particle_size_bin <= ParticleSizeBins::sand)
            {
                // feldspar, sand or smaller
                if (rock_composition_types.volcanic && !rock_composition_types.silicaceous)
                {
                    return RockType::tuff;
                }
            }
            else if (particle_size_bin >= ParticleSizeBins::granule)
            {
                // calcite,  granule or larger
                if (rock_composition_types.calcareous)
                {
                    return RockType::coquina;
                }
                // partly calcite, granule or larger
                else if (rock_composition_types.partly_calcarous)
                {
                    return RockType::caliche;
                }
            }
            else if (ParticleSizeBins::sand <= particle_size_bin)
            {
                // organics, sand or granule
                if (rock_composition_types.organic && ParticleSizeBins::sand <= particle_size_bin && particle_size_bin <= ParticleSizeBins::granule)
                {
                    return RockType::peat;
                }
            }
            else if (particle_size_bin >= ParticleSizeBins::pebble)
            {
                // organics, pebble or larger
                if (rock_composition_types.organic)
                {
                    return RockType::jet;
                }
            }
            //  sedimentary rocks defined by composition
            else if (rock_composition_types.calcareous)
            {
                return RockType::limestone; // any carbonate
            }
            // else if (stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::calcite] / total_mass > 0.9)
            // {
            //     return RockType::chalk;     // calcite
            // }
            else if (rock_composition_types.partly_calcarous)
            {
                return RockType::marl;      // partly calcite
            }
            else if (rock_composition_types.organic)
            {
                return RockType::coal;      // organics
            }
            else if (rock_composition_types.feldspathic)
            {
                return RockType::arkose;    // feldspar
            }
            else if (rock_composition_types.ferrous)
            {
                return RockType::ironstone; // iron
            }


            // sedimentary rocks defined by particle size
            else if (ParticleSizeBins::granule  <= particle_size_bin)
            {
                return RockType::breccia;
            }
            // else if (ParticleSizeBins::sand     == particle_size_bin &&
            //     stratum.particle_size_bin_relative_volume[ParticleSizeBins::clay] / total_volume > 0.15 &&
            //     stratum.particle_size_bin_relative_volume[ParticleSizeBins::clay] / total_volume < 0.75)
            // {
            //     return RockType::wacke;
            // }
            else if (ParticleSizeBins::sand     == particle_size_bin)
            {
                return RockType::sandstone;
            }
            else if (ParticleSizeBins::silt     <= particle_size_bin)
            {
                return RockType::siltstone;
            }
            else if (ParticleSizeBins::clay     >= particle_size_bin)
            {
                return RockType::shale;
            }
            else
            {
                return RockType::sedimentary;
            }
        }
        else {
            // igneous rock
            if ( composition_type == IgneousCompositionTypes::ultramafic )
            {
                return formation_type == IgneousFormationTypes::extrusive? RockType::komatiite : RockType::peridotite;
            }
            else if ( composition_type == IgneousCompositionTypes::mafic )
            {
                return formation_type == IgneousFormationTypes::extrusive? RockType::basalt : RockType::gabbro;
            }
            else if ( composition_type == IgneousCompositionTypes::intermediate )
            {
                return formation_type == IgneousFormationTypes::extrusive? RockType::andesite : RockType::diorite;
            }
            else if ( composition_type == IgneousCompositionTypes::felsic )
            {
                return formation_type == IgneousFormationTypes::extrusive? RockType::rhyolite : RockType::granite;
            }
            else
            {
                return RockType::igneous;
            }
        }

    }


}