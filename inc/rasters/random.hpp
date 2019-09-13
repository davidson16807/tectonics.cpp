#pragma once

#include <random>

#include "types.hpp"

namespace rasters
{
	namespace random {
		template <class T, class Tgenerator>
		void elias(traster<T>& a, Tgenerator generator)
		{
			std::generate_canonical<T>(generator);
		}
		template <class T, class Tgenerator>
		traster<T> elias(Tgenerator generator)
		{
			traster<T> out(ids.size());
			elias(a, ids, out);
			return out;
		}
	}
}