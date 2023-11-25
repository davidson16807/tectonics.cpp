#pragma once

// in-house libraries
#include <units/si.hpp>

#include <model/compound/table/FullTable.hpp>

namespace compound { 
namespace published { 

    table::FullTable<unsigned int> atoms_per_molecule {
        /*water*/            3u,
        /*nitrogen*/         2u,
        /*oxygen*/           2u,
        /*carbon_dioxide*/   3u,
        /*methane*/          5u,
        /*argon*/            1u,
        /*helium*/           1u,
        /*hydrogen*/         2u,
        /*ammonia*/          4u,
        /*ozone*/            3u,
        /*nitrous_oxide*/    3u,
        /*sulfur_dioxide*/   3u,
        /*nitric_oxide*/     2u,
        /*carbon_monoxide*/  2u,
        /*ethane*/           8u,
        /*hydrogen_cyanide*/ 2u,
        /*ethanol*/          9u,
        /*formaldehyde*/     4u,
        /*formic_acid*/      5u,
        /*perflouromethane*/ 5u,
        /*benzene*/          12u, 
        /*pyrimidine*/       10u, 
        /*quartz*/           3u,
        /*halite*/           2u,
        /*corundum*/         5u,
        /*apatite*/          21u,
        /*carbon*/           1u,
        /*calcite*/          4u,
        /*orthoclase*/       13u,
        /*andesine*/         13u,
        /*augite*/           10u,
        /*forsterite*/       7u,
        /*goethite*/         4u,
        /*pyrite*/           3u,
        /*hematite*/         5u,
        /*gold*/             1u,
        /*silver*/           1u,
        /*copper*/           1u,
        /*magnetite*/        7u,
        /*chalcocite*/       3u,
        /*chalcopyrite*/     3u,
    };

}}