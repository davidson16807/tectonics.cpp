#pragma once

#include <algorithm>

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/Polynomial.hpp>
#include <math/analytic/Clamped.hpp>
#include <math/analytic/Exponent.hpp>
#include <math/analytic/AlgebraicSigmoid.hpp>
#include <math/inspected/extremum.hpp>
#include <unit/si.hpp>

#include <model/compound/point.hpp>
#include <model/compound/dippr/Dippr102.hpp>

namespace compound {
namespace relation {

    using ClampedExponent = analytic::Clamped<float,analytic::Exponent<float>>;
    using ClampedSigmoid = analytic::Clamped<float,analytic::AlgebraicSigmoid<float>>;
    using ClampedDippr102 = analytic::Clamped<float,dippr::Dippr102>;
    using ClampedExponentSum = analytic::Sum<float,ClampedExponent>;
    using ClampedSigmoidSum = analytic::Sum<float,ClampedSigmoid>;
    using ClampedDippr102Sum = analytic::Sum<float,ClampedDippr102>;

    /*
    `GasPropertyStateRelation` consolidates many kinds of expressions
    that are commonly used to represent pressure/temperature relations for compounds in their gas phase.
    These properties are namely the isobaric specific heat capacity, thermal conductivity, and dynamic viscosity.

    More broadly, GasPropertyStateRelation should be capable of representing any smooth pressure/temperature relation
    in which pressure and temperature interactions are negligible.
    Clamped<Exponent>s can be used to represent arbitrary polynomials,
    and Clamped<Sigmoid>s can be used to represent arbitrary lerp functions.
    */
    template<typename Ty>
    class GasPropertyStateRelation
    {

        ClampedExponentSum pexponents;
        ClampedExponentSum Texponents;
        ClampedSigmoidSum  Tsigmoids;
        ClampedDippr102Sum Tdippr102s;

        si::pressure<double>    punits;
        si::temperature<double> Tunits;
        Ty              yunits;

        float intercept;

    public:
        float known_max_fractional_error;
        using value_type = Ty;

        // zero constructor
        constexpr GasPropertyStateRelation():
            pexponents(),
            Texponents(),
            Tsigmoids(),
            Tdippr102s(),

            punits(1.0),
            Tunits(1.0),
            yunits(1.0),

            intercept(0.0),
            known_max_fractional_error(0.0)
        {
        }

        constexpr GasPropertyStateRelation(
            const ClampedExponentSum pexponents,
            const ClampedExponentSum Texponents,
            const ClampedSigmoidSum  Tsigmoids,
            const ClampedDippr102Sum Tdippr102s,

            const si::pressure<double>    punits,
            const si::temperature<double> Tunits,
            const Ty              yunits,

            const float intercept,
            const float known_max_fractional_error
        ):
            pexponents(pexponents),
            Texponents(Texponents),
            Tsigmoids(Tsigmoids),
            Tdippr102s(Tdippr102s),

            punits(punits),
            Tunits(Tunits),
            yunits(yunits),

            intercept(intercept),
            known_max_fractional_error(known_max_fractional_error)
        {
        }

        constexpr GasPropertyStateRelation(const Ty intercept):
            pexponents(),
            Texponents(),
            Tsigmoids(),
            Tdippr102s(),

            punits(si::standard_pressure),
            Tunits(si::standard_temperature),
            yunits(Ty(1.0)),

            intercept(intercept/Ty(1.0)),
            known_max_fractional_error(0.0)
        {
        }

        constexpr GasPropertyStateRelation<Ty>& operator=(const GasPropertyStateRelation<Ty> other)
        {
            pexponents = other.pexponents;
            Texponents = other.Texponents;
            Tsigmoids  = other.Tsigmoids;
            Tdippr102s = other.Tdippr102s;

            punits = other.punits;
            Tunits = other.Tunits;
            yunits = other.yunits;

            intercept = other.intercept;
            known_max_fractional_error = other.known_max_fractional_error;

            return *this;
        }

