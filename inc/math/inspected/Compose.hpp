#pragma once

// C libraries

// std libraries

namespace inspected
{

    /*
    `Compose` is a map: (𝕋₁→𝕋₂)×(ℝ→𝕋₁)⟶(ℝ→𝕋₁→𝕋₂) for arbitrary types 𝕋₁ and 𝕋₂
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
        template<typename T>
        constexpr inline auto operator()(const T x) const
        {
            return f(g(x));
        }
    };

    /*
    */
    template<typename F, typename G>
    constexpr inline Compose<F,G> compose(const F& f, const G& g)
    {
        return Compose<F,G>(f,g);
    }

}

