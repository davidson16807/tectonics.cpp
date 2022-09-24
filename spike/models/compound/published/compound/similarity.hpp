
#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/ids.hpp>
#include <models/compound/table/FullTable.hpp>

namespace compound { 
namespace published { 

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

    table::FullTable<int> similarity {
        /*water*/            ids::water,
        /*nitrogen*/         ids::water,
        /*oxygen*/           ids::nitrogen,
        /*carbon_dioxide*/   ids::water,
        /*methane*/          ids::carbon_dioxide,
        /*argon*/            ids::helium,
        /*helium*/           ids::hydrogen,
        /*hydrogen*/         ids::water,
        /*ammonia*/          ids::nitrogen,
        /*ozone*/            ids::oxygen,
        /*nitrous_oxide*/    ids::nitrogen,
        /*sulfur_dioxide*/   ids::nitrogen,
        /*nitric_oxide*/     ids::nitrogen,
        /*carbon_monoxide*/  ids::carbon_dioxide,
        /*ethane*/           ids::methane,
        /*hydrogen_cyanide*/ ids::carbon_dioxide,
        /*ethanol*/          ids::ethane,
        /*formaldehyde*/     ids::methane,
        /*formic_acid*/      ids::formaldehyde,
        /*perflouromethane*/ ids::methane,
        /*benzene*/          ids::formaldehyde,
        /*pyrimidine*/       ids::benzene,
        /*quartz*/           ids::water,
        /*halite*/           ids::quartz,
        /*corundum*/         ids::quartz,
        /*apatite*/          ids::quartz,
        /*carbon*/           ids::quartz,
        /*calcite*/          ids::quartz,
        /*orthoclase*/       ids::quartz,
        /*andesine*/         ids::quartz,
        /*augite*/           ids::quartz,
        /*forsterite*/       ids::quartz,
        /*goethite*/         ids::quartz,
        /*pyrite*/           ids::quartz,
        /*hematite*/         ids::quartz,
        /*gold*/             ids::copper,
        /*silver*/           ids::copper,
        /*copper*/           ids::chalcopyrite,
        /*magnetite*/        ids::quartz,
        /*chalcocite*/       ids::pyrite,
        /*chalcopyrite*/     ids::pyrite,
    };

}}