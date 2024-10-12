#pragma once

// C libraries

// std libraries

namespace procedural
{

	/*
	`Get` is a map: (𝕋₁→𝕋₂)×(ℕ→𝕋₁)⟶(ℕ→𝕋₁→𝕋₂) for arbitrary types 𝕋₁ and 𝕋₂
	It represents an indexible object that can participate in functions under `known::` and `procedural::`.
	The value at each index is the result of a composition between an indexible object and a callable object.
	*/
	template<typename F, typename G>
	struct Get
	{
		F f;
		G g;
		constexpr explicit Get(const F& f, const G& g):
			f(f),
			g(g)
		{}
	    using size_type = typename G::size_type;
		using value_type = typename F::value_type;
		constexpr inline size_type size() const { return g.size(); }
		constexpr inline auto operator()(const size_type i ) const
		{
			return f[g[i]];
		}
		constexpr inline auto operator[](const size_type i ) const
		{
			return f[g[i]];
		}
	};

	/*
	NOTE: constructing `procedural::` objects can be annoying due to the number of template parameters involved, 
	so we use convenience methods for generating rasters that are compatible for a given grid.
	Typical C++ conventions might append these with `make_*`, but we forego this convention for brevity.
	For consistency, we create one such function for `Get` here.
	*/
	template<typename F, typename G>
	constexpr inline Get<F,G> get(const F& f, const G& g)
	{
		return Get<F,G>(f,g);
	}

}

