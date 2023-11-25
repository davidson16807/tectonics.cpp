#pragma once

// in-house libraries
#include <unit/si.hpp>

#include <model/compound/ids.hpp>
#include <model/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    table::PartialTable<si::length<double>> molecular_diameter {
        {compounds::water,            265.0 * si::picometer},                                    // wikipedia,  Ismail (2015)
        {compounds::nitrogen,         357.8 * si::picometer},                                    // Mehio (2014)
        {compounds::oxygen,           334.0 * si::picometer},                                    // Mehio (2014)
        {compounds::carbon_dioxide,   346.9 * si::picometer},                                    // Mehio (2014)
        {compounds::methane,          404.6 * si::picometer},                                    // Mehio (2014)
        {compounds::argon,            340.0 * si::picometer},                                    // wikipedia, Breck (1974)
        {compounds::helium,           255.7 * si::picometer}, // Mehio (2014)
        {compounds::hydrogen,         287.7 * si::picometer}, // wikipedia,  Ismail (2015)
        {compounds::ammonia,          260.0 * si::picometer}, // Breck (1974)
        {compounds::ozone,            335.0 * si::picometer}, // Streng (1961)
        {compounds::nitrous_oxide,    330.0 * si::picometer}, // wikipedia, Matteucci
        {compounds::sulfur_dioxide,   360.0 * si::picometer}, // wikipedia, Breck (1974)
        {compounds::nitric_oxide,     317.0 * si::picometer}, // wikipedia, Matteucci
        {compounds::carbon_monoxide,  357.0 * si::picometer}, // Mehio (2014)
        {compounds::ethane,           443.0 * si::picometer},  // Aguado (2012)
        {compounds::hydrogen_cyanide, 376.0 * si::picometer}, // wikipedia, Matteucci
        // {compounds::ethanol,          },
        // {compounds::formaldehyde,     },
        // {compounds::formic_acid,      },
        {compounds::perflouromethane, 470.0 * si::picometer}, // Motkuri (2014)
        // {compounds::benzene,          },
        // {compounds::pyrimidine,       },
        // {compounds::quartz,           },
        // {compounds::halite,           },
        // {compounds::corundum,         },
        // {compounds::apatite,          },
        // {compounds::carbon,           },
        // {compounds::calcite,          },
        // {compounds::orthoclase,       },
        // {compounds::andesine,         },
        // {compounds::augite,           },
        // {compounds::forsterite,       },
        // {compounds::goethite,         },
        // {compounds::pyrite,           },
        // {compounds::hematite,         },
        {compounds::gold,             2.0 * 174.0 * si::picometer}, // www.webelements.com, from calculated radius
        {compounds::silver,           2.0*165.0 * si::picometer}, // www.webelements.com, from calculated radius
        {compounds::copper,           2.0 * 145.0 * si::picometer}, // www.webelements.com, from calculated radius
        // {compounds::magnetite,        },
        // {compounds::chalcocite,       },
        // {compounds::chalcopyrite,     },
    };

}}