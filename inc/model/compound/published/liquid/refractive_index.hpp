#pragma once

// in-house libraries
#include <units/si.hpp>
#include <model/compound/ids.hpp>
#include <model/compound/relation/PolynomialRailyardRelation.hpp>
#include <model/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    using LiquidRefractiveIndexWavenumberRelation = relation::PolynomialRailyardRelation<si::wavenumber<double>,double, 0,1>;
    table::PartialTable<LiquidRefractiveIndexWavenumberRelation> refractive_index_as_liquid {
        { compounds::water,       //1.33336,
                relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.2,   0.3,   0.425,  0.55, 0.675,   0.8, 0.925,   1.4,   2.4,   2.8,  3.05,  3.3,   5.6,   6.6,   7.6,  13.0,  15.5,  18.0,  21.0,  26.0,  32.0,  42.0,  60.0, 110.0, 160.0,200.0},
                     std::vector<double>{1.396, 1.349, 1.338, 1.333, 1.331, 1.329, 1.328, 1.321, 1.279, 1.142, 1.426, 1.45, 1.289, 1.334, 1.302, 1.146, 1.297, 1.423, 1.487, 1.539, 1.546, 1.522, 1.703, 1.966, 2.081, 2.13}),
                    // Hale (1973)
            },
        { compounds::nitrogen,          1.19876,
            },
        { compounds::oxygen,            1.2243,
            },
        { compounds::carbon_dioxide,    1.6630,
            },
        { compounds::methane,           1.2730, 
            },
        { compounds::argon,             1.23,
            },
        { compounds::helium,            1.02451,
            // get_dewaele2003_pressure_function
            //     (si::kilobar, 1.0, 0.8034, 0.20256, 1.0, 0.12763, 0.8, 115.0), // Dewaele (2003)
            },
        { compounds::hydrogen,          1.1096,
            // TODO: reimplement this
            // get_dewaele2003_pressure_function
            //     (si::kilobar, 1.0, 0.949, 0.06829, 11.8, 0.2853, 3.0, 49.0), // Dewaele (2003)
            },
        { compounds::ammonia,           1.3944,
                // get_interpolated_temperature_function
                //     (si::celcius, 1.0,
                //      std::vector<double>{ -20.0,  40.0}, 
                //      std::vector<double>{1.3475, 1.321}), // Francis (1960)
            },
        { compounds::ozone,             1.2226,
            },
        { compounds::nitrous_oxide,     1.238,
            },
        { compounds::sulfur_dioxide,    1.3396,
                // get_interpolated_temperature_function
                //     (si::celcius, 1.0,
                //      std::vector<double>{-20.0, 40.0}, 
                //      std::vector<double>{ 1.37, 1.33}), // Francis (1960)
            },
        { compounds::nitric_oxide,      1.330,
            },
        // { compounds::carbon_monoxide,  },
        // { compounds::ethane,           },
        { compounds::hydrogen_cyanide,  1.2614,
            },
        { compounds::ethanol,           1.361,  // wikipedia data page
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
        { compounds::formaldehyde,      1.3714,  // wikipedia
            },
        { compounds::formic_acid,       1.3714,
            },
        // { compounds::perflouromethane, },
        { compounds::benzene,       1.5011,
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
        // { compounds::pyrimidine,       },
        // { compounds::quartz,           },
        // { compounds::halite,           },
        // { compounds::corundum,         },
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