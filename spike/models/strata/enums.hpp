#pragma once

/*
Contains a large set of enums and functions for classifying individual rock layers
We place them outside the Strata class to avoid polluting that class namespace.
*/
namespace strata
{
    enum ParticleSizeType
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

    enum IgneousCompositionTypes
    {
        ultramafic,
        mafic,
        intermediate,
        felsic,
        other,
        COUNT
    }

    enum IgneousFormationTypes
    {
        intrusive,
        extrusive,
        COUNT
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

}