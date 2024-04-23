#pragma once

// std libraries
#include <cmath>

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
    template<typename Tx, typename Ty>
    struct GenericRelation
    {
        std::function<Ty(Tx)> f;
        
        using value_type = Ty;

        // zero constructor
        constexpr GenericRelation():
            f([](Tx x){ return Ty(0); })
        {
        }

        // constant constructor
        constexpr GenericRelation(const Ty& constant):
            f([constant](Tx x){ return constant; })
        {
        }

        // copy constructor
        constexpr GenericRelation(
            const GenericRelation<Tx,Ty>& other
        ):
            f(other.f)
        {
        }

        constexpr GenericRelation(
            const std::function<Ty(Tx)> f
        ):
            f(f)
        {
        }

        template<typename F>
        constexpr GenericRelation(
            const F& other
        ):
            f([other](Tx x){return other(x);})
        {
        }

        constexpr GenericRelation<Tx,Ty>& operator=(const GenericRelation<Tx,Ty>& other)
        {
            f = other.f;
            return *this;
        }

        template<typename F>
        constexpr GenericRelation<Tx,Ty>& operator=(const F& other)
        {
            f = [other](Tx x){ return other(x); };
            return *this;
        }

        constexpr GenericRelation<Tx,Ty>& operator=(const Ty& other)
        {
            f = [other](Tx x){return Ty(other); };
            return *this;
        }

        Ty operator()(const Tx x) const
        {
            return f(x);
        }

        // GenericRelation<Tx,Ty> restriction(
        //     const si::temperature xlo, const si::temperature xhi,
        //     const double known_max_fractional_error
        // ) const
        // {
        //     return GenericRelation<Tx,Ty>(restriction(yard, xlo/xunits, xhi/xunits), xunits, yunits);
        // }

        GenericRelation<Tx,Ty> operator+=(const double scalar)
        {
            f = [=,this](const Tx x){return f(x)+scalar;};
            return *this;
        }

        GenericRelation<Tx,Ty> operator-=(const double scalar)
        {
            f = [=,this](const Tx x){return f(x)-scalar;};
            return *this;
        }

        GenericRelation<Tx,Ty> operator*=(const double scalar)
        {
            f = [=,this](const Tx x){return f(x)*scalar;};
            return *this;
        }

        GenericRelation<Tx,Ty> operator/=(const double scalar)
        {
            f = [=,this](const Tx x){return f(x)/scalar;};
            return *this;
        }

        template<typename F>
        GenericRelation<Tx,Ty>& operator+=(const F relation)
        {
            f = [=,this](const Tx x){return f(x)+relation(x);};
            return *this;
        }

        template<typename F>
        GenericRelation<Tx,Ty>& operator-=(const F relation)
        {
            f = [=,this](const Tx x){return f(x)-relation(x);};
            return *this;
        }

    };

    template<typename Tx, typename Ty>
    GenericRelation<Tx,Ty> operator-(const GenericRelation<Tx,Ty>& relation)
    {
        GenericRelation<Tx,Ty> result = relation;
        result *= -1.0;
        return result;
    }

    template<typename Tx, typename Ty>
    GenericRelation<Tx,Ty> operator+(const GenericRelation<Tx,Ty>& relation, const Ty value)
    {
        GenericRelation<Tx,Ty> result = relation;
        result += value;
        return result;
    }

    template<typename Tx, typename Ty>
    GenericRelation<Tx,Ty> operator+(const Ty value, const GenericRelation<Tx,Ty>& relation)
    {
        GenericRelation<Tx,Ty> result = relation;
        result += value;
        return result;
    }

    template<typename Tx, typename Ty>
    GenericRelation<Tx,Ty> operator-(const GenericRelation<Tx,Ty>& relation, const Ty value)
    {
        GenericRelation<Tx,Ty> result = relation;
        result -= value;
        return result;
    }

    template<typename Tx, typename Ty>
    GenericRelation<Tx,Ty> operator-(const Ty value, const GenericRelation<Tx,Ty>& relation)
    {
        GenericRelation<Tx,Ty> result = relation;
        result *= -1.0;
        result += value;
        return result;
    }

}

