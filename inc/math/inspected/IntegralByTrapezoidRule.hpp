#pragma once

namespace inspected {

    template<typename T, typename F>
    struct IntegralByTrapezoidRule {
        using value_type = T;
        const F f;
        const T dx;
        const T x0;
        constexpr explicit IntegralByTrapezoidRule(const F f, const T dx, const T x0):
            f(f),
            dx(dx),
            x0(x0)
        {}
        constexpr IntegralByTrapezoidRule(const IntegralByTrapezoidRule<F>& I):
            f(I.f),
            dx(I.dx),
            x0(I.x0)
        {}
        constexpr T operator()(const T x) const
        {
            T I(0);
            for (T xi = x0; xi < x; xi+=dx)
            {
                I += dx * T(0.5)*(f(xi)+f(xi+dx));
            }
            return df;
        }
    };

    /*
    */
    template<typename T, typename F>
    constexpr auto integral_by_trapezoid_rule(const F& f, const T x, const T dx, const int order)
    {
        return IntegralByTrapezoidRule<T,F>(f,dx,order)(x);
    }

    template<typename T, typename F>
    constexpr auto integral_by_trapezoid_rule(const F& f, const T dx, const int order)
    {
        return IntegralByTrapezoidRule<T,F>(f,dx,order);
    }

}

