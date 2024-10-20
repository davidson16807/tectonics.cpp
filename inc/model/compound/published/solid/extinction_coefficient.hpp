#pragma once

// in-house libraries
#include <unit/si.hpp>
#include <model/compound/table/PartialTable.hpp>
#include <model/compound/relation/PolynomialRailyardRelation.hpp>
#include <model/compound/relation/GenericRelation.hpp>
#include <model/compound/ids.hpp>

#include "polymorphs.hpp"

namespace compound { 
namespace published { 

    using SolidExtinctionCoefficientWavenumberRelation = relation::GenericRelation<si::spatial_frequency<double>,double>;
    table::PartialTable<SolidExtinctionCoefficientWavenumberRelation> extinction_coefficient_as_solid {

        { polymorphs::water_ice_1h,              
                relation::get_generic_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.0443,   0.0528,   0.067,    0.0886,   0.1181,   0.137,    0.1468,   0.158,    0.177,    0.197,    0.41,     0.51,     0.61,     0.71,     0.81,     0.91,     1.01,     1.11,     1.21,     1.31,     1.41,     1.515,    1.76,     1.923,    2.13,     2.29,     2.52,     2.817,    3.003,    3.175,    3.39,     3.775,    4.56,     5.714,    6.329,    7.143,    8.333,    10.31,    11.9,     14.08,    17.24,    22.22,    34.48,    46.15,    55,       76.29,    210,      140000,   240000,   2000000  },
                     std::vector<double>{0.164,    0.268,    0.492,    0.468,    0.244,    0.271,    0.345,    0.083,    1.061e-05,2.816e-10,2.669e-11,8.036e-10,6.89e-09, 3.44e-08, 1.4e-07,  4.44e-07, 2e-06,    1.76e-06, 8.66e-06, 1.31e-05, 3.442e-05,0.0005143,0.0001522,0.000706, 0.0005255,0.0002538,0.0007685,0.03245,  0.438,    0.372,    0.04286,  0.006966, 0.03007,  0.03256,  0.0617,   0.0494,   0.037,    0.075,    0.409,    0.271,    0.107,    0.029,    0.1615,   0.7684,   0.4214,   0.334,    0.04282,  9.36e-05, 0.0001066,0.0006596}),
                    // Warren (2008)
            },
        // { polymorphs::water_ice_1c,              },
        // { polymorphs::water_ice_2,               },
        // { polymorphs::water_ice_3,               },
        // { polymorphs::water_ice_4,               },
        // { polymorphs::water_ice_5,               },
        // { polymorphs::water_ice_6,               },
        // { polymorphs::water_ice_7,               },
        // { polymorphs::water_ice_8,               },
        // { polymorphs::water_ice_9,               },
        // { polymorphs::water_ice_10,              },
        // { polymorphs::water_ice_11h,             },
        // { polymorphs::water_ice_11o,             },
        // { polymorphs::water_ice_12,              },
        // { polymorphs::water_ice_13,              },
        // { polymorphs::water_ice_14,              },
        // { polymorphs::water_ice_15,              },
        // { polymorphs::water_ice_16,              },
        // { polymorphs::water_ice_17,              },
        // { polymorphs::water_ice_18,              },
        // { polymorphs::water_ice_19,              },
        // { polymorphs::nitrogen_ice_beta,         },
        // { polymorphs::nitrogen_ice_alpha,        },
        // { polymorphs::nitrogen_ice_gamma,        },
        // { polymorphs::oxygen_ice_gamma,          },
        // { polymorphs::oxygen_ice_beta,           },
        // { polymorphs::oxygen_ice_alpha,          },
        { polymorphs::carbon_dioxide_ice,        
                relation::get_generic_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer,1.0,
                     std::vector<double>{  0.051,   0.054,   0.057,   0.067,   0.072,   0.082,   0.086,   0.092,   0.097,   0.105,   0.109,   0.116,   0.123,   0.125,   0.351,   0.482,   0.993,   1.004,   2.443,   2.556,   2.654,   2.684,   2.715,   2.756,   2.757,   2.862,   3.028,   3.086,   3.253,   3.302,   3.327,   3.588,   3.754,   4.017,   4.109,   4.140,   4.219,   4.299,   4.315,   4.464,   4.481,   4.814,   4.980,   5.075,   5.132,   5.289,   5.661,   6.173,   6.483,   6.682,   6.912,   7.097,   7.259,   7.481,   7.595,   7.946,   8.128,   8.408,   8.536,   8.797,   9.101,   9.450,   9.703,   9.925,  10.076,  10.620,  11.537,  11.979,  12.345,  12.819,  13.820,  14.350,  14.901,  15.356,  15.589,  16.309,  19.101,  22.203,  23.938,  24.856,  79.194,  87.333, 100.000, 137.169, 146.780, 160.651}, 
                     std::vector<double>{1.2e-01, 3.1e-01, 5.0e-01, 3.4e-01, 3.4e-01, 7.0e-01, 5.4e-01, 8.5e-01, 7.3e-01, 1.0e+00, 1.1e+00, 2.7e+00, 2.2e-01, 9.9e-03, 6.6e-07, 7.1e-07, 2.2e-06, 2.2e-06, 7.7e-06, 1.2e-05, 2.5e-02, 8.0e-04, 1.5e-03, 2.4e-02, 1.4e-03, 3.8e-05, 8.4e-05, 8.7e-05, 3.3e-05, 5.6e-05, 2.2e-05, 2.4e-06, 2.8e-06, 1.6e-02, 7.7e-03, 1.3e-02, 2.3e+00, 8.1e-02, 1.3e-02, 3.7e-03, 1.1e-03, 8.4e-05, 3.7e-05, 1.7e-04, 3.9e-05, 1.5e-05, 5.0e-06, 2.9e-06, 5.0e-06, 7.6e-05, 1.1e-04, 4.0e-05, 6.2e-05, 1.2e-04, 6.9e-05, 2.4e-05, 3.7e-05, 2.2e-05, 1.0e-05, 3.7e-06, 4.4e-06, 1.1e-05, 7.2e-06, 7.2e-06, 1.1e-05, 2.2e-05, 7.5e-03, 4.6e-03, 4.9e-03, 1.2e-02, 2.3e-03, 7.1e-03, 2.0e+00, 9.5e-02, 2.1e-02, 4.3e-03, 3.9e-04, 1.5e-04, 1.3e-04, 1.6e-04, 3.1e-03, 1.2e-01, 2.2e-03, 3.7e-03, 1.9e-01, 3.5e-03}), // Warren (1986)
            },
        { polymorphs::methane_ice_alpha,         
                relation::get_spectral_linear_interpolation_function_of_wavenumber
                    (1.0/si::centimeter, 1.0,
                     std::vector<double>{1251.37,  1282.10, 1299.67, 1321.62, 1365.53,2959.38, 2994.51, 3012.07, 3042.81, 3126.23, 4017.56},
                     std::vector<double>{   1.503,   1.519,   2.095,   1.522,   1.506,  1.503,   1.538,   1.672,   1.528,   1.503,    1.503}),
            },
        { polymorphs::methane_ice_beta,          
                relation::get_spectral_linear_interpolation_function_of_wavenumber
                    (1.0/si::centimeter, 1.0,
                     std::vector<double>{ 1251.37, 1282.10, 1299.67, 1321.62, 1365.53, 2959.38, 2994.51, 3012.07, 3042.81, 3126.23, 4017.56 },
                     std::vector<double>{   1.503,   1.519,   2.095,   1.522,   1.506,   1.503,   1.538,   1.672,   1.528,   1.503,   1.503 }),
            },
        // { polymorphs::argon_ice,                 },
        // { polymorphs::helium_ice,                },
        // { polymorphs::hydrogen_ice,              },
        { polymorphs::ammonia_ice,               
                relation::get_spectral_linear_interpolation_function_of_wavenumber
                    (1.0/si::centimeter, 1.0,
                     std::vector<double>{   83.333, 144.73, 403.50, 473.68, 561.40, 631.57, 969.29, 1021.93, 1078.94, 1140.35, 1188.59, 1583.33, 1627.19, 1697.36, 3144.73, 3197.36, 3342.10, 3364.03, 3381.57, 3412.28, 4017.54  },
                     std::vector<double>{   1.219,   1.226,  1.395,  1.342,  1.169,  1.125,  1.110,   1.132,   1.530,   1.216,   1.113,   1.129,   1.166,   1.103,   1.097,   1.141,   1.160,   1.282,   1.285,   1.103,   1.100 }),
            },
        // { polymorphs::ozone_ice,                 },
        // { polymorphs::nitrous_oxide_ice,         },
        { polymorphs::sulfur_dioxide_ice,        
                relation::get_spectral_linear_interpolation_function_of_wavenumber
                    (1.0/si::centimeter, 1.0,
                     std::vector<double>{ 405.55, 508.00, 522.95, 525.08, 537.89, 552.83, 569.90, 1135.54, 1150.48, 1163.29, 1171.82, 1299.89, 1321.24, 1323.37, 1336.18, 1361.79, 1381.00, 1483.46, 4032.02 }, 
                     std::vector<double>{  2.601,  2.595,  2.613,  3.052,  3.040,  2.623,  2.601,   2.607,   3.052,   3.043,   2.604,   2.604,   2.724,   3.190,   3.233,   2.653,   2.617,   2.598,   2.598 }),
            },
        // { polymorphs::nitric_oxide_ice,          },
        // { polymorphs::carbon_monoxide_ice_beta,  },
        // { polymorphs::carbon_monoxide_ice_alpha, },
        // { polymorphs::ethane_ice,                },
        // { polymorphs::hydrogen_cyanide_ice,      1.876e-4/si::centimeter, // Moore 2010, 95K 5000 cm-1
        //     },
        // { polymorphs::ethanol_ice,               },
        // { polymorphs::formaldehyde_ice,          },
        // { polymorphs::formic_acid_ice,           },
        // { polymorphs::perflouromethane_ice,      },
        // { polymorphs::benzene_ice,               },
        // { polymorphs::pyrimidine_ice,            },
        // { polymorphs::quartz_alpha,              
        //         relation::get_generic_spectral_linear_interpolation_function_of_wavelength
        //             (si::micrometer, 1.0,
        //              std::vector<double>{    1.5,     7.9,     7.9,     8.6,     8.7,     8.7,     8.7,     8.7,     8.8,     8.8,     8.8,     8.9,     8.9,     8.9,     9.0,     9.1,     9.1,     9.1,     9.2,     9.2,     9.2,     9.3,  9.3,  9.3,  9.4,  9.4,  9.4,  9.5,  9.5,  9.5,  9.6,  9.6,  9.8,  9.9,  9.9, 10.0, 10.0, 10.0, 10.1, 10.1, 10.2, 10.2, 10.2, 10.3, 10.3, 10.4, 14.3},
        //              std::vector<double>{0.00002, 0.12770, 0.15315, 0.91466, 0.94547, 0.98260, 1.02616, 1.07603, 1.13242, 1.19597, 1.26754, 1.34784, 1.43703, 1.53432, 1.84907, 1.94684, 2.03267, 2.10448, 2.16419, 2.21577, 2.26057, 2.29387, 2.30736, 2.29703, 2.26706, 2.22350, 2.16431, 2.08460, 1.99475, 1.89944, 1.78965, 1.66503, 0.92326, 0.80887, 0.70568, 0.61411, 0.53217, 0.45759, 0.38927, 0.32737, 0.27266, 0.22569, 0.18645, 0.15438, 0.12857, 0.10802, 0.06946}),
        //             // Kischkat (2012)
        //     },
        // { polymorphs::quartz_beta,               },
        //         relation::get_generic_spectral_linear_interpolation_function_of_wavelength
        //             (si::micrometer,  37.5 *si::micrometer, 
        //             std::vector<double>{ 0.221, 0.239, 0.263, 0.280, 0.348, 0.416, 0.531, 0.784, 1.862, 1.918, 2.152, 2.193, 2.309, 2.317, 2.475, 2.593, 2.662, 2.692, 2.720, 2.784, 2.848, 2.886, 2.915, 2.985, 3.452, 3.667, 3.721, 3.963, 4.084, 4.192, 4.340, 4.475, 4.596, 4.905, 7.395, 7.624, 7.893, 8.216, 8.526, 8.620, 8.714, 9.239, 9.495, 9.724,10.114,10.531,11.958,12.281,12.402,12.483,12.644,12.806,12.994,13.156,13.600,14.313,14.448,14.757,17.476,17.839,17.920,18.176,18.257,18.445,18.903,19.091,19.333,19.831,20.141,20.572,20.935,21.729,22.025,22.483,22.900,23.331,23.586,23.869,24.057,24.380,24.582,24.703 },
        //             std::vector<double>{ 0.447, 0.465, 0.546, 0.550, 0.710, 0.761, 0.803, 0.831, 0.865, 0.861, 0.879, 0.867, 0.871, 0.876, 0.874, 0.861, 0.867, 0.847, 0.816, 0.720, 0.710, 0.728, 0.696, 0.729, 0.389, 0.352, 0.291, 0.303, 0.271, 0.186, 0.168, 0.071, 0.175, 0.034, 0.009, 0.031, 0.206, 0.791, 0.678, 0.406, 0.597, 0.905, 0.330, 0.210, 0.132, 0.102, 0.049, 0.022, 0.165, 0.198, 0.152, 0.192, 0.125, 0.096, 0.075, 0.053, 0.087, 0.066, 0.028, 0.039, 0.077, 0.195, 0.207, 0.188, 0.099, 0.097, 0.157, 0.684, 0.791, 0.829, 0.802, 0.584, 0.546, 0.309, 0.213, 0.175, 0.167, 0.131, 0.136, 0.104, 0.063, 0.065 }),
        //     },
        // { polymorphs::crystoballite_alpha,       },
        // { polymorphs::crystoballite_beta,        },
        { polymorphs::halite,                    
                relation::get_generic_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 0.22,  26.32,  27.78,  31.25,  33.33,  35.71,  38.46,  41.67,  45.45,  50.00,  55.56,  62.50,  71.43,  83.33, 100.00, 166.67},
                     std::vector<double>{1e-10, 0.014, 0.011, 0.049, 0.102, 0.172, 0.617, 0.792, 1.147, 1.970, 3.582, 1.948, 0.226, 0.095, 0.077, 0.056}),
                    // Querry (1987)
            },
        { polymorphs::corundum,                  
                relation::get_generic_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 0.21,  7.87,  8.62,  9.09,  9.52, 10.75, 10.87, 10.99, 11.11, 11.36, 13.89, 14.08, 14.29, 14.49, 14.71, 14.93, 15.15, 15.38, 15.62, 15.87, 16.13, 16.39, 16.67, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 21.74, 22.22, 22.73, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 30.30, 31.25, 32.26, 33.33, 34.48, 41.67, 55.56},
                     std::vector<double>{  1e-10, 0.044, 0.068, 0.080, 0.089, 0.139, 0.166, 0.239, 0.355, 0.608, 2.178, 2.311, 2.466, 2.645, 2.838, 3.049, 3.326, 3.683, 4.250, 3.460, 3.626, 4.260, 4.919, 5.835, 6.996, 8.491, 3.839, 1.299, 0.642, 0.420, 0.480, 0.662, 0.591, 0.573, 1.220, 2.824, 4.730, 7.373,11.173, 1.231, 0.312, 0.108, 0.066, 0.606, 1.312, 0.108,1e-10,1e-10, 0.010, 0.046, 0.019, 0.013, 0.077, 0.013, 0.011}),
                    // Querry (1985)
            },
        // { polymorphs::apatite,                   },
        { polymorphs::graphite,                  
                relation::get_generic_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.2, 0.24, 0.25, 0.26, 0.28, 0.29, 2.7322, 3.7736, 3.7879, 4.1841, 4.878, 5, 5.0251, 5.7143, 6.25, 6.2893, 6.3291, 6.3694, 6.4103, 8.1301, 8.9286, 10.101, 10.2041, 11.236, 11.3636, 11.4943, 11.6279, 11.7647, 12.5, 12.6582, 14.7059, 15.625, 15.873, 19.2308, 20, 20.4082, 20.8333, 21.7391, 22.7273, 23.2558, 24.3902, 25, 27.7778, 28.5714, 30.303, 31.25, 33.3333, 34.4828, 35.7143, 37.037, 38.4615, 41.6667, 43.4783, 45.4545, 47.619, 50, 52.6316, 55.5556},
                     std::vector<double>{0.723, 1.162, 1.221, 1.195, 1.061, 1.009, 2.395, 2.762, 2.785, 2.91, 3.124, 3.154, 3.179, 3.254, 3.312, 3.347, 3.343, 3.323, 3.316, 3.489, 3.598, 3.774, 3.776, 3.927, 3.934, 3.979, 4.009, 4.015, 4.153, 4.168, 4.541, 4.705, 4.784, 5.434, 5.606, 5.702, 5.823, 6.02, 6.281, 6.393, 6.645, 6.755, 7.307, 7.462, 7.891, 8.052, 8.482, 8.664, 8.868, 9.1, 9.171, 9.559, 9.451, 9.974, 10.13, 10.524, 10.803, 10.332}),
                    // Querry (1985)
            },
        { polymorphs::diamond,                   
                relation::get_generic_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.035, 0.056, 0.057, 0.058, 0.059, 0.060, 0.063, 0.087, 0.090, 0.091, 0.093, 0.094, 0.095, 0.097, 0.098, 0.100, 0.102, 0.103, 0.105, 0.107, 0.109, 0.111, 0.113, 0.115, 0.117, 0.119, 0.129, 0.163, 0.168, 0.172, 0.177, 0.182,10.000},
                     std::vector<double>{0.242, 0.858, 0.956, 1.023, 1.046, 1.120, 1.241, 2.111, 2.257, 2.354, 2.466, 2.608, 2.777, 3.017, 3.372, 3.511, 3.516, 3.220, 2.970, 2.675, 2.398, 2.174, 1.919, 1.749, 1.607, 1.449, 1.071, 0.614, 0.507, 0.386, 0.088, 0.029, 0.000}),
                    // Philip (1964)
            },
        // { polymorphs::calcite,                   },
        // { polymorphs::aragonite,                 },
        // { polymorphs::orthoclase,                },
        // { polymorphs::andesine,                  },
        // { polymorphs::augite,                    },
        // { polymorphs::forsterite,                },
        // { polymorphs::goethite,                  },
        // { polymorphs::pyrite,                    },
        { polymorphs::hematite,                  
                relation::get_generic_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 0.21,  0.22,  0.23,  0.26,  0.27,  0.29,  0.30,  0.31,  0.32,  0.34,  0.35,  0.36,  0.38,  0.39,  0.40,  0.41,  0.42,  0.43,  0.50,  0.51,  0.52,  0.54,  0.55,  0.56,  0.59,  0.60, 14.49, 14.71, 14.93, 15.15, 15.38, 16.13, 16.39, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 21.74, 22.22, 22.73, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 32.26, 33.33, 34.48, 35.71, 37.04, 38.46, 40.00, 41.67, 43.48, 45.45, 47.62, 50.00, 52.63, 90.91},
                     std::vector<double>{ 1.207,  1.291,  1.309,  1.317,  1.300,  1.233,  1.208,  1.195,  1.169,  1.085,  1.093,  1.140,  1.258,  1.291,  1.294,  1.271,  1.231,  1.173,  0.675,  0.622,  0.587,  0.538,  0.498,  0.437,  0.202,  0.149,  0.297,  0.350,  0.440,  0.586,  0.779,  1.406,  1.624,  2.096,  2.358,  2.669,  3.035,  3.436,  3.791,  3.911,  3.368,  2.292,  2.054,  2.553,  3.328,  4.068,  4.993,  6.303,  6.991,  4.077,  1.587,  0.982,  0.718,  0.656,  0.803,  1.390,  2.254,  3.245,  4.284,  5.382,  6.922,  9.062, 11.145,  8.306,  2.150,  0.782,  0.389,  0.132,  0.601,  0.668,  0.030, 1e-10, 1e-10, 1e-10, 1e-10}),
                    // Querry (1985)
            },
        { polymorphs::gold,                      
                relation::get_generic_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{8.266E-06,1.494E-05,1.55E-05, 2.48E-05, 4.133E-05,6.199E-05,8.266E-05,8.856E-05,9.537E-05,0.000124, 0.000248, 0.0004275,0.0004959,0.0006199,0.00124,  0.00155,  0.002066, 0.00248,  0.003542, 0.004133, 0.005166, 0.005636, 0.007749, 0.008856, 0.009184, 0.01033,  0.01127,  0.01181,  0.01393,  0.01409,  0.01459,  0.01467,  0.01476,  0.01485,  0.01494,  0.01512,  0.01722,  0.02,     0.02214,  0.02384,  0.02638,  0.03024,  0.03444,  0.03542,  0.03757,  0.03999,  0.04065,  0.04133,  0.04275,  0.04428,  0.04592,  0.04959,  0.05166,  0.0551,   0.05636,  0.05904,  0.06199,  0.06888,  0.07749,  0.08266,  0.09537,  0.1078,   0.1127,   0.1378,   0.155,    0.1653,   0.1771,   0.2066,   0.2254,   0.248,    0.2818,   0.2952,   0.3263,   0.3444,   0.3757,   0.3936,   0.4428,   0.4769,   0.5166,   0.6199,   0.8266,   1.24,     2.48,     12.4,     24.8,     124.0,    248.0},
                     std::vector<double>{2.56E-09, 1.95E-08, 5.22E-09, 3.05E-08, 1.85E-07, 8.83E-07, 2.28E-06, 1.94E-06, 2.56E-06, 2.53E-06, 2.87E-05, 0.000164, 0.000211, 0.00012,  0.00096,  0.00163,  0.0034,   0.00471,  0.00731,  0.00895,  0.0113,   0.0118,   0.0101,   0.0109,   0.0118,   0.0178,   0.0283,   0.0353,   0.0807,   0.0838,   0.0956,   0.0962,   0.0938,   0.0943,   0.0963,   0.101,    0.14,     0.211,    0.23,     0.255,    0.315,    0.415,    0.54,     0.573,    0.672,    0.788,    0.799,    0.804,    0.802,    0.814,    0.855,    1,        1.12,     1.28,     1.29,     1.21,     1.07,     0.865,    0.898,    0.934,    0.899,    0.92,     0.945,    1.14,     1.22,     1.15,     1.13,     1.3,      1.45,     1.65,     1.87,     1.89,     1.86,     1.81,     1.88,     1.89,     1.81,     1.71,     2.01,     3.31,     5.13,     8.36,     18.1,     80.7,     124.0,    256.0,    379.0}),
                // Hagemann (1974)
            },
        { polymorphs::silver,                    
                relation::get_generic_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{2.48e-06, 6.199e-06,1.24e-05, 1.653e-05,2.48e-05, 3.1e-05, 4.133e-05,4.769e-05,4.959e-05,6.199e-05,0.000124, 0.000155, 0.0002066,0.000248, 0.0006199,0.0008266,0.00155,0.001675, 0.002066, 0.00248,  0.0031, 0.003351, 0.003542,0.005166, 0.005636, 0.006199, 0.006888, 0.007749, 0.008266, 0.01033,0.01127, 0.01305, 0.0155, 0.01675, 0.01823, 0.01879,  0.02, 0.02296, 0.02384, 0.02638, 0.02695,  0.02952,  0.031, 0.03351,  0.03444,  0.03647,  0.03757, 0.04428,  0.04509, 0.05166,0.05276,  0.05391,  0.0551, 0.05904, 0.06199,0.07749,  0.08266, 0.08551, 0.08856,0.09537,  0.1127,   0.1378,   0.1653,  0.1907,   0.2254,   0.261,   0.2755,   0.2883,   0.2952,   0.31,   0.3179,   0.3289, 0.4133,  0.4959,  0.6199, 0.8266,   1.24,    2.48,     3.1,   4.133,   6.199,    12.4,     24.8,    49.59,    124.0,    248.0    },
                     std::vector<double>{3.98e-12, 1.15e-10, 1.49e-09, 4.33e-09, 1.92e-08, 4.64e-08,1.32e-07, 2.11e-07,  3.63e-08, 9.57e-08,1.29e-06, 2.92e-06,  8.36e-06,1.52e-05, 4.72e-05,  0.000121,0.00108, 0.00142,  0.00269, 0.00457, 0.00882,  0.00823,  0.00168,  0.0035,  0.00345,  0.00377,  0.00431,  0.00438,  0.00407, 0.00622, 0.0172,  0.0565,  0.139,   0.166,   0.208,   0.211, 0.211,   0.274,   0.276,   0.317,   0.323,    0.349,  0.368,     0.4,    0.418,    0.489,    0.514,    0.59,    0.616,   0.904,   0.93,    0.943,   0.935,   0.745,   0.714,   0.91,    0.918,   0.875,   0.783,   0.64,   0.553,    0.562,    0.911,    1.18,     1.34,    1.34,     1.28,     1.13,     1.06,    0.599,   0.36,    0.399,   2.28,    3.09,    4.18,    5.8,   9.02,    18.8,    22.9,    30.5,    45.7,    90.2,    173.0,    306.0,    506.0,    689.0    }),
                // Hagemann (1974)
            },
        { polymorphs::copper,                    
                relation::get_generic_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{2.5e-05, 4.1e-03, 6.2e-03, 8.3e-03, 1.0e-02, 3.9e-02, 4.0e-02, 4.1e-02, 4.3e-02, 4.4e-02, 4.6e-02, 4.8e-02, 5.0e-02, 5.2e-02, 5.4e-02, 5.6e-02, 5.9e-02, 6.2e-02, 6.9e-02, 7.3e-02, 7.7e-02, 8.0e-02, 8.9e-02, 9.5e-02, 1.0e-01, 1.1e-01, 1.2e-01, 1.3e-01, 1.5e-01, 1.55e-01, 1.7e-01, 1.8e-01, 1.9e-01, 2.1e-01, 2.15e-01, 2.2e-01, 2.3e-01, 2.4e-01, 2.5e-01, 2.6e-01, 2.7e-01, 2.8e-01, 3.0e-01, 3.1e-01, 3.3e-01, 3.4e-01, 3.6e-01, 3.9e-01, 4.1e-01, 4.4e-01, 4.8e-01, 5.2e-01, 5.4e-01, 5.6e-01, 5.9e-01, 6.5e-01, 7.3e-01, 8.3e-01, 1.2e+00, 2.5e+00, 1.2e+01, 2.5e+01, 1.2e+02, 2.5e+02},
                     std::vector<double>{4.5e-09, 5.1e-03, 1.6e-02, 3.1e-02, 5.4e-02, 2.2e-01, 2.4e-01, 2.6e-01, 3.0e-01, 3.5e-01, 3.8e-01, 4.0e-01, 4.0e-01, 3.7e-01, 3.7e-01, 3.8e-01, 4.1e-01, 4.6e-01, 5.6e-01, 6.2e-01, 6.7e-01, 6.9e-01, 7.2e-01, 7.2e-01, 7.1e-01, 7.5e-01, 8.2e-01, 8.7e-01, 9.8e-01, 1.0e+00,  1.1e+00, 1.2e+00, 1.4e+00, 1.6e+00, 1.7e+00,  1.7e+00, 1.8e+00, 1.8e+00, 1.8e+00, 1.7e+00, 1.7e+00, 1.6e+00, 1.6e+00, 1.7e+00, 1.8e+00, 1.9e+00, 1.9e+00, 2.1e+00, 2.2e+00, 2.4e+00, 2.5e+00, 2.6e+00, 2.6e+00, 2.6e+00, 2.8e+00, 3.7e+00, 4.4e+00, 5.3e+00, 8.5e+00, 1.8e+01, 7.1e+01, 1.0e+02, 2.1e+02, 2.9e+02}),
                // Hagemann (1974)
            },
        { polymorphs::magnetite,                 
                relation::get_generic_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 0.21,  0.22,  2.83,  2.96,  9.90, 15.38, 15.62, 16.13, 16.39, 16.67, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 32.26, 33.33, 34.48, 35.71, 37.04, 38.46, 40.00, 41.67, 43.48, 45.45, 47.62, 50.00, 52.63, 55.56},
                     std::vector<double>{0.085, 0.109, 1.194, 1.180, 1.464, 2.348, 2.427, 2.626, 2.761, 2.963, 3.249, 3.642, 3.907, 3.604, 2.966, 2.540, 2.383, 2.348, 2.365, 2.402, 2.460, 2.959, 3.078, 3.224, 3.413, 3.655, 3.958, 4.392, 4.940, 5.210, 4.290, 3.268, 2.855, 2.776, 2.807, 2.858, 2.950, 3.017, 3.058, 3.185, 3.226, 3.253, 3.357, 3.331, 3.400, 3.351, 3.209}),
                    // Querry (1985)
            },
        // { polymorphs::chalcocite_alpha,          },
        // { polymorphs::chalcocite_beta,           },
        // { polymorphs::chalcopyrite,              },

    };

}}