#pragma once

// 3rd party libraries
#include <glm/mat3x3.hpp>

// in-house libraries
#include <models/strata/StrataStore.hpp> // Strata

namespace crust
{
	class Plate
	{
		series::Series<StrataStore> crust;
		glm::mat3 orientation;

	public:
		Plate();
		~Plate();
	};
}

