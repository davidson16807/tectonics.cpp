#pragma once

namespace inspected {

    template<typename T, typename F, typename DFDX, typename D2FDX2>
    struct InverseByHalleysMethod {
        using value_type = T;
        const F f;
        const DFDX dfdx;
        const D2FDX2 d2fdx2;
        const T x0;
        const std::size_t iteration_count;
        constexpr explicit InverseByHalleysMethod(
            const F f, 
            const DFDX dfdx, 
            const D2FDX2 d2fdx2, 
            const T x0, 
            const std::size_t iteration_count
        ):
            f(f),
            dfdx(dfdx),
            d2fdx2(d2fdx2),
            x0(x0),
            iteration_count(iteration_count)
        {}
        constexpr InverseByHalleysMethod(const InverseByHalleysMethod<T,F,DFDX,D2FDX2>& inv):
            f(inv.f),
            dfdx(inv.dfdx),
            d2fdx2(inv.d2fdx2),
            x0(inv.x0),
            iteration_count(inv.iteration_count)
        {}
        constexpr T operator()(const T y) const
        {
            T x(x0);
            T e(0);
            T dedx(0);
            T d2edx2(0);
            // solve using Halley's method
            for (std::size_t i = 0; i < iteration_count; i++) {
                e = f(x)-y;
                dedx = dfdx(x);
                d2edx2 = d2fdx2(x);
                x -= T(2)*e*dedx / (T(2)*dedx*dedx - e*d2edx2);
            }
            return x;
        }
    };

    /*
    */

    template<typename T, typename F, typename DFDX, typename D2FDX2>
    constexpr auto inverse_by_halleys_method(
            const F f, 
            const DFDX dfdx, 
            const D2FDX2 d2fdx2, 
            const T x0, 
            const std::size_t iteration_count
    ) {
        return InverseByHalleysMethod<T,F,DFDX,D2FDX2>(f,dfdx,d2fdx2,x0,iteration_count);
    }

}

