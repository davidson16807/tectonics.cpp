#pragma once

#include <string>

#include <models/compound/field/state/CompletedStateField_to_string.hpp>
#include <models/compound/field/spectral/CompletedSpectralField_to_string.hpp>

#include <models/compound/relation/GasPropertyStateRelation_to_string.hpp>
#include <models/compound/relation/PolynomialRailyardRelation_to_string.hpp>

#include <models/compound/phase/gas/CompletedGas.hpp>

namespace compound {
namespace phase {
	std::string to_string(const CompletedGas& gas, const field::StateParameters& state, const field::SpectralParameters& spectrum)
	{
		std::string out("");
        out += "\n";
		out += "Isobaric Specific Heat Capacity      " + relation::to_string(gas.isobaric_specific_heat_capacity, state)    + "\n";
		out += "Thermal Conductivity                 " + relation::to_string(gas.thermal_conductivity,            state)    + "\n";
		out += "Dynamic Viscosity                    " + relation::to_string(gas.dynamic_viscosity,               state)    + "\n";
		out += "Density                              " + field::to_string(gas.density,                            state)    + "\n";
		out += "Refractive Index                     " + relation::to_string(gas.refractive_index, (spectrum.nlo+spectrum.nhi)/2.0f) + "\n";
        out += "\n";

		return out;
	}
	std::string to_string(const CompletedGas& gas)
	{
		return to_string(gas, field::StateParameters(), field::SpectralParameters());
	}

}} //namespace 



