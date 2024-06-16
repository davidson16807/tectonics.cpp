#pragma once

// std libraries
#include <cmath>

// in-house libraries
#include <math/analytic/Scaling.hpp>

namespace relation {

    /*
    `ScalarRelation` represents an arbitrary relation 
    using only an input unit (x0), an output unit (y0), and a relation (f:ℝ→ℝ) as an arbitrary callable object.
    The `ScalarRelation` is itself a callable object, and when called, it returns f(x/x0)*y0 for a given input, x.
    */
    template<typename X, typename Y, typename F>
    struct ScalarRelation
    {
        F f;
        X x0;
        Y y0;
        
        using value_type = Y;

        // copy constructor
        constexpr ScalarRelation(
            const ScalarRelation<X,Y,F>& other
        ):
            f(other.f),
            x0(1),
            y0(1)
        {
        }

        constexpr ScalarRelation(
            const X x0,
            const Y y0,
            const F f
        ):
            f(f),
            x0(x0),
            y0(y0)
        {
        }

        constexpr ScalarRelation<X,Y,F>& operator=(const ScalarRelation<X,Y,F>& other)
        {
            f = other.f;
            x0 = other.x0;
            y0 = other.y0;
            return *this;
        }

        Y operator()(const X x) const
        {
            return f(x/x0)*y0;
        }

        ScalarRelation<X,Y,F> operator+=(const Y k)
        {
            f += k;
            return *this;
        }

        ScalarRelation<X,Y,F> operator-=(const Y k)
        {
            f -= k;
            return *this;
        }

        ScalarRelation<X,Y,F> operator*=(const Y k)
        {
            y0 *= k;
            return *this;
        }

        ScalarRelation<X,Y,F> operator/=(const Y k)
        {
            y0 /= k;
            return *this;
        }

    };

    template<typename X, typename Y, typename F>
    ScalarRelation<X,Y,F> operator-(const ScalarRelation<X,Y,F>& relation)
    {
        ScalarRelation<X,Y,F> result = relation;
        result *= -1.0;
        return result;
    }

    template<typename X, typename Y, typename F>
    ScalarRelation<X,Y,F> operator+(const ScalarRelation<X,Y,F>& relation, const X value)
    {
        ScalarRelation<X,Y,F> result = relation;
        result += value;
        return result;
    }

    template<typename X, typename Y, typename F>
    ScalarRelation<X,Y,F> operator+(const X value, const ScalarRelation<X,Y,F>& relation)
    {
        ScalarRelation<X,Y,F> result = relation;
        result += value;
        return result;
    }

    template<typename X, typename Y, typename F>
    ScalarRelation<X,Y,F> operator-(const ScalarRelation<X,Y,F>& relation, const X value)
    {
        ScalarRelation<X,Y,F> result = relation;
        result -= value;
        return result;
    }

    template<typename X, typename Y, typename F>
    ScalarRelation<X,Y,F> operator-(const X value, const ScalarRelation<X,Y,F>& relation)
    {
        ScalarRelation<X,Y,F> result = relation;
        result *= -1.0;
        result += value;
        return result;
    }

}

