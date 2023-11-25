#pragma once

#include <iostream>
#include <algorithm>

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/Rational.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/ScaledComplement.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/rails/RationalRailyard.hpp>
#include <unit/si.hpp>


namespace compound {
namespace relation {

    /*
    `RationalRailyardRelation` consolidates many kinds of expressions
    that are commonly used to represent the relation between temperature and the heat capacity of liquids
    */
    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    class RationalRailyardRelation
    {
        math::Railyard<float, math::Rational<float, Plo,Phi,Qlo,Qhi>> yard;

        Tx Tunits;
        Ty yunits;

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
        RationalRailyardRelation(const Ty constant):
            yard(constant/Ty(1.0)),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        // copy constructor
        template<int Nlo, int Nhi>
        RationalRailyardRelation(
            const relation::PolynomialRailyardRelation<Tx, Ty, Nlo, Nhi> relation
        ):
            yard(relation.yard),

            Tunits(relation.xunits),
            yunits(relation.yunits),

            known_max_fractional_error(0.0)
        {
        }

        RationalRailyardRelation(
            const math::Railyard<float, math::Rational<float, Plo, Phi, Qlo, Qhi>> yard,

            const Tx Tunits,
            const Ty yunits,

            const float known_max_fractional_error
        ):
            yard(yard),

            Tunits(Tunits),
            yunits(yunits),

            known_max_fractional_error(known_max_fractional_error)
        {
        }

        RationalRailyardRelation& operator=(const RationalRailyardRelation other)
        {
            yard = other.yard;

            Tunits = other.Tunits;
            yunits = other.yunits;

            known_max_fractional_error = other.known_max_fractional_error;

            return *this;
        }

        Ty operator()(const Tx temperature) const
        {
            return (yard(temperature/Tunits)) * yunits;
        }

        RationalRailyardRelation& operator+=(const Ty offset)
        {
            using R = math::Rational<float, Plo, Phi, Qlo, Qhi>;
            using RC = math::Railcar<float, R>;
            yard.cars.push_back(RC(offset/yunits));
            return *this;
        }

        RationalRailyardRelation& operator-=(const Ty offset)
        {
            using R = math::Rational<float, Plo, Phi, Qlo, Qhi>;
            using RC = math::Railcar<float, R>;
            yard.cars.push_back(RC(-offset/yunits));
            return *this;
        }

        RationalRailyardRelation& operator*=(const float scalar)
        {
            yard *= scalar;
            return *this;
        }

        RationalRailyardRelation operator/=(const float scalar)
        {
            yard /= scalar;
            return *this;
        }

        RationalRailyardRelation& operator+=(const RationalRailyardRelation other)
        {
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            yard += yscale * compose(other.yard, math::Scaling<float>(Tscale));
            return *this;
        }

        RationalRailyardRelation& operator-=(const RationalRailyardRelation other)
        {
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            yard -= yscale * compose(other.yard, math::Scaling<float>(Tscale));
            return *this;
        }

    };


    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> operator+(const RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> relation, const RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> other)
    {
        RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> result = relation;
        result += other;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> operator-(const RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> relation, const RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> other)
    {
        RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> result = relation;
        result -= other;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> operator+(const RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> relation, const Ty offset)
    {
        RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> result = relation;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> operator-(const RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> relation, const Ty offset)
    {
        RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> result = relation;
        result -= offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> operator*(const RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> relation, const float scalar)
    {
        RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> result = relation;
        result *= scalar;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> operator/(const RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> relation, const float scalar)
    {
        RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> result = relation;
        result /= scalar;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> operator+(const Ty offset, const RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> relation)
    {
        RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> result = relation;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> operator-(const Ty offset, const RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> relation)
    {
        RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> result = relation;
        result *= 1.0f;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> operator-(const RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> relation)
    {
        RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> result = relation;
        result *= -1.0f;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> operator*(const float scalar, const RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> relation)
    {
        RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> result = relation;
        result *= scalar;
        return result;
    }


    // 5 uses, for heat capacities of liquids
    RationalRailyardRelation<si::temperature<double>, si::specific_heat_capacity<double>, 0,6,0,1> get_dippr_liquid_heat_capacity_temperature_relation_114( 
        const si::temperature<double> Tc, const si::specific_heat_capacity<double> yunits,
        const float c1, const float c2, const float c3, const float c4, const float c5,
        const float Tmin, float Tmax
    ){
        const float oo = std::numeric_limits<float>::max();
        using P = math::Polynomial<float,-1,5>;
        using R = math::Rational<float, 0,6,0,1>;
        using RC = math::Railcar<float, R>;
        math::Identity<float> T;
        R r = math::compose(
            P({c1*c1, c2, -2.0f*c1*c3, -c1*c4, -c3*c3/3.0f, -c3*c4/2.0f, -c4*c4/5.0f}), 
            1.0f-T/float(Tc/si::kelvin));
        return RationalRailyardRelation<si::temperature<double>, si::specific_heat_capacity<double>, 0,6,0,1>(
            math::Railyard<float, R>({
                RC(-oo,  Tmin, R(r(Tmin))),
                RC(Tmin, Tmax, r),
                RC(Tmax, oo,   R(r(Tmin))),
            }), si::kelvin, yunits, 0.0f);
    }


}}

