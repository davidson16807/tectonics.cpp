#pragma once

#include "Boxcar.hpp"

namespace analytic {

    /* 
    `Railcar<T,F>` is a trivial class that represents a function f(x) of type F multiplied 
    by what is commonly called the "boxcar" function, 
    which we define here as boxcar(x)=1 for lo<x≤hi and boxcar(x)=0 otherwise.†
    Here the variables "lo" and "hi" are known as the "couplers" for a given railcar.
    The discontinuities of a railcar resemble the front and back of a boxcar, 
    but the plotted shape of the railcar does not strictly resemble a box (hence "railcar").

    An equivalent definition is to say railcar(x)=f(x) if lo<x≤hi and railcar(x)=0 otherwise, 
    for some function f∈𝔽. 
    The railcar here is said to be a "railcar of 𝔽", or an "𝔽 railcar", that "contains" f(x), 
    such that f(x) is "inside" the railcar, or f(x) is its "contents", and so forth.
    By this definition, a boxcar is a railcar of the identity function.

    The intent in introducing the concept of a "railcar" is to represent piecewise functions as sums of railcars,
    thereby providing an easy way to close sums under common operations for some kinds of underlying functions.
    The name "railcar" is chosen partly because it leverages the existing concept of a "boxcar" function,
    but also because it provide a rich and intuitive metaphor that resolves ambiguities in conversation, see below.

    Two railcars, A and B, are said to "overlap" if max(A.lo,B.lo) < min(A.hi,B.hi).
    In contrast, A and B are said to have a "gap" if max(A.lo,B.lo) > min(A.hi,B.hi),
    and A and B are said to be "coupled" or "adjacent" if max(A.lo,B.lo) = min(A.hi,B.hi).
    If A and B are coupled, the value x = max(A.lo,B.lo) = min(A.hi,B.hi) is said to be the "coupling" of A and B.
    A sum of railcars is known as a "train" if no overlaps or gaps exist between railcars
    A sum of railcars is known as a "railyard" if overlaps and gaps are permitted to exist.
    Terminology regarding the "contents" of railcars will also be used to describe the trains and railyards that house them,
    so for instance a "polynomial train" is a train whose railcars contain polynomials.
    A "spline" therefore is a polynomial train where adjacent cars share the same first derivative at their coupling.
    A spline of order N additionally satisfies the same condition for derivatives up to order N.

    Railyards can be useful since they easily extend the properties of their contents, for instance:
     * for any class of function 𝔽, the railyard of 𝔽 is closed under addition
     * for any class of function 𝔽, if 𝔽 is closed under subtraction, multiplication, 
       derivation, or integration, so to will the railyard of 𝔽.
    Trains can be useful since they minimize the number of terms that need to be calculated,
    and they allow certain operations to be performed that would not be allowed on railyards.
    As an example, rational trains are closed under division, however rational railyards are not,
    and the degree of a rational train's polynomials cannot be known at compile time 
    if that train has been converted from a railyard.
    Railyards make it easy to performantly implement certain kinds of operations that 
    would not be performant if an implementation of a train were to guarantee that no overlaps or gaps exist. 
    As an example, adding two railyards simply requires contatenating two lists of cars together,
    whereas adding two trains together in such a scenario requires constructing a totally new list of cars.
    However, it is almost certainly more performant to call the operator() method on a train as opposed to a railyard,
    which is typically the only performance that matters.

    † The choice of range inclusivity ("lo<x≤hi") is mostly arbitrary and exists 
    only because a choice was needed that would allow the representation of continuous piecewise functions.
    Were this a mathematical exercise, we could define a railyard as following a certain "coupling standard", 
    but in a software framework this only introduces added work without any real benefit,
    and it would in fact pose problems since it introduces potential intercompatiblity issues between railyards.
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

    template<typename T, typename F>
    constexpr Railcar<T,F> compose(const Railcar<T,F>& f, const Shifting<T>& g)
    {
        return Railcar<T,F>(inverse(g)(f.lo), inverse(g)(f.lo), compose(f.content, g) );
    }

    template<typename T, typename F>
    constexpr Railcar<T,F> compose(const Railcar<T,F>& f, const Scaling<T>& g)
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


