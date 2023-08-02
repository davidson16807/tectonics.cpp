#pragma once

namespace math {

    /* 
    `Identity` is a trivial class that represents the function f(x)=x,
    oddly enough, it was useful for something, so we implemented it.
    (it was useful as the base case for the recursive implementation of `compose()` for polynomials)
    */
    template<typename T>
    struct Identity {
        constexpr explicit Identity()
        {}
        constexpr T operator()(const T x) const
        {
            return x;
        }
    };
    
    // operators that cause cancelation
    template<typename T>
    constexpr T operator-(const Identity<T> e1, const Identity<T> e2)
    {
        return T(0.0);
    }
    template<typename T>
    constexpr T operator/(const Identity<T> e1, const Identity<T> e2)
    {
        return T(1.0);
    }


    template<typename T, typename Expression> constexpr Expression compose(const Identity<T> e, const Expression& f) { return f;    }
    template<typename T, typename Expression> constexpr Expression compose(const Expression& f, const Identity<T> e) { return f;    }
    template<typename Expression>             constexpr double compose(const double k, const Expression& f)          { return k;    }
    template<typename Expression>             constexpr auto   compose(const Expression& f, const double k)          { return f(k); }
    template<typename Expression>             constexpr double compose(const float k, const Expression& f)           { return k;    }
    template<typename Expression>             constexpr auto   compose(const Expression& f, const float k)           { return f(k); }
    template<typename Expression>             constexpr double compose(const int k, const Expression& f)             { return k;    }
    template<typename Expression>             constexpr auto   compose(const Expression& f, const int k)             { return f(k); }

    template<typename T>
    constexpr T distance(const Identity<T> a, const Identity<T> b, const double lo, const double hi){ return T(0); }
    
    template<typename T>
    constexpr T distance(const T a, const T b, const T lo, const T hi){ return std::abs(a-b); }


}