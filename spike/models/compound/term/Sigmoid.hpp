#pragma once

namespace compound {
namespace term {

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


    constexpr Sigmoid compose(const Sigmoid f, const math::Scaling<float> g)
    {
        return Sigmoid(
            f.xscale * g.factor,
            f.xoffset,
            f.ymax
        );
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


    constexpr float max_distance(const Sigmoid relation1, const Sigmoid relation2, const float lo, const float hi)
    {
        return std::max(
            std::abs(relation1(hi)-relation2(hi)),
            std::abs(relation1(lo)-relation2(lo))
        );
    }

}}
