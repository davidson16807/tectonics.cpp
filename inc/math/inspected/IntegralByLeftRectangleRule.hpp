#pragma once

namespace inspected {

    template<typename T, typename F>
    struct IntegralByLeftRectangleRule {
        using value_type = T;
        const F f;
        const T dx;
        const T x0;
        constexpr explicit IntegralByLeftRectangleRule(const F f, const T dx, const T x0):
            f(f),
            dx(dx),
            x0(x0)
        {}
        constexpr IntegralByLeftRectangleRule(const IntegralByLeftRectangleRule<F>& I):
            f(I.f),
            dx(I.dx),
            x0(I.x0)
        {}
        constexpr T operator()(const T x) const
        {
            T I(0);
            for (T xi = x0; xi < x; xi+=dx)
            {
                I += dx * f(xi);
            }
            return df;
        }
    };

    /*
    */
    template<typename T, typename F>
    constexpr auto integral_by_left_rectangle_rule(const F& f, const T dx, const T x0, const T x1)
    {
        return IntegralByLeftRectangleRule(f,dx,x0)(x1);
    }

    template<typename T, typename F>
    constexpr auto integral_by_left_rectangle_rule(const F& f, const T dx, const T x0)
    {
        return IntegralByLeftRectangleRule(f,dx,x0)(x1);
    }

}

