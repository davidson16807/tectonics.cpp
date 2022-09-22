#pragma once

// std libraries
#include <map>

// in-house libraries
#include <units/si.hpp>

#include "../ids.hpp"
#include "../state.hpp"

namespace compound { 
namespace tables { 

    std::map<int, state<double>> freezing_point_sample {

        {ids::water,             state<double>(si::atmosphere,  100.0*si::celcius)},
        {ids::nitrogen,          state<double>(si::atmosphere,  -195.8 * si::celcius)}, // Perry
        {ids::oxygen,            state<double>(si::atmosphere,  -183.0 * si::celcius)}, // Perry
        // {ids::carbon_dioxide,                     },
        {ids::methane,           state<double>(si::atmosphere,  -161.4 * si::celcius)}, // Perry
        {ids::argon,             state<double>(si::atmosphere,  -185.7*si::celcius)},
        {ids::helium,            state<double>(si::atmosphere,  -268.9 * si::celcius)}, // Perry
        {ids::hydrogen,          state<double>(si::atmosphere,  -252.7 * si::celcius)}, // Perry
        {ids::ammonia,           state<double>(si::atmosphere,  -33.4*si::celcius)}, // Perry
        {ids::ozone,             state<double>(si::atmosphere,  -112.0 * si::celcius)}, // Perry
        {ids::nitrous_oxide,     state<double>(si::atmosphere,  -90.7 * si::celcius)},// Perry
        {ids::sulfur_dioxide,    state<double>(si::atmosphere,  -10.02*si::celcius)},
        // {ids::nitric_oxide,                       },
        {ids::carbon_monoxide,   state<double>(si::atmosphere,  -192.0 * si::celcius)},// Perry
        {ids::ethane,            state<double>(si::atmosphere,  -88.6 * si::celcius)}, // Perry
        // {ids::hydrogen_cyanide,                   },
        {ids::ethanol,           state<double>(si::atmosphere,  352.2 * si::kelvin)}, // NIST webbook
        {ids::formaldehyde,      state<double>(si::atmosphere,  -21.0 * si::celcius)}, // Perry
        {ids::formic_acid,       state<double>(si::atmosphere,  100.8 * si::celcius)}, // Perry
        {ids::perflouromethane,  state<double>(si::atmosphere,  -128.05*si::celcius)},
        {ids::benzene,           state<double>(si::atmosphere,  80.1 * si::celcius)}, // Perry
        {ids::pyrimidine,        state<double>(si::atmosphere,  397.0 * si::kelvin)}, // wikipedia
        {ids::quartz,            state<double>(si::atmosphere,  2230.0 * si::celcius)}, // Perry
        {ids::halite,            state<double>(si::atmosphere,  1413.0 * si::celcius)}, // Perry
        {ids::corundum,          state<double>(si::atmosphere,  2210.0*si::celcius)}, // Perry
        // {ids::apatite,                            },
        {ids::carbon,            state<double>(si::atmosphere,  4200.0 *si::celcius)}, // Perry, lower bound, for both forms
        // {ids::calcite,                            },
        // {ids::orthoclase,                         },
        // {ids::andesine,                           },
        // {ids::augite,                             },
        // {ids::forsterite,                         },
        // {ids::goethite,                           },
        // {ids::pyrite,                             },
        // {ids::hematite,                           },
        {ids::gold,              state<double>(si::atmosphere,  3243.0 * si::kelvin)}, // wikipedia
        {ids::silver,            state<double>(si::atmosphere,  2435.0 * si::kelvin)}, // wikipedia
        {ids::copper,            state<double>(si::atmosphere,  2835 * si::kelvin)}, // wikipedia
        // {ids::magnetite,                          },
        // {ids::chalcocite,                         },
        // {ids::chalcopyrite,                       },

    };

}}