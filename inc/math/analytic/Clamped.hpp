#pragma once

// standard libraries
#include <algorithm> // max
#include <limits>    // numeric_limits

// in-house libraries
#include "Scaling.hpp"
#include "Shifting.hpp"
#include "Identity.hpp"

namespace analytic {

    /* 
    `Clamped<T,F>` is a trivial class that represents the function f(x)=F(min(max(x,a),b)), 
    where F is a template function object.
    It was needed to provide a reoccuring pattern in function approximation, 
    where input to a function is clamped to a valid range to prevent the function from generating absurd output.
    */
    template<typename T, typename F>
    struct Clamped {
        using value_type = T;

        F f;
        T lo;
        T hi;
        constexpr explicit Clamped<T,F>(const T lo, const T hi, const F f):
            f(f),
            lo(lo),
            hi(hi)
        {}
        constexpr Clamped<T,F>(const Clamped<T,F>& fclamp):
            f(fclamp.f),
            lo(fclamp.lo),
            hi(fclamp.hi)
        {}
        // zero constructor
        constexpr Clamped<T,F>():
            f(),
            lo(-std::numeric_limits<T>::max()),
            hi( std::numeric_limits<T>::max())
        {}
        // constant constructor
        constexpr Clamped<T,F>(const T k):
            f(k),
            lo(-std::numeric_limits<T>::max()),
            hi( std::numeric_limits<T>::max())
        {}
        // unclamped constructor
        constexpr Clamped<T,F>(const F& f):
            f(f),
            lo(-std::numeric_limits<T>::max()),
            hi( std::numeric_limits<T>::max())
        {}
        constexpr T operator()(const T x) const
        {
            return f(std::clamp(x,lo,hi));
        }

        Clamped<T,F>& operator+=(const T k)
        {
            f += k;
            return *this;
        }

        Clamped<T,F>& operator-=(const T k)
        {
            f -= k;
            return *this;
        }

        Clamped<T,F>& operator*=(const T k)
        {
            f *= k;
            return *this;
        }

        Clamped<T,F>& operator/=(const T k)
        {
            f /= k;
            return *this;
        }
    };

    constexpr auto compose(const T lo, const T hi, const F f)
    {
        return Clamped<T,F>(lo, hi, f);
    }

    // operators with reals that are closed under Clamped<T,F> relations
    template<typename T, typename F>
    constexpr Clamped<T,F> operator+(const Clamped<T,F>& f, const T k)
    {
        Clamped<T,F> y(f);
        y.f += k;
        return y;
    }
    template<typename T, typename F>
    constexpr Clamped<T,F> operator+(const T k, const Clamped<T,F>& f)
    {
        Clamped<T,F> y(f);
        y.f += k;
        return y;
    }
    template<typename T, typename F>
    constexpr Clamped<T,F> operator-(const Clamped<T,F>& f, const T k)
    {
        Clamped<T,F> y(f);
        y.f -= k;
        return y;
    }
    template<typename T, typename F>
    constexpr Clamped<T,F> operator*(const Clamped<T,F>& f, const T k)
    {
        Clamped<T,F> y(f);
        y.f *= k;
        return y;
    }
    template<typename T, typename F>
    constexpr Clamped<T,F> operator*(const T k, const Clamped<T,F>& f)
    {
        Clamped<T,F> y(f);
        y.f *= k;
        return y;
    }
    template<typename T, typename F>
    constexpr Clamped<T,F> operator/(const Clamped<T,F>& f, const T k)
    {
        Clamped<T,F> y(f);
        y.f /= k;
        return y;
    }
    template<typename T, typename F>
    constexpr Clamped<T,F> operator-(const Clamped<T,F>& f)
    {
        Clamped<T,F> y(f);
        y.f *= T(-1);
        return y;
    }

    /*
    Given functions f∘clamp and g, return the composite function f∘clamp∘g.
    */
    template<typename T, typename F>
    constexpr Clamped<T,F> compose(const Clamped<T,F>& fclamp, const Shifting<T> g)
    {
        Clamped<T,F> fclampg = fclamp;
        fclampg.f = compose(fclamp.f,g);
        fclampg.lo -= g.offset;
        fclampg.hi -= g.offset;
        return fclampg;
    }

    /*
    Given functions f∘clamp and g, return the composite function f∘clamp∘g.
    */
    template<typename T, typename F>
    constexpr Clamped<T,F> compose(const Clamped<T,F>& fclamp, const Scaling<T> g)
    {
        Clamped<T,F> fclampg = fclamp;
        fclampg.f = compose(fclamp.f,g);
        fclampg.lo /= g.factor;
        fclampg.hi /= g.factor;
        if (fclampg.hi < fclampg.lo) std::swap(fclampg.lo, fclampg.hi);
        return fclampg;
    }

    /*
    Given functions f∘clamp and h, return the composite function h∘f∘clamp.
    */
    template<typename T, typename F, typename H>
    constexpr auto compose(const H h, const Clamped<T,F>& fclamp)
    {
        return clamped(fclamp.lo, fclamp.hi, compose(h, fclamp.f));
    }


    template<typename T, typename F>
    constexpr T maximum(const Clamped<T,F>& fclamp, const T lo, const T hi)
    {
        return maximum(fclamp.f, std::max(lo, fclamp.lo), std::min(hi, fclamp.hi));
    }
    template<typename T, typename F>
    constexpr T minimum(const Clamped<T,F>& fclamp, const T lo, const T hi)
    {
        return minimum(fclamp.f, std::max(lo, fclamp.lo), std::min(hi, fclamp.hi));
    }
    template<typename T, typename F>
    constexpr auto restriction(const Clamped<T,F>& fclamp, const T lo, const T hi)
    {
        return restriction(fclamp.f, std::max(lo, fclamp.lo), std::min(hi, fclamp.hi));
    }
    template<typename T, typename F>
    constexpr auto integral(const Clamped<T,F>& fclamp, const T lo, const T hi)
    {
        return integral(fclamp.f, std::max(lo, fclamp.lo), std::min(hi, fclamp.hi));
    }
    template<typename T, typename F>
    constexpr auto derivative(const Clamped<T,F>& fclamp, const T lo, const T hi)
    {
        return derivative(fclamp.f, std::max(lo, fclamp.lo), std::min(hi, fclamp.hi));
    }

    template<typename T, typename F>
    constexpr T maximum(const Clamped<T,F>& fclamp)
    {
        return maximum(fclamp.f, fclamp.lo, fclamp.hi);
    }
    template<typename T, typename F>
    constexpr T minimum(const Clamped<T,F>& fclamp)
    {
        return minimum(fclamp.f, fclamp.lo, fclamp.hi);
    }
    template<typename T, typename F>
    constexpr auto restriction(const Clamped<T,F>& fclamp)
    {
        return restriction(fclamp.f, fclamp.lo, fclamp.hi);
    }
    template<typename T, typename F>
    constexpr auto integral(const Clamped<T,F>& fclamp)
    {
        return integral(fclamp.f, fclamp.lo, fclamp.hi);
    }
    template<typename T, typename F>
    constexpr auto derivative(const Clamped<T,F>& fclamp)
    {
        return derivative(fclamp.f, fclamp.lo, fclamp.hi);
    }


}