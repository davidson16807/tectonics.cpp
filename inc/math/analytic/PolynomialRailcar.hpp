#pragma once

// std libraries
#include <algorithm> // copy, back_inserter

// in-house libraries
#include "Identity.hpp"
#include "Scaling.hpp"
#include "Shifting.hpp"
#include "Polynomial.hpp"
#include "Railcar.hpp"
#include "Railyard.hpp"

namespace analytic {

    template<typename T, int Plo, int Phi>
    using PolynomialRailcar = Railcar<T,Polynomial<T,Plo,Phi>>;


    // RAILCARS

    // with polynomials

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialRailcar<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        return Railcar<T,F>(p.lo, p.hi, p.content*q);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Polynomial<T,Qlo,Qhi> q, const PolynomialRailcar<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        return Railcar<T,F>(p.lo, p.hi, q*p.content);
    }

    template<typename T, int Plo, int Phi, int Q>
    constexpr auto operator/(const PolynomialRailcar<T,Plo,Phi>& p, const Polynomial<T,Q,Q> q)
    {
        using F = Polynomial<T,Plo-Q,Phi-Q>;
        return Railcar<T,F>(p.lo, p.hi, p.content/q);
    }

    /*
    NOTE: we cannot support division by railcars.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 



    // with other railcars

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialRailcar<T,Plo,Phi>& p, const PolynomialRailcar<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        return Railcar<T,F>(std::max(p.lo, q.lo), std::min(p.hi, q.hi), p.content*q.content);
    }




    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const PolynomialRailcar<T,Plo,Phi>& p, const Shifting<T> f)
    {
        return p * Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Shifting<T> f, const PolynomialRailcar<T,Plo,Phi>& p)
    {
        return Polynomial<T,0,1>(f) * p;
    }



    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const PolynomialRailcar<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p * Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Scaling<T> f, const PolynomialRailcar<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(f) * p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator/(const PolynomialRailcar<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p / Polynomial<T,1,1>(f);
    }



    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const PolynomialRailcar<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p * Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Identity<T> e, const PolynomialRailcar<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(e) * p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator/(const PolynomialRailcar<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p / Polynomial<T,1,1>(e);
    }




    /*
    NOTE: we cannot support division by railcars.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 




    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto compose(const PolynomialRailcar<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)>;
        return Railcar<T,F>(p.lo, p.hi, compose(p.content, q));
    }
    template<typename T, int Plo, int Phi>
    constexpr auto compose(const PolynomialRailcar<T,Plo,Phi>& p, const Scaling<T> g)
    {
        using F = Polynomial<T,Plo,Phi>;
        return Railcar<T,F>(p.lo*g.factor, p.hi*g.factor, compose(p.content, g));
    }
    template<typename T, int Plo, int Phi>
    constexpr auto compose(const PolynomialRailcar<T,Plo,Phi>& p, const Shifting<T> g)
    {
        using F = Polynomial<T,std::min(0,Plo),Phi>;
        return Railcar<T,F>(p.lo+g.offset, p.hi+g.offset, compose(p.content, g));
    }


    template<typename T, int Plo, int Phi>
    constexpr auto derivative(const PolynomialRailcar<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,Plo-1,Phi-1>;
        return Railcar<T,F>(p.lo, p.hi, derivative(p.content));
    }


    template<typename T, int Plo, int Phi>
    constexpr auto integral(const PolynomialRailcar<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,Plo+1,Phi+1>;
        const T oo = std::numeric_limits<T>::max();
        Railcar<T,F> g, gmax;
        g = Railcar<T,F>(p.lo, p.hi, integral(p.content) - integral(p.content, p.lo));
        gmax = Railcar<T,F>(g.hi, oo, F() );
        gmax.content[0] = g.content(g.hi);
        return Railyard<T,F>({g, gmax});
    }



    template<typename T, int Plo, int Phi>
    constexpr T maximum(const PolynomialRailcar<T,Plo,Phi> p, const T lo, const T hi) 
    {
        return maximum(p, std::max(lo, p.lo), std::min(hi, p.hi));
    }
    template<typename T, int Plo, int Phi>
    constexpr T minimum(const PolynomialRailcar<T,Plo,Phi> p, const T lo, const T hi) 
    {
        return minimum(p, std::max(lo, p.lo), std::min(hi, p.hi));
    }




    template<typename T, typename F>
    constexpr PolynomialRailcar<T,0,1> linear_newton_polynomial(const Railcar<T,F> f){
        return PolynomialRailcar<T,0,2>(f.lo, f.hi,
            linear_newton_polynomial(x1,x2, f.content(x1),f.content(x2))
        );
    }

    template<typename T, typename F>
    constexpr PolynomialRailcar<T,0,2> quadratic_newton_polynomial(const Railcar<T,F> f){
        const T x1 = f.lo;
        const T x3 = f.hi;
        const T x2 = x1 + T(1.0/2.0)*(x3-x1);
        return PolynomialRailcar<T,0,2>(f.lo, f.hi,
            quadratic_newton_polynomial(x1,x2,x3, f.content(x1),f.content(x2),f.content(x3))
        );
    }

    template<typename T, typename F>
    constexpr PolynomialRailcar<T,0,3> cubic_newton_polynomial(const Railcar<T,F> f){
        const T x1 = f.lo;
        const T x4 = f.hi;
        const T x2 = x1 + T(1.0/3.0)*(x4-x1);
        const T x3 = x1 + T(2.0/3.0)*(x4-x1);
        return PolynomialRailcar<T,0,3>(f.lo, f.hi,
            cubic_newton_polynomial(x1,x2,x3,x4, f.content(x1),f.content(x2),f.content(x3),f.content(x4))
        );
    }


    template<typename T, typename F>
    constexpr PolynomialRailcar<T,0,3> cubic_spline(const Railcar<T,F> f){
        const T x0 = f.lo;
        const T x1 = f.hi;
        return PolynomialRailcar<T,0,3>(f.lo, f.hi,
            cubic_newton_polynomial(x0,x1, f.content(x0), f.content(x1), derivative(f,x0), derivative(f,x1))
        );
    }





}

