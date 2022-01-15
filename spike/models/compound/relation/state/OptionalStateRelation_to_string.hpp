#pragma once

// 3rd party libraries
#include <string>

// in house libraries
#include <models/compound/field/state/StateParameters.hpp>

#include "OptionalStateRelation.hpp"

namespace compound {
namespace relation {

	template <typename T>
	std::string to_string(const OptionalStateRelation<T>& known, field::StateParameters parameters)
	{
		auto value = known(parameters);
		return value.has_value()? si::to_string(value.value()) : "[missing]";
	}

	template <typename T>
	std::string to_string(const OptionalStateRelation<T>& known)
	{
		return to_string(known, field::StateParameters());
	}

} //namespace field
} //namespace compound

