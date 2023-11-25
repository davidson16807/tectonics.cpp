#pragma once

// in-house libraries
#include <units/si.hpp>

#include <model/compound/point.hpp>
#include <model/compound/table/FullTable.hpp>

namespace compound { 
namespace published { 

    table::FullTable<point<double>> freezing_sample_point {

        /*water*/            point<double>(si::atmosphere,        si::standard_temperature ),
        /*nitrogen*/         point<double>(si::atmosphere,        -210.0*si::celcius       ),
        /*oxygen*/           point<double>(si::atmosphere,        -218.79*si::celcius      ),
        /*carbon_dioxide*/   point<double>(si::atmosphere,        -56.56 * si::celcius     ), 
        /*methane*/          point<double>(si::atmosphere,        -182.46 * si::celcius    ),
        /*argon*/            point<double>(si::atmosphere,        -189.36*si::celcius      ),
        /*helium*/           point<double>(25.0 * si::atmosphere, 0.92778 * si::kelvin     ),
        /*hydrogen*/         point<double>(si::atmosphere,        -259.198*si::celcius     ),
        /*ammonia*/          point<double>(si::atmosphere,        -77.65*si::celcius       ),
        /*ozone*/            point<double>(si::atmosphere,        -193.0*si::celcius       ),
        /*nitrous_oxide*/    point<double>(si::atmosphere,        -102.3*si::celcius       ), // Perry
        /*sulfur_dioxide*/   point<double>(si::atmosphere,        -75.45 * si::celcius     ), // Perry
        /*nitric_oxide*/     point<double>(si::atmosphere,        -163.6*si::celcius       ),
        /*carbon_monoxide*/  point<double>(si::atmosphere,        -205.02*si::celcius      ),
        /*ethane*/           point<double>(si::atmosphere,        -183.79 * si::celcius    ), 
        /*hydrogen_cyanide*/ point<double>(si::atmosphere,        -13.29 * si::celcius     ),
        /*ethanol*/          point<double>(si::atmosphere,        -114.14 * si::celcius    ),
        /*formaldehyde*/     point<double>(si::atmosphere,        181.0 * si::kelvin       ),
        /*formic_acid*/      point<double>(si::atmosphere,        8.3 * si::celcius        ),
        /*perflouromethane*/ point<double>(si::atmosphere,        -183.60*si::celcius      ),
        /*benzene*/          point<double>(si::atmosphere,        5.49 * si::celcius       ), 
        /*pyrimidine*/       point<double>(si::atmosphere,        293.0 * si::kelvin       ), // wikipedia
        /*quartz*/           point<double>(si::atmosphere,        1425.0 * si::celcius     ), // Perry
        /*halite*/           point<double>(si::atmosphere,        800.7*si::celcius        ),
        /*corundum*/         point<double>(si::atmosphere,        2000.0*si::celcius       ), // Perry
        /*apatite*/          point<double>(si::atmosphere,        1610.0*si::celcius       ), // Bhatnagar (1969)
        /*carbon*/           point<double>(si::atmosphere,        3500.0 *si::celcius      ), // Perry, lower bound, for both forms
        /*calcite*/          point<double>(si::atmosphere,        823.0 * si::celcius      ), // for calcite, 1339 for aragonite
        /*orthoclase*/       point<double>(si::atmosphere,        1170.0*si::celcius       ), // Perry
        /*andesine*/         point<double>(si::atmosphere,        1551.0 * si::celcius     ), // Perry, for anothite
        /*augite*/           point<double>(si::atmosphere,        984.0*si::celcius        ), // for Basalt, http://www.minsocam.org/msa/collectors_corner/arc/tempmagmas.htm
        /*forsterite*/       point<double>(si::atmosphere,        1890.0*si::celcius       ), // wikipedia
        /*goethite*/         point<double>(si::atmosphere,        350.0*si::celcius        ), // https://www.chemicalbook.com/ChemicalProductProperty_EN_CB8243072.htm
        /*pyrite*/           point<double>(si::atmosphere,        1180.0 * si::celcius     ), // new world encyclopedia
        /*hematite*/         point<double>(si::atmosphere,        1560.0 * si::celcius     ), // Perry
        /*gold*/             point<double>(si::atmosphere,        1064.180*si::celcius     ),
        /*silver*/           point<double>(si::atmosphere,        961.78*si::celcius       ),
        /*copper*/           point<double>(si::atmosphere,        1084.62*si::celcius      ),
        /*magnetite*/        point<double>(si::atmosphere,        1538.0 * si::celcius     ), // Perry
        /*chalcocite*/       point<double>(si::atmosphere,        1130.0 * si::celcius     ), // wikipedia
        /*chalcopyrite*/     point<double>(si::atmosphere,        950.0 * si::celcius      ), // https://www.chemicalbook.com/ChemicalProductProperty_US_CB4352044.aspx

    };

}}