        Ty operator()(const si::pressure<double> pressure, const si::temperature<double> temperature) const
        {
            const float p = float(pressure/punits);
            const float T = float(temperature/Tunits);
            ClampedExponent exponent;
            ClampedSigmoid sigmoid;
            ClampedDippr102 dippr102;
            return (intercept + pexponents(p) + Texponents(T) + Tsigmoids(T) + Tdippr102s(T)) * yunits;
        }

        Ty operator()(const si::temperature<double> temperature, const si::pressure<double> pressure) const
        {
            const float p = float(pressure/punits);
            const float T = float(temperature/Tunits);
            ClampedExponent exponent;
            ClampedSigmoid sigmoid;
            ClampedDippr102 dippr102;
            return (intercept + pexponents(p) + Texponents(T) + Tsigmoids(T) + Tdippr102s(T)) * yunits;
        }

        Ty operator()(const point<double> point) const
        {
            const float p = float(point.pressure/punits);
            const float T = float(point.temperature/Tunits);
            ClampedExponent exponent;
            ClampedSigmoid sigmoid;
            ClampedDippr102 dippr102;
            return (intercept + pexponents(p) + Texponents(T) + Tsigmoids(T) + Tdippr102s(T)) * yunits;
        }

        GasPropertyStateRelation<Ty> restriction(
            const si::pressure<double> min_pressure, const si::pressure<double> max_pressure,
            const si::temperature<double> min_temperature, const si::temperature<double> max_temperature,
            const float known_max_fractional_error
        ) const
        {
            const float plo = float(min_pressure/punits);
            const float Tlo = float(min_temperature/Tunits);
            const float phi = float(max_pressure/punits);
            const float Thi = float(max_temperature/Tunits);
            GasPropertyStateRelation<Ty> restricted(*this);
            bool is_removal_occurring(true);
            float float_precision_of_error = 1e-6;
            float least_useful_term_error;
            while(restricted.known_max_fractional_error < known_max_fractional_error) {
                auto least_useful_pexponent = std::min_element(restricted.pexponents.terms.begin(), restricted.pexponents.terms.end(),
                    [&](const ClampedExponent& f1, const ClampedExponent& f2){
                        (maximum(f1,plo,phi)-minimum(f1,plo,phi)) < (maximum(f2,plo,phi)-minimum(f2,plo,phi));
                    });
                float least_useful_pexponent_error = maximum(least_useful_pexponent, plo,phi) - minimum(least_useful_pexponent, Tlo,Thi);
                auto least_useful_Texponent = std::min_element(
                    restricted.Texponents.terms.begin(), restricted.Texponents.terms.end(),
                    [&](const ClampedExponent& f1, const ClampedExponent& f2){
                        (maximum(f1,Tlo,Thi)-minimum(f1,Tlo,Thi)) < (maximum(f2,Tlo,Thi)-minimum(f2,Tlo,Thi));
                    });
                float least_useful_Texponent_error = maximum(least_useful_Texponent, plo,phi) - minimum(least_useful_Texponent, Tlo,Thi);
                auto least_useful_Tsigmoid = std::min_element(
                    restricted.Tsigmoids.terms.begin(), restricted.Tsigmoids.terms.end(),
                    [&](const ClampedExponent& f1, const ClampedExponent& f2){
                        (maximum(f1,Tlo,Thi)-minimum(f1,Tlo,Thi)) < (maximum(f2,Tlo,Thi)-minimum(f2,Tlo,Thi));
                    });
                float least_useful_Tsigmoid_error = maximum(least_useful_Tsigmoid, plo,phi) - minimum(least_useful_Tsigmoid, Tlo,Thi);
                auto least_useful_Tdippr102 = std::min_element(
                    restricted.Tdippr102s.terms.begin(), restricted.Tdippr102s.terms.end(),
                    [&](const ClampedExponent& f1, const ClampedExponent& f2){
                        (maximum(f1,Tlo,Thi)-minimum(f1,Tlo,Thi)) < (maximum(f2,Tlo,Thi)-minimum(f2,Tlo,Thi));
                    });
                float least_useful_Tdippr102_error = maximum(least_useful_Tdippr102, plo,phi) - minimum(least_useful_Tdippr102, Tlo,Thi);

                least_useful_term_error = std::min(least_useful_pexponent_error, least_useful_Texponent_error);
                least_useful_term_error = std::min(least_useful_term_error,      least_useful_Tsigmoid_error );
                least_useful_term_error = std::min(least_useful_term_error,      least_useful_Tdippr102_error);

                if (restricted.known_max_fractional_error + least_useful_term_error < known_max_fractional_error)
                {
                    if (std::abs(least_useful_pexponent_error - least_useful_term_error) < float_precision_of_error)
                    {
                        restricted.intercept += (maximum(least_useful_pexponent, plo,phi) - minimum(least_useful_pexponent, Tlo,Thi)) / 2.0;
                        restricted.known_max_fractional_error += least_useful_pexponent_error;
                        restricted.pexponents.terms.erase(least_useful_pexponent);
                        continue;
                    }
                    if (std::abs(least_useful_Texponent_error - least_useful_term_error) < float_precision_of_error)
                    {
                        restricted.intercept += (maximum(least_useful_Texponent, plo,phi) - minimum(least_useful_Texponent, Tlo,Thi)) / 2.0;
                        restricted.known_max_fractional_error += least_useful_Texponent_error;
                        restricted.Texponents.terms.erase(least_useful_Texponent);
                        continue;
                    }
                    if (std::abs(least_useful_Tsigmoid_error - least_useful_term_error) < float_precision_of_error)
                    {
                        restricted.intercept += (maximum(least_useful_Tsigmoid, plo,phi) - minimum(least_useful_Tsigmoid, Tlo,Thi)) / 2.0;
                        restricted.known_max_fractional_error += least_useful_Tsigmoid_error;
                        restricted.Tsigmoids.terms.erase(least_useful_Tsigmoid);
                        continue;
                    }
                    if (std::abs(least_useful_Tdippr102_error - least_useful_term_error) < float_precision_of_error)
                    {
                        restricted.intercept += (maximum(least_useful_Tdippr102, plo,phi) - minimum(least_useful_Tdippr102, Tlo,Thi)) / 2.0;
                        restricted.known_max_fractional_error += least_useful_Tdippr102_error;
                        restricted.Tdippr102s.terms.erase(least_useful_Tdippr102);
                        continue;
                    }
                }
            }
            return restricted;
        }

