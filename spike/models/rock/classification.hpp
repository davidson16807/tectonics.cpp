#include <Stratum>

/*
Contains a large set of enums and functions for classifying individual rock layers
We place them outside the Strata class to avoid polluting that class namespace.
*/
namespace rock
{

    /*
    // defined in Stratum.hpp, restated here for quick reference:
    enum ParticleSizeBins
    {
        boulder,  // Krumbein phi: -10 to -8
        cobble,   // Krumbein phi: -7 to -5
        pebble,   // Krumbein phi: -4 to -2
        granule,  // Krumbein phi: -1 to 1
        sand,     // Krumbein phi:  2 to 4
        silt,     // Krumbein phi:  5 to 7
        clay,     // Krumbein phi:  8 to 10
        colloid,  // Krumbein phi: 11 to 13
        COUNT
    };
    */

    enum SoilTypes
    {
        sand = 1<<0,
        silt = 1<<1,
        clay = 1<<2,
        loam = 1<<3,
        sand_loam = 1<<0 | 1<<3,
        silt_loam = 1<<1 | 1<<3,
        clay_loam = 1<<2 | 1<<3,
        sand_clay = 1<<0 | 1<<2,
        silt_clay = 1<<1 | 1<<2,
    }

    // NOTE: based off https://upload.wikimedia.org/wikipedia/commons/6/65/SoilTextureTriangle.jpg
    SoilTypes get_soil_type(Stratum& stratum){
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

    enum IgneousCompositionTypes
    {
        ultramafic,
        mafic,
        intermediate,
        felsic,
        other,
        COUNT
    }

    // NOTE: based off https://upload.wikimedia.org/wikipedia/commons/f/f4/Mineralogy_igneous_rocks_EN.svg
    IgneousCompositionTypes get_igneous_composition_types(Stratum& stratum)
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

    enum IgneousFormationTypes
    {
        intrusive,
        extrusive,
        COUNT
    }

    IgneousCompositionTypes get_igneous_formation_types(Stratum& stratum)
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
    MetamorphicFacies get_metamorphic_facies(Stratum& stratum)
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
    enum MetamorphicGrades
    {
        //fascies            //   origin T    origin P    radius T
        igneous_or_sediment, //   0C          0kb                   
        sedimentary,         //   0C          0kb         300K/200MPa
        low,                 //   0C          0kb         600K/300MPa
        intermediate,        //   0C          0kb        1000K/600MPa
        high,                //         everything else
        COUNT
    }
    MetamorphicGrades get_metamorphic_grades(Stratum& stratum)
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
        float distance = length(PT/scale);
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
    ParticleSizeBins get_dominant_particle_size_bin(Stratum& stratum)
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
    `RockCompositionTypes` is a structure composed of booleans 
    that document all composition criteria used within `get_rock_type()`.
    We use `RockCompositionTypes` to simplify code and improve performance.
    */
    struct RockCompositionTypes
    {
        bool partly_calcarous;
        bool calcareous;
        bool silicaceous;
        bool feldspathic;
        bool volcanic;
        bool ferrous;
        bool organic;
    }

    /*
    `get_rock_composition_types` returns a `RockCompositionTypes` 
    indicating composition criteria used by `get_rock_type()`.
    We use `get_rock_composition_types()` to simplify code and improve performance.
    */
    RockCompositionTypes get_rock_composition_types(Stratum& stratum)
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
    "RockTypes" attempts to be a comprehensive collection of every earth-like rock that the model is capable of representing.
    RockTypes is used to quickly describe to the user what type of rock he's looking at.
    RockTypes is only used for descriptive purposes, it is never to be used in model behavior.
    This requirement is needed so that the logic in get_rock_type can grow to whatever absurd proportion is desired,
    while still allowing the rest of the code base to be easily reasoned with.
    RockTypes only describes the fraction of rock that is composed of minerals found on earth.
    For instance, if a rock on an icy moon is a conglomerate of silicates and water ice, 
    then the RockType will only describe the silicate fraction of the rock,
    and the user interface will describe the rock as being a mixture of that rock type plus water ice.
    */
    enum RockTypes
    {
    // IGNEOUS
        komatiite,
        peridotite,

        basalt,
        gabbro,
        
        andesite,
        diorite,
        
        // dacite,
        // ganodiorite,
        
        rhyolite,
        granite

    // SEDIMENTARY
    // NOTE: from http://www.kgs.ku.edu/General/Class/sedimentary.html
        // defined by particle size
        breccia,
        sandstone,
        wacke,
        siltstone,
        shale,

        // defined by composition
        limestone, // any carbonate
        chalk,     // calcite
        marl,      // partly calcite
        coal,      // organics
        arkose,    // feldspar
        ironstone  // iron

        // defined by composition and particle size
        chert,     // quartz,   silt or smaller
        asphalt,   // organics, silt or smaller
        tuff,      // feldspar, sand or smaller
        coquina,   // calcite,  granule or larger
        caliche,   // partly calcite, granule or larger
        peat,      // organics, sand or granule
        jet        // organics, pebble or larger

    // METAMORPHIC
        // generic
        zeolite,
        hornfels,
        // one schist, two schist...
        greenschist,
        blueschist,
        sanidinite,
        amphibolite,
        granulite,
        eclogite,

        // sedimentary based
        slate,    // silt or smaller, low grade
        phyllite, // silt or smaller, low/medium grade
        schist,   // silt or smaller, medium grade
        gneiss,   // any igneous or sedimentary, high grade
        metaconglomerate, // granule or larger

        // ultramafic
        serpentinite, // ultramafic, low grade
        soapstone,    // ultramafic, medium grade 
        jadeite,      // ultramafic, high grade 

        // special compositions
        quartzite, // silica,   medium grade or higher
        marble,    // calcite,  low grade or higher
        anthracite,// organics, low grade or higher
        nephrite,  // quartz/calcite/marl

        // miscellaneous

        // partially metamorphic, partially soidified
        // technically possible to represent but possibly difficult to express within get_rock_type()
        migmatite,
        
        COUNT
    };

    /*
    "get_rock_type_hash" returns a bitfield integer representing all aspects that might distinguish a rock stratum.
    The set of values it represents is a superset of those represented by RockTypes. 
    The function is designed to easily and efficiently determine whether two rock layers should be merged together.
    */
    int get_rock_type_hash(Stratum& stratum)
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
    RockTypes get_rock_type(Stratum& stratum)
    {
        float total_mass = stratum.mass();
        float total_volume = stratum.particle_size_bin_relative_volume_sum();
        IgneousCompositionTypes composition_type = get_igneous_composition_types(stratum);
        IgneousFormationTypes formation_type = get_igneous_formation_types(stratum);
        MetamorphicGrades metamorphic_grade = get_metamorphic_grades(stratum);
        ParticleSizeBins particle_size_bin = get_dominant_particle_size_bin(stratum);
        RockCompositionTypes rock_composition_types = get_rock_composition_types(stratum);

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
                return RockTypes::quartzite; 
            }
            // calcite,  low grade or higher
            else if (rock_composition_types.partly_calcarous && metamorphic_grade == MetamorphicGrades::low)
            {
                return RockTypes::marble;    
            }
            // organics, low grade or higher
            else if (rock_composition_types.organic && metamorphic_grade == MetamorphicGrades::low)
            {
                return RockTypes::anthracite;
            }

            // ultramafic facies
            else if(composition_type == IgneousCompositionTypes::ultramafic)
            {
                // ultramafic, low grade
                if (metamorphic_grade == MetamorphicGrades::low)
                {
                    return RockTypes::serpentinite;
                }
                // ultramafic, medium grade 
                else if (metamorphic_grade == MetamorphicGrades::intermediate)
                {
                    return RockTypes::soapstone;
                }
                // ultramafic, high grade 
                else if (metamorphic_grade == MetamorphicGrades::high)
                {
                    return RockTypes::jadeite;
                }
            }

            // sedimentary facies
            // silt or smaller, low grade
            else if (particle_size_bin <= ParticleSizeBins::silt)
            {
                // silt or smaller, low grade
                if (metamorphic_grade == MetamorphicGrades::low)
                {
                    return RockTypes::slate;
                }
                // silt or smaller, low/medium grade
                else if (metamorphic_grade == MetamorphicGrades::intermediate)
                {
                    return RockTypes::phyllite;
                }
                // silt or smaller, medium grade
                else if (metamorphic_grade == MetamorphicGrades::high)
                {
                    return RockTypes::schist;
                }
            }
            // any igneous or sedimentary, high grade
            else if (particle_size_bin <= ParticleSizeBins::sand && metamorphic_grade == MetamorphicGrades::high)
            {
                return RockTypes::gneiss;
            }
            // granule or larger
            else if (particle_size_bin >= ParticleSizeBins::granule)
            {
                return RockTypes::metaconglomerate;
            }

            // generic
            else if(metamorphic_facies == MetamorphicFacies::zeolite)
            {
                return RockTypes::zeolite;
            }
            else if(metamorphic_facies == MetamorphicFacies::hornfels)
            {
                return RockTypes::hornfels;
            }
            else if(metamorphic_facies == MetamorphicFacies::greenschist)
            {
                return RockTypes::greenschist;
            }
            else if(metamorphic_facies == MetamorphicFacies::blueschist)
            {
                return RockTypes::blueschist;
            }
            else if(metamorphic_facies == MetamorphicFacies::sanidinite)
            {
                return RockTypes::sanidinite;
            }
            else if(metamorphic_facies == MetamorphicFacies::amphibolite)
            {
                return RockTypes::amphibolite;
            }
            else if(metamorphic_facies == MetamorphicFacies::granulite)
            {
                return RockTypes::granulite;
            }
            else if(metamorphic_facies == MetamorphicFacies::eclogite)
            {
                return RockTypes::eclogite;
            }
            else
            {
                return RockTypes::metamorphic;
            }
        }
        if (metamorphic_grade == MetamorphicGrades::sedimentary)
        {
            //  sedimentary rocks defined by composition and particle size
            if (particle_size_bin <= ParticleSizeBins::silt)
            {
                // quartz,   silt or smaller
                if (rock_composition_types.silicaceous && particle_size_bin <= ParticleSizeBins::silt)
                {
                    return RockTypes::chert;
                }
                // organics, silt or smaller
                else if (rock_composition_types.organic && particle_size_bin <= ParticleSizeBins::silt)
                {
                    return RockTypes::asphalt;
                }
            }
            else if (particle_size_bin <= ParticleSizeBins::sand)
            {
                // feldspar, sand or smaller
                if (rock_composition_types.volcanic && particle_size_bin <= ParticleSizeBins::sand)
                {
                    return RockTypes::tuff;
                }
            }
            else if (particle_size_bin >= ParticleSizeBins::granule)
            {
                // calcite,  granule or larger
                if (rock_composition_types.calcareous && particle_size_bin >= ParticleSizeBins::granule)
                {
                    return RockTypes::coquina;
                }
                // partly calcite, granule or larger
                else if (rock_composition_types.partly_calcarous && particle_size_bin >= ParticleSizeBins::granule)
                {
                    return RockTypes::caliche;
                }
            }
            else if (ParticleSizeBins::sand <= particle_size_bin && particle_size_bin <= ParticleSizeBins::granule )
            {
                // organics, sand or granule
                if (rock_composition_types.organic && ParticleSizeBins::sand <= particle_size_bin && particle_size_bin <= ParticleSizeBins::granule)
                {
                    return RockTypes::peat;
                }
            }
            else if (particle_size_bin >= ParticleSizeBins::pebble)
            {
                // organics, pebble or larger
                if (rock_composition_types.organic && particle_size_bin >= ParticleSizeBins::pebble)
                {
                    return RockTypes::jet;
                }
            }
            //  sedimentary rocks defined by composition
            else if (rock_composition_types.calcareous)
            {
                return RockTypes::limestone; // any carbonate
            }
            // else if (stratum.mass_pools[OxygenPlanetStratumMassPoolTypes::calcite] / total_mass > 0.9)
            // {
            //     return RockTypes::chalk;     // calcite
            // }
            else if (rock_composition_types.partly_calcarous)
            {
                return RockTypes::marl;      // partly calcite
            }
            else if (rock_composition_types.organic)
            {
                return RockTypes::coal;      // organics
            }
            else if (rock_composition_types.feldspathic)
            {
                return RockTypes::arkose;    // feldspar
            }
            else if (rock_composition_types.ferrous)
            {
                return RockTypes::ironstone; // iron
            }


            // sedimentary rocks defined by particle size
            else if (ParticleSizeBins::granule  <= particle_size_bin)
            {
                return RockTypes::breccia;
            }
            else if (ParticleSizeBins::sand     == particle_size_bin &&
                stratum.particle_size_bin_relative_volume[ParticleSizeBins::clay] / total_volume > 0.15 &&
                stratum.particle_size_bin_relative_volume[ParticleSizeBins::clay] / total_volume < 0.75)
            {
                return RockTypes::wacke;
            }
            else if (ParticleSizeBins::sand     == particle_size_bin)
            {
                return RockTypes::sandstone;
            }
            else if (ParticleSizeBins::silt     <= particle_size_bin)
            {
                return RockTypes::siltstone;
            }
            else if (ParticleSizeBins::clay     >= particle_size_bin)
            {
                return RockTypes::shale;
            }
            else
            {
                return RockTypes::sedimentary;
            }
        }
        else {
            // igneous rock
            if ( composition_type == IgneousCompositionTypes::ultramafic )
            {
                return formation_type == IgneousFormationTypes::extrusive? RockTypes::komatiite : RockTypes::peridotite;
            }
            else if ( composition_type == IgneousCompositionTypes::mafic )
            {
                return formation_type == IgneousFormationTypes::extrusive? RockTypes::basalt : RockTypes::gabbro;
            }
            else if ( composition_type == IgneousCompositionTypes::intermediate )
            {
                return formation_type == IgneousFormationTypes::extrusive? RockTypes::andesite : RockTypes::diorite;
            }
            else if ( composition_type == IgneousCompositionTypes::felsic )
            {
                return formation_type == IgneousFormationTypes::extrusive? RockTypes::rhyolite : RockTypes::granite;
            }
            else
            {
                return RockTypes::igneous;
            }
        }

    }


}