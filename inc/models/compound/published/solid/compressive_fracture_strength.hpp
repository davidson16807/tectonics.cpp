#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    using SolidCompressiveFractureStrengthTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::pressure<double>, 0,1>;
    table::PartialTable<SolidCompressiveFractureStrengthTemperatureRelation> compressive_fracture_strength_as_solid {
        { compounds::water,               6.0 * si::megapascal, //engineering toolbox
                                },
        // { compounds::nitrogen,     
        //                             relation::StateFunction<si::pressure<double>>([](si::pressure<double> p, si::temperature<double> T){ 
        //                                 return math::mix(0.24, 6.00, math::linearstep(58.0, 5.0, T/si::kelvin))*si::megapascal;
        //                             }), // wikipedia, and Yamashita (2010)
        //                         },
        // { compounds::oxygen,           },
        // { compounds::carbon_dioxide,   },
        { compounds::methane,             8.0 * si::megapascal, // Yamashita (2010)
                                },
        // { compounds::argon,            },
        // { compounds::helium,           },
        // { compounds::hydrogen,         },
        { compounds::ammonia,     
                                    relation::get_linear_interpolation_function
                                        (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                                         std::vector<double>{ 77.0, 100.0, 120.0, 130.0, 140.0, 150.0, 160.0}, 
                                         std::vector<double>{580.0, 620.0, 540.0, 450.0, 360.0, 270.0, 190.0}), // Prokhvatilov
                                },
        // { compounds::ozone,            },
        // { compounds::nitrous_oxide,    },
        // { compounds::sulfur_dioxide,   },
        // { compounds::nitric_oxide,     },
        // { compounds::carbon_monoxide,  },
        // { compounds::ethane,           },
        // { compounds::hydrogen_cyanide, },
        // { compounds::ethanol,          },
        // { compounds::formaldehyde,     },
        // { compounds::formic_acid,      },
        // { compounds::perflouromethane, },
        // { compounds::benzene,          },
        // { compounds::pyrimidine,       },
        { compounds::quartz,              1.1 * si::gigapascal, // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
                                 },
        { compounds::halite,              20.0 * si::megapascal, // Bauer (2019)
                                 },
        { compounds::corundum,            2265.0*si::megapascal, //azom.com/article.aspx?ArticleId=1948
                                 },
        // { compounds::apatite,          },
        // { compounds::carbon,           },
        // { compounds::calcite,          },
        // { compounds::orthoclase,       },
        // { compounds::andesine,         },
        // { compounds::augite,           },
        // { compounds::forsterite,       },
        // { compounds::goethite,         },
        // { compounds::pyrite,           },
        // { compounds::hematite,         },
        // { compounds::gold,             },
        // { compounds::silver,           },
        // { compounds::copper,           },
        // { compounds::magnetite,        },
        // { compounds::chalcocite,       },
        // { compounds::chalcopyrite,     },
    };

}}