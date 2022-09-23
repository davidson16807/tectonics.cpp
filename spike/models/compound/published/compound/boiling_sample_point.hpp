#pragma once

// std libraries
#include <map>

// in-house libraries
#include <units/si.hpp>

#include <models/compound/ids.hpp>
#include <models/compound/point.hpp>

namespace compound { 
namespace published { 

    std::map<int, point<double>> boiling_sample_point {

        {ids::water,             point<double>(si::atmosphere,  100.0*si::celcius)},
        {ids::nitrogen,          point<double>(si::atmosphere,  -195.8 * si::celcius)}, // Perry
        {ids::oxygen,            point<double>(si::atmosphere,  -183.0 * si::celcius)}, // Perry
        // {ids::carbon_dioxide,                     },
        {ids::methane,           point<double>(si::atmosphere,  -161.4 * si::celcius)}, // Perry
        {ids::argon,             point<double>(si::atmosphere,  -185.7*si::celcius)},
        {ids::helium,            point<double>(si::atmosphere,  -268.9 * si::celcius)}, // Perry
        {ids::hydrogen,          point<double>(si::atmosphere,  -252.7 * si::celcius)}, // Perry
        {ids::ammonia,           point<double>(si::atmosphere,  -33.4*si::celcius)}, // Perry
        {ids::ozone,             point<double>(si::atmosphere,  -112.0 * si::celcius)}, // Perry
        {ids::nitrous_oxide,     point<double>(si::atmosphere,  -90.7 * si::celcius)},// Perry
        {ids::sulfur_dioxide,    point<double>(si::atmosphere,  -10.02*si::celcius)},
        // {ids::nitric_oxide,                       },
        {ids::carbon_monoxide,   point<double>(si::atmosphere,  -192.0 * si::celcius)},// Perry
        {ids::ethane,            point<double>(si::atmosphere,  -88.6 * si::celcius)}, // Perry
        // {ids::hydrogen_cyanide,                   },
        {ids::ethanol,           point<double>(si::atmosphere,  352.2 * si::kelvin)}, // NIST webbook
        {ids::formaldehyde,      point<double>(si::atmosphere,  -21.0 * si::celcius)}, // Perry
        {ids::formic_acid,       point<double>(si::atmosphere,  100.8 * si::celcius)}, // Perry
        {ids::perflouromethane,  point<double>(si::atmosphere,  -128.05*si::celcius)},
        {ids::benzene,           point<double>(si::atmosphere,  80.1 * si::celcius)}, // Perry
        {ids::pyrimidine,        point<double>(si::atmosphere,  397.0 * si::kelvin)}, // wikipedia
        {ids::quartz,            point<double>(si::atmosphere,  2230.0 * si::celcius)}, // Perry
        {ids::halite,            point<double>(si::atmosphere,  1413.0 * si::celcius)}, // Perry
        {ids::corundum,          point<double>(si::atmosphere,  2210.0*si::celcius)}, // Perry
        // {ids::apatite,                            },
        {ids::carbon,            point<double>(si::atmosphere,  4200.0 *si::celcius)}, // Perry, lower bound, for both forms
        // {ids::calcite,                            },
        // {ids::orthoclase,                         },
        // {ids::andesine,                           },
        // {ids::augite,                             },
        // {ids::forsterite,                         },
        // {ids::goethite,                           },
        // {ids::pyrite,                             },
        // {ids::hematite,                           },
        {ids::gold,              point<double>(si::atmosphere,  3243.0 * si::kelvin)}, // wikipedia
        {ids::silver,            point<double>(si::atmosphere,  2435.0 * si::kelvin)}, // wikipedia
        {ids::copper,            point<double>(si::atmosphere,  2835 * si::kelvin)}, // wikipedia
        // {ids::magnetite,                          },
        // {ids::chalcocite,                         },
        // {ids::chalcopyrite,                       },

    };

}}