
#include <math/inspected/CentralFiniteDifference.hpp>
#include <math/analytic/Polynomial.hpp>

namespace inspected{

    template<typename T, typename F>
    constexpr Polynomial<T,0,1> linear_taylor_series(const F f, const T x, const T dx)
    {
        return compose(
            Polynomial<T,0,1>(f(x), 
                inspected::central_finite_difference(f, x, dx, 1) / dx),
            Shifting<T>(-x)
        );
    }

    template<typename T, typename F>
    constexpr Polynomial<T,0,2> quadratic_taylor_series(const F f, const T x, const T dx)
    {
        const T dx2 = dx*dx;
        return compose(
            Polynomial<T,0,2>{f(x), 
                inspected::central_finite_difference(f, x, dx, 1) / dx, 
                inspected::central_finite_difference(f, x, dx, 2) /(dx2*T(2))},
            Shifting<T>(-x)
        );
    }

    template<typename T, typename F>
    constexpr Polynomial<T,0,3> cubic_taylor_series(const F f, const T x, const T dx)
    {
        const T dx2 = dx*dx;
        const T dx3 = dx2*dx;
        return 
            compose(
                Polynomial<T,0,3>(f(x), 
                    inspected::central_finite_difference(f, x, dx, 1) / dx, 
                    inspected::central_finite_difference(f, x, dx, 2) /(dx2*T(2)), 
                    inspected::central_finite_difference(f, x, dx, 3) /(dx3*T(6))),
                Shifting<T>(-x)
            );
    }

}

