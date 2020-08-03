#pragma once

/*
Contains a large set of enums and functions for classifying individual rock layers
We place them outside the Strata class to avoid polluting that class namespace.
*/
namespace strata
{
    enum struct ParticleSizeBins
    {
        boulder,  // Krumbein phi: -10 to -8
        cobble,   // Krumbein phi: -7 to -5
        pebble,   // Krumbein phi: -4 to -2
        granule,  // Krumbein phi: -1 to 1
        sand,     // Krumbein phi:  2 to 4
        silt,     // Krumbein phi:  5 to 7
        clay,     // Krumbein phi:  8 to 10
        colloid,  // Krumbein phi: 11 to 13
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
        MetamorphicGrades       metamorphic_grades;
        IgneousCompositionTypes igneous_composition_types;
        IgneousFormationTypes   igneous_formation_types;
        ParticleSizeBins        dominant_particle_size_bin;
        RockCompositionTypes    rock_composition_types;

        StratumTypes(
            const MetamorphicGrades metamorphic_grades, 
            const IgneousCompositionTypes igneous_composition_types, 
            const IgneousFormationTypes igneous_formation_types, 
            const ParticleSizeBins dominant_particle_size_bin, 
            const RockCompositionTypes rock_composition_types 
        ) :
            metamorphic_grades(metamorphic_grades), 
            igneous_composition_types(igneous_composition_types), 
            igneous_formation_types(igneous_formation_types), 
            dominant_particle_size_bin(dominant_particle_size_bin), 
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
                * int(MetamorphicGrades::count)       + int(metamorphic_grades)
                * int(IgneousCompositionTypes::count) + int(igneous_composition_types)
                * int(IgneousFormationTypes::count)   + int(igneous_formation_types)
                * int(ParticleSizeBins::count)        + int(dominant_particle_size_bin)
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