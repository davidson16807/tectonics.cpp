#pragma once

namespace inspected {

    template<typename T, typename F>
    struct IntegralByMidRectangleRule {
        using value_type = T;
        const F f;
        const T dx;
        const T x0;
        constexpr explicit IntegralByMidRectangleRule(const F f, const T dx, const T x0):
            f(f),
            dx(dx),
            x0(x0)
        {}
        constexpr IntegralByMidRectangleRule(const IntegralByMidRectangleRule<F>& I):
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
    constexpr auto integral_by_mid_rectangle_rule(const F& f, const T dx, const T x0, const T x1)
    {
        return IntegralByMidRectangleRule(f,dx,x0)(x1);
    }

    template<typename T, typename F>
    constexpr auto integral_by_mid_rectangle_rule(const F& f, const T dx, const T x0)
    {
        return IntegralByMidRectangleRule(f,dx,x0)(x1);
    }

}

