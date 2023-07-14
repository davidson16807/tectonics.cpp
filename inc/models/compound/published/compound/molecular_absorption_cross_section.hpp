#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/ids.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    table::PartialTable<relation::PolynomialRailyardRelation<si::wavenumber<double>,si::area<double>,0,1>>  molecular_absorption_cross_section {

        {compounds::water, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{  9.73e1, 6.05e2, 7.37e3, 1.65e4, 2.98e4, 6.50e4, 9.73e4, 1.38e5, 1.62e5, 2.63e5, 3.35e5, 4.39e5, 5.15e5, 5.89e5, 6.93e5, 9.82e5, 1.25e6, 1.64e6, 2.08e6, 2.38e6, 2.41e6, 2.44e6, 2.47e6, 2.53e6, 2.59e6, 2.63e6, 2.73e6, 2.78e6, 2.93e6, 2.98e6, 3.05e6, 3.08e6, 5.11e6, 5.63e6, 6.04e6, 6.45e6, 6.86e6, 8.04e6, 9.68e6, 1.08e7, 1.24e7, 1.37e7, 2.37e7, 3.94e7, 6.98e7, 1.69e8 },
                      std::vector<double>{  -24.98, -24.44, -23.93, -23.46, -23.46, -22.97, -23.70, -23.77, -23.11, -24.44, -22.46, -25.14, -24.47, -25.68, -25.10, -27.10, -28.15, -29.10, -30.25, -29.38, -29.28, -29.28, -29.47, -29.22, -29.47, -29.55, -29.28, -29.21, -29.27, -28.95, -28.71, -28.69, -25.41, -21.62, -21.41, -21.51, -21.76, -21.09, -20.98, -20.74, -20.82, -20.75, -20.83, -21.08, -21.54, -22.44 }),
            
                },
        {compounds::nitrogen, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_output
                    (si::meter, si::meter2,
                     std::vector<double>{ 8.22e6, 9.25e6, 9.94e6, 1.03e7, 1.13e7, 1.21e7, 1.89e7, 3.35e7, 3.93e7, 9.31e7, 1.07e9 },
                     std::vector<double>{ -26.22, -23.04, -20.64, -20.27, -20.29, -20.62, -20.58, -20.93, -20.96, -21.80, -23.38 }),
            
                },
        {compounds::oxygen, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{  5.58e6, 5.72e6, 6.36e6, 6.93e6, 7.26e6, 7.40e6, 7.50e6, 7.65e6, 7.74e6, 7.86e6, 8.05e6, 8.24e6, 8.36e6, 8.43e6, 8.52e6, 8.66e6, 8.80e6, 8.88e6, 8.99e6, 9.06e6, 9.20e6, 9.28e6, 9.75e6, 1.05e7, 1.13e7, 1.22e7, 1.36e7, 1.46e7, 1.83e7, 2.14e7, 3.50e7, 6.28e7, 1.11e8, 4.23e8  },
                      std::vector<double>{  -24.99, -22.55, -21.15, -20.81, -20.86, -21.15, -21.70, -22.61, -22.16, -23.10, -21.82, -24.11, -22.09, -24.11, -21.91, -23.78, -21.50, -23.63, -23.80, -22.07, -22.14, -22.21, -21.36, -21.04, -21.01, -20.68, -20.60, -20.65, -20.59, -20.70, -20.81, -21.28, -21.79, -23.05  }),
            
                },
        {compounds::carbon_dioxide, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{ 8.58e2,  8.58e3,  2.75e4,  3.26e4,  3.78e4,  6.70e4,  8.58e4,  9.53e4,  1.00e5,  1.07e5,  1.11e5,  1.18e5,  1.27e5,  1.32e5,  1.36e5,  1.45e5,  1.53e5,  1.56e5,  1.60e5,  1.66e5,  1.69e5,  1.73e5,  1.87e5,  1.92e5,  2.00e5,  2.23e5,  2.32e5,  2.37e5,  2.40e5,  2.47e5,  2.53e5,  2.60e5,  2.69e5,  2.76e5,  2.88e5,  3.10e5,  3.41e5,  3.59e5,  3.72e5,  3.90e5,  4.30e5,  5.01e5,  5.74e5,  6.20e5,  6.57e5,  6.99e5,  8.13e5,  9.54e5,  1.07e6,  1.29e6,  1.32e6,  2.50e6,  4.95e6,  5.00e6,  6.14e6,  6.73e6,  7.17e6,  7.62e6,  8.39e6,  8.90e6,  9.21e6,  1.11e7,  1.21e7,  1.29e7,  1.52e7,  2.91e7,  4.65e7,  1.26e8,  1.89e8,  2.41e8,  4.42e8 },
                      std::vector<double>{ -33.32,  -35.77,  -38.01,  -37.15,  -32.27,  -22.83,  -28.57,  -26.72,  -28.50,  -26.46,  -30.22,  -31.21,  -28.04,  -30.02,  -28.11,  -32.47,  -32.60,  -33.79,  -36.49,  -37.22,  -35.11,  -32.07,  -28.17,  -26.66,  -28.64,  -25.03,  -22.21,  -21.30,  -26.74,  -27.46,  -30.28,  -28.25,  -31.00,  -29.30,  -32.57,  -29.89,  -28.05,  -24.31,  -23.66,  -28.38,  -32.11,  -24.84,  -31.52,  -26.54,  -30.21,  -26.02,  -31.13,  -28.70,  -37.36,  -29.75,  -35.98,  -35.98,  -29.16,  -27.60,  -22.96,  -22.26,  -22.30,  -22.01,  -23.52,  -20.24,  -21.06,  -20.04,  -20.86,  -20.36,  -20.45,  -20.56,  -20.83,  -21.74,  -22.21,  -21.69,  -21.96 }),
            
                },
        {compounds::methane, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{  9.47e5, 9.92e5, 1.02e6, 1.05e6, 1.07e6, 1.11e6, 1.13e6, 1.14e6, 1.16e6, 1.17e6, 1.19e6, 1.21e6, 1.25e6, 1.28e6, 1.33e6, 1.38e6, 1.40e6, 1.43e6, 1.44e6, 1.48e6, 1.50e6, 1.57e6, 1.61e6, 1.70e6, 1.73e6, 1.80e6, 1.84e6, 1.91e6, 1.96e6, 2.03e6, 2.05e6, 2.10e6, 2.15e6, 2.17e6, 2.25e6, 2.28e6, 2.30e6, 6.56e6, 7.00e6, 7.66e6, 8.65e6, 1.08e7, 2.83e7, 4.90e7, 1.82e8, 2.23e8, 2.38e8, 2.57e8, 3.12e8, 3.24e8, 3.61e8, 3.94e8, 4.45e8, 4.89e8, 5.15e8, 5.95e8, 6.36e8, 7.38e8, 7.91e8, 8.66e8, 9.40e8  },
                      std::vector<double>{  -28.89, -27.28, -27.50, -29.05, -29.13, -27.31, -27.03, -28.16, -27.76, -28.91, -28.60, -30.96, -28.43, -28.67, -29.96, -27.88, -29.30, -28.99, -29.93, -29.91, -29.27, -30.48, -28.66, -31.16, -29.91, -31.27, -29.36, -31.86, -30.34, -32.07, -29.75, -31.26, -31.40, -30.69, -32.07, -30.48, -31.86, -27.37, -22.93, -20.76, -20.71, -20.22, -20.83, -21.53, -22.69, -29.75, -23.23, -21.93, -25.62, -29.16, -30.28, -28.11, -26.80, -30.93, -22.60, -27.39, -31.20, -27.26, -33.23, -27.72, -33.23  }),
            
                },
        {compounds::argon, 1e-35*si::meter2,
            
                },
        {compounds::helium, 1e-35* si::meter2,
            
                },
        {compounds::hydrogen, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{   5e6, 8.06e6, 8.48e6, 9.97e6, 1.05e7, 1.13e7, 1.41e7, 2.66e7, 5.74e7  },
                      std::vector<double>{ -29.5, -23.05, -22.54, -20.41, -20.44, -20.79, -20.97, -21.66, -22.68  }),
            
                },
        {compounds::ammonia, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{  4.35e6, 4.84e6, 5.26e6, 6.14e6, 6.61e6, 7.28e6, 8.03e6, 9.32e6, 1.06e7, 1.23e7, 2.52e7, 5.80e7, 1.93e8  },
                      std::vector<double>{  -26.43, -21.51, -21.17, -21.60, -21.58, -21.00, -20.92, -20.56, -20.76, -20.44, -20.78, -21.60, -22.61  }),
            
                },
        {compounds::ozone, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{   0.0,  2e5,  7e5,  9e5,1.6e6,  2e6,2.5e6,2.8e6,  3e6,3.5e6,4.6e6,  6e6,7.7e6,1.2e7f },
                      std::vector<double>{ -28.0,-26.0,-31.0,-28.0,-24.0,-25.0,-27.0,-24.5,-23.0,-21.0,-22.5,-22.0,-21.0,-21.0  }),
            
                },
        {compounds::nitrous_oxide, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{  3.16e6, 3.29e6, 3.85e6, 4.15e6, 5.10e6, 5.62e6, 5.95e6, 6.29e6, 6.33e6, 6.66e6, 6.91e6, 7.25e6, 7.31e6, 7.73e6, 8.00e6, 9.68e6, 1.07e7, 1.32e7, 1.54e7, 2.82e7, 4.30e7, 7.11e7, 2.34e8  },
                      std::vector<double>{  -28.02, -27.33, -27.38, -26.88, -23.17, -22.83, -23.13, -23.43, -23.43, -21.58, -21.14, -20.67, -22.14, -20.17, -20.53, -20.62, -20.75, -20.37, -20.40, -20.57, -20.85, -21.29, -22.43  }),
            
                },
        {compounds::sulfur_dioxide, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{2.47e6,2.69e6,2.91e6,3.12e6,3.41e6,3.76e6,4.25e6,4.37e6,4.95e6,5.45e6,6.01e6,6.63e6,7.36e6,7.95e6,8.51e6,8.80e6,9.07e6,9.35e6},
                      std::vector<double>{-28.29,-25.86,-25.90,-23.18,-22.04,-22.33,-23.69,-22.55,-20.88,-21.43,-22.48,-21.25,-21.45,-19.92,-21.12,-20.35,-20.88,-20.68}),
            
                },
        {compounds::nitric_oxide, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{4.82e6, 5.61e6, 5.83e6, 6.55e6, 7.23e6, 7.65e6, 8.31e6, 9.94e6, 1.11e7, 1.26e7, 1.67e7, 4.05e7, 7.18e7, 1.85e8},
                      std::vector<double>{-22.20, -21.43, -21.37, -21.56, -21.49, -21.56, -21.57, -20.71, -20.41, -20.71, -20.60, -20.94, -21.42, -22.55}),
            
                },
        {compounds::carbon_monoxide, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{  4.83e6, 4.85e6, 4.88e6, 5.00e6, 5.02e6, 5.05e6, 5.17e6, 5.19e6, 5.22e6, 5.33e6, 5.36e6, 5.38e6, 5.49e6, 5.51e6, 5.55e6, 5.64e6, 5.67e6, 5.68e6, 5.71e6, 6.02e6, 6.85e6, 7.98e6, 8.42e6, 9.28e6, 1.00e7, 1.05e7, 1.13e7, 1.21e7, 1.38e7, 2.10e7, 4.54e7, 5.15e8 },
                      std::vector<double>{  -28.38, -24.93, -28.40, -28.39, -24.91, -28.40, -28.39, -25.16, -28.42, -28.39, -25.52, -28.39, -28.38, -25.72, -28.41, -28.40, -25.96, -23.41, -28.42, -22.47, -20.89, -21.48, -22.01, -20.72, -20.93, -20.48, -20.35, -20.56, -20.56, -20.68, -21.04, -22.55 }),
            
                },
        {compounds::ethane, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{ 5.6e6, 6.25e6, 6.73e6, 7.49e6, 8.23e6, 9.26e6, 1.01e7, 1.14e7, 2.42e7, 5.43e7, 1.72e8 },
                      std::vector<double>{ -35.0, -25.67, -21.86, -20.50, -20.60, -20.27, -20.25, -20.08, -20.57, -21.44, -22.57 }),
            
                },
        {compounds::hydrogen_cyanide, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{  8.50e6, 8.58e6, 9.19e6, 9.59e6, 9.89e6, 9.99e6, 1.01e7, 1.02e7, 1.03e7, 1.05e7, 1.07e7, 1.09e7, 1.26e7, 1.35e7, 1.41e7, 1.46e7, 1.62e7 },
                      std::vector<double>{   -24.2,  -21.8,  -21.5,  -22.1,  -20.3,  -20.6,  -20.3,  -20.7,  -20.2,  -20.2,  -20.6,  -20.3,  -20.5,  -20.3,  -20.4,  -20.4,  -20.5 }),
            
                },
        {compounds::ethanol, missing(),
            
                },
        {compounds::formaldehyde, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{  2.51e6, 2.67e6, 2.70e6, 2.74e6, 2.83e6, 2.86e6, 2.95e6, 2.98e6, 3.06e6, 3.09e6, 3.18e6, 3.62e6, 4.02e6, 4.44e6 },
                      std::vector<double>{   -26.5,  -26.1,  -25.2,  -26.1,  -23.6,  -25.4,  -23.2,  -25.0,  -23.1,  -24.5,  -23.2,  -23.6,  -24.5,  -25.7 }),
            
                },
        {compounds::formic_acid, missing(),
            
                },
        {compounds::perflouromethanemissing,(),
                // // TODO: these numbers are erroneous, we need to revisit them
                // compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                //     ( 1.0/si::meter, si::meter2,
                //       std::vector<double>{  1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 7.99e6, 8.62e6, 9.25e6, 1.00e7, 1.05e7, 1.08e7, 1.13e7, 1.20e7, 1.28e7, 1.33e7, 1.79e7, 2.14e7, 3.37e7, 5.79e7, 1.71e8 },
                //       std::vector<double>{  -20.19, -20.19, -19.97, -19.91, -19.92, -20.05, -20.10, -20.12, -20.17, -23.26, -23.10, -22.58, -21.35, -21.52, -20.18, -20.24, -21.06, -20.16, -20.43, -20.13, -20.31, -20.33, -20.68, -21.63 }),
            
                },
        {compounds::benzene, 
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    ( 1.0/si::meter, si::meter2,
                      std::vector<double>{  3.63e6, 3.73e6, 3.75e6, 3.76e6, 3.82e6, 3.86e6, 3.87e6, 3.92e6, 3.95e6, 3.97e6, 4.03e6, 4.04e6, 4.07e6, 4.12e6, 4.14e6, 4.16e6, 4.21e6, 4.23e6, 4.25e6, 4.33e6, 4.52e6, 4.75e6, 4.91e6, 5.03e6, 5.33e6, 5.58e6, 5.85e6, 6.72e6, 7.54e6, 8.11e6, 8.79e6, 9.59e6, 1.03e7, 1.12e7, 1.41e7, 3.33e7, 2.11e8 },
                      std::vector<double>{   -25.0,  -23.9,  -23.0,  -23.8,  -23.2,  -21.7,  -23.2,  -22.6,  -21.7,  -23.1,  -22.5,  -21.7,  -23.1,  -22.7,  -21.9,  -23.2,  -22.9,  -22.6,  -23.3,  -23.3,  -24.0,  -23.3,  -20.7,  -20.6,  -20.8,  -19.5,  -20.2,  -21.1,  -20.4,  -20.5,  -20.1,  -20.2,  -20.0,  -20.2,  -19.8,  -20.5,  -22.3 }),
            
                },
        {compounds::pyrimidine, missing(),
            
                },
        {compounds::quartz, missing(),
            
                },
        {compounds::halite, missing(),
            
                },
        {compounds::corundum, missing(),
            
                },
        {compounds::apatite, missing(),
            
                },
        {compounds::carbon, missing(),
            
                },
        {compounds::calcite, missing(),
            
                },
        {compounds::orthoclase, missing(),
            
                },
        {compounds::andesine, missing(),
            
                },
        {compounds::augite, missing(),
            
                },
        {compounds::forsterite, missing(),
            
                },
        {compounds::goethite, missing(),
            
                },
        {compounds::pyrite, missing(),
            
                },
        {compounds::hematite, missing(),
            
                },
        {compounds::gold, missing(),
            
                },
        {compounds::silver, missing(),
            
                },
        {compounds::copper, missing(),
            
                },
        {compounds::magnetite, missing(),
            
                },
        {compounds::chalcocite, missing(),
            
                },
        {compounds::chalcopyrite, missing(),
            
                },
    };

}}