        GasPropertyStateRelation<Ty>& operator+=(const Ty offset)
        {
            intercept += offset/yunits;
            return *this;
        }

        GasPropertyStateRelation<Ty>& operator-=(const Ty offset)
        {
            intercept -= offset/yunits;
            return *this;
        }

        GasPropertyStateRelation<Ty>& operator*=(const float scalar)
        {
            pexponents *= scalar;
            Texponents *= scalar;
            Tsigmoids  *= scalar;
            Tdippr102s *= scalar;
            intercept *= scalar;
            return *this;
        }

        GasPropertyStateRelation<Ty> operator/=(const float scalar)
        {
            pexponents /= scalar;
            Texponents /= scalar;
            Tsigmoids  /= scalar;
            Tdippr102s /= scalar;
            intercept /= scalar;
            return *this;
        }

        GasPropertyStateRelation<Ty>& operator+=(const GasPropertyStateRelation<Ty> other)
        {
            const float pscale = float(other.punits / punits);
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            pexponents += yscale * compose(other.pexponents, analytic::Scaling<float>(pscale));
            Texponents += yscale * compose(other.Texponents, analytic::Scaling<float>(Tscale));
            Tdippr102s += yscale * compose(other.Tdippr102s, analytic::Scaling<float>(Tscale));
            Tsigmoids += yscale * compose(other.Tsigmoids, analytic::Scaling<float>(Tscale));
            intercept += yscale * other.intercept;
            return *this;
        }

