#pragma once

#include <string>

#include "CompletedCompound.hpp"
#include "StateParameterSamples.hpp"

#include <models/compound/field/constant/OptionalConstantField_to_string.hpp>
#include <models/compound/field/state/OptionalStateField_to_string.hpp>
#include <models/compound/field/spectral/OptionalSpectralField_to_string.hpp>

#include <models/compound/phase/solid/CompletedSolid_to_string.hpp>
#include <models/compound/phase/liquid/CompletedLiquid_to_string.hpp>
#include <models/compound/phase/gas/CompletedGas_to_string.hpp>

namespace compound
{
	std::string to_string(const CompletedCompound& compound, const si::wavenumber<double> nlo, const si::wavenumber<double> nhi)
	{
		std::string out("");
		field::StateParameters gas_sample(compound.boiling_point_sample_pressure, compound.boiling_point_sample_temperature);
		field::StateParameters liquid_sample(compound.freezing_point_sample_pressure, compound.freezing_point_sample_temperature);
		field::StateParameters solid_sample(compound.freezing_point_sample_pressure, compound.freezing_point_sample_temperature);
		field::StateParameters stp;
		field::SpectralParameters gas_spectrum   (nlo, nhi, gas_sample.pressure,    gas_sample.temperature   );
		field::SpectralParameters liquid_spectrum(nlo, nhi, liquid_sample.pressure, liquid_sample.temperature);
		field::SpectralParameters solid_spectrum (nlo, nhi, solid_sample.pressure,  solid_sample.temperature );
        out += "\n";
		out += "Molar Mass:                          " + si   ::to_string(compound.molar_mass)                        + "\n";
		out += "Atoms Per Molecule:                  " + std  ::to_string(compound.atoms_per_molecule)                + "\n";
		out += "Molecular Diameter:                  " + si   ::to_string(compound.molecular_diameter)                + "\n";
		out += "Molecular Degrees Of Freedom:        " + std  ::to_string(compound.molecular_degrees_of_freedom)      + "\n";
		out += "Acentric Factor:                     " + std  ::to_string(compound.acentric_factor)                   + "\n";
		out += "Critical Point Pressure:             " + si   ::to_string(compound.critical_point_pressure)           + "\n";
		out += "Critical Point Volume:               " + si   ::to_string(compound.critical_point_volume)             + "\n";
		out += "Critical Point Temperature:          " + si   ::to_string(compound.critical_point_temperature)        + "\n";
		out += "Critical Point Compressibility:      " + std  ::to_string(compound.critical_point_compressibility)    + "\n";
		out += "Latent Heat Of Vaporization:         " + si   ::to_string(compound.latent_heat_of_vaporization)       + "\n";
		out += "Latent Heat Of Fusion:               " + si   ::to_string(compound.latent_heat_of_fusion)             + "\n";
		out += "Triple Point Pressure:               " + si   ::to_string(compound.triple_point_pressure)             + "\n";
		out += "Triple Point Temperature:            " + si   ::to_string(compound.triple_point_temperature)          + "\n";
		out += "Freezing Point Sample Pressure:      " + si   ::to_string(compound.freezing_point_sample_pressure)    + "\n";
		out += "Freezing Point Sample Temperature:   " + si   ::to_string(compound.freezing_point_sample_temperature) + "\n";
		out += "Boiling Point Sample Pressure:       " + si   ::to_string(compound.boiling_point_sample_pressure)     + "\n";
		out += "Boiling Point Sample Temperature:    " + si   ::to_string(compound.boiling_point_sample_temperature)  + "\n";
		out += "Phase:                               " + field::to_string(compound.phase, field::StateParameters(stp.pressure, stp.temperature)) + "\n";
		out += "Molecular Absorption Cross Section:  " + relation::to_string(compound.molecular_absorption_cross_section, (nlo+nhi)/2.0) + "\n";
		out += "Gas:"    + phase::to_string(compound.gas,    gas_sample,    gas_spectrum   );
		out += "Liquid:" + phase::to_string(compound.liquid, liquid_sample, liquid_spectrum);
        for (std::size_t i = 0; i < compound.solids.size(); i++)
        {
			out += "Solid " + std::to_string(i) + ":" + phase::to_string(compound.solids[i], solid_sample, solid_spectrum);
        }
        out += "\n";
		return out;
	}
	std::string to_string(const CompletedCompound& compound)
	{
		field::SpectralParameters spectrum;
		return to_string(compound, spectrum.nlo, spectrum.nhi);
	}

} //namespace compound


