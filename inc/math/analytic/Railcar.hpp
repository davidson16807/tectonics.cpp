#pragma once

#include "Boxcar.hpp"

namespace analytic {

    /* 
    `Railcar<T,F>` is a trivial class that represents a function f(x) of type F multiplied 
    by what is commonly called the "boxcar" function, 
    which we define here as boxcar(x)=1 for lo<x≤hi and boxcar(x)=0 otherwise.*
    The discontinuities of a railcar resemble the front and back of a boxcar, 
    but the plotted shape of the railcar does not strictly resemble a box (hence "railcar").

    An equivalent definition is to say railcar(x)=f(x) if lo<x≤hi and railcar(x)=0 otherwise, 
    for some function f(x) of type F. 
    The railcar here is said to be a "railcar of F", or an "F railcar", that "contains" f(x), 
    such that f(x) is "inside" the railcar, or f(x) is its "contents", and so forth.
    By this definition, a boxcar is a railcar of unity.

    The intent in introducing "railcar" is to represent piecewise functions as sums of railcars,
    thereby providing an easy way to close sums under common operations for some kinds of underlying functions.
    The name "railcar" is chosen to provide an extended metaphor to resolve ambiguities during usage, see below.

    A sum of railcars is either a "train" (if no overlaps or gaps exist between railcars)
    or a "railyard" (if overlaps and gaps are permitted).
    These are each distinct from a "spline", whose common definition can be interpreted as a train of polynomials 
    for which continuity restrictions are imposed on adjacent railcars and their derivatives.
    Railyards can be useful since they are easily closed under arithmetic operations, derivatives, and integrals,
    just as long as certain closure properties also apply to their function types.
    Trains can be useful since they minimize the number of terms that need to calculated.
    This potentially improves performance, but only under certain circumstances.
    As an example, rationals are closed under addition, but rational addition requires cross multiplication,
    which increases the degree of the numerator and its number of terms.
    Increasing the degree of the numerator is especially troublesome, since it increases the likelihood 
    of floating point precision errors such as overflow and destructive cancellation.
    Therefore, casting a railyard of rationals as a train of rationals may reduce the number of railcars
    but increase the number of terms in the function contained by railcar, and may introduce precision errors.

    * The choice of range inclusivity ("lo<x≤hi") is mostly arbitrary and exists 
    only because a choice was needed that would allow the representation of continuous piecewise functions.
    */

    template<typename T, typename F>
    struct Railcar {

        F content;
        T lo;
        T hi;

        // the zero railcar
        constexpr Railcar<T,F>():
            content(),
            lo(-std::numeric_limits<T>::max()),
            hi(std::numeric_limits<T>::max())
        {
            assert(lo <= hi);
        }

        // the constant railcar
        constexpr Railcar<T,F>(const T k):
            content(k),
            lo(-std::numeric_limits<T>::max()),
            hi(std::numeric_limits<T>::max())
        {
            assert(lo <= hi);
        }

        // copy constructor
        constexpr Railcar<T,F>(const Railcar<T,F>& car):
            content(car.content),
            lo(car.lo),
            hi(car.hi)
        {
            assert(lo <= hi);
        }

        // cast constructor
        template<typename T2, typename F2>
        constexpr explicit Railcar<T,F>(const Railcar<T2,F2>& car):
            content(car.content),
            lo(car.lo),
            hi(car.hi)
        {
            assert(lo <= hi);
        }

        constexpr explicit Railcar<T,F>(const T lo, const T hi, const F& content):
            content(content),
            lo(lo),
            hi(hi)
        {
            assert(lo <= hi);
        }

        constexpr explicit Railcar<T,F>(const F& content) : 
            content(content),
            lo(-std::numeric_limits<T>::max()),
            hi(std::numeric_limits<T>::max())
        {
            assert(lo <= hi);
        }

        constexpr T operator()(const T x) const
        {
            return T(lo<x&&x<=hi? content(x) : 0.0);
        }

        Railcar<T,F>& operator=(const T k)
        {
            lo = -std::numeric_limits<T>::max();
            hi = std::numeric_limits<T>::max();
            content = k;
            return *this;
        }

        Railcar<T,F>& operator*=(const T k)
        {
            content *= k;
            return *this;
        }

        Railcar<T,F>& operator/=(const T k)
        {
            content /= k;
            return *this;
        }

        template<typename G>
        Railcar<T,F>& operator=(const Railcar<T,G>& car)
        {
            lo = car.lo;
            hi = car.hi;
            content = car.content;
            return *this;
        }

    };


