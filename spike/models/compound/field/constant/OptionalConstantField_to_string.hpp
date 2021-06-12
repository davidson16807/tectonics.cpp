#pragma once

// 3rd party libraries
#include <string>

// in house libraries
#include "EmptyParameters.hpp"
#include "OptionalConstantField.hpp"

namespace compound {
namespace field {
	template <typename T>
	std::string to_string(const OptionalConstantField<T>& known)
	{
		auto value = known();
		return value.has_value()? std::to_string(value.value()) : "[missing]";
	}
	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
	std::string to_string(const OptionalConstantField<si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>>& known)
	{
		auto value = known();
		return value.has_value()? si::to_string(value.value()) : "[missing]";
	}
} //namespace field
} //namespace compound

