#pragma once

// std libraries
#include <vector>

// in-house libraries
#include <models/stratum/Stratum.hpp> // Stratum

namespace crust
{
	/*
	NOTE: 
	Sediment is a degenerate case of Crust where L == 1, i.e., there is only a single stratigraphic layer.
	We declare it as a std::vector<strata::Strata<1,M>> rather than std::vector<stratum::Stratum<M>>,
	to avoid changes within Stratum from propagating up to the crust category,
	and to avoid defining a custom algebra just for Sediment (along with accompanying unit tests).
	In principle, we would prefer that objects within a category be defined 
	using only the objects in the category directly beneath it.
	This should be possible without incurring significant overhead, 
	since Strata only introduces a single 4 byte integer to track the number of layers.
	*/
	template <int M>
    using Sediment = std::vector<stratum::Strata<1,M>>;
}

