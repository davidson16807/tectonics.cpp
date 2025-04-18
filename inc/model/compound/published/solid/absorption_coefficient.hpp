#pragma once

// in-house libraries
#include <unit/si.hpp>
#include <model/compound/table/PartialTable.hpp>
#include <model/compound/relation/GenericRelation.hpp>
#include <model/compound/ids.hpp>

#include "polymorphs.hpp"

namespace compound {
namespace published {

    using SolidAbsorptionCoefficientWavenumberRelation = relation::GenericRelation<si::spatial_frequency<double>,si::spatial_frequency<double>>;
    table::PartialTable<SolidAbsorptionCoefficientWavenumberRelation> absorption_coefficient_as_solid {

        // { polymorphs::water_ice_1h,              },
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
        // { polymorphs::carbon_dioxide_ice,        },
        { polymorphs::methane_ice_alpha,         
                relation::get_generic_spectral_cubic_interpolation_of_wavenumber_for_log10_sample_output
                    (1.0/si::centimeter, 1.0/si::centimeter,
                     std::vector<double>{ 10653.5, 10534.7, 10445.5, 10297.0, 10217.8, 10108.9, 10059.4, 10029.7,  9960.4,  9871.3,  9792.1,  9752.5,  9673.3,  9623.8,  9554.5,  9435.6,  9376.2,  9148.5,  9039.6,  8990.1,  8901.0,  8851.5,  8802.0,  8703.0,  8594.1,  8455.4,  8396.0,  8346.5,  8306.9,  8257.4,  8227.7,  8128.7,  8089.1,  8069.3,  8039.6,  8009.9,  7950.5,  7861.4,  7811.9,  7623.8,  7495.0,  7415.8,  7356.4,  7277.2,  7237.6,  7217.8,  7148.5,  7128.7,  7059.4,  7009.9,  6960.4,  6871.3,  6792.1,  6732.7,  6703.0,  6643.6,  6584.2,  6524.8,  6287.1,  6059.4,  5980.2,  5940.6,  5881.2,  5782.2,  5742.6,  5673.3,  5554.5,  5514.9,  5465.3,  5376.2,  5277.2,  5168.3,  5108.9,  5039.6,  4811.9,  4594.1,  4524.8,  4475.2,  4356.4,  4316.8,  4247.5,  4207.9,  4148.5,  4079.2,  4039.6,  3920.8,  3851.5,  3772.3,  3475.2,  3188.1,  3029.7,  2940.6,  2841.6,  2752.5,  2623.8,  2505.0 },
                     std::vector<double>{  -2.134,  -1.866,  -1.966,  -0.978,  -1.514,  -0.642,  -1.078,  -0.844,  -1.346,  -0.693,  -1.698,  -1.430,  -1.564,  -1.950,  -2.218,  -2.084,  -2.101,  -1.749,  -1.045,  -1.246,  -0.173,  -0.391,   0.246,  -0.743,   0.547,  -0.994,  -0.190,  -0.860,  -0.542,  -1.263,  -1.749,  -2.050,  -1.447,  -1.749,  -1.581,  -2.117,  -2.335,  -2.067,  -2.134,  -1.179,   0.682,  -0.391,   0.330,   0.296,  -0.089,   0.145,  -0.039,   0.430,   0.313,  -0.609,  -0.927,  -0.609,  -1.279,  -0.324,  -1.849,  -2.034,  -1.782,  -2.168,  -1.028,   0.732,   1.335,   0.229,   0.078,   1.101,  -0.123,   0.061,   0.933,  -0.458,  -1.112,   0.011,  -1.715,  -0.642,  -0.911,  -1.615,  -0.642,   0.547,   1.419,   0.782,   1.419,   2.257,   1.134,   2.374,   0.430,   0.782,  -0.358,   0.464,   1.570,  -1.598,  -0.508,   1.369,   3.715,   1.268,   1.905,   0.196,   1.184,  -1.061 }),
            },
        // { polymorphs::methane_ice_beta,          },
        // { polymorphs::methane_ice_beta,          },
        // { polymorphs::argon_ice,                 },
        // { polymorphs::helium_ice,                },
        // { polymorphs::hydrogen_ice,              },
        // { polymorphs::ammonia_ice,               },
        // { polymorphs::ozone_ice,                 },
        // { polymorphs::nitrous_oxide_ice,         },
        { polymorphs::sulfur_dioxide_ice,        
                relation::get_generic_spectral_cubic_interpolation_of_wavenumber_for_log10_sample_output
                    (1.0/si::centimeter, 1.0/si::centimeter,
                     std::vector<double>{  5097.2,  5028.0,  5008.2,  4968.7,  4948.9,  4731.5,  4691.9,  4672.2,  4553.5,  4533.8,  4514.0,  4108.7,  4089.0,  4059.3,  4019.8,  3990.1,  3920.9,  3911.0,  3881.4,  3871.5,  3841.8,  3812.2,  3752.9,  3743.0,  3713.3,  3644.2,  3614.5,  3575.0,  3525.5,  3495.9,  3456.3,  3416.8,  3397.0,  3357.5,  3327.8,  3308.1,  3041.2,  2981.9,  2942.3,  2843.5,  2823.7,  2774.3,  2734.8,  2665.6,  2616.1,  2566.7,  2537.1,  2477.8,  2448.1,  2359.1,  2309.7,  2280.1,  2240.5 },
                     std::vector<double>{  -2.262,  -0.619,  -2.227,  -1.878,  -2.227,  -2.210,  -0.759,  -2.210,  -2.227,  -1.843,  -2.227,  -2.245,  -1.493,  -2.157,  -1.895,  -1.196,   0.343,  -1.161,  -0.462,  -2.000,  -2.192,  -2.192,  -0.794,  -2.210,  -1.790,  -1.493,  -0.584,   1.199,  -0.741,  -1.528,  -1.790,   0.150,  -2.087,  -2.157,  -1.476,  -2.192,  -2.227,  -0.479,  -2.227,  -2.227,   0.010,  -0.899,  -0.706,   0.675,  -0.339,   0.517,   0.430,   2.685,   1.304,   0.045,   2.108,   0.395,   0.255 }),
            },
        // { polymorphs::nitric_oxide_ice,          },
        { polymorphs::carbon_monoxide_ice_beta,  
                relation::get_generic_spectral_cubic_interpolation_of_wavenumber_for_log10_sample_output
                    (1.0/si::centimeter, 1.0/si::centimeter,
                     std::vector<double>{  6439.7,  6390.1,  6350.4,  6320.7,  6300.8,  6211.6,  6191.7,  6152.1,  4486.0,  4466.1,  4366.9,  4327.3,  4287.6,  4257.9,  4238.0,  4208.3,  4178.5,  4158.7,  4138.8,  4079.3,  4059.5,  4029.8 },
                     std::vector<double>{  -2.678,  -1.542,  -2.661,   0.407,  -2.678,  -2.695,  -1.576,  -2.695,  -2.661,  -1.814,  -0.424,   0.373,   0.017,   2.780,  -0.983,  -0.576,  -1.576,   0.797,  -2.678,  -2.695,  -1.797,  -2.678 }),
            },
        // { polymorphs::carbon_monoxide_ice_alpha, },
        // { polymorphs::ethane_ice,                },
        // { polymorphs::hydrogen_cyanide_ice,      },
        // { polymorphs::ethanol_ice,               },
        // { polymorphs::formaldehyde_ice,          },
        // { polymorphs::formic_acid_ice,           },
        // { polymorphs::perflouromethane_ice,      },
        // { polymorphs::benzene_ice,               },
        // { polymorphs::pyrimidine_ice,            },
        { polymorphs::quartz_alpha,              
                relation::get_generic_spectral_function_from_reflectance_at_wavelengths // for quartz
                    (si::micrometer,  37.5 *si::micrometer, 
                     std::vector<double>{ 0.221, 0.239, 0.263, 0.280, 0.348, 0.416, 0.531, 0.784, 1.862, 1.918, 2.152, 2.193, 2.309, 2.317, 2.475, 2.593, 2.662, 2.692, 2.720, 2.784, 2.848, 2.886, 2.915, 2.985, 3.452, 3.667, 3.721, 3.963, 4.084, 4.192, 4.340, 4.475, 4.596, 4.905, 7.395, 7.624, 7.893, 8.216, 8.526, 8.620, 8.714, 9.239, 9.495, 9.724,10.114,10.531,11.958,12.281,12.402,12.483,12.644,12.806,12.994,13.156,13.600,14.313,14.448,14.757,17.476,17.839,17.920,18.176,18.257,18.445,18.903,19.091,19.333,19.831,20.141,20.572,20.935,21.729,22.025,22.483,22.900,23.331,23.586,23.869,24.057,24.380,24.582,24.703 },
                     std::vector<double>{ 0.447, 0.465, 0.546, 0.550, 0.710, 0.761, 0.803, 0.831, 0.865, 0.861, 0.879, 0.867, 0.871, 0.876, 0.874, 0.861, 0.867, 0.847, 0.816, 0.720, 0.710, 0.728, 0.696, 0.729, 0.389, 0.352, 0.291, 0.303, 0.271, 0.186, 0.168, 0.071, 0.175, 0.034, 0.009, 0.031, 0.206, 0.791, 0.678, 0.406, 0.597, 0.905, 0.330, 0.210, 0.132, 0.102, 0.049, 0.022, 0.165, 0.198, 0.152, 0.192, 0.125, 0.096, 0.075, 0.053, 0.087, 0.066, 0.028, 0.039, 0.077, 0.195, 0.207, 0.188, 0.099, 0.097, 0.157, 0.684, 0.791, 0.829, 0.802, 0.584, 0.546, 0.309, 0.213, 0.175, 0.167, 0.131, 0.136, 0.104, 0.063, 0.065 }),
            },
        // { polymorphs::quartz_beta,               },
        // { polymorphs::crystoballite_alpha,       },
        // { polymorphs::crystoballite_beta,        },
        { polymorphs::halite,                    
                relation::get_generic_spectral_function_from_reflectance_at_wavelengths // for halite
                    (si::micrometer,  162 *si::micrometer, 
                     std::vector<double>{  0.2183,  0.2295,  0.2439,  0.2615,  0.2743,  0.2903,  0.3207,  0.4455,  0.5656,  0.7928,  1.0617,  1.3802,  1.4523,  1.5531,  1.8588,  1.9292,  1.9900,  2.0605,  2.1309,  2.2349,  2.3549,  2.6558,  2.7247,  2.7855,  2.8479,  2.8847,  2.9119,  2.9439,  2.9807,  8.1565,  9.3855, 10.7373, 12.2121, 12.7037, 18.1112, 27.6972, 30.1552, 32.4903, 35.1940, 37.1604, 38.3893, 40.2328, 42.4450, 43.7969, 45.7632, 48.5899, 50.0646, 51.1707, 55.1034, 56.4553, 58.1759, 60.0194, 65.5498, 67.7619, 71.4489, 73.7839, 78.5769, 86.4424, 93.0789, 98.7322,109.9159,114.4631,127.7360,146.2936,153.9133,167.1862 },
                     std::vector<double>{ 0.59984, 0.63333, 0.64910, 0.64975, 0.54138, 0.67077, 0.71872, 0.81461, 0.86059, 0.89146, 0.90263, 0.90328, 0.89343, 0.90657, 0.90394, 0.87176, 0.87241, 0.90066, 0.90985, 0.90394, 0.90197, 0.86256, 0.78112, 0.55057, 0.45271, 0.44154, 0.41921, 0.40805, 0.41724, 0.87214, 0.82290, 0.83550, 0.78511, 0.75534, 0.60191, 0.04198, 0.00420, 0.00305, 0.03740, 0.17023, 0.22405, 0.14504, 0.22519, 0.23435, 0.36260, 0.44275, 0.50573, 0.55840, 0.66489, 0.67405, 0.66718, 0.62137, 0.24466, 0.18969, 0.16450, 0.14389, 0.12557, 0.13130, 0.11412, 0.11756, 0.10611, 0.10382, 0.12214, 0.11641, 0.11756, 0.13244 }),
            },
        { polymorphs::corundum,                  
                relation::get_generic_spectral_function_from_reflectance_at_wavelengths // for corundum
                    (si::micrometer,  162 *si::micrometer, 
                     std::vector<double>{ 0.22736, 0.27200, 0.29592, 0.31824, 0.34216, 0.36129, 0.36926, 0.37883, 0.39318, 0.39636, 0.41071, 0.43144, 0.45217, 0.47449, 0.47608, 0.48884, 0.55261, 0.57334, 0.63552, 0.69133, 0.72162, 0.84917, 0.85236, 0.87468, 0.90019, 0.93846, 1.08036, 1.14573, 1.30676, 1.38489, 1.41199, 1.44866, 1.50925, 1.54273, 1.77232, 1.81218, 1.84885, 1.89509, 1.93973, 1.96524, 2.00510, 2.05134, 2.15179, 2.30485, 2.38297, 2.43399, 2.59343, 2.65402, 2.75128, 2.78795, 2.85013, 2.94420, 2.97449 },
                     std::vector<double>{  0.0529,  0.0582,  0.0794,  0.2286,  0.2561,  0.4741,  0.4878,  0.4455,  0.4561,  0.5175,  0.5767,  0.5968,  0.5407,  0.6159,  0.6339,  0.6508,  0.5788,  0.5778,  0.6106,  0.6233,  0.6360,  0.7249,  0.7365,  0.7418,  0.7566,  0.7672,  0.7767,  0.7926,  0.8593,  0.8677,  0.8550,  0.8614,  0.8392,  0.8127,  0.5302,  0.5153,  0.5164,  0.5323,  0.5365,  0.5249,  0.4963,  0.4878,  0.4910,  0.4772,  0.4561,  0.4265,  0.3873,  0.3820,  0.2476,  0.2233,  0.1577,  0.1016,  0.0984 }),
            },
        // { polymorphs::apatite,                   },
        { polymorphs::graphite,                  
                relation::get_generic_spectral_function_from_reflectance_at_wavelengths // for carbon
                    (si::micrometer,  0.17 *si::micrometer, 
                     std::vector<double>{ 0.22365, 0.23614, 0.25332, 0.27518, 0.28767, 0.32670, 0.33607, 0.35012, 0.35636, 0.36885, 0.44692, 0.65301, 0.77635, 0.82162, 0.82943, 0.84348, 0.85129, 0.86066, 0.87158, 0.88407, 0.89344, 0.91218, 0.93247, 0.94965, 1.44926, 1.53044, 1.55386, 1.57884, 1.96916, 1.98322, 2.02849, 2.04411, 2.06909, 2.08314, 2.18462, 2.19243, 2.28142, 2.30172, 2.31577, 2.32514, 2.36417, 2.37510, 2.41569, 2.46409, 2.49532, 2.52654, 2.58743, 2.62334, 2.66081, 2.68735, 2.75293, 2.81538, 2.84660, 2.87939, 2.90906 },
                     std::vector<double>{ 0.00272, 0.01331, 0.01585, 0.01762, 0.01675, 0.01778, 0.01685, 0.01710, 0.01630, 0.01646, 0.01572, 0.01534, 0.01556, 0.01463, 0.01303, 0.01303, 0.01540, 0.01547, 0.01672, 0.01649, 0.01598, 0.01643, 0.01736, 0.01636, 0.01351, 0.01409, 0.01315, 0.01354, 0.01238, 0.01091, 0.01290, 0.01075, 0.01219, 0.01081, 0.01235, 0.01139, 0.00924, 0.00503, 0.01884, 0.01184, 0.01165, 0.00998, 0.01181, 0.01014, 0.01071, 0.00988, 0.00921, 0.01049, 0.01068, 0.01059, 0.02173, 0.00317, 0.01152, 0.00359, 0.02734 }),
            },
        // { polymorphs::diamond,                   },
        { polymorphs::calcite,                   
                relation::get_generic_spectral_function_from_reflectance_at_wavelengths // for calcite
                    (si::micrometer,  410 *si::micrometer, 
                    std::vector<double>{  0.246,  0.271,  0.279,  0.327,  0.358,  0.450,  0.501,  0.818,  0.844,  0.920,  1.034,  1.451,  1.492,  1.584,  1.606,  1.709,  1.749,  1.777,  1.815,  1.855,  1.876,  1.898,  1.915,  1.947,  1.963,  1.983,  1.996,  2.026,  2.053,  2.090,  2.109,  2.136,  2.143,  2.162,  2.186,  2.226,  2.258,  2.296,  2.307,  2.335,  2.380,  2.389,  2.419,  2.500,  2.529,  2.560,  2.592,  2.627,  2.659,  2.690,  2.757,  2.785,  2.814,  2.879,  2.914,  2.946,  2.979,  3.694,  4.433,  5.295,  7.266,  8.497, 12.931, 15.270, 16.871, 17.610, 19.088, 20.812, 23.522, 26.477, 30.665, 33.251, 34.482, 37.438, 38.546, 41.748, 43.472, 46.305, 48.275, 49.137, 50.000, 51.724, 52.955, 54.433, 55.911, 56.773, 60.591, 61.453, 62.807, 64.162, 66.379, 69.088, 70.197, 72.044, 73.891, 77.463, 79.926, 82.142, 85.344, 87.807, 97.536, 99.753,101.477,103.571,107.512,112.561,117.487,123.522,129.433,132.758,136.330,140.147,143.842,148.522,152.586,156.896,178.940,191.871,199.630,207.019,216.1330},
                    std::vector<double>{  0.709,  0.715,  0.743,  0.794,  0.797,  0.919,  0.936,  0.955,  0.960,  0.953,  0.964,  0.958,  0.962,  0.957,  0.960,  0.947,  0.937,  0.951,  0.940,  0.910,  0.864,  0.925,  0.929,  0.921,  0.900,  0.887,  0.865,  0.929,  0.940,  0.936,  0.921,  0.908,  0.877,  0.868,  0.893,  0.859,  0.789,  0.656,  0.648,  0.573,  0.810,  0.829,  0.812,  0.552,  0.490,  0.601,  0.775,  0.841,  0.844,  0.800,  0.602,  0.496,  0.609,  0.709,  0.677,  0.682,  0.660,  0.071,  0.459,  0.024,  0.055,  0.245,  0.239,  0.199,  0.089,  0.084,  0.038,  0.019,  0.010,  0.078,  0.099,  0.128,  0.117,  0.133,  0.122,  0.033,  0.066,  0.102,  0.158,  0.157,  0.174,  0.183,  0.172,  0.176,  0.163,  0.183,  0.160,  0.165,  0.158,  0.183,  0.100,  0.092,  0.066,  0.066,  0.038,  0.039,  0.021,  0.041,  0.053,  0.071,  0.116,  0.103,  0.099,  0.104,  0.086,  0.111,  0.108,  0.147,  0.134,  0.141,  0.166,  0.178,  0.146,  0.157,  0.161,  0.177,  0.209,  0.171,  0.215,  0.355,  0.4454}),
            },
        // { polymorphs::aragonite,                 },
        { polymorphs::orthoclase,                
                relation::get_generic_spectral_function_from_reflectance_at_wavelengths // for orthoclase
                    (si::micrometer,  37.5 *si::micrometer, 
                    std::vector<double>{ 0.256, 0.281, 0.321, 0.351, 0.354, 0.367, 0.376, 0.390, 0.471, 0.542, 0.893, 1.234, 1.359, 1.626, 2.115, 2.122, 2.195, 2.301, 2.329, 2.600, 2.661, 2.721, 2.785, 2.821, 2.853, 2.883, 2.914, 2.982, 3.094, 3.334, 3.454, 3.560, 3.840, 3.987, 4.227, 4.666, 4.920, 5.013, 5.293, 5.652, 5.959, 6.265, 6.558, 6.945, 7.558, 7.784, 8.104, 8.344, 8.464, 8.704, 8.917, 9.290, 9.463,10.036,10.249,10.769,12.101,12.248,12.541,13.447,15.179,16.232,16.752,18.151,19.350,20.029,20.962,23.014,23.281,24.960 },
                    std::vector<double>{ 0.069, 0.146, 0.482, 0.551, 0.602, 0.616, 0.605, 0.654, 0.716, 0.743, 0.788, 0.767, 0.769, 0.794, 0.809, 0.816, 0.803, 0.803, 0.811, 0.801, 0.807, 0.729, 0.596, 0.598, 0.561, 0.590, 0.569, 0.586, 0.661, 0.711, 0.658, 0.745, 0.757, 0.743, 0.730, 0.510, 0.481, 0.481, 0.295, 0.178, 0.180, 0.140, 0.148, 0.117, 0.023, 0.009, 0.018, 0.082, 0.103, 0.111, 0.096, 0.106, 0.129, 0.117, 0.103, 0.112, 0.144, 0.134, 0.102, 0.121, 0.140, 0.171, 0.201, 0.223, 0.273, 0.282, 0.333, 0.376, 0.374, 0.403 }),
            },
        { polymorphs::andesine,                  
                relation::get_generic_spectral_function_from_reflectance_at_wavelengths // for andesine
                    (si::micrometer,  285 *si::micrometer, 
                    std::vector<double>{  0.253,  0.278,  0.348,  0.378,  0.563,  0.615,  1.405,  1.441,  1.460,  1.838,  1.897,  1.908,  2.075,  2.156,  2.242,  2.308,  2.327,  2.626,  2.659,  2.786,  2.821,  2.848,  2.881,  2.916,  2.973,  4.139,  5.614,  7.704,  8.688, 10.409, 10.532, 12.377, 12.991, 15.204, 19.385, 20.000, 23.811, 24.795, 25.901, 27.008, 29.344, 30.327, 37.704, 42.745, 55.409, 64.754, 66.598, 68.196, 82.704, 85.286, 93.401, 99.549,107.172,112.827,133.606,136.680,140.245,148.237,152.418,167.909,172.950,185.368,199.385,207.254,215.983 },
                    std::vector<double>{  0.097,  0.124,  0.243,  0.254,  0.472,  0.493,  0.579,  0.581,  0.595,  0.662,  0.653,  0.628,  0.682,  0.690,  0.672,  0.669,  0.680,  0.614,  0.594,  0.228,  0.226,  0.216,  0.228,  0.214,  0.207,  0.515,  0.043,  0.010,  0.224,  0.153,  0.086,  0.035,  0.057,  0.036,  0.075,  0.119,  0.135,  0.087,  0.094,  0.140,  0.072,  0.099,  0.076,  0.098,  0.115,  0.094,  0.137,  0.104,  0.102,  0.129,  0.119,  0.159,  0.159,  0.145,  0.166,  0.150,  0.153,  0.232,  0.187,  0.230,  0.220,  0.222,  0.266,  0.269,  0.216 }),
            },
        { polymorphs::augite,                    
                relation::get_generic_spectral_function_from_reflectance_at_wavelengths // for augite
                    (si::micrometer,  400 *si::micrometer, 
                    std::vector<double>{  0.221,  0.282,  0.365,  0.508,  0.586,  0.748,  0.797,  0.872,  1.029,  1.225,  2.055,  2.497,  2.625,  2.660,  2.692,  2.750,  2.791,  2.852,  2.916,  2.950,  2.977,  3.846,  8.510, 10.842, 11.947, 13.543, 14.893, 16.857, 18.085, 20.540, 21.276, 23.854, 25.327, 27.291, 28.764, 29.746, 32.446, 34.042, 35.147, 38.952, 40.671, 41.039, 42.880, 49.140, 54.787, 58.960, 62.397, 66.816, 73.199,114.934,117.757,123.649,126.841,129.418,140.220,143.535,152.127,162.315,172.381,178.273,191.530,199.018,207.119,215.589 },
                    std::vector<double>{  0.026,  0.029,  0.095,  0.168,  0.179,  0.130,  0.127,  0.145,  0.141,  0.150,  0.354,  0.442,  0.468,  0.473,  0.466,  0.337,  0.333,  0.299,  0.325,  0.326,  0.360,  0.496,  0.037,  0.144,  0.099,  0.093,  0.032,  0.038,  0.181,  0.137,  0.148,  0.077,  0.095,  0.074,  0.070,  0.081,  0.062,  0.093,  0.095,  0.080,  0.095,  0.111,  0.122,  0.095,  0.094,  0.081,  0.089,  0.084,  0.103,  0.114,  0.106,  0.122,  0.118,  0.118,  0.132,  0.141,  0.127,  0.137,  0.131,  0.141,  0.051,  0.048,  0.100,  0.133 }),
            },
        { polymorphs::forsterite,                
                relation::get_generic_spectral_function_from_reflectance_at_wavelengths // for olivine
                    (si::micrometer,  162.0 *si::micrometer, 
                    std::vector<double>{  0.221,  0.232,  0.256,  0.303,  0.371,  0.405,  0.411,  0.463,  0.464,  0.557,  0.584,  0.636,  0.666,  0.696,  0.837,  0.853,  1.013,  1.045,  1.085,  1.149,  1.258,  1.335,  1.562,  1.639,  1.780,  1.842,  1.903,  1.956,  2.154,  2.170,  2.353,  2.364,  2.563,  2.627,  2.657,  2.716,  2.758,  2.788,  2.822,  2.886,  2.915,  2.945,  2.977,  3.524,  4.754,  5.614,  6.721,  8.688, 10.655, 12.254, 13.114, 14.959, 15.942, 17.172, 18.893, 20.737, 23.811, 24.795, 26.762, 27.991, 32.049, 34.139, 35.000, 35.737, 40.655, 57.008, 68.565, 72.131, 80.122, 82.459, 85.040, 87.991, 89.344, 92.172,105.942,129.549,136.188,152.172,157.213,168.155,191.639,199.508,207.131,215.614 },
                    std::vector<double>{  0.039,  0.056,  0.057,  0.088,  0.321,  0.362,  0.382,  0.404,  0.418,  0.476,  0.472,  0.448,  0.455,  0.445,  0.299,  0.296,  0.165,  0.155,  0.159,  0.191,  0.218,  0.254,  0.428,  0.448,  0.457,  0.454,  0.455,  0.447,  0.465,  0.460,  0.444,  0.454,  0.439,  0.446,  0.437,  0.164,  0.213,  0.237,  0.249,  0.303,  0.299,  0.316,  0.319,  0.419,  0.267,  0.046,  0.143,  0.014,  0.234,  0.057,  0.032,  0.014,  0.062,  0.030,  0.169,  0.051,  0.180,  0.146,  0.092,  0.129,  0.058,  0.137,  0.135,  0.118,  0.085,  0.070,  0.081,  0.078,  0.105,  0.103,  0.128,  0.128,  0.114,  0.134,  0.142,  0.199,  0.187,  0.239,  0.242,  0.232,  0.260,  0.303,  0.359,  0.380 }),
            },
        { polymorphs::goethite,                  
                relation::get_generic_spectral_function_from_reflectance_at_wavelengths // for goethite
                    (si::micrometer,  125.0 *si::micrometer, 
                    std::vector<double>{   0.22471,   0.43278,   0.44558,   0.48880,   0.52561,   0.58003,   0.60403,   0.65205,   0.68246,   0.74808,   0.77529,   0.85531,   0.90333,   0.94814,   1.03777,   1.07778,   1.21223,   1.27465,   1.57234,   1.66677,   1.88284,   1.93406,   2.00288,   2.12132,   2.17734,   2.32778,   2.39981,   2.44782,   2.59187,   2.66229,   2.72311,   2.78233,   2.88316,   2.91357,   2.95198,   2.98079,   3.27869,   4.50820,   6.10656,   6.84426,   7.95082,   8.81148,  10.40984,  12.37705,  13.97541,  14.83607,  17.04918,  21.47541,  27.50000,  32.17213,  35.24590,  38.19672,  40.65574,  46.43443,  49.87705,  56.27049,  77.04918,  82.58197,  90.81967,  94.38525, 130.04098, 133.11475, 140.36885, 148.11475, 156.96721, 167.78689, 178.85246, 192.13115, 207.86885, 215.61475 },
                    std::vector<double>{  0.0113,  0.0225,  0.0317,  0.0352,  0.0725,  0.1824,  0.2007,  0.2049,  0.2176,  0.2782,  0.2824,  0.2275,  0.2190,  0.2218,  0.2627,  0.2979,  0.4803,  0.5282,  0.5549,  0.5331,  0.5373,  0.5254,  0.5282,  0.5662,  0.5599,  0.4979,  0.4380,  0.4296,  0.4359,  0.3958,  0.3007,  0.1570,  0.0732,  0.0761,  0.0563,  0.0627,  0.0348,  0.2310,  0.1034,  0.2304,  0.1926,  0.0458,  0.0163,  0.0293,  0.0218,  0.0122,  0.0190,  0.0753,  0.0753,  0.0348,  0.0828,  0.0870,  0.0794,  0.0918,  0.0815,  0.1151,  0.1336,  0.1295,  0.1604,  0.1501,  0.1988,  0.1933,  0.2214,  0.2125,  0.2516,  0.2654,  0.2516,  0.2928,  0.3031,  0.2935 }),
            },
        { polymorphs::pyrite,                    
                relation::get_generic_spectral_function_from_reflectance_at_wavelengths // for pyrite
                    (si::micrometer,  162.0 *si::micrometer, 
                    std::vector<double>{  0.23550,  0.24665,  0.31676,  0.34226,  0.37731,  0.46176,  0.50000,  0.58764,  0.62588,  0.69280,  0.73263,  0.84098,  0.90153,  1.02900,  1.17559,  1.36998,  1.60739,  1.66953,  1.70459,  2.05672,  2.32760,  2.49490,  2.66061,  2.68929,  2.72275,  2.75621,  2.78649,  2.88050,  2.91396,  2.94583,  3.03656,  3.56466,  3.87610,  4.03859,  4.93230,  5.14895,  5.41977,  6.04265,  6.44888,  6.69262,  6.93636,  7.07177,  7.65403,  7.83006,  8.10088,  8.16858,  8.43940,  8.58835,  8.73731,  8.89980,  9.02167,  9.36019,  9.59039,  9.71225,  9.84766,  9.92891, 10.09140, 11.35071, 12.20379, 12.65064, 12.81313, 14.08599, 14.65471, 14.83074, 14.99323, 15.20988, 15.39946, 15.88693, 16.15775, 16.87542, 17.03791, 17.29519, 17.95870, 18.13473, 19.24509, 21.39810, 22.27827, 22.72512, 22.86053, 23.11781, 24.22817, 24.81043, 24.93230 },
                    std::vector<double>{ 0.00702, 0.02027, 0.03712, 0.03753, 0.05559, 0.07873, 0.09264, 0.10428, 0.10428, 0.10936, 0.10896, 0.09612, 0.09425, 0.08421, 0.08047, 0.07900, 0.08060, 0.07953, 0.08033, 0.07405, 0.07766, 0.08087, 0.08528, 0.08475, 0.08876, 0.07833, 0.07659, 0.04890, 0.04582, 0.03418, 0.02283, 0.05607, 0.06561, 0.07341, 0.07890, 0.08584, 0.08237, 0.05549, 0.08064, 0.06676, 0.08121, 0.08410, 0.08988, 0.09509, 0.10896, 0.10405, 0.09913, 0.08844, 0.08844, 0.07717, 0.07601, 0.07890, 0.09827, 0.07399, 0.07225, 0.06301, 0.06012, 0.06532, 0.05289, 0.04971, 0.05202, 0.05665, 0.06474, 0.06965, 0.06734, 0.05780, 0.05694, 0.06590, 0.06156, 0.07572, 0.07457, 0.06879, 0.06647, 0.06850, 0.06734, 0.05723, 0.04364, 0.02601, 0.03064, 0.08497, 0.20318, 0.24451, 0.23988 }),
            },
        { polymorphs::hematite,                  
                relation::get_generic_spectral_function_from_reflectance_at_wavelengths // for hematite
                    (si::micrometer,  233.0*si::micrometer, 
                    std::vector<double>{  0.231,  0.243,  0.281,  0.310,  0.319,  0.334,  0.358,  0.412,  0.440,  0.468,  0.519,  0.552,  0.574,  0.614,  0.673,  0.723,  0.771,  0.846,  0.852,  0.870,  0.884,  0.897,  0.925,  1.179,  1.246,  1.347,  1.591,  1.712,  1.755,  1.951,  2.441,  2.468,  2.597,  2.656,  2.685,  2.720,  2.754,  2.784,  2.818,  2.848,  2.883,  2.913,  2.948,  2.980,  4.139,  8.442, 14.221, 15.450, 18.155, 20.122, 21.475, 23.073, 24.303, 25.532, 26.885, 28.729, 33.032, 35.245, 36.844, 42.131, 42.868, 50.122, 67.581,101.639,108.032,112.827,117.868,132.868,136.188,140.122,143.442,148.360,152.172,156.967,167.418,173.073,185.122,191.762,199.262 },
                    std::vector<double>{ 0.1100, 0.0969, 0.0950, 0.0843, 0.0856, 0.0766, 0.0741, 0.0884, 0.0878, 0.0820, 0.0781, 0.0802, 0.0792, 0.0850, 0.0865, 0.0926, 0.0918, 0.0850, 0.0893, 0.0875, 0.0902, 0.0858, 0.0857, 0.1266, 0.1279, 0.1235, 0.1217, 0.1254, 0.1250, 0.1274, 0.1450, 0.1476, 0.1503, 0.1485, 0.1380, 0.1342, 0.0908, 0.1087, 0.0900, 0.0828, 0.0979, 0.0836, 0.0890, 0.1104, 0.1633, 0.0549, 0.0067, 0.1623, 0.2621, 0.0837, 0.2109, 0.1704, 0.0939, 0.1131, 0.0625, 0.2890, 0.4106, 0.2601, 0.2180, 0.1689, 0.1881, 0.1562, 0.1390, 0.1253, 0.1476, 0.1192, 0.1562, 0.1476, 0.1668, 0.1430, 0.1456, 0.1897, 0.1947, 0.1461, 0.1435, 0.1861, 0.3650, 0.2317, 0.2023 }),
            },
        // { polymorphs::gold,                      },
        // { polymorphs::silver,                    },
        // { polymorphs::copper,                    },
        { polymorphs::magnetite,                 
                relation::get_generic_spectral_function_from_reflectance_at_wavelengths // for magnetite
                    (si::micrometer,  233.0 *si::micrometer, 
                    std::vector<double>{  0.242,  0.257,  0.273,  0.301,  0.333,  0.392,  0.469,  0.555,  0.600,  0.703,  1.009,  1.111,  1.271,  1.755,  1.887,  1.914,  2.000,  2.019,  2.138,  2.442,  2.465,  2.496,  2.561,  2.595,  2.652,  2.720,  2.755,  2.779,  2.846,  2.882,  2.912,  2.980,  4.870, 13.431, 15.719, 16.678, 17.785, 18.745, 20.000, 22.361, 24.059, 28.929, 30.996, 32.915, 33.800, 38.671, 39.704, 40.811, 42.878, 44.428, 46.346, 55.424, 57.490, 62.287, 66.346, 70.258, 72.988, 84.575, 89.889,101.107,109.594,119.926,126.568 },
                    std::vector<double>{  0.052,  0.051,  0.058,  0.056,  0.059,  0.061,  0.057,  0.057,  0.058,  0.057,  0.044,  0.044,  0.048,  0.071,  0.075,  0.073,  0.079,  0.077,  0.081,  0.086,  0.084,  0.087,  0.086,  0.089,  0.087,  0.095,  0.081,  0.088,  0.090,  0.090,  0.102,  0.080,  0.110,  0.141,  0.164,  0.182,  0.222,  0.180,  0.175,  0.179,  0.186,  0.305,  0.270,  0.268,  0.277,  0.303,  0.300,  0.303,  0.319,  0.310,  0.324,  0.321,  0.315,  0.329,  0.315,  0.330,  0.321,  0.323,  0.340,  0.324,  0.332,  0.331,  0.324 }),
            },
        // { polymorphs::chalcocite_alpha,          },
        // { polymorphs::chalcocite_beta,           },
        { polymorphs::chalcopyrite,              
                relation::get_generic_spectral_function_from_reflectance_at_wavelengths // for chalcopyrite
                    (si::micrometer,  162 *si::micrometer, 
                    std::vector<double>{ 0.235, 0.246, 0.315, 0.342, 0.382, 0.406, 0.428, 0.546, 0.585, 0.700, 0.824, 0.840, 0.902, 0.951, 1.147, 1.459, 1.618, 1.656, 1.941, 2.423, 2.465, 2.527, 2.589, 2.750, 2.786, 2.812, 2.842, 2.883, 2.910, 2.942, 2.979, 3.512, 3.806, 5.509, 6.126, 6.233, 6.501, 6.675, 6.863, 7.627, 7.949, 8.190, 8.565, 8.686, 9.410, 9.651, 9.772,10.415,10.831,13.176,13.458,14.383,15.335,15.911,16.099,18.069,18.860,19.195,19.705,19.919,20.093,20.482,21.179,21.434,22.050,22.144,23.391,23.592,24.008,24.262,24.450,24.691 },
                    std::vector<double>{ 0.020, 0.031, 0.033, 0.027, 0.024, 0.026, 0.031, 0.084, 0.092, 0.099, 0.096, 0.094, 0.097, 0.098, 0.107, 0.109, 0.106, 0.107, 0.101, 0.103, 0.102, 0.104, 0.102, 0.103, 0.093, 0.103, 0.102, 0.114, 0.109, 0.110, 0.101, 0.111, 0.113, 0.119, 0.120, 0.121, 0.122, 0.122, 0.124, 0.126, 0.128, 0.124, 0.125, 0.127, 0.120, 0.120, 0.122, 0.122, 0.123, 0.123, 0.122, 0.123, 0.121, 0.121, 0.122, 0.121, 0.119, 0.117, 0.121, 0.119, 0.122, 0.118, 0.117, 0.118, 0.113, 0.114, 0.109, 0.107, 0.109, 0.107, 0.107, 0.105 }),
            },

    };


}}