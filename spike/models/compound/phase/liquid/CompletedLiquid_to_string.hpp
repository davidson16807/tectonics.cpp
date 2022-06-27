#pragma once

#include <string>

#include <models/compound/field/state/CompletedStateField_to_string.hpp>
#include <models/compound/field/spectral/CompletedSpectralField_to_string.hpp>

#include <models/compound/relation/PolynomialRailyardRelation_to_string.hpp>

#include <models/compound/phase/liquid/CompletedLiquid.hpp>

namespace compound {
namespace phase {
	std::string to_string(const CompletedLiquid& liquid, const field::StateParameters& state, const field::SpectralParameters& spectrum)
	{
		std::string out("");
        out += "\n";
		out += "Isobaric Specific Heat Capacity      " + field::to_string(liquid.isobaric_specific_heat_capacity, state)    + "\n";
		out += "Thermal Conductivity                 " + field::to_string(liquid.thermal_conductivity,            state)    + "\n";
		out += "Dynamic Viscosity                    " + field::to_string(liquid.dynamic_viscosity,               state)    + "\n";
		out += "Density                              " + field::to_string(liquid.density,                         state)    + "\n";
		out += "Vapor Pressure                       " + field::to_string(liquid.vapor_pressure,                  state)    + "\n";
		out += "Surface Tension                      " + field::to_string(liquid.surface_tension,                 state)    + "\n";
		out += "Refractive Index                     " + relation::to_string(liquid.refractive_index,       (spectrum.nlo+spectrum.nhi)/2.0f) + "\n";
		out += "Extinction Coefficient               " + relation::to_string(liquid.extinction_coefficient, (spectrum.nlo+spectrum.nhi)/2.0f) + "\n";
        out += "\n";
		return out;
	}
	std::string to_string(const CompletedLiquid& liquid)
	{
		return to_string(liquid, field::StateParameters(), field::SpectralParameters());
	}

}} //namespace 


