#pragma once

// std libraries
#include <algorithm> // copy, back_inserter

// in-house libraries
#include "../Identity.hpp"
#include "../Scaling.hpp"
#include "../Shifting.hpp"
#include "../Polynomial.hpp"

#include "Railcar.hpp"
#include "Railyard.hpp"

namespace math {

    template<typename T, int P1lo, int P1hi>
    using PolynomialRailcar = Railcar<T,Polynomial<T,P1lo,P1hi>>;


    // RAILCARS

    /*
    NOTE: we cannot support division by railcars.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 



    // with other railcars

    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator*(const PolynomialRailcar<T,P1lo,P1hi>& p, const PolynomialRailcar<T,P2lo,P2hi> q)
    {
        using F = Polynomial<T,P1lo+P2lo,P1hi+P2hi>;
        return Railcar<T,F>(std::max(p.lo, q.lo), std::min(p.hi, q.hi), p.content*q.content);
    }




    // with polynomials

    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator*(const PolynomialRailcar<T,P1lo,P1hi>& p, const Polynomial<T,P2lo,P2hi> q)
    {
        using F = Polynomial<T,P1lo+P2lo,P1hi+P2hi>;
        return Railcar<T,F>(p.lo, p.hi, p.content*q);
    }

    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator*(const Polynomial<T,P2lo,P2hi> q, const PolynomialRailcar<T,P1lo,P1hi>& p)
    {
        using F = Polynomial<T,P1lo+P2lo,P1hi+P2hi>;
        return Railcar<T,F>(p.lo, p.hi, q*p.content);
    }

    template<typename T, int P1lo, int P1hi, int Q>
    constexpr auto operator/(const PolynomialRailcar<T,P1lo,P1hi>& p, const Polynomial<T,Q,Q> q)
    {
        using F = Polynomial<T,P1lo-Q,P1hi-Q>;
        return Railcar<T,F>(p.lo, p.hi, p.content/q);
    }

    

    template<typename T, int P1lo, int P1hi>
    constexpr auto operator*(const PolynomialRailcar<T,P1lo,P1hi>& p, const Shifting<T> f)
    {
        return p * Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator*(const Shifting<T> f, const PolynomialRailcar<T,P1lo,P1hi>& p)
    {
        return Polynomial<T,0,1>(f) * p;
    }



    template<typename T, int P1lo, int P1hi>
    constexpr auto operator*(const PolynomialRailcar<T,P1lo,P1hi>& p, const Scaling<T> f)
    {
        return p * Polynomial<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator*(const Scaling<T> f, const PolynomialRailcar<T,P1lo,P1hi>& p)
    {
        return Polynomial<T,1,1>(f) * p;
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator/(const PolynomialRailcar<T,P1lo,P1hi>& p, const Scaling<T> f)
    {
        return p / Polynomial<T,1,1>(f);
    }



    template<typename T, int P1lo, int P1hi>
    constexpr auto operator*(const PolynomialRailcar<T,P1lo,P1hi>& p, const Identity<T> e)
    {
        return p * Polynomial<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator*(const Identity<T> e, const PolynomialRailcar<T,P1lo,P1hi>& p)
    {
        return Polynomial<T,1,1>(e) * p;
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator/(const PolynomialRailcar<T,P1lo,P1hi>& p, const Identity<T> e)
    {
        return p / Polynomial<T,1,1>(e);
    }




    /*
    NOTE: we cannot support division by railcars.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 




    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto compose(const PolynomialRailcar<T,P1lo,P1hi>& p, const Polynomial<T,P2lo,P2hi> q)
    {
        using F = Polynomial<T,std::min(P1lo*P2lo,P1hi*P2hi),std::max(P1lo*P2lo,P1hi*P2hi)>;
        return Railcar<T,F>(p.lo, p.hi, compose(p.content, q));
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto compose(const PolynomialRailcar<T,P1lo,P1hi>& p, const Scaling<T> g)
    {
        using F = Polynomial<T,P1lo,P1hi>;
        return Railcar<T,F>(p.lo*g.factor, p.hi*g.factor, compose(p.content, g));
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto compose(const PolynomialRailcar<T,P1lo,P1hi>& p, const Shifting<T> g)
    {
        using F = Polynomial<T,std::min(0,P1lo),P1hi>;
        return Railcar<T,F>(p.lo+g.offset, p.hi+g.offset, compose(p.content, g));
    }


    template<typename T, int P1lo, int P1hi>
    constexpr auto derivative(const PolynomialRailcar<T,P1lo,P1hi>& p)
    {
        using F = Polynomial<T,P1lo-1,P1hi-1>;
        return Railcar<T,F>(p.lo, p.hi, derivative(p.content));
    }


    template<typename T, int P1lo, int P1hi>
    constexpr auto integral(const PolynomialRailcar<T,P1lo,P1hi>& p)
    {
        using F = Polynomial<T,0,P1hi+1>;
        const T oo = std::numeric_limits<T>::max();
        Railcar<T,F> g, gmax;
        g = Railcar<T,F>(p.lo, p.hi, integral(p.content) - integral(p.content, p.lo));
        gmax = Railcar<T,F>(g.hi, oo, F() );
        gmax.content[0] = g.content(g.hi);
        return Railyard<T,F>({g, gmax});
    }



    template<typename T, int P1lo, int P1hi>
    constexpr T maximum(const PolynomialRailcar<T,P1lo,P1hi> p, const T lo, const T hi) 
    {
        return maximum(p, std::max(lo, p.lo), std::min(hi, p.hi));
    }
    template<typename T, int P1lo, int P1hi>
    constexpr T minimum(const PolynomialRailcar<T,P1lo,P1hi> p, const T lo, const T hi) 
    {
        return minimum(p, std::max(lo, p.lo), std::min(hi, p.hi));
    }




    template<typename T, typename F>
    constexpr PolynomialRailcar<T,0,1> linear_newton_polynomial(const Railcar<T,F> f){
        const T x1 = f.lo;
        const T x2 = f.hi;
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

