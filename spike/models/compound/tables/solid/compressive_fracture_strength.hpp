#pragma once

// std libraries
#include <map>

// in-house libraries
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include "../ids.hpp"

namespace compound { 
namespace tables { 

    std::map<int, relation::PolynomialRailyardRelation<si::temperature<double>,si::pressure<double>,0,1>> compressive_fracture_strength_as_solid {
        { ids::water,               6.0 * si::megapascal, //engineering toolbox
                                },
        // { ids::nitrogen,     
        //                             relation::StateFunction<si::pressure<double>>([](si::pressure<double> p, si::temperature<double> T){ 
        //                                 return math::mix(0.24, 6.00, math::linearstep(58.0, 5.0, T/si::kelvin))*si::megapascal;
        //                             }), // wikipedia, and Yamashita (2010)
        //                         },
        // { ids::oxygen,           },
        // { ids::carbon_dioxide,   },
        { ids::methane,             8.0 * si::megapascal, // Yamashita (2010)
                                },
        // { ids::argon,            },
        // { ids::helium,           },
        // { ids::hydrogen,         },
        { ids::ammonia,     
                                    relation::get_linear_interpolation_function
                                        (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                                         std::vector<double>{ 77.0, 100.0, 120.0, 130.0, 140.0, 150.0, 160.0}, 
                                         std::vector<double>{580.0, 620.0, 540.0, 450.0, 360.0, 270.0, 190.0}), // Prokhvatilov
                                },
        // { ids::ozone,            },
        // { ids::nitrous_oxide,    },
        // { ids::sulfur_dioxide,   },
        // { ids::nitric_oxide,     },
        // { ids::carbon_monoxide,  },
        // { ids::ethane,           },
        // { ids::hydrogen_cyanide, },
        // { ids::ethanol,          },
        // { ids::formaldehyde,     },
        // { ids::formic_acid,      },
        // { ids::perflouromethane, },
        // { ids::benzene,          },
        // { ids::pyrimidine,       },
        { ids::quartz,              1.1 * si::gigapascal, // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
                                 },
        { ids::halite,              20.0 * si::megapascal, // Bauer (2019)
                                 },
        { ids::corundum,            2265.0*si::megapascal, //azom.com/article.aspx?ArticleId=1948
                                 },
        // { ids::apatite,          },
        // { ids::carbon,           },
        // { ids::calcite,          },
        // { ids::orthoclase,       },
        // { ids::andesine,         },
        // { ids::augite,           },
        // { ids::forsterite,       },
        // { ids::goethite,         },
        // { ids::pyrite,           },
        // { ids::hematite,         },
        // { ids::gold,             },
        // { ids::silver,           },
        // { ids::copper,           },
        // { ids::magnetite,        },
        // { ids::chalcocite,       },
        // { ids::chalcopyrite,     },
    };

}}