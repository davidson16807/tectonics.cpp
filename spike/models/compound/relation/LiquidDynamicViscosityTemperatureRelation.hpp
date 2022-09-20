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


    struct Dippr105 {
        float c1;
        float c2;
        float c3;
        float c4;
        constexpr explicit Dippr105(
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
        constexpr explicit Dippr105():
            c1(0.0f),
            c2(1.0f),
            c3(0.0f),
            c4(0.0f)
        {}
        constexpr Dippr105(const Dippr105& f):
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
        constexpr Dippr105& operator*=(const float scalar)
        {
            c1 *= scalar;
            return *this;
        }
        constexpr Dippr105& operator/=(const float scalar)
        {
            c1 /= scalar;
            return *this;
        }
    };
    constexpr Dippr105 operator*(const Dippr105 relation, const float scalar)
    {
        return Dippr105(relation.c1 * scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr105 operator*(const float scalar, const Dippr105 relation)
    {
        return Dippr105(relation.c1 * scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr105 operator/(const Dippr105 relation, const float scalar)
    {
        return Dippr105(relation.c1 / scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr105 operator-(const Dippr105 relation)
    {
        return Dippr105(relation.c1 * -1.0f, relation.c2, relation.c3, relation.c4);
    }


    constexpr Dippr105 compose(Dippr105 f, const math::Scaling<float> g)
    {
        return Dippr105(
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





    struct Dippr119 {
        float c1;
        float c2;
        float c3;
        float c4;
        constexpr explicit Dippr119(
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
        constexpr explicit Dippr119():
            c1(0.0f),
            c2(1.0f),
            c3(0.0f),
            c4(0.0f)
        {}
        constexpr Dippr119(const Dippr119& f):
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
        constexpr Dippr119& operator*=(const float scalar)
        {
            c1 *= scalar;
            return *this;
        }
        constexpr Dippr119& operator/=(const float scalar)
        {
            c1 /= scalar;
            return *this;
        }
    };
    constexpr Dippr119 operator*(const Dippr119 relation, const float scalar)
    {
        return Dippr119(relation.c1 * scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr119 operator*(const float scalar, const Dippr119 relation)
    {
        return Dippr119(relation.c1 * scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr119 operator/(const Dippr119 relation, const float scalar)
    {
        return Dippr119(relation.c1 / scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr119 operator-(const Dippr119 relation)
    {
        return Dippr119(relation.c1 * -1.0f, relation.c2, relation.c3, relation.c4);
    }













    using ClampedDippr105 = math::Clamped<float,Dippr105>;
    using ClampedDippr119 = math::Clamped<float,Dippr119>;


    /*
    `LiquidDynamicViscosityTemperatureRelation` consolidates many kinds of expressions
    that are commonly used to represent the relation between temperature and the density of liquids
    */
    class LiquidDynamicViscosityTemperatureRelation
    {
        std::vector<ClampedDippr105> dippr105s;
        std::vector<ClampedDippr119> dippr119s;

        si::temperature<double> Tunits;
        si::density<double>     yunits;

        float intercept;

    public:
        float known_max_fractional_error;

        // zero constructor
        constexpr LiquidDynamicViscosityTemperatureRelation():
            dippr105s(),
            dippr119s(),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        constexpr LiquidDynamicViscosityTemperatureRelation(
            const std::vector<ClampedDippr105> dippr105s,
            const std::vector<ClampedDippr119> dippr119s,

            const si::temperature<double> Tunits,
            const si::density<double> yunits,

            const float intercept,
            const float known_max_fractional_error
        ):
            dippr105s(dippr105s),
            dippr119s(dippr119s),

            Tunits(Tunits),
            yunits(yunits),

            intercept(intercept),
            known_max_fractional_error(known_max_fractional_error)
        {
        }

        constexpr LiquidDynamicViscosityTemperatureRelation(const si::density<double> intercept):
            dippr105s(0),
            dippr119s(0),

            Tunits(si::standard_temperature),
            yunits(si::density<double>(1.0)),

            intercept(intercept/si::density<double>(1.0)),
            known_max_fractional_error(0.0)
        {
        }

        constexpr LiquidDynamicViscosityTemperatureRelation<si::density<double>>& operator=(const LiquidDynamicViscosityTemperatureRelation<si::density<double>> other)
        {
            pexponents = other.pexponents;
            Texponents = other.Texponents;
            Tsigmoids  = other.Tsigmoids;
            dippr105s = other.dippr105s;

            Tunits = other.Tunits;
            yunits = other.yunits;

            intercept = other.intercept;
            known_max_fractional_error = other.known_max_fractional_error;

            return *this;
        }

        si::density<double> operator()(const si::temperature<double> temperature) const
        {
            const float T = float(temperature/Tunits);
            ClampedDippr105 dippr105;
            ClampedDippr119 dippr119;
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
            for (std::size_t i = 0; i < dippr105s.size(); ++i)
            {
                dippr105 = dippr105s[i];
                u = std::clamp(T, dippr105.lo, dippr105.hi);
                y += dippr105.f.c1 * std::pow(u, dippr105.f.c2) / (1.0f + dippr105.f.c3/u + dippr105.f.c4/(u*u));
            }

            // return intercept * yunits;
            return y * yunits;
        }

        LiquidDynamicViscosityTemperatureRelation<si::density<double>> restriction(
            const si::temperature<double> min_temperature, const si::temperature<double> max_temperature,
            const float known_max_fractional_error
        ) const
        {
            const float plo = float(min_pressure/punits);
            const float Tlo = float(min_temperature/Tunits);
            const float phi = float(max_pressure/punits);
            const float Thi = float(max_temperature/Tunits);
            LiquidDynamicViscosityTemperatureRelation<si::density<double>> restricted(*this);
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
                auto least_useful_Tdippr105 = std::min_element(
                    restricted.dippr105s.begin(), restricted.dippr105s.end(),
                    [&](const ClampedExponent& f1, const ClampedExponent& f2){
                        (maximum(f1,Tlo,Thi)-minimum(f1,Tlo,Thi)) < (maximum(f2,Tlo,Thi)-minimum(f2,Tlo,Thi));
                    });
                float least_useful_Tdippr105_error = maximum(least_useful_Tdippr105, plo,phi) - minimum(least_useful_Tdippr105, Tlo,Thi);

                least_useful_term_error = std::min(least_useful_pexponent_error, least_useful_Texponent_error);
                least_useful_term_error = std::min(least_useful_term_error,      least_useful_Tsigmoid_error );
                least_useful_term_error = std::min(least_useful_term_error,      least_useful_Tdippr105_error);

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
                    if (std::abs(least_useful_Tdippr105_error - least_useful_term_error) < float_precision_of_error)
                    {
                        restricted.intercept += (maximum(least_useful_Tdippr105, plo,phi) - minimum(least_useful_Tdippr105, Tlo,Thi)) / 2.0;
                        restricted.known_max_fractional_error += least_useful_Tdippr105_error;
                        restricted.dippr105s.erase(least_useful_Tdippr105);
                        continue;
                    }
                }
            }
            return restricted;
        }

        LiquidDynamicViscosityTemperatureRelation<si::density<double>>& operator+=(const si::density<double> offset)
        {
            intercept += offset/yunits;
            return *this;
        }

        LiquidDynamicViscosityTemperatureRelation<si::density<double>>& operator-=(const si::density<double> offset)
        {
            intercept -= offset/yunits;
            return *this;
        }

        LiquidDynamicViscosityTemperatureRelation<si::density<double>>& operator*=(const float scalar)
        {
            for (std::size_t i = 0; i < pexponents.size(); ++i)
            {
                pexponents[i] *= scalar;
            }
            for (std::size_t i = 0; i < Texponents.size(); ++i)
            {
                Texponents[i] *= scalar;
            }
            for (std::size_t i = 0; i < dippr105s.size(); ++i)
            {
                dippr105s[i] *= scalar;
            }
            for (std::size_t i = 0; i < Tsigmoids.size(); ++i)
            {
                Tsigmoids[i] *= scalar;
            }
            intercept *= scalar;
            return *this;
        }

        LiquidDynamicViscosityTemperatureRelation<si::density<double>> operator/=(const float scalar)
        {
            for (std::size_t i = 0; i < pexponents.size(); ++i)
            {
                pexponents[i] /= scalar;
            }
            for (std::size_t i = 0; i < Texponents.size(); ++i)
            {
                Texponents[i] /= scalar;
            }
            for (std::size_t i = 0; i < dippr105s.size(); ++i)
            {
                dippr105s[i] /= scalar;
            }
            for (std::size_t i = 0; i < Tsigmoids.size(); ++i)
            {
                Tsigmoids[i] /= scalar;
            }
            intercept /= scalar;
            return *this;
        }

        LiquidDynamicViscosityTemperatureRelation<si::density<double>>& operator+=(const LiquidDynamicViscosityTemperatureRelation<si::density<double>> other)
        {
            const float pscale = float(other.punits / punits);
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            for (std::size_t i = 0; i < other.pexponents.size(); ++i)
            {
                pexponents.push_back(yscale * compose(other.pexponents[i], math::Scaling<float>(pscale)));
            }
            for (std::size_t i = 0; i < other.Texponents.size(); ++i)
            {
                Texponents.push_back(yscale * compose(other.Texponents[i], math::Scaling<float>(Tscale)));
            }
            for (std::size_t i = 0; i < other.dippr105s.size(); ++i)
            {
                dippr105s.push_back(yscale * compose(other.dippr105s[i], math::Scaling<float>(Tscale)));
            }
            for (std::size_t i = 0; i < other.Tsigmoids.size(); ++i)
            {
                Tsigmoids.push_back(yscale * compose(other.Tsigmoids[i], math::Scaling<float>(Tscale)));
            }
            intercept += yscale * other.intercept;
            return *this;
        }

        LiquidDynamicViscosityTemperatureRelation<si::density<double>>& operator-=(const LiquidDynamicViscosityTemperatureRelation<si::density<double>> other)
        {
            const float pscale = float(other.punits / punits);
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = -float(other.yunits / yunits);
            for (std::size_t i = 0; i < other.pexponents.size(); ++i)
            {
                pexponents.push_back(yscale * compose(other.pexponents[i], math::Scaling<float>(pscale)));
            }
            for (std::size_t i = 0; i < other.Texponents.size(); ++i)
            {
                Texponents.push_back(yscale * compose(other.Texponents[i], math::Scaling<float>(Tscale)));
            }
            for (std::size_t i = 0; i < other.dippr105s.size(); ++i)
            {
                dippr105s.push_back(yscale * compose(other.dippr105s[i], math::Scaling<float>(Tscale)));
            }
            for (std::size_t i = 0; i < other.Tsigmoids.size(); ++i)
            {
                Tsigmoids.push_back(yscale * compose(other.Tsigmoids[i], math::Scaling<float>(Tscale)));
            }
            intercept += yscale * other.intercept;
            return *this;
        }

    };

    // Direct port from the DIPPR 102 equation
    // 9 uses, for viscosity and thermal conductivity of gas
    template<typename si::density<double>>
    constexpr LiquidDynamicViscosityTemperatureRelation<si::density<double>> get_dippr_temperature_relation_102(
        const si::temperature<double> Tunits, const si::density<double> yunits,
        const float c1, const float c2, const float c3, const float c4,
        const float Tmin, const float Tmax
    ) {
        return LiquidDynamicViscosityTemperatureRelation<si::density<double>>(
            std::vector<ClampedExponent>(0),
            std::vector<ClampedExponent>(0),
            std::vector<ClampedSigmoid>(0),
            std::vector<ClampedDippr105>{ClampedDippr105(Tmin, Tmax, Dippr105(c1, c2, c3, c4))},

            si::pressure<double>(1.0), Tunits, yunits,

            0.0, // intercept
            0.0 // known_max_fractional_error
        );
    }

    template<typename si::density<double>>
    LiquidDynamicViscosityTemperatureRelation<si::density<double>> operator+(const LiquidDynamicViscosityTemperatureRelation<si::density<double>> relation, const LiquidDynamicViscosityTemperatureRelation<si::density<double>> other)
    {
        LiquidDynamicViscosityTemperatureRelation<si::density<double>> result = relation;
        result += other;
        return result;
    }

    template<typename si::density<double>>
    LiquidDynamicViscosityTemperatureRelation<si::density<double>> operator-(const LiquidDynamicViscosityTemperatureRelation<si::density<double>> relation, const LiquidDynamicViscosityTemperatureRelation<si::density<double>> other)
    {
        LiquidDynamicViscosityTemperatureRelation<si::density<double>> result = relation;
        result -= other;
        return result;
    }

    template<typename si::density<double>>
    LiquidDynamicViscosityTemperatureRelation<si::density<double>> operator+(const LiquidDynamicViscosityTemperatureRelation<si::density<double>> relation, const si::density<double> offset)
    {
        LiquidDynamicViscosityTemperatureRelation<si::density<double>> result = relation;
        result += offset;
        return result;
    }

    template<typename si::density<double>>
    LiquidDynamicViscosityTemperatureRelation<si::density<double>> operator-(const LiquidDynamicViscosityTemperatureRelation<si::density<double>> relation, const si::density<double> offset)
    {
        LiquidDynamicViscosityTemperatureRelation<si::density<double>> result = relation;
        result -= offset;
        return result;
    }

    template<typename si::density<double>>
    LiquidDynamicViscosityTemperatureRelation<si::density<double>> operator*(const LiquidDynamicViscosityTemperatureRelation<si::density<double>> relation, const float scalar)
    {
        LiquidDynamicViscosityTemperatureRelation<si::density<double>> result = relation;
        result *= scalar;
        return result;
    }

    template<typename si::density<double>>
    LiquidDynamicViscosityTemperatureRelation<si::density<double>> operator/(const LiquidDynamicViscosityTemperatureRelation<si::density<double>> relation, const float scalar)
    {
        LiquidDynamicViscosityTemperatureRelation<si::density<double>> result = relation;
        result /= scalar;
        return result;
    }

    template<typename si::density<double>>
    LiquidDynamicViscosityTemperatureRelation<si::density<double>> operator+(const si::density<double> offset, const LiquidDynamicViscosityTemperatureRelation<si::density<double>> relation)
    {
        LiquidDynamicViscosityTemperatureRelation<si::density<double>> result = relation;
        result += offset;
        return result;
    }

    template<typename si::density<double>>
    LiquidDynamicViscosityTemperatureRelation<si::density<double>> operator-(const si::density<double> offset, const LiquidDynamicViscosityTemperatureRelation<si::density<double>> relation)
    {
        LiquidDynamicViscosityTemperatureRelation<si::density<double>> result = relation;
        result *= 1.0f;
        result += offset;
        return result;
    }

    template<typename si::density<double>>
    LiquidDynamicViscosityTemperatureRelation<si::density<double>> operator-(const LiquidDynamicViscosityTemperatureRelation<si::density<double>> relation)
    {
        LiquidDynamicViscosityTemperatureRelation<si::density<double>> result = relation;
        result *= 1.0f;
        return result;
    }

    template<typename si::density<double>>
    LiquidDynamicViscosityTemperatureRelation<si::density<double>> operator*(const float scalar, const LiquidDynamicViscosityTemperatureRelation<si::density<double>> relation)
    {
        LiquidDynamicViscosityTemperatureRelation<si::density<double>> result = relation;
        result *= scalar;
        return result;
    }

    /*
    `distance` is a distance metric that is designed to test whether two objects are equal 
    up to some threshold that the user defines to reflect the precision of underlying types.
    LiquidDynamicViscosityTemperatureRelation does not have a convenient closed form distance metric,
    so equality is determined pragmatically by sampling at given pressures/temperatures
    */
    // template<typename T1>
    // float distance(
    //     const LiquidDynamicViscosityTemperatureRelation<T1>& first, 
    //     const LiquidDynamicViscosityTemperatureRelation<T1>& second,
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

