#pragma once

// in-house libraries
#include <unit/si.hpp>
#include <model/compound/ids.hpp>
#include <model/compound/relation/GenericRelation.hpp>
#include <model/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    using LiquidSurfaceTensionTemperatureRelation = relation::GenericRelation<si::temperature<double>,si::surface_energy<double>>;
    table::PartialTable<LiquidSurfaceTensionTemperatureRelation> surface_tension_as_liquid {
        { compounds::water,            
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     647.01, -0.1306, 2.471, 0.2151, 1.233, 0.0, 0.0, 
                     233.22, 646.15), // Mulero (2012)
            },
        { compounds::nitrogen,        
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     126.21, 0.02898, 1.246, 0.0, 0.0, 0.0, 0.0,
                     64.8, 120.24), // Mulero (2012)
            },
        { compounds::oxygen,        
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     154.59, 0.03843, 1.225, 0.0, 0.0, 0.0, 0.0,
                     55.7, 152.08), // Mulero (2012)
            },
        { compounds::carbon_dioxide,        
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     304.13, 0.07863, 1.254, 0.0, 0.0, 0.0, 0.0,
                     216.55, 304.11), // Mulero (2012)
            },
        { compounds::methane,        
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     190.56, 0.03825, 1.191, -0.006024, 5.422, -0.0007065, 0.6161,
                     90.67, 188.84), // Mulero (2012)
            },
        { compounds::argon,        
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     150.87, 0.037, 1.25, 0.0, 0.0, 0.0, 0.0,
                     83.82, 148.15), // Mulero (2012)
            },
        { compounds::helium,        
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     5.19, 0.0004656, 1.040, 0.001889, 2.468, -0.002006, 2.661,
                     0.0, 5.11), // Mulero (2012)
            },
        { compounds::hydrogen,        
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     32.97, -1.4165, 0.63882, 0.746383, 0.659804, 0.675625, 0.619149,
                     13.8, 31.0), // Mulero (2012)
            },
        { compounds::ammonia,        // 0.021 * si::newton/si::meter, // 25C, engineering toolbox
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     405.56, 0.1028, 1.211, -0.09453, 5.585, 0.0, 0.0,
                     197.85, 403.15), // Mulero (2012)
            },
        { compounds::ozone,        38.1 * si::dyne/si::centimeter, // -182.7C, Jasper (1972)
            },
        { compounds::nitrous_oxide,        
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     309.52, 0.07087, 1.204, 0.0, 0.0, 0.0, 0.0,
                     182.50, 293.15), // Mulero (2012)
            },
        { compounds::sulfur_dioxide,        
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     430.64, 0.0803, 0.928, 0.0139, 1.570, -0.0114, 0.364,
                     189.15, 373.15), // Mulero (2012)
            },
        { compounds::nitric_oxide,        
                relation::get_linear_interpolation_anonymous_temperature_relation
                    (si::kelvin, si::dyne/si::centimeter,
                     std::vector<double>{-160.0,-156.0,-152.0}, 
                     std::vector<double>{ 26.17, 23.83, 21.49},
                     -160.0, 152.0), // Jasper (1972)
            },
        { compounds::carbon_monoxide,        
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     132.86, 0.02843, 1.148, 0.0, 0.0, 0.0, 0.0,
                     70.0, 91.89), // Mulero (2012)
            },
        { compounds::ethane,        
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     305.36, 0.07602, 1.320, -0.02912, 1.676, 0.0, 0.0,
                     89.87, 304.93), // Mulero (2012)
            },
        // { compounds::hydrogen_cyanide,        
        //         relation::get_quadratic_liquid_surface_tension_temperature_function
        //             (si::celcius, si::dyne/si::centimeter, 
        //             20.62, -0.1155, 7e-5, 
        //             -13.3, 25.0), 
        //     },
        { compounds::ethanol,        
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     513.9, 0.05, 0.952, 0.0, 0.0, 0.0, 0.0,
                     180.12, 513.15), // Mulero (2012)
            },
        { compounds::formaldehyde,        27.3797 * si::dyne/si::centimeter, // 25 °C, PubChem
            },
        { compounds::formic_acid,        
                relation::get_linear_interpolation_anonymous_temperature_relation
                    (si::celcius, si::millinewton/si::meter,
                     std::vector<double>{ 25.0,  50.0,  75.0 }, 
                     std::vector<double>{37.13, 34.38, 31.64 },
                      25.0, 75.0), 
            },
        // { compounds::perflouromethane, },
        { compounds::benzene,        
                relation::get_refprop_liquid_surface_tension_temperature_relation
                    (si::kelvin, si::newton/si::meter,
                     562.0, 0.07298, 1.232, -0.0007802, 0.8635, -0.0001756, 0.3065,
                     272.95, 553.15), // Mulero (2012)
            },
        { compounds::pyrimidine,        
                relation::get_linear_interpolation_anonymous_temperature_relation
                    (si::celcius, si::millinewton/si::meter,
                     std::vector<double>{ 25.0,  50.0,  75.0, 100.0 }, 
                     std::vector<double>{30.33, 27.80, 25.28, 22.75 },
                      25.0, 100.0), 
            },
        { compounds::quartz,                 312.0 * si::dyne / si::centimeter, // 1400C, Shartsis (1951)
            },
        { compounds::halite,        
                relation::get_linear_interpolation_anonymous_temperature_relation
                    (si::celcius, si::dyne/si::centimeter,
                     std::vector<double>{ 1080.0, 1250.0 },
                     std::vector<double>{  112.5,  102.5 },
                      1080.0, 1250.0), // Sato (1990)
            },
        // { compounds::corundum,         },
        // { compounds::apatite,          },
        // { compounds::carbon,           },
        // { compounds::calcite,          },
        // { compounds::orthoclase,       },
        { compounds::andesine,        
                relation::get_linear_interpolation_anonymous_temperature_relation
                    (si::celcius, si::dyne/si::centimeter,
                     std::vector<double>{ 1300.0, 1600.0 },
                     std::vector<double>{  400.0,  300.0 },
                      1300.0, 1600.0), // from Taniguchi (1988), for Anorthite
            },
        { compounds::augite,        
                relation::get_linear_interpolation_anonymous_temperature_relation
                    (si::celcius, si::dyne/si::centimeter,
                     std::vector<double>{ 1228.0, 1438.0 },
                     std::vector<double>{  388.5,  364.4 },
                      1228.0, 1438.0), // from Walker (1981), for Basalt
            },
        { compounds::forsterite,        
                relation::get_linear_interpolation_anonymous_temperature_relation
                    (si::celcius, si::dyne/si::centimeter,
                     std::vector<double>{ 1246.0, 1450.0 },
                     std::vector<double>{  380.0,  350.0 },
                      1246.0, 1450.0), // from Walker (1981), for Limburgite
            },
        // { compounds::goethite,         },
        // { compounds::pyrite,           },
        // { compounds::hematite,         },
        { compounds::gold,        
                relation::get_linear_liquid_surface_tension_temperature_function
                        (si::kelvin, si::newton/si::meter, 
                         1338.0, 1.162, -1.8e-4,
                         1360.0, 1480.0), // Egry(2010)
                        // TODO: implement Tmax and Tmin for this function
            },
        { compounds::silver,        
                relation::get_linear_liquid_surface_tension_temperature_function
                        (si::kelvin, si::newton/si::meter, 
                         1234.0, 0.914, -1.5e-4,
                         1360.0, 1480.0), // Egry(2010)
                        // TODO: implement Tmax and Tmin for this function
            },
        { compounds::copper,        
                relation::get_linear_liquid_surface_tension_temperature_function
                        (si::kelvin, si::newton/si::meter, 
                         1357.0, 1.339, -1.8e-4,
                         1360.0, 1480.0), // Egry(2010)
                        // TODO: implement Tmax and Tmin for this function
            },
        // { compounds::magnetite,        },
        // { compounds::chalcocite,       },
        // { compounds::chalcopyrite,     },
    };

}}