        GasPropertyStateRelation<Ty>& operator-=(const GasPropertyStateRelation<Ty> other)
        {
            const float pscale = float(other.punits / punits);
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            pexponents -= yscale * compose(other.pexponents, analytic::Scaling<float>(pscale));
            Texponents -= yscale * compose(other.Texponents, analytic::Scaling<float>(Tscale));
            Tdippr102s -= yscale * compose(other.Tdippr102s, analytic::Scaling<float>(Tscale));
            Tsigmoids -= yscale * compose(other.Tsigmoids, analytic::Scaling<float>(Tscale));
            intercept -= yscale * other.intercept;
            return *this;
        }

    };

    // Custom parameterization, offers high accuracy for most pure compounds
    // 36 uses, for dynamic_viscosity, isobaric_specific_heat_capacity, and thermal_conductivity of gases
    template<typename Ty>
    constexpr GasPropertyStateRelation<Ty> get_sigmoid_exponent_pressure_temperature_relation(
        const si::temperature<double> Tunits, const si::pressure<double> punits, const Ty yunits,
        const float pslope, const float pexponent,
        const float Tslope, const float Texponent,
        const float Tsigmoid_max, const float Tsigmoid_scale, const float Tsigmoid_center,
        const float intercept,
        const float Tmin, const float Tmax,
        const float pmin, const float pmax,
        const float known_max_fractional_error
    ) {
        return GasPropertyStateRelation<Ty>(
            ClampedExponentSum{ClampedExponent(pmin, pmax, analytic::Exponent<float>(pslope, pexponent))},
            ClampedExponentSum{ClampedExponent(Tmin, Tmax, analytic::Exponent<float>(Tslope, Texponent))},
            ClampedSigmoidSum{ClampedSigmoid(Tmin, Tmax, analytic::AlgebraicSigmoid<float>(1.0f/Tsigmoid_scale, -Tsigmoid_center/Tsigmoid_scale, Tsigmoid_max))},
            ClampedDippr102Sum(),

            punits, Tunits, yunits,

            intercept,
            known_max_fractional_error
        );
    }

    template<typename Ty>
    constexpr GasPropertyStateRelation<Ty> get_exponent_pressure_temperature_relation(
        const si::temperature<double> Tunits, const si::pressure<double> punits, const Ty yunits,
        const float pslope, const float pexponent,
        const float Tslope, const float Texponent,
        const float intercept,
        const float Tmin, const float Tmax,
        const float pmin, const float pmax,
        const float known_max_fractional_error
    ) {
        return GasPropertyStateRelation<Ty>(
            ClampedExponentSum{ClampedExponent(pmin, pmax, analytic::Exponent<float>(pslope, pexponent))},
            ClampedExponentSum{ClampedExponent(Tmin, Tmax, analytic::Exponent<float>(Tslope, Texponent))},
            ClampedSigmoidSum{},
            ClampedDippr102Sum(),

            punits, Tunits, yunits,

            intercept,
            known_max_fractional_error
        );
    }

    // Direct port from the DIPPR 102 equation
    // 9 uses, for viscosity and thermal conductivity of gas
    template<typename Ty>
    constexpr GasPropertyStateRelation<Ty> get_dippr_temperature_relation_102(
        const si::temperature<double> Tunits, const Ty yunits,
        const float c1, const float c2, const float c3, const float c4,
        const float Tmin, const float Tmax
    ) {
        return GasPropertyStateRelation<Ty>(
            ClampedExponentSum(),
            ClampedExponentSum(),
            ClampedSigmoidSum(),
            ClampedDippr102Sum{ClampedDippr102(Tmin, Tmax, dippr::Dippr102(c1, c2, c3, c4))},

            si::pressure<double>(1.0), Tunits, yunits,

            0.0, // intercept
            0.0 // known_max_fractional_error
        );
    }

