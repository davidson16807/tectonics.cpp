#pragma once

#include <algorithm>
#include <limits>    // numeric_limits

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/Rational.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/ScaledComplement.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/rails/RationalRailyard.hpp>
#include <unit/si.hpp>


namespace relation {

    /*
    `RationalRailyardRelation` consolidates many kinds of expressions
    that are commonly used to represent the relation between temperature and the heat capacity of liquids
    */
    template<typename X, typename Y, int Plo, int Phi, int Qlo, int Qhi>
    class RationalRailyardRelation
    {
        analytic::Railyard<float, analytic::Rational<float, Plo,Phi,Qlo,Qhi>> yard;

        X Tunits;
        Y yunits;

    public:
        float known_max_fractional_error;

        // zero constructor
        RationalRailyardRelation():
            yard(),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        // constant constructor
        RationalRailyardRelation(const Y constant):
            yard(constant/Y(1.0)),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        // copy constructor
        template<int Nlo, int Nhi>
        RationalRailyardRelation(
            const relation::PolynomialRailyardRelation<X, Y, Nlo, Nhi> relation
        ):
            yard(relation.yard),

            Tunits(relation.xunits),
            yunits(relation.yunits),

            known_max_fractional_error(0.0)
        {
        }

        RationalRailyardRelation(
            const analytic::Railyard<float, analytic::Rational<float, Plo, Phi, Qlo, Qhi>> yard,

            const X Tunits,
            const Y yunits,

            const float known_max_fractional_error
        ):
            yard(yard),

            Tunits(Tunits),
            yunits(yunits),

            known_max_fractional_error(known_max_fractional_error)
        {
        }

        RationalRailyardRelation& operator=(const RationalRailyardRelation& other)
        {
            yard = other.yard;

            Tunits = other.Tunits;
            yunits = other.yunits;

            known_max_fractional_error = other.known_max_fractional_error;

            return *this;
        }

        Y operator()(const X temperature) const
        {
            return (yard(temperature/Tunits)) * yunits;
        }

        RationalRailyardRelation& operator+=(const Y offset)
        {
            using R = analytic::Rational<float, Plo, Phi, Qlo, Qhi>;
            using RC = analytic::Railcar<float, R>;
            yard.cars.push_back(RC(offset/yunits));
            return *this;
        }

        RationalRailyardRelation& operator-=(const Y offset)
        {
            using R = analytic::Rational<float, Plo, Phi, Qlo, Qhi>;
            using RC = analytic::Railcar<float, R>;
            yard.cars.push_back(RC(-offset/yunits));
            return *this;
        }

        template<typename scalar>
        RationalRailyardRelation& operator*=(const scalar k)
        {
            yard *= k;
            return *this;
        }

        template<typename scalar>
        RationalRailyardRelation& operator/=(const scalar k)
        {
            yard /= k;
            return *this;
        }

        RationalRailyardRelation& operator+=(const RationalRailyardRelation other)
        {
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            yard += yscale * compose(other.yard, analytic::Scaling<float>(Tscale));
            return *this;
        }

        RationalRailyardRelation& operator-=(const RationalRailyardRelation other)
        {
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            yard -= yscale * compose(other.yard, analytic::Scaling<float>(Tscale));
            return *this;
        }

    };


    template<typename X, typename Y, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> operator+(const RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> relation, const RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> other)
    {
        RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> result = relation;
        result += other;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> operator-(const RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> relation, const RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> other)
    {
        RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> result = relation;
        result -= other;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> operator+(const RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> relation, const Y offset)
    {
        RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> result = relation;
        result += offset;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> operator-(const RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> relation, const Y offset)
    {
        RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> result = relation;
        result -= offset;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> operator*(const RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> relation, const float k)
    {
        RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> result = relation;
        result *= k;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> operator/(const RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> relation, const float k)
    {
        RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> result = relation;
        result /= k;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> operator+(const Y offset, const RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> relation)
    {
        RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> result = relation;
        result += offset;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> operator-(const Y offset, const RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> relation)
    {
        RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> result = relation;
        result *= -1.0f;
        result += offset;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> operator-(const RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> relation)
    {
        RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> result = relation;
        result *= -1.0f;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi, int Qlo, int Qhi, typename scalar=float>
    RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> operator*(const scalar k, const RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> relation)
    {
        RationalRailyardRelation<X,Y,Plo,Phi,Qlo,Qhi> result = relation;
        result *= k;
        return result;
    }


    // 5 uses, for heat capacities of liquids
    RationalRailyardRelation<si::temperature<double>, si::specific_heat_capacity<double>, 0,6,0,1> get_dippr_liquid_heat_capacity_temperature_relation_114( 
        const si::temperature<double> Tc, const si::specific_heat_capacity<double> yunits,
        const float c1, const float c2, const float c3, const float c4, const float c5,
        const float Tmin, float Tmax
    ){
        const float oo = std::numeric_limits<float>::max();
        using P = analytic::Polynomial<float,-1,5>;
        using R = analytic::Rational<float, 0,6,0,1>;
        using RC = analytic::Railcar<float, R>;
        analytic::Identity<float> T;
        R r = analytic::compose(
            P({c1*c1, c2, -2.0f*c1*c3, -c1*c4, -c3*c3/3.0f, -c3*c4/2.0f, -c4*c4/5.0f}), 
            1.0f-T/float(Tc/si::kelvin));
        return RationalRailyardRelation<si::temperature<double>, si::specific_heat_capacity<double>, 0,6,0,1>(
            analytic::Railyard<float, R>({
                RC(-oo,  Tmin, R(r(Tmin))),
                RC(Tmin, Tmax, r),
                RC(Tmax, oo,   R(r(Tmax))),
            }), si::kelvin, yunits, 0.0f);
    }


}

