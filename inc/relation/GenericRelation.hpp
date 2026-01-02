#pragma once

// c libraries
#include <cmath>

// std libraries
#include <functional>

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/rails/PolynomialRailyard_library.hpp>
#include <unit/si.hpp>
#include <relation/PolynomialRailyardRelation.hpp>
#include <model/compound/relation/PolynomialRailyardRelation.hpp>
#include <model/compound/relation/ExponentiatedPolynomialRailyardRelation.hpp>
#include <model/compound/correlation/published.hpp>

namespace relation {

    /*
    `GenericRelation` represents an arbitrary relation 
    that uses an arbitrary anonymous function to map quantities from one unit to another
    This class is incapable of introspection and is expected to have poor data locality, 
    however it is useful when representing relations with high fidelity when those relations resist other attempts of description.
    It is expected that instances of this relation will be mapped to lower fidelity approximations before use,
    such as PolynomialRailyardRelation or (if implemented) some kind of GriddedSplineRelation.
    */
    template<typename X, typename Y>
    struct GenericRelation
    {
        std::function<Y(X)> f;
        
        using value_type = Y;

        // zero constructor
        constexpr GenericRelation():
            f([](X x){ return Y(0); })
        {
        }

        // constant constructor
        constexpr GenericRelation(const Y& constant):
            f([constant](X x){ return constant; })
        {
        }

        // copy constructor
        constexpr GenericRelation(
            const GenericRelation<X,Y>& other
        ):
            f(other.f)
        {
        }

        constexpr GenericRelation(
            const std::function<Y(X)> f
        ):
            f(f)
        {
        }

        template<typename F>
        constexpr GenericRelation(
            const F& other
        ):
            f([other](X x){return other(x);})
        {
        }

        constexpr GenericRelation<X,Y>& operator=(const GenericRelation<X,Y>& other)
        {
            f = other.f;
            return *this;
        }

        template<typename F>
        constexpr GenericRelation<X,Y>& operator=(const F& other)
        {
            f = [other](X x){ return other(x); };
            return *this;
        }

        constexpr GenericRelation<X,Y>& operator=(const Y& other)
        {
            f = [other](X x){return Y(other); };
            return *this;
        }

        Y operator()(const X x) const
        {
            return f(x);
        }

        // GenericRelation<X,Y> restriction(
        //     const si::temperature xlo, const si::temperature xhi,
        //     const double known_max_fractional_error
        // ) const
        // {
        //     return GenericRelation<X,Y>(restriction(yard, xlo/xunits, xhi/xunits), xunits, yunits);
        // }

        template<typename scalar>
        GenericRelation<X,Y>& operator+=(const scalar k)
        {
            f = [=,this](const X x){return f(x)+k;};
            return *this;
        }

        template<typename scalar>
        GenericRelation<X,Y>& operator-=(const scalar k)
        {
            f = [=,this](const X x){return f(x)-k;};
            return *this;
        }

        template<typename scalar>
        GenericRelation<X,Y>& operator*=(const scalar k)
        {
            f = [=,this](const X x){return f(x)*k;};
            return *this;
        }

        template<typename scalar>
        GenericRelation<X,Y>& operator/=(const scalar k)
        {
            f = [=,this](const X x){return f(x)/k;};
            return *this;
        }

        template<typename F>
        GenericRelation<X,Y>& operator+=(const F relation)
        {
            f = [=,this](const X x){return f(x)+relation(x);};
            return *this;
        }

        template<typename F>
        GenericRelation<X,Y>& operator-=(const F relation)
        {
            f = [=,this](const X x){return f(x)-relation(x);};
            return *this;
        }

    };

    template<typename X, typename Y>
    GenericRelation<X,Y> operator-(const GenericRelation<X,Y>& relation)
    {
        GenericRelation<X,Y> result = relation;
        result *= -1.0;
        return result;
    }

    template<typename X, typename Y>
    GenericRelation<X,Y> operator+(const GenericRelation<X,Y>& relation, const Y value)
    {
        GenericRelation<X,Y> result = relation;
        result += value;
        return result;
    }

    template<typename X, typename Y>
    GenericRelation<X,Y> operator+(const Y value, const GenericRelation<X,Y>& relation)
    {
        GenericRelation<X,Y> result = relation;
        result += value;
        return result;
    }

    template<typename X, typename Y>
    GenericRelation<X,Y> operator-(const GenericRelation<X,Y>& relation, const Y value)
    {
        GenericRelation<X,Y> result = relation;
        result -= value;
        return result;
    }

    template<typename X, typename Y>
    GenericRelation<X,Y> operator-(const Y value, const GenericRelation<X,Y>& relation)
    {
        GenericRelation<X,Y> result = relation;
        result *= -1.0;
        result += value;
        return result;
    }

}

