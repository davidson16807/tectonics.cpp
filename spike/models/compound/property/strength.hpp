#pragma once

#include <units/si.hpp>

namespace compound {
namespace property {

	// derived from Von Misces Theorem
	constexpr si::pressure get_shear_yield_strength_from_tensile_yield_strength ( const si::pressure tensile) { 
		return tensile / sqrt(3.0);
	}
	// derived from Von Misces Theorem
	constexpr si::pressure get_tensile_yield_strength_from_shear_yield_strength ( const si::pressure shear) { 
		return shear * sqrt(3.0);
	}
}}