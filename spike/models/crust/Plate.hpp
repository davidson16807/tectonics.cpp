#pragma once

#include <models/strata/StrataStore.hpp> // Strata

namespace crust
{
	class Plate
	{
		many::tmany<StrataStore> crust;
		many::tmany<bool> mask;

	public:
		Plate();
		~Plate();
	};
}

