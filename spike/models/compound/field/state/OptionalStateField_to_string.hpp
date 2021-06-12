#pragma once

// 3rd party libraries
#include <string>

// in house libraries
#include "StateParameters.hpp"
#include "OptionalStateField.hpp"

namespace compound {
namespace field {
	template <typename T>
	std::string to_string(const OptionalStateField<T>& known, StateParameters parameters)
	{
		auto value = known(parameters);
		return value.has_value()? std::to_string(value.value()) : "[missing]";
	}
	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
	std::string to_string(const OptionalStateField<si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>>& known, StateParameters parameters)
	{
		auto value = known(parameters);
		return value.has_value()? si::to_string(value.value()) : "[missing]";
	}
	template <typename T>
	std::string to_string(const OptionalStateField<T>& known)
	{
		return to_string(known, StateParameters());
	}
} //namespace field
} //namespace compound

