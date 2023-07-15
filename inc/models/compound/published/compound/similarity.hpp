
#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/ids.hpp>
#include <models/compound/table/FullTable.hpp>

namespace compound { 

    /*
    water

        hydrogen
            helium
         
        nitrogen
            oxygen
            argon
            ammonia
            ozone
            nitrous_oxide
            sulfur_dioxide
            hydrogen_cyanide
            nitric_oxide

        carbon_dioxide
            carbon_monoxide
            ethane
                ethanol
            formaldehyde
                formic_acid
                benzene
                    pyrimidine
            methane
                perflouromethane
            quartz
                corundum
                    carbon
                orthoclase
                andesine
                augite
                forsterite
                hematite
                goethite
                magnetite
                pyrite
                    chalcocite
                    chalcopyrite
                    copper
                        silver
                        gold

        halite
            calcite
            apatite

    */

    table::FullTable<int> compound_similarity {
        /*water*/            compounds::water,
        /*nitrogen*/         compounds::water,
        /*oxygen*/           compounds::nitrogen,
        /*carbon_dioxide*/   compounds::water,
        /*methane*/          compounds::carbon_dioxide,
        /*argon*/            compounds::helium,
        /*helium*/           compounds::hydrogen,
        /*hydrogen*/         compounds::water,
        /*ammonia*/          compounds::nitrogen,
        /*ozone*/            compounds::oxygen,
        /*nitrous_oxide*/    compounds::nitrogen,
        /*sulfur_dioxide*/   compounds::nitrogen,
        /*nitric_oxide*/     compounds::nitrogen,
        /*carbon_monoxide*/  compounds::carbon_dioxide,
        /*ethane*/           compounds::methane,
        /*hydrogen_cyanide*/ compounds::carbon_dioxide,
        /*ethanol*/          compounds::ethane,
        /*formaldehyde*/     compounds::methane,
        /*formic_acid*/      compounds::formaldehyde,
        /*perflouromethane*/ compounds::methane,
        /*benzene*/          compounds::formaldehyde,
        /*pyrimidine*/       compounds::benzene,
        /*quartz*/           compounds::halite,
        /*halite*/           compounds::water,
        /*corundum*/         compounds::quartz,
        /*apatite*/          compounds::halite,
        /*carbon*/           compounds::corundum,
        /*calcite*/          compounds::halite,
        /*orthoclase*/       compounds::quartz,
        /*andesine*/         compounds::quartz,
        /*augite*/           compounds::quartz,
        /*forsterite*/       compounds::quartz,
        /*goethite*/         compounds::quartz,
        /*pyrite*/           compounds::quartz,
        /*hematite*/         compounds::quartz,
        /*gold*/             compounds::copper,
        /*silver*/           compounds::copper,
        /*copper*/           compounds::chalcopyrite,
        /*magnetite*/        compounds::quartz,
        /*chalcocite*/       compounds::pyrite,
        /*chalcopyrite*/     compounds::pyrite,
    };

}