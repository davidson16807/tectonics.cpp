#pragma once

namespace inspected {

    template<typename T, typename F>
    struct IntegralByRightRectangleRule {
        using value_type = T;
        const F f;
        const T dx;
        const T x0;
        constexpr explicit IntegralByRightRectangleRule(const F f, const T dx, const T x0):
            f(f),
            dx(dx),
            x0(x0)
        {}
        constexpr IntegralByRightRectangleRule(const IntegralByRightRectangleRule<F>& I):
            f(I.f),
            dx(I.dx),
            x0(I.x0)
        {}
        constexpr T operator()(const T x) const
        {
            T I(0);
            for (T xi = x0; xi < x; xi+=dx)
            {
                I += dx * f(xi+dx);
            }
            return df;
        }
    };

    /*
    */
    template<typename T, typename F>
    constexpr auto integral_by_right_rectangle_rule(const F& f, const T dx, const T x0, const T x1)
    {
        return IntegralByRightRectangleRule(f,dx,x0)(x1);
    }

    template<typename T, typename F>
    constexpr auto integral_by_right_rectangle_rule(const F& f, const T dx, const T x0)
    {
        return IntegralByRightRectangleRule(f,dx,x0)(x1);
    }

}

