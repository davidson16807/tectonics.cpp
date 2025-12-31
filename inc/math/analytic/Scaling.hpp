#pragma once

// std libraries
#include <string>
#include <ostream>

#include "Identity.hpp"

namespace analytic {

    /* 
    `Scaling` is a trivial class that represents the function f(x)=ax,
    it was needed to provide an idiomatic way to request that input to other functions be scaled using compose() - 
    some functions may only be closed under composition with scalar multiplication and division,
    so there was need for a function that restricted users to these kinds of operations.
    Since having implemented it though, it has found other uses.
    */
    template<typename T>
    struct Scaling {
        using value_type = T;

        T factor;
        constexpr explicit Scaling(const T factor):
            factor(factor)
        {}
        constexpr explicit Scaling(const Identity<T> e):
            factor(T(1.0))
        {}
        constexpr T operator()(const T x) const
        {
            return factor*x;
        }

        constexpr Scaling<T>& operator*=(const T k)
        {
            factor *= k;
            return *this;
        }

        constexpr Scaling<T>& operator/=(const T k)
        {
            factor /= k;
            return *this;
        }

        constexpr Scaling<T>& operator+=(const Scaling<T> f)
        {
            factor += f.factor;
            return *this;
        }

        constexpr Scaling<T>& operator-=(const Scaling<T> f)
        {
            factor -= f.factor;
            return *this;
        }

    };

    template<typename T>
    constexpr std::string to_string(const Scaling<T>& f)
    {
        // const std::string exponents("⁰¹²³⁴⁵⁶⁷⁸⁹");
        return std::to_string(f.factor) + "*x";
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Scaling<T>& f) { 
        os << to_string(f);
        return os;
    }

    // operators that are closed under Scaling relations
    template<typename T>
    constexpr Scaling<T> operator+(const Scaling<T> f, const Scaling<T> g)
    {
        Scaling y = f;
        y += g;
        return y;
    }
    template<typename T>
    constexpr Scaling<T> operator-(const Scaling<T> f, const Scaling<T> g)
    {
        Scaling y = f;
        y -= g;
        return y;
    }
    template<typename T>
    constexpr Scaling<T> operator-(const Scaling<T> f)
    {
        return Scaling<T>(-f.factor);
    }

    // operators with reals that are closed under Scaling relations
    template<typename T>
    constexpr Scaling<T> operator*(const Scaling<T> f, const T k)
    {
        return Scaling<T>(f.factor * k);
    }
    template<typename T>
    constexpr Scaling<T> operator*(const T k, const Scaling<T> f)
    {
        return Scaling<T>(k * f.factor);
    }
    template<typename T>
    constexpr Scaling<T> operator/(const Scaling<T> f, const T k)
    {
        return Scaling<T>(f.factor / k);
    }

    // operators that produce Scaling relations when given other relations as input
    template<typename T>
    constexpr Scaling<T> operator*(const Identity<T> f, const T k)
    {
        return Scaling<T>(k);
    }
    template<typename T>
    constexpr Scaling<T> operator*(const T k, const Identity<T> f)
    {
        return Scaling<T>(k);
    }
    template<typename T>
    constexpr Scaling<T> operator/(const Identity<T> f, const T k)
    {
        return Scaling<T>(T(1) / k);
    }
    template<typename T>
    constexpr Scaling<T> operator-(const Identity<T> e)
    {
        return Scaling<T>(T(-1));
    }

    template<typename T>
    constexpr auto operator+(const Identity<T> e, const Scaling<T> f)
    {
        return Scaling<T>(f.factor+T(1));
    }
    template<typename T>
    constexpr auto operator+(const Scaling<T> f, const Identity<T> e)
    {
        return Scaling<T>(f.factor+T(1));
    }
    template<typename T>
    constexpr auto operator-(const Identity<T> e, const Scaling<T> f)
    {
        return Scaling<T>(e) - Scaling<T>(f);
    }
    template<typename T>
    constexpr auto operator-(const Scaling<T> f, const Identity<T> e)
    {
        return Scaling<T>(f) - Scaling<T>(e);
    }

    // operators that cause cancelation
    template<typename T>
    constexpr T operator/(const Scaling<T> f, const Scaling<T> g)
    {
        return f.factor/g.factor;
    }
    template<typename T>
    constexpr T operator/(const Identity<T> e, const Scaling<T> f)
    {
        return T(1)/f.factor;
    }
    template<typename T>
    constexpr T operator/(const Scaling<T> f, const Identity<T> e)
    {
        return f.factor;
    }



    template<typename T, typename Expression>
    constexpr auto compose(const Scaling<T> f, const Expression& g)
    {
        return g*f.factor;
    }

    template<typename T>
    constexpr auto compose(const Scaling<T> f, const Identity<T>& g)
    {
        return g*f.factor;
    }

    template<typename T> constexpr T compose(const Scaling<T>& f, const int k)    { return f(k); }
    template<typename T> constexpr T compose(const Scaling<T>& f, const float k)  { return f(k); }
    template<typename T> constexpr T compose(const Scaling<T>& f, const double k) { return f(k); }


    template<typename T>
    constexpr Scaling<T> inverse(const Scaling<T> f) 
    {
        return Scaling<T>(T(1)/f.factor);
    }


    template<typename T>
    constexpr T derivative(const Scaling<T> f) 
    {
        return f.factor;
    }

    template<typename T>
    constexpr Scaling<T> integral(const T k) 
    {
        return Scaling<T>(k);
    }

}

