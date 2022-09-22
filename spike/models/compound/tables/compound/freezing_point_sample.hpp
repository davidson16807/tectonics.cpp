#pragma once

// std libraries
#include <vector>

// in-house libraries
#include <units/si.hpp>

#include "../ids.hpp"
#include "../state.hpp"

namespace compound { 
namespace tables { 

    std::vector<state<double>> freezing_point_sample {

        /*water*/            state<double>(si::atmosphere,        si::standard_temperature ),
        /*nitrogen*/         state<double>(si::atmosphere,        -210.0*si::celcius       ),
        /*oxygen*/           state<double>(si::atmosphere,        -218.79*si::celcius      ),
        /*carbon_dioxide*/   state<double>(si::atmosphere,        -56.56 * si::celcius     ),
        /*methane*/          state<double>(si::atmosphere,        -182.46 * si::celcius    ),
        /*argon*/            state<double>(si::atmosphere,        -189.36*si::celcius      ),
        /*helium*/           state<double>(25.0 * si::atmosphere, 0.92778 * si::kelvin     ),
        /*hydrogen*/         state<double>(si::atmosphere,        -259.198*si::celcius     ),
        /*ammonia*/          state<double>(si::atmosphere,        -77.65*si::celcius       ),
        /*ozone*/            state<double>(si::atmosphere,        -193.0*si::celcius       ),
        /*nitrous_oxide*/    state<double>(si::atmosphere,        -102.3*si::celcius       ), // Perry
        /*sulfur_dioxide*/   state<double>(si::atmosphere,        -75.45 * si::celcius     ), // Perry
        /*nitric_oxide*/     state<double>(si::atmosphere,        -163.6*si::celcius       ),
        /*carbon_monoxide*/  state<double>(si::atmosphere,        -205.02*si::celcius      ),
        /*ethane*/           state<double>(si::atmosphere,        -183.79 * si::celcius    ), 
        /*hydrogen_cyanide*/ state<double>(si::atmosphere,        -13.29 * si::celcius     ),
        /*ethanol*/          state<double>(si::atmosphere,        -114.14 * si::celcius    ),
        /*formaldehyde*/     state<double>(si::atmosphere,        181.0 * si::kelvin       ),
        /*formic_acid*/      state<double>(si::atmosphere,        8.3 * si::celcius        ),
        /*perflouromethane*/ state<double>(si::atmosphere,        -183.60*si::celcius      ),
        /*benzene*/          state<double>(si::atmosphere,        5.49 * si::celcius       ), 
        /*pyrimidine*/       state<double>(si::atmosphere,        293.0 * si::kelvin       ), // wikipedia
        /*quartz*/           state<double>(si::atmosphere,        1425.0 * si::celcius     ), // Perry
        /*halite*/           state<double>(si::atmosphere,        800.7*si::celcius        ),
        /*corundum*/         state<double>(si::atmosphere,        2000.0*si::celcius       ), // Perry
        /*apatite*/          state<double>(si::atmosphere,        1610.0*si::celcius       ), // Bhatnagar (1969)
        /*carbon*/           state<double>(si::atmosphere,        3500.0 *si::celcius      ), // Perry, lower bound, for both forms
        /*calcite*/          state<double>(si::atmosphere,        823.0 * si::celcius      ), // for calcite, 1339 for aragonite
        /*orthoclase*/       state<double>(si::atmosphere,        1170.0*si::celcius       ), // Perry
        /*andesine*/         state<double>(si::atmosphere,        1551.0 * si::celcius     ), // Perry, for anothite
        /*augite*/           state<double>(si::atmosphere,        984.0*si::celcius        ), // for Basalt, http://www.minsocam.org/msa/collectors_corner/arc/tempmagmas.htm
        /*forsterite*/       state<double>(si::atmosphere,        1890.0*si::celcius       ), // wikipedia
        /*goethite*/         state<double>(si::atmosphere,        350.0*si::celcius        ), // https://www.chemicalbook.com/ChemicalProductProperty_EN_CB8243072.htm
        /*pyrite*/           state<double>(si::atmosphere,        1180.0 * si::celcius     ), // new world encyclopedia
        /*hematite*/         state<double>(si::atmosphere,        1560.0 * si::celcius     ), // Perry
        /*gold*/             state<double>(si::atmosphere,        1064.180*si::celcius     ),
        /*silver*/           state<double>(si::atmosphere,        961.78*si::celcius       ),
        /*copper*/           state<double>(si::atmosphere,        1084.62*si::celcius      ),
        /*magnetite*/        state<double>(si::atmosphere,        1538.0 * si::celcius     ), // Perry
        /*chalcocite*/       state<double>(si::atmosphere,        1130.0 * si::celcius     ), // wikipedia
        /*chalcopyrite*/     state<double>(si::atmosphere,        950.0 * si::celcius      ), // https://www.chemicalbook.com/ChemicalProductProperty_US_CB4352044.aspx

    };

}}