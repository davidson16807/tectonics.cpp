#pragma once

#include <units/si.hpp>

namespace compound {
namespace correlation {

	// derived from Von Misces Theorem
	constexpr si::pressure<double> get_shear_yield_strength_from_tensile_yield_strength ( const si::pressure<double> tensile) { 
		return tensile / sqrt(3.0);
	}
	// derived from Von Misces Theorem
	constexpr si::pressure<double> get_tensile_yield_strength_from_shear_yield_strength ( const si::pressure<double> shear) { 
		return shear * sqrt(3.0);
	}
}}