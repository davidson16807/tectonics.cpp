#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/ids.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    table::PartialTable<si::pressure<double>> critical_point_pressure {
        {compounds::water,            22.06 *  si::megapascal },
        {compounds::nitrogen,         3.39 * si::megapascal   },
        {compounds::oxygen,           5.043 * si::megapascal  },
        {compounds::carbon_dioxide,   7.375 * si::megapascal  },
        {compounds::methane,          4.60 * si::megapascal   },
        {compounds::argon,            4.898* si::megapascal   },
        {compounds::helium,           0.227 * si::megapascal  },
        {compounds::hydrogen,         1.293 *  si::megapascal },
        {compounds::ammonia,          11.357*si::megapascal   },
        {compounds::ozone,            5.57 *  si::megapascal  },
        {compounds::nitrous_oxide,    7.245 *  si::megapascal },
        {compounds::sulfur_dioxide,   7.884 * si::megapascal  },
        {compounds::nitric_oxide,     6.48 *  si::megapascal  },
        {compounds::carbon_monoxide,  3.494 *  si::megapascal },
        {compounds::ethane,           4.88 * si::megapascal   },
        {compounds::hydrogen_cyanide, 5.39 * si::megapascal   }, // Lange
        {compounds::ethanol,          6.25 * si::megapascal   },
        {compounds::formaldehyde,     6.788 * si::megapascal  },
        // {compounds::formic_acid,      ,                       },
        {compounds::perflouromethane, 3.73 * si::megapascal   },
        {compounds::benzene,          4.9 * si::megapascal    },
        // {compounds::pyrimidine,       ,                       },
        {compounds::quartz,           1.7e8 * si::pascal      },
        {compounds::halite,           26.0 * si::megapascal   }, // wikipedia data page
        // {compounds::corundum,         ,                       },
        // {compounds::apatite,          ,                       },
        {compounds::carbon,           2200.0 * si::atmosphere },     // Leider (1973)
        // {compounds::calcite,          ,                       },
        // {compounds::orthoclase,       ,                       },
        // {compounds::andesine,         ,                       },
        // {compounds::augite,           ,                       },
        // {compounds::forsterite,       ,                       },
        // {compounds::goethite,         ,                       },
        // {compounds::pyrite,           ,                       },
        // {compounds::hematite,         ,                       },
        {compounds::gold,             510e3 * si::kilopascal  }, // wow!
        // {compounds::silver,           ,                       },
        // {compounds::copper,           ,                       },
        // {compounds::magnetite,        ,                       },
        // {compounds::chalcocite,       ,                       },
        // {compounds::chalcopyrite,     ,                       },
    };

    table::PartialTable<si::temperature<double>> critical_point_temperature {
        {compounds::water,            647.01 * si::kelvin },
        {compounds::nitrogen,         126.21 * si::kelvin },
        {compounds::oxygen,           154.59 * si::kelvin },
        {compounds::carbon_dioxide,   304.13 * si::kelvin },
        {compounds::methane,          190.56 * si::kelvin },
        {compounds::argon,            150.87 * si::kelvin },
        {compounds::helium,           5.19 * si::kelvin   },
        {compounds::hydrogen,         32.97 * si::kelvin  },
        {compounds::ammonia,          405.56 * si::kelvin },
        {compounds::ozone,            261.1 * si::kelvin  },
        {compounds::nitrous_oxide,    309.52 * si::kelvin },
        {compounds::sulfur_dioxide,   430.64 * si::kelvin },
        {compounds::nitric_oxide,     180.0 * si::kelvin  },
        {compounds::carbon_monoxide,  132.86 * si::kelvin },
        {compounds::ethane,           305.36 * si::kelvin },
        {compounds::hydrogen_cyanide, 183.5 * si::kelvin  }, // Lange
        {compounds::ethanol,          513.9 * si::kelvin  },
        {compounds::formaldehyde,     410.3 * si::kelvin  },
        {compounds::formic_acid,      588.0 * si::kelvin  },
        {compounds::perflouromethane, 227.54 * si::kelvin },
        {compounds::benzene,          562.0 * si::kelvin  },
        // {compounds::pyrimidine,       ,                                         },
        {compounds::quartz,           5300.0* si::kelvin  },
        {compounds::halite,           3900.0 * si::kelvin }, // wikipedia data page
        // {compounds::corundum,         ,                                         },
        // {compounds::apatite,          ,                                         },
        {compounds::carbon,           6810.0 * si::kelvin },     // Leider (1973)
        // {compounds::calcite,          ,                                         },
        // {compounds::orthoclase,       ,                                         },
        // {compounds::andesine,         ,                                         },
        // {compounds::augite,           ,                                         },
        // {compounds::forsterite,       ,                                         },
        // {compounds::goethite,         ,                                         },
        // {compounds::pyrite,           ,                                         },
        // {compounds::hematite,         ,                                         },
        {compounds::gold,             7250.0 * si::kelvin }, // wow!
        {compounds::silver,           6410.0 * si::kelvin },
        // {compounds::copper,           ,                                         },
        // {compounds::magnetite,        ,                                         },
        // {compounds::chalcocite,       ,                                         },
        // {compounds::chalcopyrite,     ,                                         },
    };

    table::PartialTable<double> critical_point_compressibility {
        {compounds::water,            0.230},  // engineering toolbox
        // {compounds::nitrogen,         },
        // {compounds::oxygen,           },
        // {compounds::carbon_dioxide,   },
        // {compounds::methane,          },
        // {compounds::argon,            },
        // {compounds::helium,           },
        // {compounds::hydrogen,         },
        // {compounds::ammonia,          },
        // {compounds::ozone,            },
        // {compounds::nitrous_oxide,    },
        // {compounds:: sulfur_dioxide,  },
        // {compounds::nitric_oxide,     },
        // {compounds::carbon_monoxide,  },
        // {compounds::ethane,           },
        // {compounds::hydrogen_cyanide, },
        // {compounds::ethanol,          },
        // {compounds::formaldehyde,     },
        // {compounds::formic_acid,      },
        // {compounds::perflouromethane, },
        // {compounds::benzene,          },
        // {compounds::pyrimidine,       },
        // {compounds:: quartz,          },
        // {compounds:: halite,          },
        // {compounds:: corundum,        },
        // {compounds:: apatite,         },
        // {compounds::carbon,           },
        // {compounds:: calcite,         },
        // {compounds:: orthoclase,      },
        // {compounds::andesine,         },
        // {compounds::augite,           },
        // {compounds::forsterite,       },
        // {compounds:: goethite,        },
        // {compounds:: pyrite,          },
        // {compounds::hematite,         },
        // {compounds:: gold,            },
        // {compounds:: silver,          },
        // {compounds:: copper,          },
        // {compounds:: magnetite,       },
        // {compounds::chalcocite,       },
        // {compounds:: chalcopyrite,    },
    };

    table::PartialTable<si::molar_volume<double>> critical_point_volume {
        {compounds::water,            56.0 *  si::centimeter3/si::mole},
        {compounds::nitrogen,         90.0 * si::centimeter3/si::mole},
        {compounds::oxygen,           73.0 * si::centimeter3/si::mole},
        {compounds::carbon_dioxide,   94.0 * si::centimeter3/si::mole},
        {compounds::methane,          99.0 * si::centimeter3/si::mole},
        {compounds::argon,            75.0 * si::centimeter3/si::mole},
        {compounds::helium,           57.0 * si::centimeter3/si::mole},
        {compounds::hydrogen,         65.0 * si::centimeter3/si::mole},
        {compounds::ammonia,          69.8*si::centimeter3/si::mole},
        {compounds::ozone,            89.0 * si::centimeter3/si::mole},
        // {compounds::nitrous_oxide,    }
        {compounds::sulfur_dioxide,   122.0 * si::centimeter3/si::mole},
        {compounds::nitric_oxide,     58.0 *  si::centimeter3/si::mole},
        {compounds::carbon_monoxide,  93.0 * si::centimeter3/si::mole},
        {compounds::ethane,           146.0 * si::centimeter3/si::mole},
        // {compounds::hydrogen_cyanide, }
        {compounds::ethanol,          169.0 * si::centimeter3/si::mole},
        // {compounds::formaldehyde,     }
        {compounds::formic_acid,      115.9*9*si::centimeter3/si::mole},
        {compounds::perflouromethane, 140.0 * si::centimeter3/si::mole},
        {compounds::benzene,          257.0*si::centimeter3/si::mole},
        // {compounds::pyrimidine,       }
        // {compounds::quartz,           }
        // {compounds::halite,           }
        // {compounds::corundum,         }
        // {compounds::apatite,          }
        // {compounds::carbon,           }
        // {compounds::calcite,          }
        // {compounds::orthoclase,       }
        // {compounds::andesine,         }
        // {compounds::augite,           }
        // {compounds::forsterite,       }
        // {compounds::goethite,         }
        // {compounds::pyrite,           }
        // {compounds::hematite,         }
        // {compounds::gold,             }
        // {compounds::silver,           }
        // {compounds::copper,           }
        // {compounds::magnetite,        }
        // {compounds::chalcocite,       }
        // {compounds::chalcopyrite,     }
    };

}}