    /*
    `approx_inferred_pressure_temperature_gas_relation()` attempts to encode a bivariate function f(p,T)
    into an equation that can be expressed using GasPropertyStateRelation. 
    It is intended that the function represents a property whose values were inferred from other properties.
    The inference methods tend to be error prone, and are only used as a last resort,
    so underfitting is not a significant concern, 
    and is in fact desireable to err on the side of a conservative approximation
    that experiences no sudden changes throughout the relationship.
    For this reason, we encode the function as a bivariate quadratic Newton polynomial 
    over the range defined by [pmin,pmax]×[Tmin,Tmax]. 
    Interaction terms within the polynomial (such as pT, pT², and p²T) are ignored,
    since GasPropertyStateRelation does not represent properties that exhibit strong pressure/temperature interaction.
    After fitting, the maximum error for the approximation is estimated using Newton's method.

    Newton polynomials explode past the range of their observed values, 
    but GasPropertyStateRelation already clamps input to the valid range to prevent this from happening.
    Newton polynomials also have a tendency to overfit,
    but this is not a significant issue when the degree of the polynomial is small.
    Underfitting may occur if the degree is small and the approximated function has sudden jumps,
    but as mentioned, a conservative smooth approximation is desireable. 
    Furthermore, observed relationships for GasPropertyStateRelation do not tend to to have sudden changes,
    and if any jump does occur it will be at a temperature close to absolute zero.
    More importantly, the quadratic newton polynomial provides a conservative approximation 
    that is guaranteed to be found in constant time without using iterative optimization. 
    Error estimation is iterative, but the iterative method converges fast and can be done in inconsequential time.
    */
    template<typename Ty, typename F>
    constexpr GasPropertyStateRelation<Ty> approx_inferred_pressure_temperature_gas_relation(
        const si::temperature<double> Tunits, const si::pressure<double> punits, const Ty yunits,
        const F& f,
        const float Tmin, const float Tmax,
        const float pmin, const float pmax,
        const float known_max_fractional_error
    ) {
        //pick STP as the middle sample point if it falls within the valid range, otherwise pick the halfway point.
        const float Tmid = Tmin*Tunits < si::standard_temperature && si::standard_temperature < Tmax*Tunits? 
            si::standard_temperature/Tunits : (Tmin+Tmax)/2.0f;
        const float pmid = pmin*punits < si::standard_pressure    && si::standard_pressure    < pmax*punits? 
            si::standard_pressure   /punits : (pmin+pmax)/2.0f;
        auto fp   = analytic::quadratic_newton_polynomial(
            pmin, float(f(pmin*punits,  Tmid*Tunits) / yunits), 
            pmid, float(f(pmid*punits,  Tmid*Tunits) / yunits), 
            pmax, float(f(pmax*punits,  Tmid*Tunits) / yunits));
        auto fT   = analytic::quadratic_newton_polynomial(
            Tmin, float(f(pmin*punits,  Tmin*Tunits) / yunits), 
            Tmid, float(f(pmin*punits,  Tmid*Tunits) / yunits), 
            Tmax, float(f(pmin*punits,  Tmax*Tunits) / yunits));
        auto fhat = GasPropertyStateRelation<Ty>(
            analytic::Sum<float,ClampedExponent>{
                ClampedExponent(pmin, pmax, analytic::Exponent<float>(fp[1], 1.0f)),
                ClampedExponent(pmin, pmax, analytic::Exponent<float>(fp[2], 2.0f))
            },
            analytic::Sum<float,ClampedExponent>{
                ClampedExponent(Tmin, Tmax, analytic::Exponent<float>(fT[1], 1.0f)),
                ClampedExponent(Tmin, Tmax, analytic::Exponent<float>(fT[2], 2.0f))
            },
            ClampedSigmoidSum(),
            ClampedDippr102Sum(),

            si::pressure<double>(1.0), Tunits, yunits,

            fT[0], 
            known_max_fractional_error
        );
        // Estimate max fractional error by iteratively solving for the maximum of a loss function
        // Do this once independantly for each parameter.
        auto fractional_error = [&](float p, float T) -> float
        {
            auto fpT    = f(p*punits,T*Tunits)/yunits;
            auto fpThat = f(p*punits,T*Tunits)/yunits;
            return std::abs(fpThat-fpT) / std::abs(fpT); 
        };
        float Tworst = inspected::maximum([&](float T){ return fractional_error(pmid,T);   }, Tmin, Tmax, 10);
        float pworst = inspected::maximum([&](float p){ return fractional_error(p,Tworst); }, pmin, pmax, 10);
        fhat.known_max_fractional_error += fractional_error(pworst, Tworst);
        return fhat;
    }

