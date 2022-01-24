#pragma once

#include <iostream>
#include <algorithm>

// in-house libraries
#include <math/expression/Scaling.hpp>
#include <math/expression/Polynomial.hpp>
#include <math/expression/Clamped.hpp>
#include <math/inspection.hpp>
#include <units/si.hpp>

namespace compound {
namespace relation {

    struct Exponent {
        float weight;
        float exponent;
        constexpr explicit Exponent(const float weight, const float exponent):
            weight(weight),
            exponent(exponent)
        {}
        constexpr explicit Exponent():
            weight(0.0f),
            exponent(1.0f)
        {}
        constexpr Exponent(const Exponent& f):
            weight(f.weight),
            exponent(f.exponent)
        {}
        constexpr float operator()(const float x) const
        {
            return weight*std::pow(x, exponent);
        }
        constexpr Exponent& operator*=(const float k)
        {
            weight *= k;
            return *this;
        }
        constexpr Exponent& operator/=(const float k)
        {
            weight /= k;
            return *this;
        }
    };

    constexpr Exponent operator*(const Exponent f, const float k)
    {
        return Exponent(f.weight*k, f.exponent);
    }
    constexpr Exponent operator*(const float k, const Exponent f)
    {
        return Exponent(f.weight*k, f.exponent);
    }
    constexpr Exponent operator/(const Exponent f, const float k)
    {
        return Exponent(f.weight/k, f.exponent);
    }
    constexpr Exponent operator/(const float k, const Exponent f)
    {
        return Exponent(k/f.weight, -f.exponent);
    }

    constexpr Exponent compose(const Exponent f, const math::Scaling g)
    {
        return Exponent(
            f.weight*std::pow(g.factor, f.exponent),
            f.exponent
        );
    }

    struct Sigmoid {
        float xscale;
        float xoffset;
        float ymax;
        constexpr explicit Sigmoid(const float xscale, const float xoffset, const float ymax):
            xscale(xscale),
            xoffset(xoffset),
            ymax(ymax)
        {}
        constexpr explicit Sigmoid():
            xscale(0.0f),
            xoffset(0.0f),
            ymax(0.0f)
        {}
        constexpr Sigmoid(const Sigmoid& f):
            xscale(f.xscale),
            xoffset(f.xoffset),
            ymax(f.ymax)
        {}
        constexpr float operator()(const float x) const
        {
            float u = xscale * x + xoffset;
            return ymax * u / std::sqrt(1.0f + u*u);
        }
        constexpr Sigmoid& operator*=(const float scalar)
        {
            ymax *= scalar;
            return *this;
        }
        constexpr Sigmoid& operator/=(const float scalar)
        {
            ymax /= scalar;
            return *this;
        }
    };
    constexpr Sigmoid operator*(const Sigmoid relation, const float scalar)
    {
        return Sigmoid(relation.xscale, relation.xoffset, relation.ymax*scalar);
    }
    constexpr Sigmoid operator*(const float scalar, const Sigmoid relation)
    {
        return Sigmoid(relation.xscale, relation.xoffset, relation.ymax*scalar);
    }
    constexpr Sigmoid operator/(const Sigmoid relation, const float scalar)
    {
        return Sigmoid(relation.xscale, relation.xoffset, relation.ymax/scalar);
    }


    constexpr Sigmoid compose(const Sigmoid f, const math::Scaling g)
    {
        return Sigmoid(
            f.xscale * g.factor,
            f.xoffset,
            f.ymax
        );
    }

