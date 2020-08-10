#pragma once

// std libraries
#include <vector>

// in-house libraries
#include <models/strata/StrataStore.hpp> // Strata

namespace crust
{
	template <int L, int M>
    using Crust = std::vector<strata::StrataStore<L,M>>;
}

