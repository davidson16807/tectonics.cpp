#pragma once

/*
Contains a large set of enums and functions for classifying individual rock layers
We place them outside the Strata class to avoid polluting that class namespace.
*/
namespace strata
{
    enum struct ParticleSizeBins
    {
        colloid,  // Krumbein phi: 11 to 13
        clay,     // Krumbein phi:  8 to 10
        silt,     // Krumbein phi:  5 to 7
        sand,     // Krumbein phi:  2 to 4
        granule,  // Krumbein phi: -1 to 1
        pebble,   // Krumbein phi: -4 to -2
        cobble,   // Krumbein phi: -7 to -5
        boulder,  // Krumbein phi: -10 to -8
        count
    };

    enum struct SoilTypes
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
        count = 1<<3 | 1<<2 | 1<<1 | 1<<0
    };

    enum struct IgneousCompositionTypes
    {
        ultramafic,
        mafic,
        intermediate,
        felsic,
        other,
        count
    };

    enum struct IgneousFormationTypes
    {
        intrusive,
        extrusive,
        count
    };

    enum struct MetamorphicGrades
    {
        //fascies            //   origin T    origin P    radius T
        igneous_or_sediment, //   0C          0kb                   
        sedimentary,         //   0C          0kb         300K/200MPa
        low,                 //   0C          0kb         600K/300MPa
        intermediate,        //   0C          0kb        1000K/600MPa
        high,                //         everything else
        count
    };

    enum struct MetamorphicFacies
    {
        igneous_or_sediment,  
        sedimentary,          
        zeolite,              
        magmatism,            
        blueschist,           
        eclogite,             
        hornfels,             
        prehnite_pumpellyte,  
        greenschist,          
        epidote_amphibiolite, 
        amphibolite,         
        granulite,            
        count
    };

    /*
    `RockCompositionTypes` is a structure composed of booleans 
    that document all composition criteria used within `get_rock_type()`.
    We use `RockCompositionTypes` to simplify code and improve performance.
    */
    struct RockCompositionTypes
    {
        bool partly_calcareous;
        bool calcareous;
        bool silicaceous;
        bool feldspathic;
        bool volcanic;
        bool ferrous;
        bool organic;
    };

    struct StratumTypes
    {
        MetamorphicGrades       metamorphic_grade;
        MetamorphicFacies       metamorphic_facies;
        IgneousCompositionTypes igneous_composition_type;
        IgneousFormationTypes   igneous_formation_type;
        ParticleSizeBins        dominant_particle_size_bin;
        SoilTypes               soil_type;
        RockCompositionTypes    rock_composition_types;

        StratumTypes(
            const MetamorphicGrades metamorphic_grade, 
            const MetamorphicFacies metamorphic_facies, 
            const IgneousCompositionTypes igneous_composition_type, 
            const IgneousFormationTypes igneous_formation_type, 
            const ParticleSizeBins dominant_particle_size_bin, 
            const SoilTypes soil_type,
            const RockCompositionTypes rock_composition_types
        ) :
            metamorphic_grade(metamorphic_grade), 
            metamorphic_facies(metamorphic_facies), 
            igneous_composition_type(igneous_composition_type), 
            igneous_formation_type(igneous_formation_type), 
            dominant_particle_size_bin(dominant_particle_size_bin), 
            soil_type(soil_type),
            rock_composition_types(rock_composition_types)
        { }
        /*
        "hash" returns a bitfield integer representing all aspects that might distinguish a rock stratum.
        The set of values it represents is a superset of those represented by RockTypes. 
        The function is designed to easily and efficiently determine whether two rock layers should be merged together.
        */
        int hash()
        {
            return 0
                * int(MetamorphicGrades::count)       + int(metamorphic_grade)
                * int(MetamorphicFacies::count)       + int(metamorphic_facies)
                * int(IgneousCompositionTypes::count) + int(igneous_composition_type)
                * int(IgneousFormationTypes::count)   + int(igneous_formation_type)
                * int(ParticleSizeBins::count)        + int(dominant_particle_size_bin)
                * int(SoilTypes::count)               + int(soil_type)
                * 2 + rock_composition_types.partly_calcareous
                * 2 + rock_composition_types.calcareous
                * 2 + rock_composition_types.silicaceous
                * 2 + rock_composition_types.feldspathic
                * 2 + rock_composition_types.volcanic
                * 2 + rock_composition_types.ferrous
                * 2 + rock_composition_types.organic;
            ;
        }
    };
}