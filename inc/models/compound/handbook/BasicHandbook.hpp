#pragma once

// std libraries
#include <string>

// in-house libraries
#include <models/compound/correlation/published.hpp>
#include <models/compound/table/FullTable.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound{
namespace estimated{


    /*
    `BasicHandbook` serves the role of a chemical handbook 
    that contains "basic" properties that are guaranteed to be known for any chemical compound
    */
    struct BasicHandbook
    {
        table::FullTable<std::string> name;
        table::FullTable<std::string> formula;
        table::FullTable<unsigned int> atoms_per_molecule;
        table::FullTable<si::molar_mass<double>> molar_mass;
        BasicHandbook(
            const table::FullTable<std::string> name,
            const table::FullTable<std::string> formula,
            const table::FullTable<unsigned int> atoms_per_molecule,
            const table::FullTable<si::molar_mass<double>> molar_mass
        ):
            name(name),
            formula(formula),
            atoms_per_molecule(atoms_per_molecule),
            molar_mass(molar_mass)
        {

        }
    };


}}

