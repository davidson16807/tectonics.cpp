#pragma once

// in-house libraries
#include <unit/si.hpp>

#include <model/compound/ids.hpp>
#include <model/compound/point.hpp>
#include <model/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    table::PartialTable<point<double>> boiling_sample_point {

        {compounds::water,             point<double>(si::atmosphere,  100.0*si::celsius)},
        {compounds::nitrogen,          point<double>(si::atmosphere,  -195.8 * si::celsius)}, // Perry
        {compounds::oxygen,            point<double>(si::atmosphere,  -183.0 * si::celsius)}, // Perry
        {compounds::carbon_dioxide,    point<double>(si::atmosphere,  -56.56 * si::celsius)}, // sublimates, equivalent to freezing point
        {compounds::methane,           point<double>(si::atmosphere,  -161.4 * si::celsius)}, // Perry
        {compounds::argon,             point<double>(si::atmosphere,  -185.7*si::celsius)},
        {compounds::helium,            point<double>(si::atmosphere,  -268.9 * si::celsius)}, // Perry
        {compounds::hydrogen,          point<double>(si::atmosphere,  -252.7 * si::celsius)}, // Perry
        {compounds::ammonia,           point<double>(si::atmosphere,  -33.4*si::celsius)}, // Perry
        {compounds::ozone,             point<double>(si::atmosphere,  -112.0 * si::celsius)}, // Perry
        {compounds::nitrous_oxide,     point<double>(si::atmosphere,  -90.7 * si::celsius)}, // Perry
        {compounds::sulfur_dioxide,    point<double>(si::atmosphere,  -10.02 * si::celsius)},
        {compounds::nitric_oxide,      point<double>(si::atmosphere,  -151.76 * si::celsius)}, // Lange
        {compounds::carbon_monoxide,   point<double>(si::atmosphere,  -192.0 * si::celsius)},// Perry
        {compounds::ethane,            point<double>(si::atmosphere,  -88.6 * si::celsius)}, // Perry
        {compounds::hydrogen_cyanide,  point<double>(si::atmosphere,  25.6 * si::celsius)}, // Lange
        {compounds::ethanol,           point<double>(si::atmosphere,  352.2 * si::kelvin)}, // NIST webbook
        {compounds::formaldehyde,      point<double>(si::atmosphere,  -21.0 * si::celsius)}, // Perry
        {compounds::formic_acid,       point<double>(si::atmosphere,  100.8 * si::celsius)}, // Perry
        {compounds::perflouromethane,  point<double>(si::atmosphere,  -128.05*si::celsius)},
        {compounds::benzene,           point<double>(si::atmosphere,  80.1 * si::celsius)}, // Perry
        {compounds::pyrimidine,        point<double>(si::atmosphere,  397.0 * si::kelvin)}, // wikipedia
        {compounds::quartz,            point<double>(si::atmosphere,  2230.0 * si::celsius)}, // Perry
        {compounds::halite,            point<double>(si::atmosphere,  1413.0 * si::celsius)}, // Perry
        {compounds::corundum,          point<double>(si::atmosphere,  2210.0*si::celsius)}, // Perry
        // {compounds::apatite,                            },
        {compounds::carbon,            point<double>(si::atmosphere,  4200.0 *si::celsius)}, // Perry, lower bound, for both forms
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