#pragma once

// C libraries
#include <cmath>

// std libraries
#include <string>
#include <ostream>
#include <type_traits>

namespace analytic {

    /* 
    `Identity` is a trivial class that represents the function f(x)=x,
    oddly enough, it was useful for something, so we implemented it.
    (it was useful as the base case for the recursive implementation of `compose()` for polynomials)
    */
    template<typename T>
    struct Identity {
        using value_type = T;

        constexpr explicit Identity()
        {}
        constexpr T operator()(const T x) const
        {
            return x;
        }
    };
    
    template<typename T>
    constexpr std::string to_string(const Identity<T>& p)
    {
        return "x";
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Identity<T>& p) { 
        os << to_string(p);
        return os;
    }

    // operators that cause cancelation
    template<typename T>
    constexpr T operator-(const Identity<T>, const Identity<T>)
    {
        return T(0);
    }

    template<typename T>
    constexpr T operator/(const Identity<T>, const Identity<T>)
    {
        return T(1);
    }

    template<typename T, typename F> constexpr F compose(const Identity<T> e, const F& f) { return f;    }
    template<typename T, typename F> constexpr F compose(const F& f, const Identity<T> e) { return f;    }
    template<typename F>             constexpr double compose(const double k, const F& f) { return k;    }
    template<typename F>             constexpr auto   compose(const F& f, const double k) { return f(k); }
    template<typename F>             constexpr float  compose(const float k, const F& f)  { return k;    }
    template<typename F>             constexpr auto   compose(const F& f, const float k)  { return f(k); }
    template<typename F>             constexpr int    compose(const int k, const F& f)    { return k;    }
    template<typename F>             constexpr auto   compose(const F& f, const int k)    { return f(k); }

    template<typename T>
    constexpr T distance(const Identity<T> a, const Identity<T> b, const T lo, const T hi){ return T(0); }
    
    template<typename T>
    constexpr T distance(const T a, const T b, const T lo, const T hi){ return std::abs(a-b); }

    constexpr double derivative(const double k) { return double(0); }
    constexpr float derivative(const float k) { return float(0); }
    constexpr int derivative(const int k) { return int(0); }
    template<typename T> constexpr T derivative(const Identity<T> e) { return T(1); }

    template<int N, typename F, typename = std::enable_if_t<(N==0)>>
    constexpr F higher_order_derivative(const F& f)
    {
        return f;
    }

    template<int N, typename F, typename = std::enable_if_t<(N>0)>>
    constexpr auto higher_order_derivative(const F& f)
    {
        return higher_order_derivative<N-1>(derivative(f));
    }

}

