#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/ids.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    table::PartialTable<si::pressure<double>> critical_point_pressure {
        {ids::water,            22.06 *  si::megapascal },
        {ids::nitrogen,         3.39 * si::megapascal   },
        {ids::oxygen,           5.043 * si::megapascal  },
        {ids::carbon_dioxide,   7.375 * si::megapascal  },
        {ids::methane,          4.60 * si::megapascal   },
        {ids::argon,            4.898* si::megapascal   },
        {ids::helium,           0.227 * si::megapascal  },
        {ids::hydrogen,         1.293 *  si::megapascal },
        {ids::ammonia,          11.357*si::megapascal   },
        {ids::ozone,            5.57 *  si::megapascal  },
        {ids::nitrous_oxide,    7.245 *  si::megapascal },
        {ids::sulfur_dioxide,   7.884 * si::megapascal  },
        {ids::nitric_oxide,     6.48 *  si::megapascal  },
        {ids::carbon_monoxide,  3.494 *  si::megapascal },
        {ids::ethane,           4.88 * si::megapascal   },
        {ids::hydrogen_cyanide, 5.4 * si::megapascal    },
        {ids::ethanol,          6.25 * si::megapascal   },
        {ids::formaldehyde,     6.788 * si::megapascal  },
        // {ids::formic_acid,      ,                       },
        {ids::perflouromethane, 3.73 * si::megapascal   },
        {ids::benzene,          4.9 * si::megapascal    },
        // {ids::pyrimidine,       ,                       },
        {ids::quartz,           1.7e8 * si::pascal      },
        {ids::halite,           26.0 * si::megapascal   }, // wikipedia data page
        // {ids::corundum,         ,                       },
        // {ids::apatite,          ,                       },
        {ids::carbon,           2200.0 * si::atmosphere },     // Leider (1973)
        // {ids::calcite,          ,                       },
        // {ids::orthoclase,       ,                       },
        // {ids::andesine,         ,                       },
        // {ids::augite,           ,                       },
        // {ids::forsterite,       ,                       },
        // {ids::goethite,         ,                       },
        // {ids::pyrite,           ,                       },
        // {ids::hematite,         ,                       },
        {ids::gold,             510e3 * si::kilopascal  }, // wow!
        // {ids::silver,           ,                       },
        // {ids::copper,           ,                       },
        // {ids::magnetite,        ,                       },
        // {ids::chalcocite,       ,                       },
        // {ids::chalcopyrite,     ,                       },
    };

    table::PartialTable<si::temperature<double>> critical_point_temperature {
        {ids::water,            647.01 * si::kelvin },
        {ids::nitrogen,         126.21 * si::kelvin },
        {ids::oxygen,           154.59 * si::kelvin },
        {ids::carbon_dioxide,   304.13 * si::kelvin },
        {ids::methane,          190.56 * si::kelvin },
        {ids::argon,            150.87 * si::kelvin },
        {ids::helium,           5.19 * si::kelvin   },
        {ids::hydrogen,         32.97 * si::kelvin  },
        {ids::ammonia,          405.56 * si::kelvin },
        {ids::ozone,            261.1 * si::kelvin  },
        {ids::nitrous_oxide,    309.52 * si::kelvin },
        {ids::sulfur_dioxide,   430.64 * si::kelvin },
        {ids::nitric_oxide,     180.0 * si::kelvin  },
        {ids::carbon_monoxide,  132.86 * si::kelvin },
        {ids::ethane,           305.36 * si::kelvin },
        {ids::hydrogen_cyanide, 456.65 * si::kelvin },
        {ids::ethanol,          351.44 * si::kelvin },
        {ids::formaldehyde,     410.3 * si::kelvin  },
        {ids::formic_acid,      588.0 * si::kelvin  },
        {ids::perflouromethane, 227.54 * si::kelvin },
        {ids::benzene,          562.0 * si::kelvin  },
        // {ids::pyrimidine,       ,                                         },
        {ids::quartz,           5300.0* si::kelvin  },
        {ids::halite,           3900.0 * si::kelvin }, // wikipedia data page
        // {ids::corundum,         ,                                         },
        // {ids::apatite,          ,                                         },
        {ids::carbon,           6810.0 * si::kelvin },     // Leider (1973)
        // {ids::calcite,          ,                                         },
        // {ids::orthoclase,       ,                                         },
        // {ids::andesine,         ,                                         },
        // {ids::augite,           ,                                         },
        // {ids::forsterite,       ,                                         },
        // {ids::goethite,         ,                                         },
        // {ids::pyrite,           ,                                         },
        // {ids::hematite,         ,                                         },
        {ids::gold,             7250.0 * si::kelvin }, // wow!
        {ids::silver,           6410.0 * si::kelvin },
        // {ids::copper,           ,                                         },
        // {ids::magnetite,        ,                                         },
        // {ids::chalcocite,       ,                                         },
        // {ids::chalcopyrite,     ,                                         },
    };

    table::PartialTable<double> critical_point_compressibility {
        {ids::water,            0.230},  // engineering toolbox
        // {ids::nitrogen,         },
        // {ids::oxygen,           },
        // {ids::carbon_dioxide,   },
        // {ids::methane,          },
        // {ids::argon,            },
        // {ids::helium,           },
        // {ids::hydrogen,         },
        // {ids::ammonia,          },
        // {ids::ozone,            },
        // {ids::nitrous_oxide,    },
        // {ids:: sulfur_dioxide,  },
        // {ids::nitric_oxide,     },
        // {ids::carbon_monoxide,  },
        // {ids::ethane,           },
        // {ids::hydrogen_cyanide, },
        // {ids::ethanol,          },
        // {ids::formaldehyde,     },
        // {ids::formic_acid,      },
        // {ids::perflouromethane, },
        // {ids::benzene,          },
        // {ids::pyrimidine,       },
        // {ids:: quartz,          },
        // {ids:: halite,          },
        // {ids:: corundum,        },
        // {ids:: apatite,         },
        // {ids::carbon,           },
        // {ids:: calcite,         },
        // {ids:: orthoclase,      },
        // {ids::andesine,         },
        // {ids::augite,           },
        // {ids::forsterite,       },
        // {ids:: goethite,        },
        // {ids:: pyrite,          },
        // {ids::hematite,         },
        // {ids:: gold,            },
        // {ids:: silver,          },
        // {ids:: copper,          },
        // {ids:: magnetite,       },
        // {ids::chalcocite,       },
        // {ids:: chalcopyrite,    },
    };

    table::PartialTable<si::molar_volume<double>> critical_point_volume {
        {ids::water,            56.0 *  si::centimeter3/si::mole},
        {ids::nitrogen,         90.0 * si::centimeter3/si::mole},
        {ids::oxygen,           73.0 * si::centimeter3/si::mole},
        {ids::carbon_dioxide,   94.0 * si::centimeter3/si::mole},
        {ids::methane,          99.0 * si::centimeter3/si::mole},
        {ids::argon,            75.0 * si::centimeter3/si::mole},
        {ids::helium,           57.0 * si::centimeter3/si::mole},
        {ids::hydrogen,         65.0 * si::centimeter3/si::mole},
        {ids::ammonia,          69.8*si::centimeter3/si::mole},
        {ids::ozone,            89.0 * si::centimeter3/si::mole},
        // {ids::nitrous_oxide,    }
        {ids::sulfur_dioxide,   122.0 * si::centimeter3/si::mole},
        {ids::nitric_oxide,     58.0 *  si::centimeter3/si::mole},
        {ids::carbon_monoxide,  93.0 * si::centimeter3/si::mole},
        {ids::ethane,           146.0 * si::centimeter3/si::mole},
        // {ids::hydrogen_cyanide, }
        {ids::ethanol,          169.0 * si::centimeter3/si::mole},
        // {ids::formaldehyde,     }
        {ids::formic_acid,      115.9*9*si::centimeter3/si::mole},
        {ids::perflouromethane, 140.0 * si::centimeter3/si::mole},
        {ids::benzene,          257.0*si::centimeter3/si::mole},
        // {ids::pyrimidine,       }
        // {ids::quartz,           }
        // {ids::halite,           }
        // {ids::corundum,         }
        // {ids::apatite,          }
        // {ids::carbon,           }
        // {ids::calcite,          }
        // {ids::orthoclase,       }
        // {ids::andesine,         }
        // {ids::augite,           }
        // {ids::forsterite,       }
        // {ids::goethite,         }
        // {ids::pyrite,           }
        // {ids::hematite,         }
        // {ids::gold,             }
        // {ids::silver,           }
        // {ids::copper,           }
        // {ids::magnetite,        }
        // {ids::chalcocite,       }
        // {ids::chalcopyrite,     }
    };

}}