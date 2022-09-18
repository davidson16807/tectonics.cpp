#pragma once

// 3rd party libraries

namespace compound {
namespace property {
    /*
    NOTE: 
    The namespace here each serves as a "category of primitives" where objects are variables of primitive types.
    We may consider moving to the academic layer if we discover its implementation can be done in a general way.

    This namespace maps to and from aspects of a compound's shininess, namely measures of refractivity
    */

    /*
    "get_molecular_refraction_of_refractive_index_at_stp" returns molecular refraction 
    from refractive index at standard temperature and pressure
    see description from https://encyclopedia2.thefreedictionary.com/Molecular+Refraction
    or https://goldbook.iupac.org/terms/view/M03979 for the related concept, "molar refraction"
    see trying-to-find-molecular-refraction.md for how this was derived

    "molecular_refractions" can be thought of as a refractive tendency when applied to a volume per molecule
    see description from https://encyclopedia2.thefreedictionary.com/Molecular+Refraction
    or https://goldbook.iupac.org/terms/view/M03979 for the related concept, "molar refraction"
    derived using refractive indices from https://www.engineeringtoolbox.com/refractive-index-d_1264.html
    as well as https://refractiveindex.info/?shelf=organic&book=ethane&page=Loria
    */
    constexpr float get_molecular_refraction_of_refractive_index_at_stp(const float n) {
        return ((n*n-1.0f)/(n*n+2.0f)) * si::standard_molar_volume / si::mole;
    }
}}