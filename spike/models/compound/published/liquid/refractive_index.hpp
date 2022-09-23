#pragma once

// std libraries
#include <map>

// in-house libraries
#include <units/si.hpp>
#include <math/expression/PolynomialRailyard.hpp>
#include <models/compound/relation/Relation.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    using LiquidRefractiveIndexWavenumberRelation = relation::Relation<si::wavenumber<double>,double, math::PolynomialRailyard<float,0,1>>;
    std::map<int, LiquidRefractiveIndexWavenumberRelation> refractive_index_as_liquid {
        { ids::water,       //1.33336,
                relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.2,   0.3,   0.425,  0.55, 0.675,   0.8, 0.925,   1.4,   2.4,   2.8,  3.05,  3.3,   5.6,   6.6,   7.6,  13.0,  15.5,  18.0,  21.0,  26.0,  32.0,  42.0,  60.0, 110.0, 160.0,200.0},
                     std::vector<double>{1.396, 1.349, 1.338, 1.333, 1.331, 1.329, 1.328, 1.321, 1.279, 1.142, 1.426, 1.45, 1.289, 1.334, 1.302, 1.146, 1.297, 1.423, 1.487, 1.539, 1.546, 1.522, 1.703, 1.966, 2.081, 2.13}),
                    // Hale (1973)
            },
        { ids::nitrogen,          1.19876,
            },
        { ids::oxygen,            1.2243,
            },
        { ids::carbon_dioxide,    1.6630,
            },
        { ids::methane,           1.2730, 
            },
        { ids::argon,             1.23,
            },
        { ids::helium,            1.02451,
            // get_dewaele2003_pressure_function
            //     (si::kilobar, 1.0, 0.8034, 0.20256, 1.0, 0.12763, 0.8, 115.0), // Dewaele (2003)
            },
        { ids::hydrogen,          1.1096,
            // TODO: reimplement this
            // get_dewaele2003_pressure_function
            //     (si::kilobar, 1.0, 0.949, 0.06829, 11.8, 0.2853, 3.0, 49.0), // Dewaele (2003)
            },
        { ids::ammonia,           1.3944,
                // get_interpolated_temperature_function
                //     (si::celcius, 1.0,
                //      std::vector<double>{ -20.0,  40.0}, 
                //      std::vector<double>{1.3475, 1.321}), // Francis (1960)
            },
        { ids::ozone,             1.2226,
            },
        { ids::nitrous_oxide,     1.238,
            },
        { ids::sulfur_dioxide,    1.3396,
                // get_interpolated_temperature_function
                //     (si::celcius, 1.0,
                //      std::vector<double>{-20.0, 40.0}, 
                //      std::vector<double>{ 1.37, 1.33}), // Francis (1960)
            },
        { ids::nitric_oxide,      1.330,
            },
        // { ids::carbon_monoxide,  },
        // { ids::ethane,           },
        { ids::hydrogen_cyanide,  1.2614,
            },
        { ids::ethanol,           1.361,  // wikipedia data page
            // // TODO: reimplement this
            // field::SpectralFunction<double>([](
            //     const si::wavenumber<double> nlo, 
            //     const si::wavenumber<double> nhi, 
            //     const si::pressure<double> p, 
            //     const si::temperature<double> T
            // ) {
            //     double l = (2.0 / (nhi+nlo) / si::micrometer);
            //     double invl2 = 1.0/(l*l);
            //     return 1.35265f
            //         + 0.00306 * invl2
            //         + 0.00002 * invl2*invl2;
            // }),
            },
        { ids::formaldehyde,      1.3714,  // wikipedia
            },
        { ids::formic_acid,       1.3714,
            },
        // { ids::perflouromethane, },
        { ids::benzene,       1.5011,
            // // TODO: reimplement this
            // field::SpectralFunction<double>([](
            //     const si::wavenumber<double> nlo, 
            //     const si::wavenumber<double> nhi, 
            //     const si::pressure<double> p, 
            //     const si::temperature<double> T
            // ) {
            //     double l = (2.0 / (nhi+nlo) / si::micrometer);
            //     double l2 = l*l;
            //     double invl2 = 1.0/(l*l);
            //     return sqrt(
            //         2.170184597f
            //         + 0.00059399 * l2
            //         + 0.02303464 * invl2
            //         - 0.000499485 * invl2*invl2
            //         + 0.000178796 * invl2*invl2*invl2
            //     );
            // }),
            },
        // { ids::pyrimidine,       },
        // { ids::quartz,           },
        // { ids::halite,           },
        // { ids::corundum,         },
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