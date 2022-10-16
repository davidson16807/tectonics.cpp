#pragma once

// std libraries
#include <algorithm> // copy, back_inserter
#include <iostream> 

// in-house libraries
#include "../Identity.hpp"
#include "../Scaling.hpp"
#include "../Shifting.hpp"
#include "../Polynomial.hpp"

#include "Railcar.hpp"
#include "Railyard.hpp"
#include "PolynomialRailcar.hpp"

namespace analytic {

    template<typename T, int Plo, int Phi>
    using PolynomialRailyard = Railyard<T,Polynomial<T,Plo,Phi>>;


    // RAILYARDS

    // ADDITION AND SUBTRACTION

    // with polynomials

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const PolynomialRailyard<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y += F(q);
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const Polynomial<T,Qlo,Qhi>& q, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y += F(q);
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const PolynomialRailyard<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y += F(-q);
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const Polynomial<T,Qlo,Qhi>& q, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(q);
        y += F(-p);
        return y;
    }






    // with railcars

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailcar<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y += Railcar<T,F>(q);
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const PolynomialRailcar<T,Qlo,Qhi>& q, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y += Railcar<T,F>(q);
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailcar<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y += Railcar<T,F>(-q);
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const PolynomialRailcar<T,Qlo,Qhi>& q, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(q);
        y += Railcar<T,F>(-p);
        return y;
    }



    // with other railyards

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y += Railyard<T,F>(q);
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y -= Railyard<T,F>(q);
        return y;
    }




    // MULTIPLICATION

    // with polynomials

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialRailyard<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        Railyard<T,F> y;
        for (auto pi : p.cars)
        {
            y += pi*q;
        }
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Polynomial<T,Qlo,Qhi> q, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        Railyard<T,F> y;
        for (auto pi : p.cars)
        {
            y += pi*q;
        }
        return y;
    }

    template<typename T, int Plo, int Phi, int Q>
    constexpr auto operator/(const PolynomialRailyard<T,Plo,Phi>& p, const Polynomial<T,Q,Q> q)
    {
        using F = Polynomial<T,Plo-Q,Phi-Q>;
        Railyard<T,F> y;
        for (auto pi : p.cars)
        {
            y += pi/q;
        }
        return y;
    }

    /*
    NOTE: we cannot support division by railcars.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    We can only be reasonably confident this will not be the case for trains.
    */ 



    // with railcars

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailcar<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        Railyard<T,F> y;
        Railcar<T,F> yi;
        for (auto pi : p.cars)
        {
            yi = pi*q;
            if (yi.lo < yi.hi)
            {
                y += yi;
            }
        }
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialRailcar<T,Qlo,Qhi> q, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        std::vector<Railcar<T,F>> cars;
        Railyard<T,F> y;
        Railcar<T,F> yi;
        for (auto pi : p.cars)
        {
            yi = q*pi;
            if (yi.lo < yi.hi)
            {
                y += yi;
            }
        }
        return y;
    }



    // with other railyards

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        std::vector<Railcar<T,F>> cars;
        Railyard<T,F> y;
        for (auto pi : p.cars)
        {
            for (auto qj : q.cars)
            {
                Railcar<T,F> yij = (pi*qj);
                if (yij.lo < yij.hi)
                {
                    y += yij;
                }
            }
        }
        return y;
    }

    /*
    NOTE: we cannot support division by railyards.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    We can only be reasonably confident this will not be the case for trains.
    */ 







    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const PolynomialRailyard<T,Plo,Phi>& p, const Shifting<T> f)
    {
        return p + Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Shifting<T> f, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,0,1>(f) + p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const PolynomialRailyard<T,Plo,Phi>& p, const Shifting<T> f)
    {
        return p - Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Shifting<T> f, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,0,1>(f) - p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const PolynomialRailyard<T,Plo,Phi>& p, const Shifting<T> f)
    {
        return p * Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Shifting<T> f, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,0,1>(f) * p;
    }



    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const PolynomialRailyard<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p + Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Scaling<T> f, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(f) + p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const PolynomialRailyard<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p - Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Scaling<T> f, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(f) - p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const PolynomialRailyard<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p * Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Scaling<T> f, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(f) * p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator/(const PolynomialRailyard<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p / Polynomial<T,1,1>(f);
    }



    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const PolynomialRailyard<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p + Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Identity<T> e, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(e) + p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const PolynomialRailyard<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p - Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Identity<T> e, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(e) - p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const PolynomialRailyard<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p * Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Identity<T> e, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(e) * p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator/(const PolynomialRailyard<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p / Polynomial<T,1,1>(e);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr Railyard<T,Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)>> 
        compose(const PolynomialRailcar<T,Plo,Phi>& f, const PolynomialRailcar<T,Qlo,Qhi> g)
    {
        // Each coupler in f needs an associated set of couplers for f∘g 
        //  that each represent where the output for g crosses a coupler for f.
        std::vector<T> couplers; 
        auto lo_preimage = solutions(g.content, f.lo);
        auto hi_preimage = solutions(g.content, f.hi);
        reals<T>(lo_preimage.begin(), lo_preimage.end(), std::back_inserter(couplers));
        reals<T>(hi_preimage.begin(), hi_preimage.end(), std::back_inserter(couplers));
        auto removal_begin = std::remove_if(couplers.begin(), couplers.end(), 
            [g](T coupler) { return !(g.lo < coupler && coupler < g.hi); });
        couplers.erase(removal_begin, couplers.end());
        std::sort(couplers.begin(), couplers.end());
        using FG = Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)>;
        std::vector<Railcar<T,FG>> cars;
        T lo, hi, mid, gmid;
        for (std::size_t i=1; i<couplers.size(); i++)
        {
            lo = couplers[i-1];
            hi = couplers[i];
            mid = (lo + hi)/T(2);
            gmid = g(mid);
            if(f.lo < gmid && gmid < f.hi){
                cars.emplace_back(
                    std::max(lo, g.lo), 
                    std::min(hi, g.hi), 
                    compose(f.content, g.content));
            }
        }
        return Railyard<T,FG>(cars);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto compose(const PolynomialRailyard<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)>;
        std::vector<Railcar<T,F>> cars;
        for (auto pi : p.cars)
        {
            cars.push_back(compose(pi, q));
        }
        return Railyard<T,F>(cars);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto compose(const PolynomialRailyard<T,Plo,Phi>& p, const Scaling<T> g)
    {
        using F = Polynomial<T,Plo,Phi>;
        std::vector<Railcar<T,F>> cars;
        for (auto pi : p.cars)
        {
            cars.push_back(compose(pi, g));
        }
        return Railyard<T,F>(cars);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto compose(const PolynomialRailyard<T,Plo,Phi>& p, const Shifting<T> g)
    {
        using F = Polynomial<T,std::min(0,Plo),Phi>;
        std::vector<Railcar<T,F>> cars;
        for (auto pi : p.cars)
        {
            cars.push_back(compose(pi, g));
        }
        return Railyard<T,F>(cars);
    }



    template<typename T, int Plo, int Phi>
    constexpr auto derivative(const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,Plo-1,Phi-1>;
        Railyard<T,F> y;
        for (auto pi : p.cars)
        {
            y.cars.push_back(Railcar<T,F>(pi.lo, pi.hi, derivative(pi.content)));
            // y += derivative(pi.content);
        }
        return y;
    }

    template<typename T, int Plo, int Phi>
    constexpr auto integral(const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,0,Phi+1>;
        // const T oo(std::numeric_limits<T>::max());
        Railyard<T,F> y;
        Railcar<T,F> g, gmax;
        for (auto pi : p.cars)
        {
            y += integral(pi);
        }
        return y;
    }



    template<int N, typename T, int Plo, int Phi, 
        typename = std::enable_if_t<(N==0)>>
    constexpr PolynomialRailyard<T,0,0> pow(const PolynomialRailyard<T,Plo,Phi>& p){
        return PolynomialRailyard<T,0,0>(T(1));
    }

    template<int N, typename T, int Plo, int Phi, 
        typename = std::enable_if_t<(N>0)>>
    constexpr PolynomialRailyard<T,Plo*N,Phi*N> pow(const PolynomialRailyard<T,Plo,Phi>& p){
        return p*pow<N-1>(p);
    }


    /*
    `distance` is the root of the integrated squared difference 
    between two polynomials over a given range divided by the range.
    It provides a metric that expresses the extent to which two polynomials differ 
    using the same units as the output of the polynomial. 
    It is analogous to the standard deviation of a dataset in statistics,
    and it satisfies all criteria needed to be considered a metric:
    * d(a,b) ∈ ℝ⁺ 
    * d(a,b) = 0 ⟺ a=b
    * d(a,b) = d(b,a)
    * d(a,b) ≤ d(a,c)+d(c,b)
    We do not presume that this metric serves more naturally as a distance metric compared to others,
    however we address this function as `distance` since the alternative names
    are either unwieldy (`root_mean_square_difference`) or nonobvious (`rmsd`).
    */
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(
        const PolynomialRailyard<T,Plo,Phi>& p, 
        const PolynomialRailyard<T,Qlo,Qhi>& q, 
        const T lo, 
        const T hi
    ){
        return std::sqrt((std::max(T(0), integral(pow<2>(p-q), lo, hi))) / (hi-lo));
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const PolynomialRailyard<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(pow<2>(p-q), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const Polynomial<T,Plo,Phi> p, const PolynomialRailyard<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(pow<2>(p-q), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi>
    constexpr T distance(const PolynomialRailyard<T,Plo,Phi>& p, const T k, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(pow<2>(p-k), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi>
    constexpr T distance(const T k, const PolynomialRailyard<T,Plo, Phi> p, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(pow<2>(p-k), lo, hi))) / (hi-lo);
    }




    template<typename T, typename F>
    PolynomialRailyard<T,0,1> linear_newton_polynomial(
        const Railyard<T,F> yard
    ){
        PolynomialRailyard<T,0,1> y;
        for (auto car : yard.cars)
        {
            y += linear_newton_polynomial(car);
        }
        return y;
    }

    template<typename T, typename F>
    PolynomialRailyard<T,0,2> quadratic_newton_polynomial(
        const Railyard<T,F> yard
    ){
        PolynomialRailyard<T,0,2> y;
        for (auto car : yard.cars)
        {
            y += quadratic_newton_polynomial(car);
        }
        return y;
    }

    template<typename T, typename F>
    PolynomialRailyard<T,0,3> cubic_newton_polynomial(
        const Railyard<T,F> yard
    ){
        PolynomialRailyard<T,0,3> y;
        for (auto car : yard.cars)
        {
            y += cubic_newton_polynomial(car);
        }
        return y;
    }

    template<typename T, typename F>
    PolynomialRailyard<T,0,3> cubic_spline(
        const Railyard<T,F> yard
    ){
        PolynomialRailyard<T,0,3> y;
        for (auto car : yard.cars)
        {
            y += cubic_spline(car);
        }
        return y;
    }


}
