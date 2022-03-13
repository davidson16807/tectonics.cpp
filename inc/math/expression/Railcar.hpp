#pragma once

namespace math {

    /* 
    `Railcar<T,F>` is a trivial class that represents a function f(x) of type F multiplied 
    by the well-known "boxcar" function, which we define here as boxcar(x)=1 for lo<x≤hi and boxcar(x)=0 otherwise.*
    This gives a function whose discontinuities resemble the front and back of a boxcar, 
    but whose plotted shape does not strictly resemble a box (hence "railcar").

    Therefore, railcar(x)=f(x) if lo<x≤hi and railcar(x)=0 otherwise, given a function f(x) of type F. 
    In this case, the railcar is said to be a "railcar of F", or an "F railcar", that "contains" f(x).
    By this definition, a boxcar is a railcar of unity.

    The intent in introducing "railcar" is to represent piecewise functions as sums of railcars,
    its name providing an extended metaphor to resolve ambiguities in such cases.
    A sum of railcars is either a "train" (if no overlaps or gaps exist between railcars)
    or a "railyard" (if overlaps or gaps are permitted).
    These are each distinct from a "spline", which is commonly understood as a train of polynomials 
    that imposes continuity restrictions on adjacent railcars and their derivatives.
    Railyards can be useful since they are easily closed under arithmetic operations, derivatives, and integrals,
    presuming certain closure properties also apply to their function types.
    Mapping a railyard to a train can be useful if it reduces the number of terms that need to calculated.
    This could potentially improve performance, but may come with added risk for some underlying function types. 
    As an example, rationals are closed under addition, but rational addition requires cross multiplication,
    which increases the degree of the numerator and its number of terms.
    Increasing the degree of the numerator is especially troublesome, since it increases the likelihood 
    of floating point precision errors such as overflow and destructive cancellation.
    Therefore, casting a railyard of rationals as a train of rationals may reduce the number of railcars
    but increase the number of terms in the function contained by railcar, and may introduce precision errors.

    * The choice of range ("lo<x≤hi") is mostly arbitrary and exists 
    only because a choice was needed that would allow the creation of continuous piecewise functions.
    */
    template<typename T, typename F>
    struct Railcar {
        F f;
        T lo;
        T hi;
        constexpr explicit Railcar<T,F>(const T lo, const T hi, const F f):
            f(f),
            lo(lo),
            hi(hi)
        {}
        constexpr Railcar<T,F>(const Railcar<T,F>& piece):
            f(piece.f),
            lo(piece.lo),
            hi(piece.hi)
        {}
        constexpr Railcar<T,F>():
            f(),
            lo(),
            hi()
        {}
        constexpr T operator()(const T x) const
        {
            return lo<x && x<=hi? f(x) : T(0.0);
        }
    };

}
