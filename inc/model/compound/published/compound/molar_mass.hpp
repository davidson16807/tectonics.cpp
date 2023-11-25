#pragma once

// std libraries
#include <vector>

// in-house libraries
#include <units/si.hpp>
#include <model/compound/table/FullTable.hpp>

namespace compound { 
namespace published { 

    table::FullTable<si::molar_mass<double>> molar_mass {
        /*water*/            18.015  * si::gram/si::mole,
        /*nitrogen*/         28.013  * si::gram/si::mole,
        /*oxygen*/           31.9988 * si::gram/si::mole,
        /*carbon_dioxide*/   44.01   * si::gram/si::mole,
        /*methane*/          16.043  * si::gram/si::mole,
        /*argon*/            39.948  * si::gram/si::mole,
        /*helium*/           4.0026  * si::gram/si::mole,
        /*hydrogen*/         2.016   * si::gram/si::mole,
        /*ammonia*/          17.031  * si::gram/si::mole,
        /*ozone*/            47.998  * si::gram/si::mole,
        /*nitrous_oxide*/    44.012  * si::gram/si::mole,
        /*sulfur_dioxide*/   64.064  * si::gram/si::mole,
        /*nitric_oxide*/     30.006  * si::gram/si::mole,
        /*carbon_monoxide*/  28.010  * si::gram/si::mole, 
        /*ethane*/           30.070  * si::gram/si::mole,
        /*hydrogen_cyanide*/ 27.026  * si::gram/si::mole,
        /*ethanol*/          46.068  * si::gram/si::mole,
        /*formaldehyde*/     30.026  * si::gram/si::mole,
        /*formic_acid*/      46.026  * si::gram/si::mole,
        /*perflouromethane*/ 88.0    * si::gram/si::mole,
        /*benzene*/          79.102  * si::gram/si::mole,
        /*pyrimidine*/       80.088  * si::gram/si::mole,
        /*quartz*/           60.08   * si::gram/si::mole, 
        /*halite*/           90.442  * si::gram/si::mole,
        /*corundum*/         101.96  * si::gram/si::mole, 
        /*apatite*/          509.1   * si::gram/si::mole, 
        /*carbon*/           12.011  * si::gram/si::mole, 
        /*calcite*/          100.087 * si::gram/si::mole, 
        /*orthoclase*/       278.33  * si::gram/si::mole, 
        /*andesine*/         268.6   * si::gram/si::mole, 
        /*augite*/           236.4   * si::gram/si::mole, 
        /*forsterite*/       153.31  * si::gram/si::mole, 
        /*goethite*/         88.85   * si::gram/si::mole, 
        /*pyrite*/           119.98  * si::gram/si::mole, 
        /*hematite*/         159.69  * si::gram/si::mole, 
        /*gold*/             196.967 * si::gram/si::mole, 
        /*silver*/           107.868 * si::gram/si::mole, 
        /*copper*/           63.546  * si::gram/si::mole, 
        /*magnetite*/        231.53  * si::gram/si::mole,
        /*chalcocite*/       159.16  * si::gram/si::mole, 
        /*chalcopyrite*/     183.5   * si::gram/si::mole,
    };

}}