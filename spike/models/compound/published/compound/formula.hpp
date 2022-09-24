#pragma once

// std libraries
#include <string>

#include <models/compound/table/FullTable.hpp>

namespace compound { 
namespace published { 

    table::FullTable<std::string> formula {
        /*water*/            "H₂O",
        /*nitrogen*/         "N₂",
        /*oxygen*/           "O₂",
        /*carbon_dioxide*/   "CO₂",
        /*methane*/          "CH₄",
        /*argon*/            "Ar",
        /*helium*/           "He",
        /*hydrogen*/         "H",
        /*ammonia*/          "NH₃",
        /*ozone*/            "O₃",
        /*nitrous_oxide*/    "N₂O",
        /*sulfur_dioxide*/   "SO₂",
        /*nitric_oxide*/     "NO",
        /*carbon_monoxide*/  "CO",
        /*ethane*/           "C₂H₆",
        /*hydrogen_cyanide*/ "HCN",
        /*ethanol*/          "C₂H₆O",
        /*formaldehyde*/     "CH₂O",
        /*formic_acid*/      "HCOOH",
        /*perflouromethane*/ "CF₄",
        /*benzene*/          "C₆H₆",
        /*pyrimidine*/       "C₄H₄N₂",
        /*quartz*/           "SiO₂",
        /*halite*/           "NaCl",
        /*corundum*/         "Al₂O₃",
        /*apatite*/          "Ca₅(PO₄)₃(F,Cl,OH)",
        /*carbon*/           "C",
        /*calcite*/          "CaCO₃",
        /*orthoclase*/       "KAlSi₃O₈",
        /*andesine*/         "(Ca,Na)(Al,Si)₄O₈",
        /*augite*/           "(Ca,Na)(Mg,Fe,Al,Ti)(Si,Al)₂O₆",
        /*forsterite*/       "Mg₂SiO₄",
        /*goethite*/         "FeO(OH)",
        /*pyrite*/           "FeS₂",
        /*hematite*/         "Fe₂O₃",
        /*gold*/             "Au",
        /*silver*/           "Ag",
        /*copper*/           "Cu",
        /*magnetite*/        "Fe₃O₄",
        /*chalcocite*/       "Cu₂S",
        /*chalcopyrite*/     "CuFeS₂",
    };

}}