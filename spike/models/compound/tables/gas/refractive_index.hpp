#pragma once

// std libraries
#include <map>

// in-house libraries
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include "../ids.hpp"

namespace compound { 
namespace tables { 

    std::map<int, relation::PolynomialRailyardRelation<si::wavenumber<double>,double,0,1>> refractive_index_as_gas {
        {ids::water,         1.000261},
        // {ids::nitrogen,      
                // // TODO: reimplement this
                // field::SpectralFunction<double>([](
                //     const si::wavenumber<double> nlo, 
                //     const si::wavenumber<double> nhi, 
                //     const si::pressure<double> p, 
                //     const si::temperature<double> T
                // ) {
                //     double l = (2.0 / (nhi+nlo) / si::micrometer);
                //     double invl2 = 1.0/(l*l);
                //     return 1.0 + 6.497378e-5 + 3.0738649e-2 / (144.0 - invl2);
                // }) // Peck (1966)
            // },
        {ids::oxygen,        1.0002709,
                // // TODO: reimplement this
                // field::SpectralFunction<double>([](
                //     const si::wavenumber<double> nlo, 
                //     const si::wavenumber<double> nhi, 
                //     const si::pressure<double> p, 
                //     const si::temperature<double> T
                // ) {
                //     double l = (2.0 / (nhi+nlo) / si::micrometer);
                //     constexpr double n = 1.0002709f;
                //     constexpr double dndl = -0.00027966;
                //     return n + dndl * l;
                // }) 
            },
        {ids::carbon_dioxide, 1.0004493
                // // TODO: reimplement this
                // field::SpectralFunction<double>([](
                //     const si::wavenumber<double> nlo, 
                //     const si::wavenumber<double> nhi, 
                //     const si::pressure<double> p, 
                //     const si::temperature<double> T
                // ) {
                //     double l = (2.0 / (nhi+nlo) / si::micrometer);
                //     double invl2 = 1.0/(l*l);
                //     return 1.0
                //         + 6.99100e-2/(166.175   - invl2)
                //         + 1.44720e-3/(79.609    - invl2)
                //         + 6.42941e-5/(56.3064   - invl2)
                //         + 5.21306e-5/(46.0196   - invl2)
                //         + 1.46847e-6/(0.0584738 - invl2);
                // }) 
            },
        {ids::methane,       // 1.000444,
                    relation::get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_input
                        (si::micrometer, 1.0,
                         std::vector<double>{    1.67,     2.70,     3.01,     3.66,     3.79,     4.46,     5.66,     6.51,     7.00,     8.38,     8.95,    10.09,    10.86,    11.54,    11.93,    12.37,    13.22,    13.63,    14.02,    14.83},
                         std::vector<double>{1.000430, 1.000425, 1.000417, 1.000440, 1.000437, 1.000431, 1.000427, 1.000419, 1.000402, 1.000466, 1.000451, 1.000445, 1.000442, 1.000443, 1.000440, 1.000441, 1.000440, 1.000439, 1.000444, 1.000439}),
            },
        {ids::argon,       1.000281
                // // TODO: reimplement this
                // field::SpectralFunction<double>([](
                //     const si::wavenumber<double> nlo, 
                //     const si::wavenumber<double> nhi, 
                //     const si::pressure<double> p, 
                //     const si::temperature<double> T
                // ) {
                //     double l = (2.0 / (nhi+nlo) / si::micrometer);
                //     double invl2 = 1.0/(l*l);
                //     return 1.0
                //         + 2.50141e-3/(91.012  - invl2)
                //         + 5.00283e-4/(87.892  - invl2)
                //         + 5.22343e-2/(214.02  - invl2);
                // })
            },
        {ids::helium,       1.000036
                // // TODO: reimplement this
                // field::SpectralFunction<double>([](
                //     const si::wavenumber<double> nlo, 
                //     const si::wavenumber<double> nhi, 
                //     const si::pressure<double> p, 
                //     const si::temperature<double> T
                // ) {
                //     double l = (2.0 / (nhi+nlo) / si::micrometer);
                //     double invl2 = 1.0/(l*l);
                //     return 1.0
                //         + 0.01470091/(423.98  - invl2);
                // }) // Mansfield (1969)
            },
        {ids::hydrogen,       1.0001392
                // // TODO: reimplement this
                // field::SpectralFunction<double>([](
                //     const si::wavenumber<double> nlo, 
                //     const si::wavenumber<double> nhi, 
                //     const si::pressure<double> p, 
                //     const si::temperature<double> T
                // ) {
                //     double l = (2.0 / (nhi+nlo) / si::micrometer);
                //     return 1.0 + 0.0148956/(180.7-1.0/(l*l)) + 0.0049037/(92.0-1.0/(l*l));
                // }) // Peck & Hung (1977)
            },
        {ids::ammonia,       1.000376,
                // // TODO: reimplement this
                // field::SpectralFunction<double>([](
                //     const si::wavenumber<double> nlo, 
                //     const si::wavenumber<double> nhi, 
                //     const si::pressure<double> p, 
                //     const si::temperature<double> T
                // ) {
                //     double l = (2.0 / (nhi+nlo) / si::micrometer);
                //     double invl2 = 1.0/(l*l);
                //     return 1.0
                //         + 0.032953f/(90.392f  - invl2);
                // }) 
            },
        {ids::ozone,                 1.00052},
        {ids::nitrous_oxide,         1.000516},
        {ids::sulfur_dioxide,        1.000686},
        {ids::nitric_oxide,          1.000297},
        {ids::carbon_monoxide,       1.00036320, //https://refractiveindex.info
                // // TODO: reimplement this
                // field::SpectralFunction<double>([](
                //     const si::wavenumber<double> nlo, 
                //     const si::wavenumber<double> nhi, 
                //     const si::pressure<double> p, 
                //     const si::temperature<double> T
                // ) {
                //     double l = (2.0 / (nhi+nlo) / si::micrometer);
                //     constexpr double n = 1.00036350f;
                //     constexpr double dndl = -0.00027275f;
                //     return n + dndl * l;
                // }) 
            },
        {ids::ethane,       1.00075276,
                // // TODO: reimplement this
                // field::SpectralFunction<double>([](
                //     const si::wavenumber<double> nlo, 
                //     const si::wavenumber<double> nhi, 
                //     const si::pressure<double> p, 
                //     const si::temperature<double> T
                // ) {
                //     double l = (2.0 / (nhi+nlo) / si::micrometer);
                //     double invl2 = 1.0/(l*l);
                //     return 1.0007330f
                //         + 6.822764e-6 * invl2;
                // }) 
            },
        // {ids::hydrogen_cyanide, },
        // {ids::ethanol,          },
        // {ids::formaldehyde,     },
        // {ids::formic_acid,      },
        {ids::perflouromethane, 1.0004823},
        // {ids::benzene,          },
        // {ids::pyrimidine,       },
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
        // {ids::gold,             },
        // {ids::silver,           },
        // {ids::copper,           },
        // {ids::magnetite,        },
        // {ids::chalcocite,       },
        // {ids::chalcopyrite,     },
    };

}}