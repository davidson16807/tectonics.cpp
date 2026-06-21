#pragma once

// std libraries
#include <string>  // std::string
#include <limits>  // std::numeric_limits
#include <ostream> // std::ostream

// in-house libraries
#include "Boxcar.hpp"

namespace analytic {

    /* 
    `Railcar<T,F>` is a trivial class that represents a function f(x) of type F multiplied 
    by what is commonly called the "boxcar" function, 
    which we define here as boxcar(x)=1 for lo<x≤hi and boxcar(x)=0 otherwise.
    See README.txt for motivation and extended description.
    */

    template<typename T, typename F>
    struct Railcar {
        using value_type = T;

        F content;
        T lo;
        T hi;

        // the zero railcar
        constexpr Railcar<T,F>():
            content(),
            lo(-std::numeric_limits<T>::max()),
            hi(std::numeric_limits<T>::max())
        {
            // assert(lo <= hi);
        }

        // the constant railcar
        constexpr Railcar<T,F>(const T k):
            content(k),
            lo(-std::numeric_limits<T>::max()),
            hi(std::numeric_limits<T>::max())
        {
            // assert(lo <= hi);
        }

        // cast constructor
        template<typename T2, typename F2>
        constexpr explicit Railcar<T,F>(const Railcar<T2,F2>& car):
            content(car.content),
            lo(car.lo),
            hi(car.hi)
        {
            // assert(lo <= hi);
        }

        constexpr explicit Railcar<T,F>(const T lo, const T hi, const F& content):
            content(content),
            lo(lo),
            hi(hi)
        {
            // assert(lo <= hi);
        }

        constexpr explicit Railcar<T,F>(const F& content) : 
            content(content),
            lo(-std::numeric_limits<T>::max()),
            hi(std::numeric_limits<T>::max())
        {
            // assert(lo <= hi);
        }

        constexpr T operator()(const T x) const
        {
            return T(lo<x&&x<=hi? content(x) : T(0));
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


    template<typename T, typename F>
    std::string to_string(const Railcar<T,F>& p)
    {
        std::string output;
        const T oo (std::numeric_limits<T>::max());
        output += p.lo == -oo? "-∞" : std::to_string(p.lo);
        output += " < x ≤ ";
        output += p.hi == oo? "∞" : std::to_string(p.hi);
        output += ": ";
        output += to_string(p.content);
        return output;
    }

    template<typename T, typename F>
    std::ostream& operator<<(std::ostream& os, const Railcar<T,F>& p) { 
        os << to_string(p);
        return os;
    }

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
        return Railcar<T,F>(car.lo, car.hi, k/car.content);
    }




    template<typename T, typename F>
    constexpr auto operator*(const Railcar<T,F>& car, const F& k)
    {
        return Railcar<T,F>(car.lo, car.hi, car.content*k);
    }

    template<typename T, typename F>
    constexpr auto operator*(const F& k, const Railcar<T,F>& car)
    {
        return Railcar<T,F>(car.lo, car.hi, k*car.content);
    }

    template<typename T, typename F>
    constexpr auto operator/(const Railcar<T,F>& car, const F& k)
    {
        return Railcar<T,F>(car.lo, car.hi, car.content/k);
    }

    template<typename T, typename F>
    constexpr auto operator/(const F& k, const Railcar<T,F>& car)
    {
        return Railcar<T,F>(car.lo, car.hi, k/car.content);
    }





    template<typename T, typename F>
    constexpr Railcar<T,F> compose(const Railcar<T,F>& f, const Identity<T> e)
    {
        return f;
    }

    template<typename T, typename F>
    constexpr Railcar<T,F> compose(const Railcar<T,F>& f, const Shifting<T>& g)
    {
        return Railcar<T,F>(inverse(g)(f.lo), inverse(g)(f.hi), compose(f.content, g) );
    }

    template<typename T, typename F>
    constexpr Railcar<T,F> compose(const Railcar<T,F>& f, const Scaling<T>& g)
    {
        return Railcar<T,F>(inverse(g)(f.lo), inverse(g)(f.hi), compose(f.content, g) );
    }

    /*
    `derivative` returns a function object for the derivative of a polynomial.
    */
    template<typename T, typename F>
    auto derivative(const Railcar<T,F>& car)
    {
        return Railcar<T,F>(car.lo, car.hi, derivative(car.content));
    }

    /*
    `derivative` returns the derivative of a polynomial at a point.
    It is meant to compliment the method signature for integral(p, lo, hi)
    */
    template<typename T, typename F>
    T derivative(const Railcar<T,F>& car, const T x)
    {
        return (car.lo < x && x <= car.hi)?
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

    template<typename T, typename F>
    Railcar<T,F> railcar(const T lo, const T hi, const F& content)
    {
        return Railcar<T,F>(lo, hi, content);
    }

}


