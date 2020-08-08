#pragma once

namespace stratum
{

    /*
    "RockType" attempts to be a comprehensive collection of every earth-like rock that the model is capable of representing.
    RockType is used to quickly describe to the user what type of rock he's looking at.
    RockType is only used for descriptive purposes, it is never to be used in model behavior.
    This requirement is needed so that the logic in get_rock_type can grow to whatever absurd proportion is desired,
    while still allowing the rest of the code base to be easily reasoned with.
    RockType only describes the fraction of rock that is composed of minerals found on earth.
    For instance, if a rock on an icy moon is a conglomerate of silicates and water ice, 
    then the RockType will only describe the silicate fraction of the rock,
    and the user interface will describe the rock as being a mixture of that rock type plus water ice.
    */
    enum RockType
    {
    // IGNEOUS
        igneous,

        komatiite,
        peridotite,

        basalt,
        gabbro,
        
        andesite,
        diorite,
        
        // dacite,
        // ganodiorite,
        
        rhyolite,
        granite,

    // SEDIMENT
        sediment,

        sand,
        silt,
        clay,
        loam,
        sand_loam,
        silt_loam,
        clay_loam,
        sand_clay,
        silt_clay,

    // SEDIMENTARY
    // NOTE: from http://www.kgs.ku.edu/General/Class/sedimentary.html
    // defined by particle size
        sedimentary,
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
        ironstone, // iron

        // defined by composition and particle size
        chert,     // quartz,   silt or smaller
        asphalt,   // organics, silt or smaller
        tuff,      // feldspar, sand or smaller
        coquina,   // calcite,  granule or larger
        caliche,   // partly calcite, granule or larger
        peat,      // organics, sand or granule
        jet,       // organics, pebble or larger

    // METAMORPHIC
        metamorphic,
        
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
        
        count
    };



}