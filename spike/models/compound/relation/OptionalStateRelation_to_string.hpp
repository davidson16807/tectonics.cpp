#pragma once

// 3rd party libraries
#include <string>

// in house libraries
#include <models/compound/field/state/StateParameters.hpp>

#include "OptionalStateRelation.hpp"

namespace compound {
namespace relation {


	template <typename T1>
	std::string to_string(const OptionalStateRelation<T1>& known)
	{
		return known.has_value()? to_string(known.value()) : "[missing]";
	}

} //namespace field
} //namespace compound

