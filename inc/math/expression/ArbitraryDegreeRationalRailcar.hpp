#pragma once

// std libraries
#include <algorithm> // copy, back_inserter

// in-house libraries
#include "Identity.hpp"
#include "Scaling.hpp"
#include "Shifting.hpp"
#include "Rational.hpp"
#include "Railcar.hpp"
#include "Railyard.hpp"

namespace math {

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    using RationalRailcar = Railcar<T,Rational<T,Plo,Phi,Qlo,Qhi>>;


    // RAILCARS

    // with arbitrary degree rationals

    template<typename T, typename I, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const ArbitraryDegreeRationalRailcar<T,I>& p, const ArbitraryDegreeRational<T,I> q)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo, p.hi, p.content*q);
    }

    template<typename T, typename I, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const ArbitraryDegreeRational<T,I> q, const ArbitraryDegreeRationalRailcar<T,I>& p)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo, p.hi, q*p.content);
    }


    template<typename T, typename I, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator/(const ArbitraryDegreeRationalRailcar<T,I>& p, const ArbitraryDegreeRational<T,I> q)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo, p.hi, p.content/q);
    }

    // with arbitrary degree polynomials

    template<typename T, typename I, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const ArbitraryDegreeRationalRailcar<T,I>& p, const ArbitraryDegreePolynomial<T,I> q)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo, p.hi, p.content*q);
    }

    template<typename T, typename I, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const ArbitraryDegreePolynomial<T,I> q, const ArbitraryDegreeRationalRailcar<T,I>& p)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo, p.hi, q*p.content);
    }


    template<typename T, typename I, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator/(const ArbitraryDegreeRationalRailcar<T,I>& p, const ArbitraryDegreePolynomial<T,I> q)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo, p.hi, p.content/q);
    }

    // with rationals

    template<typename T, typename I, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const ArbitraryDegreeRationalRailcar<T,I>& p, const Rational<T,P2lo,P2hi,Q2lo,Q2hi> q)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo, p.hi, p.content*q);
    }

    template<typename T, typename I, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const Rational<T,P2lo,P2hi,Q2lo,Q2hi> q, const ArbitraryDegreeRationalRailcar<T,I>& p)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo, p.hi, q*p.content);
    }


    template<typename T, typename I, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator/(const ArbitraryDegreeRationalRailcar<T,I>& p, const Rational<T,P2lo,P2hi,Q2lo,Q2hi> q)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo, p.hi, p.content/q);
    }

    /*
    NOTE: we cannot support division by railcars.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 



    // with polynomials

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const ArbitraryDegreeRationalRailcar<T,I>& p, const Polynomial<T,P2lo,P2hi> q)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo, p.hi, p.content*q);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const Polynomial<T,P2lo,P2hi> q, const ArbitraryDegreeRationalRailcar<T,I>& p)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo, p.hi, q*p.content);
    }

    template<typename T, int P1lo, int P1hi, int Q>
    constexpr auto operator/(const ArbitraryDegreeRationalRailcar<T,I>& p, const Polynomial<T,Q,Q> q)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo, p.hi, p.content/q);
    }


    // with other railcars

    template<typename T, typename I, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const ArbitraryDegreeRationalRailcar<T,I>& p, const ArbitraryDegreeRationalRailcar<T,P2lo,P2hi> q)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(std::max(p.lo, q.lo), std::min(p.hi, q.hi), p.content*q.content);
    }


    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const ArbitraryDegreeRationalRailcar<T,I>& p, const Shifting<T> f)
    {
        return p * Rational<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Shifting<T> f, const ArbitraryDegreeRationalRailcar<T,I>& p)
    {
        return Rational<T,0,1>(f) * p;
    }



    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const ArbitraryDegreeRationalRailcar<T,I>& p, const Scaling<T> f)
    {
        return p * Rational<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Scaling<T> f, const ArbitraryDegreeRationalRailcar<T,I>& p)
    {
        return Rational<T,1,1>(f) * p;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const ArbitraryDegreeRationalRailcar<T,I>& p, const Scaling<T> f)
    {
        return p / Rational<T,1,1>(f);
    }



    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const ArbitraryDegreeRationalRailcar<T,I>& p, const Identity<T> e)
    {
        return p * Rational<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Identity<T> e, const ArbitraryDegreeRationalRailcar<T,I>& p)
    {
        return Rational<T,1,1>(e) * p;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const ArbitraryDegreeRationalRailcar<T,I>& p, const Identity<T> e)
    {
        return p / Rational<T,1,1>(e);
    }




    /*
    NOTE: we cannot support division by railcars.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 




    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto compose(const ArbitraryDegreeRationalRailcar<T,I>& p, const Scaling<T> g)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo*g.factor, p.hi*g.factor, compose(p.content, g));
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto compose(const ArbitraryDegreeRationalRailcar<T,I>& p, const Shifting<T> g)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo+g.offset, p.hi+g.offset, compose(p.content, g));
    }


    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto derivative(const ArbitraryDegreeRationalRailcar<T,I>& p)
    {
        using F = ArbitraryDegreeRational<T,I>;
        return Railcar<T,F>(p.lo, p.hi, derivative(p.content));
    }






}

