#pragma once

// in-house libraries
#include <math/lerp.hpp>
#include <units/si.hpp>

#include "CompletedCompound.hpp"
#include "field/missing.hpp"

// test utility libraries
#include <units/si_test_utils.hpp>

#include "field/constant/OptionalConstantField_test_utils.hpp"
#include "field/state/OptionalStateField_test_utils.hpp"
#include "field/spectral/OptionalSpectralField_test_utils.hpp"

#include "phase/gas/CompletedGas_test_utils.hpp"
#include "phase/liquid/CompletedLiquid_test_utils.hpp"
#include "phase/solid/CompletedSolid_test_utils.hpp"

namespace compound 
{
    bool operator==(const CompletedCompound& first, const CompletedCompound& second)
    {
        if(
            first.molar_mass                         != second.molar_mass                         ||
            first.atoms_per_molecule                 != second.atoms_per_molecule                 ||
            first.molecular_diameter                 != second.molecular_diameter                 ||
            first.molecular_degrees_of_freedom       != second.molecular_degrees_of_freedom       ||
            first.acentric_factor                    != second.acentric_factor                    ||
            first.critical_point_pressure            != second.critical_point_pressure            ||
            first.critical_point_volume              != second.critical_point_volume              ||
            first.critical_point_temperature         != second.critical_point_temperature         ||
            first.critical_point_compressibility     != second.critical_point_compressibility     ||
            first.latent_heat_of_vaporization        != second.latent_heat_of_vaporization        ||
            first.latent_heat_of_fusion              != second.latent_heat_of_fusion              ||
            first.triple_point_pressure              != second.triple_point_pressure              ||
            first.triple_point_temperature           != second.triple_point_temperature           ||
            first.freezing_point_sample_pressure     != second.freezing_point_sample_pressure     ||
            first.freezing_point_sample_temperature  != second.freezing_point_sample_temperature  ||
            first.boiling_point_sample_pressure      != second.boiling_point_sample_pressure      ||
            first.boiling_point_sample_temperature   != second.boiling_point_sample_temperature   ||
            first.simon_glatzel_slope                != second.simon_glatzel_slope                ||
            first.simon_glatzel_exponent             != second.simon_glatzel_exponent             ||
            first.molecular_absorption_cross_section != second.molecular_absorption_cross_section ||
            first.gas                                != second.gas                                ||
            first.liquid                             != second.liquid                             
        ) {
            return false;
        }
        for (std::size_t i = 0; i <first.solids.size(); i++)
        {
            if (first.solids[i] != second.solids[i])
            {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const CompletedCompound& first, const CompletedCompound& second)
    {
        return !(first==second);
    }

}