    // Direct port from the Perry equation used for some gas properties
    // 1 use, for heat capacity of nitric oxide
    template<typename Ty>
    constexpr GasPropertyStateRelation<Ty> get_perry_temperature_gas_relation(
        const si::temperature<double> Tunits, const Ty yunits,
        const float intercept, const float linear, const float inverse_square, const float square,
        const float Tmin, const float Tmax
    ) {
        return GasPropertyStateRelation<Ty>(
            analytic::Sum<float,ClampedExponent>(),
            analytic::Sum<float,ClampedExponent>{
                ClampedExponent(Tmin, Tmax, analytic::Exponent<float>(linear, 1.0f)),
                ClampedExponent(Tmin, Tmax, analytic::Exponent<float>(inverse_square, -2.0f)),
                ClampedExponent(Tmin, Tmax, analytic::Exponent<float>(square, 2.0f))
            },
            ClampedSigmoidSum(),
            ClampedDippr102Sum(),

            si::pressure<double>(1.0), Tunits, yunits,

            intercept,
            0.0 // known_max_fractional_error
        );
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator+(const GasPropertyStateRelation<Ty> relation, const GasPropertyStateRelation<Ty> other)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result += other;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator-(const GasPropertyStateRelation<Ty> relation, const GasPropertyStateRelation<Ty> other)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result -= other;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator+(const GasPropertyStateRelation<Ty> relation, const Ty offset)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result += offset;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator-(const GasPropertyStateRelation<Ty> relation, const Ty offset)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result -= offset;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator*(const GasPropertyStateRelation<Ty> relation, const float scalar)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result *= scalar;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator/(const GasPropertyStateRelation<Ty> relation, const float scalar)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result /= scalar;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator+(const Ty offset, const GasPropertyStateRelation<Ty> relation)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result += offset;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator-(const Ty offset, const GasPropertyStateRelation<Ty> relation)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result *= 1.0f;
        result += offset;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator-(const GasPropertyStateRelation<Ty> relation)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result *= 1.0f;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator*(const float scalar, const GasPropertyStateRelation<Ty> relation)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result *= scalar;
        return result;
    }

    /*
    `distance` is a distance metric that is designed to test whether two objects are equal 
    up to some threshold that the user defines to reflect the precision of underlying types.
    GasPropertyStateRelation does not have a convenient closed form distance metric,
    so equality is determined pragmatically by sampling at given pressures/temperatures
    */
    // template<typename T1>
    // float distance(
    //     const GasPropertyStateRelation<T1>& first, 
    //     const GasPropertyStateRelation<T1>& second,
    //     const field::StateParameters lo, 
    //     const field::StateParameters hi
    // ){
    //     float result(0.0f);
    //     si::pressure<double> lop = lo.pressure;
    //     si::temperature<double> loT = lo.temperature;
    //     si::pressure<double> hip = hi.pressure;
    //     si::temperature<double> hiT = hi.temperature;
    //     for(si::pressure<double> p = lop; p<hip; p*=3.0)
    //     {
    //         for(si::temperature<double> T = loT; T<hiT; T*=3.0)
    //         {
    //             result = std::max(result, 
    //                 float(std::abs((first(p, T) - second(p, T)) / T1(1))));
    //         }
    //     }
    //     return result;
    // }
}}

