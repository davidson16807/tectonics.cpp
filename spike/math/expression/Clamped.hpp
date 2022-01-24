#pragma once

// standard libraries
#include <algorithm>

// in-house libraries
#include <math/expression/Scaling.hpp>
#include <math/expression/Shifting.hpp>
#include "Identity.hpp"

namespace math {

    /* 
    `Clamped<F>` is a trivial class that represents the function f(x)=F(min(max(x,a),b)), 
    where F is a template function object.
    It was needed to provide a reoccuring pattern in function approximation, 
    where input to a function is clamped to a valid range to prevent the function from generating absurd output.
    */
    template<typename F>
    struct Clamped {
        F f;
        float lo;
        float hi;
        constexpr explicit Clamped<F>(const float lo, const float hi, const F f):
            f(f),
            lo(lo),
            hi(hi)
        {}
        constexpr Clamped<F>(const Clamped<F>& fclamp):
            f(fclamp.f),
            lo(fclamp.lo),
            hi(fclamp.hi)
        {}
        constexpr Clamped<F>():
            f(),
            lo(),
            hi()
        {}
        constexpr float operator()(const float x) const
        {
            return f(std::clamp(x,lo,hi));
        }

        Clamped<F>& operator+=(const float k)
        {
            f += k;
            return *this;
        }

        Clamped<F>& operator-=(const float k)
        {
            f -= k;
            return *this;
        }

        Clamped<F>& operator*=(const float k)
        {
            f *= k;
            return *this;
        }

        Clamped<F>& operator/=(const float k)
        {
            f /= k;
            return *this;
        }
    };

    // operators with reals that are closed under Clamped<F> relations
    template<typename F>
    constexpr Clamped<F> operator+(const Clamped<F>& f, const float k)
    {
        Clamped<F> y(f);
        y.f += f;
        return y;
    }
    template<typename F>
    constexpr Clamped<F> operator+(const float k, const Clamped<F>& f)
    {
        Clamped<F> y(f);
        y.f += f;
        return y;
    }
    template<typename F>
    constexpr Clamped<F> operator-(const Clamped<F>& f, const float k)
    {
        Clamped<F> y(f);
        y.f -= f;
        return y;
    }
    template<typename F>
    constexpr Clamped<F> operator*(const Clamped<F>& f, const float k)
    {
        Clamped<F> y(f);
        y.f *= f;
        return y;
    }
    template<typename F>
    constexpr Clamped<F> operator*(const float k, const Clamped<F>& f)
    {
        Clamped<F> y(f);
        y.f *= f;
        return y;
    }
    template<typename F>
    constexpr Clamped<F> operator/(const Clamped<F>& f, const float k)
    {
        Clamped<F> y(f);
        y.f /= f;
        return y;
    }

    /*
    Given functions f∘clamp and g, return the composite function f∘clamp∘g.
    If reclamp==true, adjust the bounds of clamp so that the range of possible input fed to f remains the same.
    */
    template<typename F>
    constexpr Clamped<F> compose(const Clamped<F>& fclamp, const Shifting g, const bool reclamp = true)
    {
        Clamped<F> fclampg = fclamp;
        fclampg.f = compose(fclamp.f,g);
        if (reclamp)
        {
            fclampg.lo += g.offset;
            fclampg.hi += g.offset;
        }
        return fclampg;
    }

    /*
    Given functions f∘clamp and g, return the composite function f∘clamp∘g.
    If reclamp==true, adjust the bounds of clamp so that the range of possible input fed to f remains the same.
    */
    template<typename F>
    constexpr Clamped<F> compose(const Clamped<F>& fclamp, const Scaling g, const bool reclamp = true)
    {
        Clamped<F> fclampg = fclamp;
        fclampg.f = compose(fclamp.f,g);
        if (reclamp)
        {
            fclampg.lo *= g.factor;
            fclampg.hi *= g.factor;
        }
        return fclampg;
    }

    template<typename F>
    constexpr float maximum(const Clamped<F>& fclamp, const float lo, const float hi)
    {
        return maximum(fclamp.f, std::max(lo, fclamp.lo), std::min(hi, fclamp.hi));
    }
    template<typename F>
    constexpr float minimum(const Clamped<F>& fclamp, const float lo, const float hi)
    {
        return minimum(fclamp.f, std::max(lo, fclamp.lo), std::min(hi, fclamp.hi));
    }
    template<typename F>
    constexpr auto restriction(const Clamped<F>& fclamp, const float lo, const float hi)
    {
        return restriction(fclamp.f, std::max(lo, fclamp.lo), std::min(hi, fclamp.hi));
    }
    template<typename F>
    constexpr auto integral(const Clamped<F>& fclamp, const float lo, const float hi)
    {
        return integral(fclamp.f, std::max(lo, fclamp.lo), std::min(hi, fclamp.hi));
    }
    template<typename F>
    constexpr auto derivative(const Clamped<F>& fclamp, const float lo, const float hi)
    {
        return derivative(fclamp.f, std::max(lo, fclamp.lo), std::min(hi, fclamp.hi));
    }

    template<typename F>
    constexpr float maximum(const Clamped<F>& fclamp)
    {
        return maximum(fclamp.f, fclamp.lo, fclamp.hi);
    }
    template<typename F>
    constexpr float minimum(const Clamped<F>& fclamp)
    {
        return minimum(fclamp.f, fclamp.lo, fclamp.hi);
    }
    template<typename F>
    constexpr auto restriction(const Clamped<F>& fclamp)
    {
        return restriction(fclamp.f, fclamp.lo, fclamp.hi);
    }
    template<typename F>
    constexpr auto integral(const Clamped<F>& fclamp)
    {
        return integral(fclamp.f, fclamp.lo, fclamp.hi);
    }
    template<typename F>
    constexpr auto derivative(const Clamped<F>& fclamp)
    {
        return derivative(fclamp.f, fclamp.lo, fclamp.hi);
    }

}