#pragma once

#include <string>

#include <models/compound/field/state/CompletedStateField_to_string.hpp>
#include <models/compound/field/spectral/CompletedSpectralField_to_string.hpp>

#include <models/compound/phase/gas/CompletedGas.hpp>

namespace compound {
namespace phase {
	std::string to_string(const CompletedGas& gas, const field::StateParameters& state, const field::SpectralParameters& spectrum)
	{
		std::string out("");
        out += "\n";
		out += "Specific Heat Capacity               " + field::to_string(gas.specific_heat_capacity, state)    + "\n";
		out += "Thermal Conductivity                 " + field::to_string(gas.thermal_conductivity,   state)    + "\n";
		out += "Dynamic Viscosity                    " + field::to_string(gas.dynamic_viscosity,      state)    + "\n";
		out += "Density                              " + field::to_string(gas.density,                state)    + "\n";
		out += "Refractive Index                     " + field::to_string(gas.refractive_index,       spectrum) + "\n";
        out += "\n";
		return out;
	}
	std::string to_string(const CompletedGas& gas)
	{
		return to_string(gas, field::StateParameters(), field::SpectralParameters());
	}

}} //namespace 



