#pragma once

#include <math/combinatorics.hpp>

namespace inspected {

    template<typename T, typename F>
    struct DerivativeByCentralFiniteDifference {
        using value_type = T;
        const F f;
        const T dx;
        const int order;
        constexpr explicit DerivativeByCentralFiniteDifference(const F f, const T dx, const int order):
            f(f),
            dx(dx),
            order(order)
        {}
        constexpr DerivativeByCentralFiniteDifference(const DerivativeByCentralFiniteDifference<T,F>& dfdx):
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
                df += weight * combinatoric::combination(order, i) * f(x+(T(0.5)*order - i)*dx);
                weight *= -1;
            }
            return df;
        }
    };

    /*
    `derivative_by_central_finite_difference()` is used to approximate the derivative of a function for the parameter value, x.
    It does so using the finite difference method. 
    */
    template<typename T, typename F>
    constexpr auto derivative_by_central_finite_difference(const F& f, const T x, const T dx, const int order)
    {
        return DerivativeByCentralFiniteDifference<T,F>(f,dx,order)(x);
    }

    template<typename T, typename F>
    constexpr auto derivative_by_central_finite_difference(const F& f, const T dx, const int order)
    {
        return DerivativeByCentralFiniteDifference<T,F>(f,dx,order);
    }

}

