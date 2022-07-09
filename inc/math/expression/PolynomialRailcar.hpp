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

namespace math {

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

}

