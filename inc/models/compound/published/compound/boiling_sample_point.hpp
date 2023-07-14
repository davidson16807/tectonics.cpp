#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/ids.hpp>
#include <models/compound/point.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    table::PartialTable<point<double>> boiling_sample_point {

        {compounds::water,             point<double>(si::atmosphere,  100.0*si::celcius)},
        {compounds::nitrogen,          point<double>(si::atmosphere,  -195.8 * si::celcius)}, // Perry
        {compounds::oxygen,            point<double>(si::atmosphere,  -183.0 * si::celcius)}, // Perry
        {compounds::carbon_dioxide,    point<double>(si::atmosphere,  -56.56 * si::celcius)}, // sublimates, equivalent to freezing point
        {compounds::methane,           point<double>(si::atmosphere,  -161.4 * si::celcius)}, // Perry
        {compounds::argon,             point<double>(si::atmosphere,  -185.7*si::celcius)},
        {compounds::helium,            point<double>(si::atmosphere,  -268.9 * si::celcius)}, // Perry
        {compounds::hydrogen,          point<double>(si::atmosphere,  -252.7 * si::celcius)}, // Perry
        {compounds::ammonia,           point<double>(si::atmosphere,  -33.4*si::celcius)}, // Perry
        {compounds::ozone,             point<double>(si::atmosphere,  -112.0 * si::celcius)}, // Perry
        {compounds::nitrous_oxide,     point<double>(si::atmosphere,  -90.7 * si::celcius)}, // Perry
        {compounds::sulfur_dioxide,    point<double>(si::atmosphere,  -10.02 * si::celcius)},
        {compounds::nitric_oxide,      point<double>(si::atmosphere,  -151.76 * si::celcius)}, // Lange
        {compounds::carbon_monoxide,   point<double>(si::atmosphere,  -192.0 * si::celcius)},// Perry
        {compounds::ethane,            point<double>(si::atmosphere,  -88.6 * si::celcius)}, // Perry
        {compounds::hydrogen_cyanide,  point<double>(si::atmosphere,  25.6 * si::celcius)}, // Lange
        {compounds::ethanol,           point<double>(si::atmosphere,  352.2 * si::kelvin)}, // NIST webbook
        {compounds::formaldehyde,      point<double>(si::atmosphere,  -21.0 * si::celcius)}, // Perry
        {compounds::formic_acid,       point<double>(si::atmosphere,  100.8 * si::celcius)}, // Perry
        {compounds::perflouromethane,  point<double>(si::atmosphere,  -128.05*si::celcius)},
        {compounds::benzene,           point<double>(si::atmosphere,  80.1 * si::celcius)}, // Perry
        {compounds::pyrimidine,        point<double>(si::atmosphere,  397.0 * si::kelvin)}, // wikipedia
        {compounds::quartz,            point<double>(si::atmosphere,  2230.0 * si::celcius)}, // Perry
        {compounds::halite,            point<double>(si::atmosphere,  1413.0 * si::celcius)}, // Perry
        {compounds::corundum,          point<double>(si::atmosphere,  2210.0*si::celcius)}, // Perry
        // {compounds::apatite,                            },
        {compounds::carbon,            point<double>(si::atmosphere,  4200.0 *si::celcius)}, // Perry, lower bound, for both forms
        // {compounds::calcite,                            },
        // {compounds::orthoclase,                         },
        // {compounds::andesine,                           },
        // {compounds::augite,                             },
        // {compounds::forsterite,                         },
        // {compounds::goethite,                           },
        // {compounds::pyrite,                             },
        // {compounds::hematite,                           },
        {compounds::gold,              point<double>(si::atmosphere,  2856.0 * si::kelvin)}, // Lange
        {compounds::silver,            point<double>(si::atmosphere,  2164.0 * si::kelvin)}, // Lange
        {compounds::copper,            point<double>(si::atmosphere,  2561.5 * si::kelvin)}, // Lange
        // {compounds::magnetite,                          },
        // {compounds::chalcocite,                         },
        // {compounds::chalcopyrite,                       },

    };

}}