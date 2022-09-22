#pragma once

// std libraries
#include <map>

// in-house libraries
#include <units/si.hpp>

#include "../ids.hpp"

namespace compound { 
namespace tables { 

    std::map<int, si::length<double>> molecular_diameter {
        {ids::water,            265.0 * si::picometer},                                    // wikipedia,  Ismail (2015)
        {ids::nitrogen,         357.8 * si::picometer},                                    // Mehio (2014)
        {ids::oxygen,           334.0 * si::picometer},                                    // Mehio (2014)
        {ids::carbon_dioxide,   346.9 * si::picometer},                                    // Mehio (2014)
        {ids::methane,          404.6 * si::picometer},                                    // Mehio (2014)
        {ids::argon,            340.0 * si::picometer},                                    // wikipedia, Breck (1974)
        {ids::helium,           255.7 * si::picometer}, // Mehio (2014)
        {ids::hydrogen,         287.7 * si::picometer}, // wikipedia,  Ismail (2015)
        {ids::ammonia,          260.0 * si::picometer}, // Breck (1974)
        {ids::ozone,            335.0 * si::picometer}, // Streng (1961)
        {ids::nitrous_oxide,    330.0 * si::picometer}, // wikipedia, Matteucci
        {ids::sulfur_dioxide,   360.0 * si::picometer}, // wikipedia, Breck (1974)
        {ids::nitric_oxide,     317.0 * si::picometer}, // wikipedia, Matteucci
        {ids::carbon_monoxide,  357.0 * si::picometer}, // Mehio (2014)
        {ids::ethane,           443.0 * si::picometer},  // Aguado (2012)
        {ids::hydrogen_cyanide, 376.0 * si::picometer}, // wikipedia, Matteucci
        // {ids::ethanol,          },
        // {ids::formaldehyde,     },
        // {ids::formic_acid,      },
        {ids::perflouromethane, 470.0 * si::picometer}, // Motkuri (2014)
        // {ids::benzene,          },
        // {ids::pyrimidine,       },
        // {ids::quartz,           },
        // {ids::halite,           },
        // {ids::corundum,         },
        // {ids::apatite,          },
        // {ids::carbon,           },
        // {ids::calcite,          },
        // {ids::orthoclase,       },
        // {ids::andesine,         },
        // {ids::augite,           },
        // {ids::forsterite,       },
        // {ids::goethite,         },
        // {ids::pyrite,           },
        // {ids::hematite,         },
        {ids::gold,             2.0 * 174.0 * si::picometer}, // www.webelements.com, from calculated radius
        {ids::silver,           2.0*165.0 * si::picometer}, // www.webelements.com, from calculated radius
        {ids::copper,           2.0 * 145.0 * si::picometer}, // www.webelements.com, from calculated radius
        // {ids::magnetite,        },
        // {ids::chalcocite,       },
        // {ids::chalcopyrite,     },
    };

}}