#pragma once

#include <math/combinatorics.hpp>
#include <math/calculus.hpp>
#include <math/expression/Polynomial.hpp>
#include <math/expression/Shifting.hpp>

/*
inspection.hpp contains functionality that solves common problems 
using solution by inspection. 

This technique is only useful when no analytic expression exists for a problem,
so the functions here operate as function templates that serve as overloads
to problems for function parameters that offer no specialized analytic solution.
*/

namespace math {
    template<typename F>
    float maximum(const F& f, float lo, float hi, const int iteration_count)
    {
        /*
        Start with an initial estimate of lo or hi, whichever produces greater output.
        Approximate the function using a second order Taylor series based around the estimate, then solve for the extremum. 
        Accept extremum as an updated estimate if it is higher than the previous estimate, then repeat.
        */
        const float dx = (hi-lo)/1000.0f;
        const float dx2 = dx*dx;
        math::Polynomial<0,2> approximation(0.0f, 0.0f, 0.0f);
        float x = f(lo) > f(hi)? lo : hi;
        float x2;
        for (int i = 0; i < iteration_count; ++i)
        {
            approximation = compose(
                math::Polynomial<0,2>(float(f(x)), 
                    math::central_finite_difference(f, x, dx, 1)/(dx * math::factorial(1.0)), 
                    math::central_finite_difference(f, x, dx, 2)/(dx2* math::factorial(2.0))),
                math::Shifting(-x)
            );
            x2 = solve(derivative(approximation), 0.0f);
            x = f(x2) > f(x)? x2 : x;
        }
        return f(x);
    }

    template<typename F>
    float minimum(const F& f, float lo, float hi, const int iteration_count) 
    {
        /*
        See equivalent implementation for maximum() for details
        */
        const float dx = (hi-lo)/1000.0f;
        const float dx2 = dx*dx;
        math::Polynomial<0,2> approximation(0.0f, 0.0f, 0.0f);
        float x = f(lo) > f(hi)? lo : hi;
        float x2;
        for (int i = 0; i < iteration_count; ++i)
        {
            approximation = compose(
                math::Polynomial<0,2>(float(f(x)), 
                    math::central_finite_difference(f, x, dx, 1)/(dx * math::factorial(1.0)), 
                    math::central_finite_difference(f, x, dx, 2)/(dx2* math::factorial(2.0))),
                math::Shifting(-x)
            );
            x2 = solve(derivative(approximation), 0.0f);
            x = f(x2) > f(x)? x2 : x;
        }
        return f(x);
    }

}