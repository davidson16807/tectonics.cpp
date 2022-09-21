#pragma once

namespace compound {
namespace term {

    struct Exponent {
        float weight;
        float exponent;
        constexpr explicit Exponent(const float weight, const float exponent):
            weight(weight),
            exponent(exponent)
        {}
        // zero constructor
        constexpr explicit Exponent():
            weight(0.0f),
            exponent(1.0f)
        {}
        // constant constructor
        constexpr explicit Exponent(const float k):
            weight(k),
            exponent(0.0f)
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

    constexpr Exponent compose(const Exponent f, const math::Scaling<float> g)
    {
        return Exponent(
            f.weight*std::pow(g.factor, f.exponent),
            f.exponent
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

}}
