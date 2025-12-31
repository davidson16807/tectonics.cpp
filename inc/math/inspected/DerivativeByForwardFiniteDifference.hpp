#pragma once

#include <math/combinatorics.hpp>

namespace inspected {

    template<typename T, typename F>
    struct ForwardFiniteDifference {
        using value_type = T;
        const F f;
        const T dx;
        const int order;
        constexpr explicit ForwardFiniteDifference(const F f, const T dx, const int order):
            f(f),
            dx(dx),
            order(order)
        {}
        constexpr ForwardFiniteDifference(const ForwardFiniteDifference<T,F>& dfdx):
            f(dfdx.f),
            dx(dfdx.dx),
            order(dfdx.order)
        {}
        constexpr T operator()(const T x) const
        {
            T df(0);
            T weight(1);
            for (int i = 0; i <= order; ++i)
            {
                df += weight * combinatoric::combination(order, i) * f(x+i*dx);
                weight *= T(-1);
            }
            return df;
        }
    };

    /*
    `forward_finite_difference()` is used to approximate the derivative of a function for the parameter value, x.
    It does so using the finite difference method. 
    */
    template<typename T, typename F>
    constexpr auto forward_finite_difference(const F& f, const T x, const T dx, const int order)
    {
        return ForwardFiniteDifference<T,F>(f,dx,order)(x);
    }

    template<typename T, typename F>
    constexpr auto forward_finite_difference(const F& f, const T dx, const int order)
    {
        return ForwardFiniteDifference<T,F>(f,dx,order);
    }

}

