#pragma once

// 3rd party libraries
#include <string>
#include <optional>

// in house libraries
#include <models/compound/field/state/StateParameters.hpp>

#include "GasPropertyStateRelation.hpp"

namespace compound {
namespace relation {

	template <typename T>
	std::string to_string(const GasPropertyStateRelation<T>& known, field::StateParameters parameters)
	{
		return std::to_string(known(parameters));
	}

	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
	std::string to_string(const GasPropertyStateRelation<si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>>& known, field::StateParameters parameters)
	{
		return si::to_string(known(parameters));
	}

	template <typename T>
	std::string to_string(const GasPropertyStateRelation<T>& known)
	{
		return to_string(known, field::StateParameters());
	}

	template <typename T>
	std::string to_string(const std::optional<GasPropertyStateRelation<T>>& value, field::StateParameters parameters)
	{
		return value.has_value()? si::to_string(value.value()(parameters)) : "[missing]";
	}

	template <typename T>
	std::string to_string(const std::optional<GasPropertyStateRelation<T>>& value)
	{
		return to_string(value, field::StateParameters());
	}

} //namespace field
} //namespace compound

