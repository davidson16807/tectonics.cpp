#pragma once

#include <models/rock/StrataValues.hpp> // Strata

namespace crust
{
	template<typename T>
    typedef many::tmany<StrataValues<T>> CrustValues;
}

