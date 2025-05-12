#pragma once

// C libraries

// std libraries

// 3rd party libraries

namespace field
{

	/*
	`Uniform` is a map: (ℝ³→𝕋) for an arbitrary type 𝕋
	The value at each index is the result of a composition between an indexible object and a callable object.
	*/
	template<typename K>
	struct Uniform
	{
		K k;
		constexpr explicit Uniform(const K& k):
			k(k)
		{}
		using value_type = K;
		template<int L, typename T, glm::qualifier Q>
		constexpr inline auto operator()(const glm::vec<L,T,Q> V) const
		{
			return k(V);
		}
	};

	/*
	NOTE: constructing `field::` objects can be annoying due to the number of template parameters involved, 
	so we use convenience methods for generating rasters that are compatible for a given grid.
	Typical C++ conventions might append these with `make_*`, but we forego this convention for brevity.
	For consistency, we create one such function for `Uniform` here.
	*/
	template<typename K>
	constexpr inline Uniform<K> uniform(const K& k)
	{
		return Uniform<K>(k);
	}

}

