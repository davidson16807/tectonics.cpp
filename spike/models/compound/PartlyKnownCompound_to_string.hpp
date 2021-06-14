#pragma once

#include <string>

#include "PartlyKnownCompound.hpp"
#include "StateParameterSamples.hpp"

#include <models/compound/field/constant/OptionalConstantField_to_string.hpp>
#include <models/compound/field/state/OptionalStateField_to_string.hpp>
#include <models/compound/field/spectral/OptionalSpectralField_to_string.hpp>

#include <models/compound/phase/solid/PartlyKnownSolid_to_string.hpp>
#include <models/compound/phase/liquid/PartlyKnownLiquid_to_string.hpp>
#include <models/compound/phase/gas/PartlyKnownGas_to_string.hpp>

namespace compound
{
	std::string to_string(const PartlyKnownCompound& compound, const si::wavenumber nlo, const si::wavenumber nhi)
	{
		std::string out("");
		StateParameterSamples samples(compound);
		field::StateParameters stp;
		field::SpectralParameters gas_spectrum   (nlo, nhi, samples.gas.pressure,    samples.gas.temperature   );
		field::SpectralParameters liquid_spectrum(nlo, nhi, samples.liquid.pressure, samples.liquid.temperature);
		field::SpectralParameters solid_spectrum (nlo, nhi, samples.solid.pressure,  samples.solid.temperature );
        out += "\n";
		out += "Molar Mass:                          " + si   ::to_string(compound.molar_mass)                        + "\n";
		out += "Atoms Per Molecule:                  " + std  ::to_string(compound.atoms_per_molecule)                + "\n";
		out += "Molecular Diameter:                  " + si   ::to_string(compound.molecular_diameter)                + "\n";
		out += "Molecular Degrees Of Freedom:        " + field::to_string(compound.molecular_degrees_of_freedom)      + "\n";
		out += "Acentric Factor:                     " + field::to_string(compound.acentric_factor)                   + "\n";
		out += "Critical Point Pressure:             " + si   ::to_string(compound.critical_point_pressure)           + "\n";
		out += "Critical Point Volume:               " + si   ::to_string(compound.critical_point_volume)             + "\n";
		out += "Critical Point Temperature:          " + si   ::to_string(compound.critical_point_temperature)        + "\n";
		out += "Critical Point Compressibility:      " + std  ::to_string(compound.critical_point_compressibility)    + "\n";
		out += "Latent Heat Of Vaporization:         " + field::to_string(compound.latent_heat_of_vaporization)       + "\n";
		out += "Latent Heat Of Fusion:               " + field::to_string(compound.latent_heat_of_fusion)             + "\n";
		out += "Triple Point Pressure:               " + field::to_string(compound.triple_point_pressure)             + "\n";
		out += "Triple Point Temperature:            " + field::to_string(compound.triple_point_temperature)          + "\n";
		out += "Freezing Point Sample Pressure:      " + field::to_string(compound.freezing_point_sample_pressure)    + "\n";
		out += "Freezing Point Sample Temperature:   " + field::to_string(compound.freezing_point_sample_temperature) + "\n";
		out += "Boiling Point Sample Pressure:       " + field::to_string(compound.boiling_point_sample_pressure)     + "\n";
		out += "Boiling Point Sample Temperature:    " + field::to_string(compound.boiling_point_sample_temperature)  + "\n";
		out += "Simon Glatzel Slope:                 " + field::to_string(compound.simon_glatzel_slope)               + "\n";
		out += "Simon Glatzel Exponent:              " + field::to_string(compound.simon_glatzel_exponent)            + "\n";
        out += "\n";
		out += "Molecular Absorption Cross Section:  " + field::to_string(compound.molecular_absorption_cross_section, field::SpectralParameters(nlo, nhi, stp.pressure, stp.temperature)) + "\n";
		out += "Gas:"    + phase::to_string(compound.gas,    samples.gas,    gas_spectrum   );
		out += "Liquid:" + phase::to_string(compound.liquid, samples.liquid, liquid_spectrum);
        for (std::size_t i = 0; i < compound.solids.size(); i++)
        {
			out += "Solid " + std::to_string(i) + ":" + phase::to_string(compound.solids[i], samples.solid, solid_spectrum);
        }
        out += "\n";
		return out;
	}
	std::string to_string(const PartlyKnownCompound& compound)
	{
		field::SpectralParameters spectrum;
		return to_string(compound, spectrum.nlo, spectrum.nhi);
	}

} //namespace compound



