#pragma once

// std libraries
#include <string>

#include <model/compound/table/FullTable.hpp>

namespace compound { 
namespace published { 

    table::FullTable<std::string> formula {


        /*water*/            "H₂O",
        // for the oceans and ice caps of earth, and the surface and mantle of europa, the surface of pluto, 
        // and to guarantee that default values exist for any other compound whose properties cannot be found

        /*nitrogen*/         "N₂",
        // for the atmosphere of Earth, and the surfaces of pluto or triton

        /*oxygen*/           "O₂",
        // for atmospheres of earth-like planets

        /*carbon_dioxide*/   "CO₂",
        // for the atmospheres earth, venus, and mars, as well as the ice caps of mars

        /*methane*/          "CH₄",
        // for the atmosphere of Titan, and surfaces of pluto or other kuiper belt objects

        /*argon*/            "Ar",
        // for the atmospheres of earth like planets

        /*helium*/           "He",
        // for the atmospheres of gas giants

        /*hydrogen*/         "H",
        // for the atmospheres of gas giants

        /*ammonia*/          "NH₃",
        // for the atmosphere of Titan

        /*ozone*/            "O₃",
        // for modeling the ozone layer of earth like planets

        /*nitrous_oxide*/    "N₂O",
        // representative of industrial emissions

        /*sulfur_dioxide*/   "SO₂",
        // representative of industrial emissions

        /*nitric_oxide*/     "NO",
        // representative of industrial emissions

        /*carbon_monoxide*/  "CO",
        // for the surface of pluto

        /*ethane*/           "C₂H₆",
        // for the lakes of Titan

        /*hydrogen_cyanide*/ "HCN",
        // for small bodies in the outer solar system and interstellar space
        // one of the most abundant compounds following from elemental abundances in the universe

        /*ethanol*/          "C₂H₆O",
        // for small bodies in the outer solar system and interstellar space
        // one of the most abundant compounds following from elemental abundances in the universe

        /*formaldehyde*/     "CH₂O",
        // for small bodies in the outer solar system and interstellar space
        // one of the most abundant compounds following from elemental abundances in the universe

        /*formic_acid*/      "HCOOH",
        // for small bodies in the outer solar system and interstellar space
        // one of the most abundant compounds following from elemental abundances in the universe

        /*perflouromethane*/ "CF₄",
        // perflouromethane, tetraflouromethane, carbon tetraflouride, CF4
        // for modeling industrial emissions and the terraforming of Mars as suggested by Zubrin (1996) 

        /*benzene*/          "C₆H₆",
        // representative of aromatic compounds, predominantly low-grade coal,
        // and serves as a template for compounds in prebiotic chemistry such as nucleic acids or tholins

        /*pyrimidine*/       "C₄H₄N₂",
        // representative of prebiotic chemistry and tholins,
        // since it's been observed from Kawai (2019) that tholins may contain them among other nucleotides

        /*quartz*/           "SiO₂",
        // silicon dioxide, silica, glass
        // representative of felsic rocks, namely sand, and biomineral deposits like diatomaceous earth  

        /*halite*/           "NaCl",
        // sodium chloride, table salt
        // for salt bed flats and potentially modeling ocean salinity

        /*corundum*/         "Al₂O₃",
        // aluminum sequioxide, aluminum oxide, alumina, ruby, sapphire, beryl
        // representative of precious stones, excluding diamond

        /*apatite*/          "Ca₅(PO₄)₃(F,Cl,OH)",
        // theoretical biomineral deposits, analogous to limestone, that could occur on alien planets

        /*carbon*/           "C",
        // for diamonds, graphite, high-grade anthracite coal, 
        // and theoretical exobiominerals deposits analogous to limestone 

        /*calcite*/          "CaCO₃",
        // calcium carbonate
        // for biomineral deposits like limestone

        /*orthoclase*/       "KAlSi₃O₈",
        // representative of felsic rocks

        /*andesine*/         "(Ca,Na)(Al,Si)₄O₈",
        // representative of plagioclase as a common plagioclase mineral in andesite and diorite,
        // representative of intermediate rocks in general

        /*augite*/           "(Ca,Na)(Mg,Fe,Al,Ti)(Si,Al)₂O₆",
        // representative of pyroxenes as the most common pyroxene mineral
        // representative of mafic rocks in general

        /*forsterite*/       "Mg₂SiO₄",
        // representative of Olivine ((Mg,Fe)SiO) as its much more common Mg-rich end member (Smyth 2006)
        // representative of ultramafic rocks in general
        // also appears most common on the surface of Mercury (Namur 2016)

        /*goethite*/         "FeO(OH)",
        // for surface of mars, representative of iron oxides and red soils in general, and siderophile ores

        /*pyrite*/           "FeS₂",
        // iron sulfide
        // for surfaces of the heaviest planets, maybe venus and mercury, and representative of siderophile ores

        /*hematite*/         "Fe₂O₃",
        // iron oxide
        // representative of iron oxides and red soils in general, surfaces of early earth, and siderophile ores

        /*gold*/             "Au",
        // for precious metals

        /*silver*/           "Ag",
        // for precious metals

        /*copper*/           "Cu",
        // for precious metals

        /*magnetite*/        "Fe₃O₄",
        // representative of siderophile ores, and for surfaces of mars and maybe venus

        /*chalcocite*/       "Cu₂S",
        // representative of chalcophile ores, and for surfaces of maybe venus and mercury

        /*chalcopyrite*/     "CuFeS₂",
        // representative of chalcophile ores, and for surfaces of maybe venus and mercury

    };

}}