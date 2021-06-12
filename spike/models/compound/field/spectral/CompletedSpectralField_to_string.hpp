#pragma once

// 3rd party libraries
#include <string>

// in house libraries
#include "SpectralParameters.hpp"
#include "CompletedSpectralField.hpp"

namespace compound {
namespace field {
	template <typename T>
	std::string to_string(const CompletedSpectralField<T>& known, SpectralParameters parameters)
	{
		auto value = known(parameters);
		return std::to_string(value);
	}
	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
	std::string to_string(const CompletedSpectralField<si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>>& known, SpectralParameters parameters)
	{
		auto value = known(parameters);
		return si::to_string(value);
	}
	template <typename T>
	std::string to_string(const CompletedSpectralField<T>& known)
	{
		return to_string(known, SpectralParameters());
	}
} //namespace field
} //namespace compound

