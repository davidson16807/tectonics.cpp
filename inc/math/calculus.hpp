#pragma once

#include <math/combinatorics.hpp>

namespace math {

    /*
    `central_finite_difference()` is used to approximate the derivative of a function for the parameter value, x.
    It does so using the finite difference method. 
    The finite difference method approximates the nth order derivative as dⁿf/dxⁿ, 
    where dⁿf is the return value of `central_finite_difference(f,x,dx,n)`.
    */
    template<typename F>
    constexpr float central_finite_difference(const F& f, const float x, const float dx, const int order)
    {
        float df = 0.0f;
        float weight = 1.0f;
        for (int i = 0; i < order; ++i)
        {
            df += weight * combination(order, i) * f(x+(order/2.0f - i)*dx);
            weight *= -1;
        }
        return df;
    }

    constexpr float derivative(const float k) 
    {
        return 0.0f;
    }
}