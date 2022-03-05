#pragma once

// 3rd party libraries
#include <string>

// in house libraries
#include <models/compound/field/state/StateParameters.hpp>

#include "OptionalStateRelation.hpp"

namespace compound {
namespace relation {


	template <typename T1, typename T2>
	std::string to_string(const OptionalStateRelation<T1>& known, const T2 parameters)
	{
		return known.has_value()? to_string(known.value(), parameters) : "[missing]";
	}

	template <typename T>
	std::string to_string(const OptionalStateRelation<T>& known)
	{
		return to_string(known, field::StateParameters());
	}

} //namespace field
} //namespace compound

