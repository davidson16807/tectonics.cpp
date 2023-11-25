#pragma once

// in-house libraries
#include <unit/si.hpp>
#include <model/compound/ids.hpp>
#include <model/compound/relation/PolynomialRailyardRelation.hpp>
#include <model/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    using GasRefractiveIndexWavenumberRelation = relation::PolynomialRailyardRelation<si::wavenumber<double>,double, 0,1>;
    table::PartialTable<GasRefractiveIndexWavenumberRelation> refractive_index_as_gas {
        {compounds::water,         1.000261},
        // {compounds::nitrogen,      
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
        {compounds::oxygen,        1.0002709,
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
        {compounds::carbon_dioxide, 1.0004493
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
        {compounds::methane,       // 1.000444,
                    relation::get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_input
                        (si::micrometer, 1.0,
                         std::vector<double>{    1.67,     2.70,     3.01,     3.66,     3.79,     4.46,     5.66,     6.51,     7.00,     8.38,     8.95,    10.09,    10.86,    11.54,    11.93,    12.37,    13.22,    13.63,    14.02,    14.83},
                         std::vector<double>{1.000430, 1.000425, 1.000417, 1.000440, 1.000437, 1.000431, 1.000427, 1.000419, 1.000402, 1.000466, 1.000451, 1.000445, 1.000442, 1.000443, 1.000440, 1.000441, 1.000440, 1.000439, 1.000444, 1.000439}),
            },
        {compounds::argon,       1.000281
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
        {compounds::helium,       1.000036
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
        {compounds::hydrogen,       1.0001392
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
        {compounds::ammonia,       1.000376,
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
        {compounds::ozone,                 1.00052},
        {compounds::nitrous_oxide,         1.000516},
        {compounds::sulfur_dioxide,        1.000686},
        {compounds::nitric_oxide,          1.000297},
        {compounds::carbon_monoxide,       1.00036320, //https://refractiveindex.info
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
        {compounds::ethane,       1.00075276,
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
        // {compounds::hydrogen_cyanide, },
        // {compounds::ethanol,          },
        // {compounds::formaldehyde,     },
        // {compounds::formic_acid,      },
        {compounds::perflouromethane, 1.0004823},
        // {compounds::benzene,          },
        // {compounds::pyrimidine,       },
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
        // {compounds::gold,             },
        // {compounds::silver,           },
        // {compounds::copper,           },
        // {compounds::magnetite,        },
        // {compounds::chalcocite,       },
        // {compounds::chalcopyrite,     },
    };

}}