    // operations of other types that produce railcars
    template<typename T, typename F>
    constexpr auto operator*(const F& content, const Boxcar<T> b)
    {
        return Railcar<T,F>(b.lo, b.hi, content);
    }

    template<typename T, typename F>
    constexpr auto operator*(const Boxcar<T> b, const F& content)
    {
        return Railcar<T,F>(b.lo, b.hi, content);
    }

    /*
    NOTE: we cannot support addition between railcars and other types.
    This is because the railcar cannot represent the entire codomain of resulting function objects in the general case.
    */ 

    // operations that are closed under railcars
    template<typename T, typename F>
    constexpr auto operator-(const Railcar<T,F>& car)
    {
        return Railcar<T,F>(car.lo, car.hi, -car.content);
    }





    template<typename T, typename F>
    constexpr auto operator*(const Railcar<T,F>& car, const T k)
    {
        return Railcar<T,F>(car.lo, car.hi, car.content*k);
    }

    template<typename T, typename F>
    constexpr auto operator*(const T k, const Railcar<T,F>& car)
    {
        return Railcar<T,F>(car.lo, car.hi, k*car.content);
    }

    template<typename T, typename F>
    constexpr auto operator/(const Railcar<T,F>& car, const T k)
    {
        return Railcar<T,F>(car.lo, car.hi, car.content/k);
    }

    template<typename T, typename F>
    constexpr auto operator/(const T k, const Railcar<T,F>& car)
    {
        return Railcar(car.lo, car.hi, k/car.content);
    }




    template<typename T, typename F>
    constexpr auto operator*(const Railcar<T,F>& car, const F& k)
    {
        return Railcar(car.lo, car.hi, car.content*k);
    }

    template<typename T, typename F>
    constexpr auto operator*(const F& k, const Railcar<T,F>& car)
    {
        return Railcar(car.lo, car.hi, k*car.content);
    }

    template<typename T, typename F>
    constexpr auto operator/(const Railcar<T,F>& car, const F& k)
    {
        return Railcar(car.lo, car.hi, car.content/k);
    }

    template<typename T, typename F>
    constexpr auto operator/(const F& k, const Railcar<T,F>& car)
    {
        return Railcar(car.lo, car.hi, k/car.content);
    }





    template<typename T, typename F>
    constexpr Railcar<T,F> compose(const Railcar<T,F>& f, const Identity<T> e)
    {
        return f;
    }


    template<typename T, typename F, typename G>
    constexpr Railcar<T,F> compose(const Railcar<T,F>& f, const G& g)
    {
        return Railcar<T,F>(inverse(g)(f.lo), inverse(g)(f.lo), compose(f.content, g) );
    }





    /*
    `derivative` returns a function object for the derivative of a polynomial.
    */
    template<typename T, typename F>
    auto derivative(const Railcar<T,F>& car)
    {
        return Railcar(car.lo, car.hi, derivative(car.content));
    }

    /*
    `derivative` returns the derivative of a polynomial at a point.
    It is meant to compliment the method signature for integral(p, lo, hi)
    */
    template<typename T, typename F>
    T derivative(const Railcar<T,F>& car, const T x)
    {
        return (car.lo < x && x < car.hi)?
            derivative(car.content, x) : T(0);
    }

    /*
    `integral` returns the definite integral of a polynomial 
    without representing the integral as its own function object.
    This is meant to be used as a fallback in the event the function is a 
    Laurent polynomial with a coefficient of degree -1, 
    since the integral for such a function includes a logarithmic term.
    */
    template<typename T, typename F>
    T integral(const Railcar<T,F>& car, const T x)
    {
        return (car.lo < x)?
            integral(car.content, std::min(x, car.hi)) 
          - integral(car.content, car.lo) : T(0);
    }

    template<typename T, typename F>
    T integral(const Railcar<T,F>& car, const T lo, const T hi)
    {
        /*
        Q: Why do we check for lo < car.hi?
        A: If car.hi < lo, then car.hi < hi as well, 
           so we know the difference in the integral between lo and hi is 0.
           However the result of the integral may still be big,
           so we could be trying to find a nonexistant difference between two big numbers.
           To avoid destructive cancellation we do not calculate.
           Similar statements could be made for car.lo < hi.
        */
        return (lo < car.hi && car.lo < hi)?
            integral(car.content, std::min(hi, car.hi)) 
          - integral(car.content, std::max(lo, car.lo)) : T(0);
    }

}


