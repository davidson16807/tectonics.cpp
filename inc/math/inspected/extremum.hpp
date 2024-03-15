#pragma once

#include <math/analytic/Polynomial.hpp>
#include <math/analytic/Shifting.hpp>
#include <math/inspected/DerivativeByCentralFiniteDifference.hpp>

/*
inspection.hpp contains functionality that solves common problems 
using solution by inspection. 

This technique is only useful when no analytic expression exists for a problem,
so the functions here operate as function templates that serve as overloads
to problems for function parameters that offer no specialized analytic solution.
*/

namespace inspected {
    template<typename T, typename F>
    T maximum(const F& f, T lo, T hi, const int iteration_count)
    {
        /*
        Start with an initial estimate of lo or hi, whichever produces greater output.
        Approximate the function using a second order Taylor series based around the estimate, then solve for the extremum. 
        Accept extremum as an updated estimate if it is higher than the previous estimate, then repeat.
        */
        const T dx = (hi-lo)/T(1000);
        const T dx2 = dx*dx;
        analytic::Polynomial<T,0,2> approximation({T(0), T(0), T(0)});
        T x = f(lo) > f(hi)? lo : hi;
        T x2;
        for (int i = 0; i < iteration_count; ++i)
        {
            approximation = compose(
                analytic::Polynomial<T,0,2>({T(f(x)), 
                    inspected::derivative_by_central_finite_difference(f, x, dx, 1)/(dx * math::factorial(1.0)), 
                    inspected::derivative_by_central_finite_difference(f, x, dx, 2)/(dx2* math::factorial(2.0))}),
                analytic::Shifting(-x)
            );
            x2 = solve(derivative(approximation), T(0));
            x = f(x2) > f(x)? x2 : x;
        }
        return f(x);
    }

    template<typename T, typename F>
    T minimum(const F& f, T lo, T hi, const int iteration_count) 
    {
        /*
        See equivalent implementation for maximum() for details
        */
        const T dx = (hi-lo)/T(1000);
        const T dx2 = dx*dx;
        analytic::Polynomial<T,0,2> approximation({T(0), T(0), T(0)});
        T x = f(lo) > f(hi)? lo : hi;
        T x2;
        for (int i = 0; i < iteration_count; ++i)
        {
            approximation = compose(
                analytic::Polynomial<T,0,2>({T(f(x)), 
                    inspected::derivative_by_central_finite_difference(f, x, dx, 1)/(dx * math::factorial(1.0)), 
                    inspected::derivative_by_central_finite_difference(f, x, dx, 2)/(dx2* math::factorial(2.0))}),
                analytic::Shifting(-x)
            );
            x2 = solve(derivative(approximation), T(0));
            x = f(x2) > f(x)? x2 : x;
        }
        return f(x);
    }

}