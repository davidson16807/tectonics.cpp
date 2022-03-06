#pragma once

#include <string>

#include <models/compound/field/constant/OptionalConstantField_to_string.hpp>
#include <models/compound/field/state/OptionalStateField_to_string.hpp>
#include <models/compound/field/spectral/OptionalSpectralField_to_string.hpp>

#include <models/compound/relation/OptionalStateRelation_to_string.hpp>
#include <models/compound/relation/SplineRelation_to_string.hpp>

#include <models/compound/phase/solid/PartlyKnownSolid.hpp>

namespace compound {
namespace phase {
	std::string to_string(const PartlyKnownSolid& solid, const field::StateParameters& state, const field::SpectralParameters& spectrum)
	{
		std::string out("");
        out += "\n";
        out += "Isobaric Specific Heat Capacity      " + field::to_string(solid.isobaric_specific_heat_capacity,  state)     + "\n";
		out += "Thermal Conductivity                 " + field::to_string(solid.thermal_conductivity,             state)     + "\n";
		out += "Dynamic Viscosity                    " + field::to_string(solid.dynamic_viscosity,                state)     + "\n";
		out += "Density                              " + field::to_string(solid.density,                          state)     + "\n";
		out += "Vapor Pressure                       " + field::to_string(solid.vapor_pressure,                   state)     + "\n";
		out += "Refractive Index                     " + relation::to_string(solid.refractive_index,       (spectrum.nlo+spectrum.nhi)/2.0f)  + "\n";
		out += "Extinction Coefficient               " + relation::to_string(solid.extinction_coefficient, (spectrum.nlo+spectrum.nhi)/2.0f)  + "\n";
		out += "Absorption Coefficient               " + relation::to_string(solid.absorption_coefficient, (spectrum.nlo+spectrum.nhi)/2.0f)  + "\n";
		out += "Bulk Modulus                         " + field::to_string(solid.bulk_modulus,                     state)     + "\n";
		out += "Tensile Modulus                      " + field::to_string(solid.tensile_modulus,                  state)     + "\n";
		out += "Shear Modulus                        " + field::to_string(solid.shear_modulus,                    state)     + "\n";
		out += "Pwave Modulus                        " + field::to_string(solid.pwave_modulus,                    state)     + "\n";
		out += "Lame Parameter                       " + field::to_string(solid.lame_parameter,                   state)     + "\n";
		out += "Poisson Ratio                        " + field::to_string(solid.poisson_ratio,                    state)     + "\n";
		out += "Compressive Fracture Strength        " + field::to_string(solid.compressive_fracture_strength,    state)     + "\n";
		out += "Tensile Fracture Strength            " + field::to_string(solid.tensile_fracture_strength,        state)     + "\n";
		out += "Shear Fracture Strength              " + field::to_string(solid.shear_fracture_strength,          state)     + "\n";
		out += "Compressive Yield Strength           " + field::to_string(solid.compressive_yield_strength,       state)     + "\n";
		out += "Tensile Yield Strength               " + field::to_string(solid.tensile_yield_strength,           state)     + "\n";
		out += "Shear Yield Strength                 " + field::to_string(solid.shear_yield_strength,             state)     + "\n";
		out += "Chemical Susceptibility Estimate     " + field::to_string(solid.chemical_susceptibility_estimate       )     + "\n";
        out += "\n";
		return out;
	}
	std::string to_string(const PartlyKnownSolid& solid)
	{
		return to_string(solid, field::StateParameters(), field::SpectralParameters());
	}

}} //namespace 