    struct Dippr102 {
        float c1;
        float c2;
        float c3;
        float c4;
        constexpr explicit Dippr102(
            const float c1,
            const float c2,
            const float c3,
            const float c4
        ):
            c1(c1),
            c2(c2),
            c3(c3),
            c4(c4)
        {}
        constexpr explicit Dippr102():
            c1(0.0f),
            c2(1.0f),
            c3(0.0f),
            c4(0.0f)
        {}
        constexpr Dippr102(const Dippr102& f):
            c1(f.c1),
            c2(f.c2),
            c3(f.c3),
            c4(f.c4)
        {}
        constexpr float operator()(const float x) const
        {
            const float u = x;
            return c1 * std::pow(u, c2) / (1.0f + c3/u + c4/(u*u));
        }
        constexpr Dippr102& operator*=(const float scalar)
        {
            c1 *= scalar;
            return *this;
        }
        constexpr Dippr102& operator/=(const float scalar)
        {
            c1 /= scalar;
            return *this;
        }
    };
    constexpr Dippr102 operator*(const Dippr102 relation, const float scalar)
    {
        return Dippr102(relation.c1 * scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr102 operator*(const float scalar, const Dippr102 relation)
    {
        return Dippr102(relation.c1 * scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr102 operator/(const Dippr102 relation, const float scalar)
    {
        return Dippr102(relation.c1 / scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr102 operator-(const Dippr102 relation)
    {
        return Dippr102(relation.c1 * -1.0f, relation.c2, relation.c3, relation.c4);
    }


    constexpr Dippr102 compose(Dippr102 f, const math::Scaling g)
    {
        return Dippr102(
            f.c1 * std::pow(g.factor, f.c2),
            f.c2,
            f.c3 / g.factor,
            f.c4 / (g.factor*g.factor)
        );
    }

    constexpr float maximum(const Exponent& f, const float lo, const float hi)
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) > f(lo)? hi : lo;
    }

    constexpr float minimum(const Exponent& f, const float lo, const float hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) < f(lo)? hi : lo;
    }

    constexpr float maximum(const Sigmoid& f, const float lo, const float hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) > f(lo)? hi : lo;
    }

    constexpr float minimum(const Sigmoid& f, const float lo, const float hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) < f(lo)? hi : lo;
    }

    /*
    `max_distance` measures the largest absolute difference between the output of two functions over the given range.
    This is useful when determining whether two functions should be consolidated when used in a larger equation.
    */
    constexpr float max_distance(const Exponent relation1, const Exponent relation2, const float lo, const float hi)
    {
        return std::max(
            std::abs(relation1(hi)-relation2(hi)),
            std::abs(relation1(lo)-relation2(lo))
        );
    }

    constexpr float max_distance(const Sigmoid relation1, const Sigmoid relation2, const float lo, const float hi)
    {
        return std::max(
            std::abs(relation1(hi)-relation2(hi)),
            std::abs(relation1(lo)-relation2(lo))
        );
    }

    using ClampedExponent = math::Clamped<Exponent>;
    using ClampedSigmoid = math::Clamped<Sigmoid>;
    using ClampedDippr102 = math::Clamped<Dippr102>;


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
        std::vector<ClampedExponent> pexponents;
        std::vector<ClampedExponent> Texponents;
        std::vector<ClampedSigmoid>  Tsigmoids;
        std::vector<ClampedDippr102> Tdippr102s;

        si::pressure    punits;
        si::temperature Tunits;
        Ty              yunits;

        float intercept;

    public:
        float known_max_fractional_error;
        using value_type = Ty;
        using parameter_type = field::StateParameters;

        constexpr GasPropertyStateRelation(
            const std::vector<ClampedExponent> pexponents,
            const std::vector<ClampedExponent> Texponents,
            const std::vector<ClampedSigmoid>  Tsigmoids,
            const std::vector<ClampedDippr102> Tdippr102s,

            const si::pressure    punits,
            const si::temperature Tunits,
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
            pexponents(0),
            Texponents(0),
            Tsigmoids(0),
            Tdippr102s(0),

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

        Ty operator()(const si::pressure pressure, const si::temperature temperature) const
        {
            const float p = float(pressure/punits);
            const float T = float(temperature/Tunits);
            ClampedExponent exponent;
            ClampedSigmoid sigmoid;
            ClampedDippr102 dippr102;
            float u = 0.0f;
            float y = intercept;
            for (std::size_t i = 0; i < pexponents.size(); ++i)
            {
                exponent = pexponents[i];
                y += exponent.f.weight * std::pow(std::clamp(p, exponent.lo, exponent.hi), exponent.f.exponent);
            }
            for (std::size_t i = 0; i < Texponents.size(); ++i)
            {
                exponent = Texponents[i];
                y += exponent.f.weight * std::pow(std::clamp(T, exponent.lo, exponent.hi), exponent.f.exponent);
            }
            for (std::size_t i = 0; i < Tsigmoids.size(); ++i)
            {
                sigmoid = Tsigmoids[i];
                u = sigmoid.f.xscale * std::clamp(T, sigmoid.lo, sigmoid.hi) + sigmoid.f.xoffset;
                y += sigmoid.f.ymax * u / std::sqrt(1.0f + u*u);
            }
            for (std::size_t i = 0; i < Tdippr102s.size(); ++i)
            {
                dippr102 = Tdippr102s[i];
                u = std::clamp(T, dippr102.lo, dippr102.hi);
                y += dippr102.f.c1 * std::pow(u, dippr102.f.c2) / (1.0f + dippr102.f.c3/u + dippr102.f.c4/(u*u));
            }

            // return intercept * yunits;
            return y * yunits;
        }

        Ty operator()(const field::StateParameters parameters) const
        {
            return (*this)(parameters.pressure, parameters.temperature);
        }

        GasPropertyStateRelation<Ty> restriction(
            const si::pressure min_pressure, const si::pressure max_pressure,
            const si::temperature min_temperature, const si::temperature max_temperature,
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
                auto least_useful_pexponent = std::min_element(restricted.pexponents.begin(), restricted.pexponents.end(),
                    [&](const ClampedExponent& f1, const ClampedExponent& f2){
                        (maximum(f1,plo,phi)-minimum(f1,plo,phi)) < (maximum(f2,plo,phi)-minimum(f2,plo,phi));
                    });
                float least_useful_pexponent_error = maximum(least_useful_pexponent, plo,phi) - minimum(least_useful_pexponent, Tlo,Thi);
                auto least_useful_Texponent = std::min_element(
                    restricted.Texponents.begin(), restricted.Texponents.end(),
                    [&](const ClampedExponent& f1, const ClampedExponent& f2){
                        (maximum(f1,Tlo,Thi)-minimum(f1,Tlo,Thi)) < (maximum(f2,Tlo,Thi)-minimum(f2,Tlo,Thi));
                    });
                float least_useful_Texponent_error = maximum(least_useful_Texponent, plo,phi) - minimum(least_useful_Texponent, Tlo,Thi);
                auto least_useful_Tsigmoid = std::min_element(
                    restricted.Tsigmoids.begin(), restricted.Tsigmoids.end(),
                    [&](const ClampedExponent& f1, const ClampedExponent& f2){
                        (maximum(f1,Tlo,Thi)-minimum(f1,Tlo,Thi)) < (maximum(f2,Tlo,Thi)-minimum(f2,Tlo,Thi));
                    });
                float least_useful_Tsigmoid_error = maximum(least_useful_Tsigmoid, plo,phi) - minimum(least_useful_Tsigmoid, Tlo,Thi);
                auto least_useful_Tdippr102 = std::min_element(
                    restricted.Tdippr102s.begin(), restricted.Tdippr102s.end(),
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
                        restricted.pexponents.erase(least_useful_pexponent);
                        continue;
                    }
                    if (std::abs(least_useful_Texponent_error - least_useful_term_error) < float_precision_of_error)
                    {
                        restricted.intercept += (maximum(least_useful_Texponent, plo,phi) - minimum(least_useful_Texponent, Tlo,Thi)) / 2.0;
                        restricted.known_max_fractional_error += least_useful_Texponent_error;
                        restricted.Texponents.erase(least_useful_Texponent);
                        continue;
                    }
                    if (std::abs(least_useful_Tsigmoid_error - least_useful_term_error) < float_precision_of_error)
                    {
                        restricted.intercept += (maximum(least_useful_Tsigmoid, plo,phi) - minimum(least_useful_Tsigmoid, Tlo,Thi)) / 2.0;
                        restricted.known_max_fractional_error += least_useful_Tsigmoid_error;
                        restricted.Tsigmoids.erase(least_useful_Tsigmoid);
                        continue;
                    }
                    if (std::abs(least_useful_Tdippr102_error - least_useful_term_error) < float_precision_of_error)
                    {
                        restricted.intercept += (maximum(least_useful_Tdippr102, plo,phi) - minimum(least_useful_Tdippr102, Tlo,Thi)) / 2.0;
                        restricted.known_max_fractional_error += least_useful_Tdippr102_error;
                        restricted.Tdippr102s.erase(least_useful_Tdippr102);
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
            for (std::size_t i = 0; i < pexponents.size(); ++i)
            {
                pexponents[i] *= scalar;
            }
            for (std::size_t i = 0; i < Texponents.size(); ++i)
            {
                Texponents[i] *= scalar;
            }
            for (std::size_t i = 0; i < Tdippr102s.size(); ++i)
            {
                Tdippr102s[i] *= scalar;
            }
            for (std::size_t i = 0; i < Tsigmoids.size(); ++i)
            {
                Tsigmoids[i] *= scalar;
            }
            intercept *= scalar;
            return *this;
        }

        GasPropertyStateRelation<Ty> operator/=(const float scalar)
        {
            for (std::size_t i = 0; i < pexponents.size(); ++i)
            {
                pexponents[i] /= scalar;
            }
            for (std::size_t i = 0; i < Texponents.size(); ++i)
            {
                Texponents[i] /= scalar;
            }
            for (std::size_t i = 0; i < Tdippr102s.size(); ++i)
            {
                Tdippr102s[i] /= scalar;
            }
            for (std::size_t i = 0; i < Tsigmoids.size(); ++i)
            {
                Tsigmoids[i] /= scalar;
            }
            intercept /= scalar;
            return *this;
        }

        GasPropertyStateRelation<Ty>& operator+=(const GasPropertyStateRelation<Ty> other)
        {
            const float pscale = float(other.punits / punits);
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            for (std::size_t i = 0; i < other.pexponents.size(); ++i)
            {
                pexponents.push_back(yscale * compose(other.pexponents[i], math::Scaling(pscale)));
            }
            for (std::size_t i = 0; i < other.Texponents.size(); ++i)
            {
                Texponents.push_back(yscale * compose(other.Texponents[i], math::Scaling(Tscale)));
            }
            for (std::size_t i = 0; i < other.Tdippr102s.size(); ++i)
            {
                Tdippr102s.push_back(yscale * compose(other.Tdippr102s[i], math::Scaling(Tscale)));
            }
            for (std::size_t i = 0; i < other.Tsigmoids.size(); ++i)
            {
                Tsigmoids.push_back(yscale * compose(other.Tsigmoids[i], math::Scaling(Tscale)));
            }
            intercept += yscale * other.intercept;
            return *this;
        }

        GasPropertyStateRelation<Ty>& operator-=(const GasPropertyStateRelation<Ty> other)
        {
            const float pscale = float(other.punits / punits);
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = -float(other.yunits / yunits);
            for (std::size_t i = 0; i < other.pexponents.size(); ++i)
            {
                pexponents.push_back(yscale * compose(other.pexponents[i], math::Scaling(pscale)));
            }
            for (std::size_t i = 0; i < other.Texponents.size(); ++i)
            {
                Texponents.push_back(yscale * compose(other.Texponents[i], math::Scaling(Tscale)));
            }
            for (std::size_t i = 0; i < other.Tdippr102s.size(); ++i)
            {
                Tdippr102s.push_back(yscale * compose(other.Tdippr102s[i], math::Scaling(Tscale)));
            }
            for (std::size_t i = 0; i < other.Tsigmoids.size(); ++i)
            {
                Tsigmoids.push_back(yscale * compose(other.Tsigmoids[i], math::Scaling(Tscale)));
            }
            intercept += yscale * other.intercept;
            return *this;
        }

    };

    // Custom parameterization, offers high accuracy for most pure compounds
    // 36 uses, for dynamic_viscosity, isobaric_specific_heat_capacity, and thermal_conductivity of gases
    template<typename Ty>
    constexpr GasPropertyStateRelation<Ty> get_sigmoid_exponent_pressure_temperature_relation(
        const si::temperature Tunits, const si::pressure punits, const Ty yunits,
        const float pslope, const float pexponent,
        const float Tslope, const float Texponent,
        const float Tsigmoid_max, const float Tsigmoid_scale, const float Tsigmoid_center,
        const float intercept,
        const float Tmin, const float Tmax,
        const float pmin, const float pmax,
        const float known_max_fractional_error
    ) {
        return GasPropertyStateRelation<Ty>(
            std::vector<ClampedExponent>{ClampedExponent(pmin, pmax, Exponent(pslope, pexponent))},
            std::vector<ClampedExponent>{ClampedExponent(Tmin, Tmax, Exponent(Tslope, Texponent))},
            std::vector<ClampedSigmoid>{ClampedSigmoid(Tmin, Tmax, Sigmoid(1.0f/Tsigmoid_scale, -Tsigmoid_center/Tsigmoid_scale, Tsigmoid_max))},
            std::vector<ClampedDippr102>(),

            punits, Tunits, yunits,

            intercept,
            known_max_fractional_error
        );
    }

    template<typename Ty>
    constexpr GasPropertyStateRelation<Ty> get_exponent_pressure_temperature_relation(
        const si::temperature Tunits, const si::pressure punits, const Ty yunits,
        const float pslope, const float pexponent,
        const float Tslope, const float Texponent,
        const float intercept,
        const float Tmin, const float Tmax,
        const float pmin, const float pmax,
        const float known_max_fractional_error
    ) {
        return GasPropertyStateRelation<Ty>(
            std::vector<ClampedExponent>{ClampedExponent(pmin, pmax, Exponent(pslope, pexponent))},
            std::vector<ClampedExponent>{ClampedExponent(Tmin, Tmax, Exponent(Tslope, Texponent))},
            std::vector<ClampedSigmoid>{},
            std::vector<ClampedDippr102>(),

            punits, Tunits, yunits,

            intercept,
            known_max_fractional_error
        );
    }

    // Direct port from the DIPPR 102 equation
    // 9 uses, for viscosity and thermal conductivity of gas
    template<typename Ty>
    constexpr GasPropertyStateRelation<Ty> get_dippr_temperature_relation_102(
        const si::temperature Tunits, const Ty yunits,
        const float c1, const float c2, const float c3, const float c4,
        const float Tmin, const float Tmax
    ) {
        return GasPropertyStateRelation<Ty>(
            std::vector<ClampedExponent>(0),
            std::vector<ClampedExponent>(0),
            std::vector<ClampedSigmoid>(0),
            std::vector<ClampedDippr102>{ClampedDippr102(Tmin, Tmax, Dippr102(c1, c2, c3, c4))},

            si::pressure(1.0), Tunits, yunits,

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
    over the ranges defined by [pmin,pmax] and [Tmin,Tmax]. 
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
        const si::temperature Tunits, const si::pressure punits, const Ty yunits,
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
        auto fp   = math::quadratic_newton_polynomial([&](float p) -> float{ return f(p*punits,   Tmid*Tunits) / yunits; }, pmin, pmid, pmax);
        auto fT   = math::quadratic_newton_polynomial([&](float T) -> float{ return f(pmin*punits,T*Tunits   ) / yunits; }, Tmin, Tmid, Tmax);
        auto fhat = GasPropertyStateRelation<Ty>(
            std::vector<ClampedExponent>{
                ClampedExponent(pmin, pmax, Exponent(fp[1], 1.0f)),
                ClampedExponent(pmin, pmax, Exponent(fp[2], 2.0f))
            },
            std::vector<ClampedExponent>{
                ClampedExponent(Tmin, Tmax, Exponent(fT[1], 1.0f)),
                ClampedExponent(Tmin, Tmax, Exponent(fT[2], 2.0f))
            },
            std::vector<ClampedSigmoid>(),
            std::vector<ClampedDippr102>(),

            si::pressure(1.0), Tunits, yunits,

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
        float Tworst = math::maximum([&](float T){ return fractional_error(pmid,T);   }, Tmin, Tmax, 10);
        float pworst = math::maximum([&](float p){ return fractional_error(p,Tworst); }, pmin, pmax, 10);
        fhat.known_max_fractional_error += fractional_error(pworst, Tworst);
        return fhat;
    }

    // Direct port from the Perry equation used for some gas properties
    // 1 use, for heat capacity of nitric oxide
    template<typename Ty>
    constexpr GasPropertyStateRelation<Ty> get_perry_temperature_gas_relation(
        const si::temperature Tunits, const Ty yunits,
        const float intercept, const float linear, const float inverse_square, const float square,
        const float Tmin, const float Tmax
    ) {
        return GasPropertyStateRelation<Ty>(
            std::vector<ClampedExponent>(),
            std::vector<ClampedExponent>{
                ClampedExponent(Tmin, Tmax, Exponent(linear, 1.0f)),
                ClampedExponent(Tmin, Tmax, Exponent(inverse_square, -2.0f)),
                ClampedExponent(Tmin, Tmax, Exponent(square, 2.0f))
            },
            std::vector<ClampedSigmoid>(),
            std::vector<ClampedDippr102>(),

            si::pressure(1.0), Tunits, yunits,

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
}}

