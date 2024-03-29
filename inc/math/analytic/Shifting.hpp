#pragma once

#include "Identity.hpp"

namespace analytic {

    /* 
    `Shifting` is a trivial class that represents the function f(x)=ax,
    it was needed to provide an idiomatic way to request that input to other functions be scaled using compose() - 
    some functions may only be closed under composition with scalar addition and subtraction,
    so there was need for a function that restricted users to these kinds of operations.
    Since having implemented it though, it has found other uses.
    */
    template<typename T>
    struct Shifting {
        using value_type = T;

        T offset;
        constexpr explicit Shifting(const T offset):
            offset(offset)
        {}
        constexpr T operator()(const T x) const
        {
            return x+offset;
        }

        Shifting& operator+=(const T k)
        {
            offset += k;
            return *this;
        }

        Shifting& operator-=(const T k)
        {
            offset -= k;
            return *this;
        }
    };

    template<typename T>
    constexpr std::string to_string(const Shifting<T>& f)
    {
        // const std::string exponents("⁰¹²³⁴⁵⁶⁷⁸⁹");
        return std::string("x") + (f.offset>=0?"+":"") + std::to_string(f.offset);
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Shifting<T>& f) { 
        os << to_string(f);
        return os;
    }

    // operators with reals that are closed under Shifting relations
    template<typename T>
    constexpr Shifting<T> operator+(const Shifting<T> f, const T k)
    {
        return Shifting<T>(f.offset + k);
    }
    template<typename T>
    constexpr Shifting<T> operator+(const T k, const Shifting<T> f)
    {
        return Shifting<T>(f.offset + k);
    }
    template<typename T>
    constexpr Shifting<T> operator-(const Shifting<T> f, const T k)
    {
        return Shifting<T>(f.offset - k);
    }

    // operators that produce Shifting relations when given other relations as input
    template<typename T>
    constexpr Shifting<T> operator+(const Identity<T> f, const T k)
    {
        return Shifting<T>(k);
    }
    template<typename T>
    constexpr Shifting<T> operator+(const T k, const Identity<T> f)
    {
        return Shifting<T>(k);
    }
    template<typename T>
    constexpr Shifting<T> operator-(const Identity<T> f, const T k)
    {
        return Shifting<T>(-k);
    }

    // operators that cause cancelation
    template<typename T>
    constexpr T operator-(const Shifting<T> f, const Shifting<T> g)
    {
        return f.offset-g.offset;
    }
    


    template<typename T, typename Expression>
    constexpr auto compose(const Shifting<T> f, const Expression& g)
    {
        return g+f.offset;
    }
    template<typename T>
    constexpr auto compose(const Shifting<T> f, const Identity<T>& g)
    {
        return g+f.offset;
    }

    template<typename T> constexpr T compose(const Shifting<T>& f, const int k)    { return f(k); }
    template<typename T> constexpr T compose(const Shifting<T>& f, const float k)  { return f(k); }
    template<typename T> constexpr T compose(const Shifting<T>& f, const double k) { return f(k); }

    template<typename T>
    constexpr Shifting<T> inverse(const Shifting<T> f) 
    {
        return Shifting<T>(-f.offset);
    }

    template<typename T>
    constexpr T derivative(const Shifting<T> f) 
    {
        return T(1);
    }

}