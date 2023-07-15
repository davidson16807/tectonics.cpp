#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/relation/ExponentiatedPolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

#include "polymorphs.hpp"

namespace compound { 
namespace published { 

    using SolidVaporPressureTemperatureRelation = relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,si::pressure<double>,-1,1>;
    table::PartialTable<SolidVaporPressureTemperatureRelation> vapor_pressure_as_solid {

        { polymorphs::water_ice_1h,              
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-17.3,  11.2,   51.6,   100.0  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
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
        { polymorphs::nitrogen_ice_beta,         
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-226.1, -219.1, -209.7, -195.8 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        // { polymorphs::nitrogen_ice_alpha,        },
        // { polymorphs::nitrogen_ice_gamma,        },
        { polymorphs::oxygen_ice_gamma,          
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-219.1, -210.6, -198.8, -183.1 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        // { polymorphs::oxygen_ice_beta,           },
        // { polymorphs::oxygen_ice_alpha,          },
        { polymorphs::carbon_dioxide_ice,        
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-134.3, -119.5, -100.2, -78.2  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { polymorphs::methane_ice_alpha,         
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::kelvin, si::kilopascal,
                      std::vector<double>{65.0,     75.0,     85.0},     
                      std::vector<double>{0.1 ,      0.8 ,     4.9 }),
            },
        // { polymorphs::methane_ice_beta,          },
        // { polymorphs::methane_ice_beta,          },
        { polymorphs::argon_ice,                 
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::kelvin, si::kilopascal,
                     std::vector<double>{55.0,     65.0,     75.0}, 
                     std::vector<double>{0.2,      2.8,      18.7}),
            },
        { polymorphs::helium_ice,                
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-271.7, -271.3, -270.3, -268.6 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { polymorphs::hydrogen_ice,              
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-263.3, -261.3, -257.9, -252.5 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { polymorphs::ammonia_ice,               
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::kelvin, si::kilopascal,
                     std::vector<double>{160.0,     180.0,     190.0},     
                     std::vector<double>{0.1  ,     1.2  ,     3.5  }),
            },
        { polymorphs::ozone_ice,                 
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-180.4, -163.2, -141.0, -111.1 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { polymorphs::nitrous_oxide_ice,         
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-143.4, -128.7, -110.3, -85.5  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { polymorphs::sulfur_dioxide_ice,        
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-95.5,  -76.8,  -46.9,  -10.0  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { polymorphs::nitric_oxide_ice,          
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-184.5, -178.2, -166.0, -151.7 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { polymorphs::carbon_monoxide_ice_beta,  
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-222.0, -215.0, -205.7, -191.3 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        // { polymorphs::carbon_monoxide_ice_alpha, },
        { polymorphs::ethane_ice,                
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-159.5, -142.9, -119.3, -88.6  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { polymorphs::hydrogen_cyanide_ice,      
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-71.0,  -47.7,  -17.8,  25.9   }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        // { polymorphs::ethanol_ice,               },
        { polymorphs::formaldehyde_ice,          
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{ -88.0,  51.6,   100.0  }, 
                     std::vector<double>{ 10.0,   100.0,  760.0  }), // Perry
            },
        { polymorphs::formic_acid_ice,           
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::pascal,
                      std::vector<double>{-56.0,     -40.4,     -0.8}, 
                     std::vector<double>{1.0 ,       100.0,     1000.0}),
            },
        { polymorphs::perflouromethane_ice,      
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-184.6, -169.3, -150.7, -127.7 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { polymorphs::benzene_ice,               
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-36.7,  -11.5,  26.1,   80.1   }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        // { polymorphs::pyrimidine_ice,            },
        { polymorphs::quartz_alpha,              
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::kelvin, si::millimeter_mercury,
                     std::vector<double>{1732.0, 1969.0, 2227.0 }, 
                     std::vector<double>{10.0,   100.0,  760.0  }), // Perry, nonspecific solid phase
            },
        // { polymorphs::quartz_beta,               },
        // { polymorphs::crystoballite_alpha,       },
        // { polymorphs::crystoballite_beta,        },
        { polymorphs::halite,                    
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{865.0,  1017.0, 1220.0, 1460.0 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        // { polymorphs::corundum,                  },
        // { polymorphs::apatite,                   },
        { polymorphs::graphite,                  
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::pascal,
                     std::vector<double>{2566.0,     3016.0,     3635.0}, 
                     std::vector<double>{10.0 ,      1e3,        100e3}),
            },
        // { polymorphs::diamond,                   },
        // { polymorphs::calcite,                   },
        // { polymorphs::aragonite,                 },
        // { polymorphs::orthoclase,                },
        // { polymorphs::andesine,                  },
        // { polymorphs::augite,                    },
        // { polymorphs::goethite,                  },
        // { polymorphs::pyrite,                    },
        // { polymorphs::hematite,                  },
        { polymorphs::gold,                      
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{1869.0, 2154.0, 2521.0, 2966.0 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { polymorphs::silver,                    
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{1357.0, 1575.0, 1865.0, 2212.0 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { polymorphs::copper,                    
                relation::get_left_unbounded_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{1628.0, 1879.0, 2207.0, 2595.0 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        // { polymorphs::magnetite,                 },
        // { polymorphs::chalcocite_alpha,          },
        // { polymorphs::chalcocite_beta,           },
        // { polymorphs::chalcopyrite,              },

    };

}}
