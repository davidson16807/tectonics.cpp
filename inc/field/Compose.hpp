#pragma once

// C libraries

// std libraries

namespace field
{

	/*
	`Compose` is a map: (𝕋₁→𝕋₂)×(ℕ→𝕋₁)⟶(ℕ→𝕋₁→𝕋₂) for arbitrary types 𝕋₁ and 𝕋₂
	It is an indexible object that can participate in functions under `each::` and `store::`.
	The value at each index is the result of a composition between an indexible object and a callable object.
	*/
	template<typename F, typename G>
	struct Compose
	{
		F f;
		G g;
		constexpr explicit Compose(const F& f, const G& g):
			f(f),
			g(g)
		{}
		using value_type = typename F::value_type;
		template<int L, typename T, glm::qualifier Q>
		constexpr inline auto operator()(const glm::vec<L,T,Q> V) const
		{
			return f(g(V));
		}
	};

	/*
	NOTE: constructing `field::` objects can be annoying due to the number of template parameters involved, 
	so we use convenience methods for generating rasters that are compatible for a given grid.
	Typical C++ conventions might append these with `make_*`, but we forego this convention for brevity.
	For consistency, we create one such function for `Compose` here.
	*/
	template<typename F, typename G>
	constexpr inline Compose<F,G> compose(const F& f, const G& g)
	{
		return Compose<F,G>(f,g);
	}

}

