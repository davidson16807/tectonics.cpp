#pragma once

#include <math/combinatorics.hpp>

namespace inspected {

    template<typename T, typename F>
    struct DerivativeByForwardFiniteDifference {
        using value_type = T;
        const F f;
        const T dx;
        const int order;
        constexpr explicit DerivativeByForwardFiniteDifference(const F f, const T dx, const int order):
            f(f),
            dx(dx),
            order(order)
        {}
        constexpr DerivativeByForwardFiniteDifference(const DerivativeByForwardFiniteDifference<T,F>& dfdx):
            f(dfdx.f),
            dx(dfdx.dx),
            order(dfdx.order)
        {}
        constexpr T operator()(const T x) const
        {
            T df(0);
            T weight(1);
            for (int i = 0; i < order; ++i)
            {
                df += weight * math::combination(order, i) * f(x+i*dx);
                weight *= -1;
            }
            return df;
        }
    };

    /*
    `derivative_by_forward_finite_difference()` is used to approximate the derivative of a function for the parameter value, x.
    It does so using the finite difference method. 
    The finite difference method approximates the nth order derivative as dⁿf/dxⁿ, 
    where dⁿf is the return value of `derivative_by_forward_finite_difference(f,x,dx,n)`.
    */
    template<typename T, typename F>
    constexpr auto derivative_by_forward_finite_difference(const F& f, const T x, const T dx, const int order)
    {
        return DerivativeByForwardFiniteDifference<T,F>(f,dx,order)(x);
    }

    template<typename T, typename F>
    constexpr auto derivative_by_forward_finite_difference(const F& f, const T dx, const int order)
    {
        return DerivativeByForwardFiniteDifference<T,F>(f,dx,order);
    }

}

