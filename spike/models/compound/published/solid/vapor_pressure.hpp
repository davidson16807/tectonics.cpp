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

    using SolidVaporPressureTemperatureRelation = relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,si::pressure<double>,0,1>;
    std::map<int, SolidVaporPressureTemperatureRelation> vapor_pressure_as_solid {
        { ids::water,                   
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-17.3,  11.2,   51.6,   100.0  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::nitrogen,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-226.1, -219.1, -209.7, -195.8 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::oxygen,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-219.1, -210.6, -198.8, -183.1 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::carbon_dioxide,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-134.3, -119.5, -100.2, -78.2  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::methane,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::kelvin, si::kilopascal,
                      std::vector<double>{65.0,     75.0,     85.0},     
                      std::vector<double>{0.1 ,      0.8 ,     4.9 }),
            },
        { ids::argon,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::kelvin, si::kilopascal,
                     std::vector<double>{55.0,     65.0,     75.0}, 
                     std::vector<double>{0.2,      2.8,      18.7}),
            },
        { ids::helium,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-271.7, -271.3, -270.3, -268.6 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::hydrogen,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-263.3, -261.3, -257.9, -252.5 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::ammonia,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::kelvin, si::kilopascal,
                     std::vector<double>{160.0,     180.0,     190.0},     
                     std::vector<double>{0.1  ,     1.2  ,     3.5  }),
            },
        { ids::ozone,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-180.4, -163.2, -141.0, -111.1 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::nitrous_oxide,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-143.4, -128.7, -110.3, -85.5  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::sulfur_dioxide,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-95.5,  -76.8,  -46.9,  -10.0  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::nitric_oxide,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-184.5, -178.2, -166.0, -151.7 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::carbon_monoxide,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-222.0, -215.0, -205.7, -191.3 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::ethane,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-159.5, -142.9, -119.3, -88.6  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::hydrogen_cyanide,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-71.0,  -47.7,  -17.8,  25.9   }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        // { ids::ethanol,       },
        { ids::formaldehyde,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{ -88.0,  51.6,   100.0  }, 
                     std::vector<double>{ 10.0,   100.0,  760.0  }), // Perry
            },
        { ids::formic_acid,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::pascal,
                      std::vector<double>{-56.0,     -40.4,     -0.8}, 
                     std::vector<double>{1.0 ,       100.0,     1000.0}),
            },
        { ids::perflouromethane,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-184.6, -169.3, -150.7, -127.7 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::benzene,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-36.7,  -11.5,  26.1,   80.1   }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        // { ids::pyrimidine,    },
        { ids::quartz,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::kelvin, si::millimeter_mercury,
                     std::vector<double>{1732.0, 1969.0, 2227.0 }, 
                     std::vector<double>{10.0,   100.0,  760.0  }), // Perry, nonspecific solid phase
            },
        { ids::halite,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{865.0,  1017.0, 1220.0, 1460.0 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        // { ids::corundum,      },
        // { ids::apatite,       },
        { ids::carbon,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::pascal,
                     std::vector<double>{2566.0,     3016.0,     3635.0}, 
                     std::vector<double>{10.0 ,      1e3,        100e3}),
            },
        // { ids::calcite,       },
        // { ids::orthoclase,    },
        // { ids::andesine,      },
        // { ids::augite,        },
        // { ids::forsterite,    },
        // { ids::goethite,      },
        // { ids::pyrite,        },
        // { ids::hematite,      },
        { ids::gold,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{1869.0, 2154.0, 2521.0, 2966.0 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::silver,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{1357.0, 1575.0, 1865.0, 2212.0 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        { ids::copper,                    
                relation::get_exponential_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{1628.0, 1879.0, 2207.0, 2595.0 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            },
        // { ids::magnetite,     },
        // { ids::chalcocite,    },
        // { ids::chalcopyrite,  },
    };


}}