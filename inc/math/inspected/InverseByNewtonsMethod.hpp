#pragma once

namespace inspected {

    template<typename T, typename F, typename DFDX>
    struct InverseByNewtonsMethod {
        using value_type = T;
        const F f;
        const DFDX dfdx;
        const T x0;
        const int iteration_count;
        constexpr explicit InverseByNewtonsMethod(const F f, const DFDX dfdx, const T x0, const int iteration_count):
            f(f),
            dfdx(dfdx),
            x0(x0),
            iteration_count(iteration_count)
        {}
        constexpr InverseByNewtonsMethod(const InverseByNewtonsMethod<T,F,DFDX>& inv):
            f(inv.f),
            dfdx(inv.dfdx),
            x0(inv.x0),
            iteration_count(inv.iteration_count)
        {}
        constexpr T operator()(const T y) const
        {
            T x(x0);
            // solve using Newton's method
            for (std::size_t i = 0; i < iteration_count; i++) {
                x += (y - f(x))/dfdx(x);
            }
            return x;
        }
    };

    /*
    */

    template<typename T, typename F, typename DFDX>
    constexpr auto inverse_by_newtons_method(const F& f, const DFDX& dfdx, const T x0, const int iteration_count)
    {
        return InverseByNewtonsMethod<T,F,DFDX>(f,dfdx,x0,iteration_count);
    }

}

