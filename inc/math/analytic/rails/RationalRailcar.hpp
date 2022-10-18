#pragma once

// std libraries
#include <algorithm> // copy, back_inserter

// in-house libraries
#include "../Identity.hpp"
#include "../Scaling.hpp"
#include "../Shifting.hpp"
#include "../Polynomial.hpp"
#include "../Rational.hpp"

#include "Railcar.hpp"
#include "Railyard.hpp"

namespace analytic {

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    using RationalRailcar = Railcar<T,Polynomial<T,Plo,Phi>>;




    // OPERATIONS CLOSED UNDER RAILCARS

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi>& q)
    {
        using F = Polynomial<T,P1lo+P2lo,P1hi+P2hi,Q1lo+Q2lo,Q1hi+Q2hi>;
        return Railcar<T,F>(std::max(p.lo, q.lo), std::min(p.hi, q.hi), p.content*q.content);
    }
    // constexpr auto operator/(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi>& q)
    // {
    //     using F = Polynomial<T,P1lo+Q2lo,P1hi+Q2hi,Q1lo+P2lo,Q1hi+P2hi>;
    //     return Railcar<T,F>(std::max(p.lo, q.lo), std::min(p.hi, q.hi), p.content/q.content);
    // }
    /*
    NOTE: we cannot support division by railcars.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 




    // OPERATIONS WITH POLYNOMIAL RAILCARS

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const PolynomialRailcar<T,P2lo,P2hi> q)
    {
        using F = RationalRailcar<T,P1lo+P2lo,P1hi+P2hi,Q1lo,Q1hi>;
        return Railcar<T,F>(p.lo, p.hi, p.content*q);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const PolynomialRailcar<T,P2lo,P2hi> q, const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p)
    {
        using F = RationalRailcar<T,P1lo+P2lo,P1hi+P2hi,Q1lo,Q1hi>;
        return Railcar<T,F>(p.lo, p.hi, q*p.content);
    }

    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    // constexpr auto operator/(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const PolynomialRailcar<T,P2lo,P2hi>& q)
    // {
    //     using F = RationalRailcar<T,P1lo+P2lo,P1hi+P2hi,Q1lo,Q1hi>;
    //     return Railcar<T,F>(p.lo, p.hi, p.content/q.content);
    // }
    /*
    NOTE: we cannot support division by railcars.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 



    // OPERATIONS WITH RATIONALS

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const Rational<T,P2lo,P2hi,Q2lo,Q2hi> q)
    {
        using F = RationalRailcar<T,P1lo+P2lo,P1hi+P2hi,Q1lo,Q1hi>;
        return Railcar<T,F>(p.lo, p.hi, p.content*q);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const Rational<T,P2lo,P2hi,Q2lo,Q2hi> q, const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p)
    {
        using F = RationalRailcar<T,P1lo+P2lo,P1hi+P2hi,Q1lo,Q1hi>;
        return Railcar<T,F>(p.lo, p.hi, q*p.content);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator/(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const Rational<T,P2lo,P2hi,Q2lo,Q2hi>& q)
    {
        using F = RationalRailcar<T,P1lo+P2lo,P1hi+P2hi,Q1lo,Q1hi>;
        return Railcar<T,F>(p.lo, p.hi, p.content/q.content);
    }
    /*
    NOTE: we cannot support division by railcars.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 


    // OPERATIONS WITH POLYNOMIALS

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const Polynomial<T,P2lo,P2hi> q)
    {
        using F = RationalRailcar<T,P1lo+P2lo,P1hi+P2hi,Q1lo,Q1hi>;
        return Railcar<T,F>(p.lo, p.hi, p.content*q);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const Polynomial<T,P2lo,P2hi> q, const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p)
    {
        using F = RationalRailcar<T,P1lo+P2lo,P1hi+P2hi,Q1lo,Q1hi>;
        return Railcar<T,F>(p.lo, p.hi, q*p.content);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const Polynomial<T,P2lo,P2hi>& q)
    {
        using F = RationalRailcar<T,P1lo+P2lo,P1hi+P2hi,Q1lo,Q1hi>;
        return Railcar<T,F>(p.lo, p.hi, p.content/q.content);
    }


    // OPERATIONS WITH SHIFTING
    

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const Shifting<T> f)
    {
        return p * Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Shifting<T> f, const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p)
    {
        return Polynomial<T,0,1>(f) * p;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const Shifting<T> f)
    {
        return p / Polynomial<T,0,1>(f);
    }


    // OPERATIONS WITH SCALING

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const Scaling<T> f)
    {
        return p * Polynomial<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Scaling<T> f, const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p)
    {
        return Polynomial<T,1,1>(f) * p;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const Scaling<T> f)
    {
        return p / Polynomial<T,1,1>(f);
    }

    // OPERATIONS WITH IDENTITY

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const Identity<T> e)
    {
        return p * Polynomial<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Identity<T> e, const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p)
    {
        return Polynomial<T,1,1>(e) * p;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const Identity<T> e)
    {
        return p / Polynomial<T,1,1>(e);
    }




    /*
    NOTE: we cannot support division by railcars.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 




    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    // constexpr auto compose(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const Polynomial<T,P2lo,P2hi> q)
    // {
    //     using F = Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)>;
    //     return Railcar<T,F>(p.lo, p.hi, compose(p.content, q));
    // }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    // constexpr auto compose(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const Scaling<T> g)
    // {
    //     using F = Polynomial<T,Plo,Phi>;
    //     return Railcar<T,F>(p.lo*g.factor, p.hi*g.factor, compose(p.content, g));
    // }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    // constexpr auto compose(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& p, const Shifting<T> g)
    // {
    //     using F = Polynomial<T,std::min(0,Plo),Phi>;
    //     return Railcar<T,F>(p.lo+g.offset, p.hi+g.offset, compose(p.content, g));
    // }


    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    auto derivative(const RationalRailcar<T,Plo,Phi,Qlo,Qhi>& r)
    {
        return RationalRailcar(r.lo, r.hi, derivative(r.content));
    }



}

