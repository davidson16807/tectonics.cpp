#pragma once

#include <iostream>
#include <algorithm>

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/Rational.hpp>
#include <math/analytic/ScaledComplement.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/rails/RationalRailyard.hpp>

#include <unit/si.hpp>

#include <relation/RationalRailyardRelation.hpp>

namespace compound {
namespace relation {

    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    using RationalRailyardRelation = ::relation::RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi>;

    // 5 uses, for heat capacities of liquids
    relation::RationalRailyardRelation<si::temperature<double>, si::specific_heat_capacity<double>, 0,6,0,1> get_dippr_liquid_heat_capacity_temperature_relation_114( 
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
        return relation::RationalRailyardRelation<si::temperature<double>, si::specific_heat_capacity<double>, 0,6,0,1>(
            analytic::Railyard<float, R>({
                RC(-oo,  Tmin, R(r(Tmin))),
                RC(Tmin, Tmax, r),
                RC(Tmax, oo,   R(r(Tmin))),
            }), si::kelvin, yunits, 0.0f);
    }


}}

