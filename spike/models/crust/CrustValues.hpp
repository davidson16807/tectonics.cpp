#pragma once

// std libraries
#include <vector>

// in-house libraries
#include <models/strata/StrataValues.hpp> // Strata

namespace crust
{
	template <typename T, int L>
    using CrustValues = std::vector<strata::StrataValues<T, L>>;
}

