#pragma once

// std libraries
#include <map>

// in-house libraries
#include <units/si.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    using SolidRefractiveIndexWavenumberRelation = relation::PolynomialRailyardRelation<si::wavenumber<double>,double, 0,1>;
    std::map<int, SolidRefractiveIndexWavenumberRelation> refractive_index_as_solid {
        { ids::water,       
                relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.0443, 0.0528, 0.067,  0.0886, 0.1181, 0.137,  0.1468, 0.158,  0.177,  0.197,  0.41,   0.51,   0.81,   1.515,  1.76,   1.923,  2.29,   2.52,   2.817,  2.985,  3.175,  3.39,   3.775,  4.56,   5.714,  6.329,  7.143,  8.333,  10.31,  11.9,   14.08,  17.24,  22.22,  34.48,  46.15,  55,     62.5,   76.29,  210, 1300.0},
                     std::vector<double>{0.8228, 0.8505, 0.9679, 1.3807, 1.3783, 1.2849, 1.5729, 1.6309, 1.4462, 1.3974, 1.3185, 1.3126, 1.3047, 1.2914, 1.2843, 1.2771, 1.2545, 1.2232, 1.0657, 1.018, 1.6336, 1.5086, 1.385, 1.3482, 1.2933, 1.3178, 1.3236, 1.2964, 1.1501, 1.2546, 1.5701, 1.5303, 1.4575, 1.253, 1.2543, 1.7955, 1.6372, 1.8504, 1.8114, 1.7868}),
                    // Warren (2008)
            },
        { ids::nitrogen,                  1.25, // wikipedia
            },
        // // { ids::oxygen,           },
        { ids::carbon_dioxide,                  
                relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer,1.0,
                     std::vector<double>{  0.051,   0.055,   0.060,   0.064,   0.068,   0.078,   0.086,   0.091,   0.098,   0.103,   0.110,   0.116,   0.120,   0.129,   0.142,   0.168,   0.238,   0.437,   0.985,   0.989,   3.235,   3.838,   4.095,   4.189,   4.220,   4.221,   4.302,   4.384,   4.606,   5.322,  9.628,  9.887, 13.141,  14.284,  14.637,  14.724,  15.528,  17.137,  23.390,  70.273,  81.169,  85.596,  90.265,  98.121, 134.942, 146.687, 150.064, 158.249, 180.034, 948.278}, 
                     std::vector<double>{   0.51,    0.51,    0.79,    0.88,    0.88,    0.70,    0.93,    0.79,    1.03,    0.93,    1.06,    0.79,    3.79,    2.32,    1.82,    1.60,    1.47,    1.40,    1.38,    1.41,    1.40,    1.36,    1.29,    0.98,    0.45,    3.02,    1.81,    1.54,    1.46,    1.42,   1.41,   1.40,   1.36,    1.26,    0.39,    3.21,    1.57,    1.46,    1.43,    1.41,    1.38,    1.34,    1.48,    1.45,    1.41,    1.33,    1.57,    1.48,    1.45,    1.43}), // Warren (1986)
            },
        { ids::methane,                1.3219,
            },
        { ids::argon,                  1.2703,
            },
        // // { ids::helium,                    
        //     // // TODO: reimplement this
        //     // get_dewaele2003_pressure_function
        //     //     (si::kilobar, 1.0, -0.1033, 1.0, 1.0, 0.052, 117.0, 202.0), // Dewaele (2003)
        //     // }
        // // { ids::hydrogen,                  
        //     // // TODO: reimplement this
        //     // field::SpectralFunction<double>([](
        //     //     const si::wavenumber<double> nlo, 
        //     //     const si::wavenumber<double> nhi, 
        //     //     const si::pressure<double> p, 
        //     //     const si::temperature<double> T
        //     // ) {
        //     //     return 0.95+0.1144*std::pow(std::clamp(p/si::kilobar, 52.0, 350.0), 0.3368);
        //     // }), // Peck & Hung (1977)
        //     // }
        { ids::ammonia,                  1.5,
            },
        // // { ids::ozone,            },
        { ids::nitrous_oxide,                  1.317, // Hudson (2020)
            },
        // // { ids::sulfur_dioxide,   },
        // // { ids::nitric_oxide,     },
        // // { ids::carbon_monoxide,  },
        // // { ids::ethane,           },
        { ids::hydrogen_cyanide,                  1.37,    // Moore 2010, 95K 5000 cm-1
            },
        { ids::ethanol,                  1.319, // Hudson (2020)
            },
        // // { ids::formaldehyde,     },
        { ids::formic_acid,                  1.291, // Hudson (2020)
            },
        // // { ids::perflouromethane, },
        // // { ids::benzene,          },
        // // { ids::pyrimidine,       },
        { ids::quartz,      
                relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 1.5,  8.0,  8.6,  8.8,  8.9,  9.0,  9.1,  9.2,  9.3,  9.4,  9.5,  9.6,  9.7,  9.8,  9.9, 14.3},
                     std::vector<double>{1.44, 0.52, 0.56, 0.47, 0.47, 0.53, 0.74, 1.07, 1.47, 1.94, 2.36, 2.68, 2.83, 2.93, 2.92, 1.75}),
                    // Kischkat (2012)
            },
        { ids::halite,      
                relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{  0.22,   0.24,   0.26,   0.28,  23.81,  26.32,  27.78,  29.41,  31.25,  33.33,  38.46,  41.67,  45.45,  50.00,  55.56,  62.50,  71.43,  83.33, 100.00, 125.00, 166.67},
                     std::vector<double>{1.66, 1.71, 1.68, 1.64, 1.30, 1.23, 1.17, 1.09, 1.00, 0.87, 0.48, 0.50, 0.27, 0.19, 0.43, 6.29, 3.66, 3.05, 2.76, 2.58, 2.49}),
                    // Querry (1987)
            },
        { ids::corundum,                  
                relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 0.21,  7.94,  8.33,  8.55,  9.26, 10.64, 10.75, 10.99, 11.11, 11.24, 11.36, 14.49, 14.71, 14.93, 15.15, 15.38, 15.62, 15.87, 16.13, 16.39, 16.67, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 21.74, 22.22, 22.73, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 33.33, 34.48, 37.04, 55.56},
                     std::vector<double>{1.831, 1.345, 1.294, 1.243, 1.113, 0.592, 0.514, 0.279, 0.182, 0.124, 0.098, 0.117, 0.135, 0.172, 0.243, 0.369, 0.664, 1.672, 0.735, 0.774, 1.062, 1.523, 2.382, 5.146, 8.559, 6.585, 5.308, 4.497, 3.797, 3.232, 2.725, 1.951, 0.629, 0.283, 0.512, 1.472, 7.250, 9.484, 6.881, 5.718, 5.020, 3.879, 5.677, 4.903, 4.560, 4.279, 4.122, 4.000, 3.893, 3.723, 3.667, 3.617, 3.326}),
                    // Querry (1985)
            },
        { ids::apatite,                  1.645,
            },
        { ids::carbon,      
                relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.2, 0.22, 0.23, 0.24, 0.26, 0.27, 0.28, 0.29, 3.9526, 4.2017, 4.8544, 4.902, 5, 5.0505, 6.25, 6.2893, 6.3291, 6.4516, 8.0645, 9.009, 10.2041, 10.3093, 11.3636, 11.4943, 11.6279, 12.3457, 14.4928, 14.9254, 15.3846, 15.625, 15.873, 16.129, 20, 20.4082, 20.8333, 21.2766, 22.2222, 23.8095, 25.641, 27.027, 27.7778, 28.5714, 29.4118, 30.303, 31.25, 32.2581, 33.3333, 34.4828, 35.7143, 37.037, 38.4615, 40, 41.6667, 43.4783, 45.4545, 47.619, 50, 52.6316, 55.5556},
                     std::vector<double>{0.84, 0.962, 1.045, 1.162, 1.49, 1.591, 1.646, 1.679, 3.747, 3.844, 4.141, 4.173, 4.192, 4.237, 4.693, 4.713, 4.764, 4.78, 5.113, 5.26, 5.458, 5.458, 5.572, 5.571, 5.616, 5.677, 5.877, 5.935, 5.971, 5.972, 5.996, 6.042, 6.438, 6.46, 6.511, 6.582, 6.686, 6.95, 7.247, 7.469, 7.602, 7.709, 7.83, 8.035, 8.224, 8.425, 8.682, 8.969, 9.225, 9.586, 9.984, 10.214, 10.813, 10.952, 11.266, 11.898, 12.39, 13.618, 14.474}),
                    // Querry (1985)
            },
        // // { ids::calcite,                  
        //     // // TODO: reimplement this
        //     // field::SpectralFunction<double>([](
        //     //     const si::wavenumber<double> nlo, 
        //     //     const si::wavenumber<double> nhi, 
        //     //     const si::pressure<double> p, 
        //     //     const si::temperature<double> T
        //     // ) {
        //     //     double l = (2.0 / (nhi+nlo) / si::micrometer);
        //     //     return sqrt(
        //     //         1.0
        //     //         + 0.73358749f 
        //     //         + 0.96464345 * l*l / (l*l - 1.94325203f)
        //     //         + 1.82831454 * l*l / (l*l - 120.0)
        //     //     );
        //     // }),
        //     // },
        { ids::orthoclase,                  1.527,
            },
        { ids::andesine,                  1.553,
            },
        { ids::augite,                  1.707,
            },
        { ids::forsterite,                  1.651,
            },
        { ids::goethite,                  2.401,
            },
        { ids::pyrite,                  1.78, //gemologyproject.com
            },
        { ids::hematite,                  
                relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 0.21,  0.22,  0.23,  0.24,  0.27,  0.28,  0.29,  0.32,  0.33,  0.34,  0.35,  0.36,  0.37,  0.38,  0.43,  0.44,  0.45,  0.46,  0.49,  0.50,  0.52,  0.53,  0.54,  0.56,  0.57,  0.58, 14.29, 14.49, 14.71, 15.15, 15.38, 15.62, 16.13, 16.39, 16.67, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 21.74, 22.22, 22.73, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 32.26, 33.33, 34.48, 35.71, 37.04, 38.46, 40.00, 41.67, 43.48, 45.45, 47.62, 50.00, 52.63, 55.56, 58.82, 62.50, 66.67, 71.43, 90.91},
                     std::vector<double>{ 1.20,  1.47,  1.66,  1.79,  2.11,  2.20,  2.26,  2.42,  2.45,  2.45,  2.43,  2.43,  2.47,  2.54,  3.05,  3.13,  3.18,  3.22,  3.28,  3.28,  3.26,  3.27,  3.29,  3.35,  3.36,  3.35,  1.15,  1.04,  0.91,  0.60,  0.48,  0.40,  0.33,  0.31,  0.30,  0.34,  0.40,  0.52,  0.79,  1.26,  2.01,  2.82,  2.67,  1.76,  1.10,  0.90,  1.01,  1.27,  2.23,  4.99,  7.43,  6.23,  4.97,  4.10,  3.26,  2.38,  1.55,  1.03,  0.81,  0.85,  1.00,  1.40,  2.68,  6.49, 12.59, 11.93,  9.43,  8.31,  7.29,  6.65,  7.02,  6.79,  6.36,  6.09,  5.84,  5.66,  5.51,  5.39,  5.29,  5.20,  5.00}),
                    // Querry (1985)
            },
        { ids::gold,                  
                relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.003542,0.005636,0.008266,0.008856,0.009537,0.01127, 0.01378, 0.01459, 0.01631, 0.01722, 0.01771, 0.01879, 0.02,    0.02138, 0.02214, 0.02296, 0.02339, 0.02384, 0.0248,  0.0253,  0.02818, 0.03263, 0.03542, 0.03875, 0.03999, 0.04133, 0.04592, 0.04679, 0.04862, 0.05061, 0.05391, 0.0551,  0.05636, 0.05767, 0.05904, 0.06199, 0.06525, 0.07293, 0.07749, 0.08266, 0.08551, 0.08856, 0.1127,  0.124,   0.155,   0.1653,  0.2066,  0.261,   0.2755,  0.2952,  0.3024,  0.31,    0.3179,  0.3263, 0.3351, 0.3444, 0.3542, 0.3815, 0.4133, 0.4959, 0.6199, 0.8266, 1.24,  4.133, 6.199, 12.4,  24.8, 49.59, 124.0, 248.0},
                     std::vector<double>{1.001,   0.987,   0.963,   0.955,   0.943,   0.902,   0.849,   0.846,   0.853,   0.851,   0.859,   0.883,   0.881,   0.868,   0.871,   0.885,   0.89,    0.889,   0.884,   0.885,   0.899,   0.892,   0.865,   0.921,   0.932,   0.93,    0.886,   0.889,   0.913,   0.989,   1.175,   1.262,   1.338,   1.368,   1.346,   1.29,    1.266,   1.33,    1.421,   1.66,    1.644,   1.72,    1.545,   1.462,   1.16,    1.142,   1.339,   1.612,   1.694,   1.752,   1.729,   1.615,   1.295,   0.727,  0.3,    0.23,   0.211,  0.229,  0.226,  0.237,  0.269,  0.271,  0.278, 1.411, 2.842, 9.936, 36.7, 118.0, 309.0, 531.0}),
                // Hagemann (1974)
            },
        { ids::silver,                  
                relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.003542,0.005636,0.008266,0.008856,0.009537,0.01127, 0.01378, 0.01459, 0.01631, 0.01722, 0.01771, 0.01879, 0.02,    0.02138, 0.02214, 0.02296, 0.02339, 0.02384, 0.0248,  0.0253,  0.02818, 0.03263, 0.03542, 0.03875, 0.03999, 0.04133, 0.04592, 0.04679, 0.04862, 0.05061, 0.05391, 0.0551,  0.05636, 0.05767, 0.05904, 0.06199, 0.06525, 0.07293, 0.07749, 0.08266, 0.08551, 0.08856, 0.1127,  0.124,   0.155,   0.1653,  0.2066,  0.261,   0.2755,  0.2952,  0.3024,  0.31,    0.3179,  0.3263, 0.3351, 0.3444, 0.3542, 0.3815, 0.4133, 0.4959, 0.6199, 0.8266, 1.24,  4.133, 6.199, 12.4,  24.8, 49.59, 124.0, 248.0},
                     std::vector<double>{1.001,   0.987,   0.963,   0.955,   0.943,   0.902,   0.849,   0.846,   0.853,   0.851,   0.859,   0.883,   0.881,   0.868,   0.871,   0.885,   0.89,    0.889,   0.884,   0.885,   0.899,   0.892,   0.865,   0.921,   0.932,   0.93,    0.886,   0.889,   0.913,   0.989,   1.175,   1.262,   1.338,   1.368,   1.346,   1.29,    1.266,   1.33,    1.421,   1.66,    1.644,   1.72,    1.545,   1.462,   1.16,    1.142,   1.339,   1.612,   1.694,   1.752,   1.729,   1.615,   1.295,   0.727,  0.3,    0.23,   0.211,  0.229,  0.226,  0.237,  0.269,  0.271,  0.278, 1.411, 2.842, 9.936, 36.7, 118.0, 309.0, 531.0}),
                // Hagemann (1974)
            },
        { ids::copper,                  
                relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{2.5e-05, 2.5e-03, 4.1e-03, 8.3e-03, 1.0e-02, 3.9e-02, 4.0e-02, 4.1e-02, 4.3e-02, 4.4e-02, 4.6e-02, 5.0e-02, 5.2e-02, 5.4e-02, 5.6e-02, 5.9e-02, 6.2e-02, 6.5e-02, 6.9e-02, 7.3e-02, 7.7e-02, 8.6e-02, 8.9e-02, 9.5e-02, 1.0e-01, 1.1e-01, 1.2e-01, 1.3e-01, 1.4e-01, 1.5e-01, 1.7e-01, 1.8e-01, 1.9e-01, 2.1e-01,  2.2e-01, 2.3e-01, 2.4e-01, 2.5e-01, 2.6e-01, 2.7e-01, 2.8e-01, 3.0e-01, 3.1e-01, 3.3e-01, 3.4e-01, 3.9e-01, 4.1e-01, 4.8e-01, 5.2e-01, 5.4e-01, 5.6e-01, 5.9e-01, 6.2e-01, 6.5e-01, 7.1e-01, 7.3e-01, 8.3e-01, 1.2e+00, 2.5e+00, 1.2e+01, 2.5e+01, 1.2e+02, 2.5e+02},
                     std::vector<double>{  1.00,   1.00,   0.99,   0.97,   0.96,   0.89,   0.88,   0.86,   0.85,   0.86,   0.88,   0.96,   0.96,   0.94,   0.92,   0.90,   0.88,   0.88,   0.89,   0.91,   0.95,   1.03,   1.05,   1.08,   1.09,   1.07,   1.04,   1.03,   1.03,   1.03,   1.00,   0.97,   0.96,   1.04,   1.10,   1.18,   1.28,   1.38,       1.53,   1.52,   1.49,   1.42,   1.34,   1.34,   1.31,   1.23,   1.18,   1.15,   1.12,   1.04,   0.83,   0.47,   0.27,   0.21,   0.21,   0.22,   0.26,   0.43,   1.69,  29.10,  62.80, 186.00, 270.00}),
                // Hagemann (1974)
            },
        { ids::magnetite,                  
                relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 0.21,  0.22,  4.18, 4.20, 15.62, 16.13, 16.39, 16.67, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 32.26, 33.33, 34.48, 35.71, 37.04, 38.46, 40.00, 41.67, 43.48, 47.62, 50.00, 52.63, 55.56},
                     std::vector<double>{2.3,     2.3,   3.3,  3.3,   3.5,   3.5,   3.5,   3.5, 3.5, 3.8, 4.4, 5.1, 5.3, 5.1, 4.9, 4.7, 4.6, 4.5, 4.5, 4.4, 4.4, 4.4, 4.4, 4.5, 4.7, 5.2, 6.3, 7.4, 7.3, 6.9, 6.6, 6.4, 6.3, 6.3, 6.4, 6.4, 6.5, 6.7, 6.7, 7.1, 7.3, 7.7, 7.9}),
                    // Querry (1985)
            },
        // // { ids::chalcocite,       },
        // // { ids::chalcopyrite,     },
    };

}}