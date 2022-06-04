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

    template<typename T, typename I>
    using ArbitraryDegreePolynomialRailcar = Railcar<T,ArbitraryDegreePolynomial<T,I>>;


    // RAILCARS

    // with arbitrary degree polynomials

    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreePolynomialRailcar<T,I>& p, const ArbitraryDegreePolynomial<T,I> q)
    {
        using F = ArbitraryDegreePolynomial<T,I>;
        return Railcar<T,F>(p.lo, p.hi, p.content*q);
    }

    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreePolynomial<T,I> q, const ArbitraryDegreePolynomialRailcar<T,I>& p)
    {
        using F = ArbitraryDegreePolynomial<T,I>;
        return Railcar<T,F>(p.lo, p.hi, q*p.content);
    }



    // with other railcars

    template<typename T, int Qlo, int Qhi>
    constexpr auto operator*(const ArbitraryDegreePolynomialRailcar<T,I>& p, const ArbitraryDegreePolynomialRailcar<T,I> q)
    {
        using F = ArbitraryDegreePolynomial<T,I>;
        return Railcar<T,F>(std::max(p.lo, q.lo), std::min(p.hi, q.hi), p.content*q.content);
    }



    // with polynomials

    template<typename T, int Qlo, int Qhi>
    constexpr auto operator*(const ArbitraryDegreePolynomialRailcar<T,I>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using F = ArbitraryDegreePolynomial<T,I>;
        return Railcar<T,F>(p.lo, p.hi, p.content*q);
    }

    template<typename T, int Qlo, int Qhi>
    constexpr auto operator*(const Polynomial<T,Qlo,Qhi> q, const ArbitraryDegreePolynomialRailcar<T,I>& p)
    {
        using F = ArbitraryDegreePolynomial<T,I>;
        return Railcar<T,F>(p.lo, p.hi, q*p.content);
    }

    template<typename T, int Plo, int Phi, int Q>
    constexpr auto operator/(const ArbitraryDegreePolynomialRailcar<T,I>& p, const Polynomial<T,Q,Q> q)
    {
        using F = ArbitraryDegreePolynomial<T,I>;
        return Railcar<T,F>(p.lo, p.hi, p.content/q);
    }

    /*
    NOTE: we cannot support division by railcars.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 




    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreePolynomialRailcar<T,I>& p, const Shifting<T> f)
    {
        return p * Polynomial<T,0,1>(f);
    }
    template<typename T, typename I>
    constexpr auto operator*(const Shifting<T> f, const ArbitraryDegreePolynomialRailcar<T,I>& p)
    {
        return Polynomial<T,0,1>(f) * p;
    }



    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreePolynomialRailcar<T,I>& p, const Scaling<T> f)
    {
        return p * Polynomial<T,1,1>(f);
    }
    template<typename T, typename I>
    constexpr auto operator*(const Scaling<T> f, const ArbitraryDegreePolynomialRailcar<T,I>& p)
    {
        return Polynomial<T,1,1>(f) * p;
    }
    template<typename T, typename I>
    constexpr auto operator/(const ArbitraryDegreePolynomialRailcar<T,I>& p, const Scaling<T> f)
    {
        return p / Polynomial<T,1,1>(f);
    }



    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreePolynomialRailcar<T,I>& p, const Identity<T> e)
    {
        return p * Polynomial<T,1,1>(e);
    }
    template<typename T, typename I>
    constexpr auto operator*(const Identity<T> e, const ArbitraryDegreePolynomialRailcar<T,I>& p)
    {
        return Polynomial<T,1,1>(e) * p;
    }
    template<typename T, typename I>
    constexpr auto operator/(const ArbitraryDegreePolynomialRailcar<T,I>& p, const Identity<T> e)
    {
        return p / Polynomial<T,1,1>(e);
    }

    /*
    NOTE: we cannot support division by railcars.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 



    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto compose(const ArbitraryDegreePolynomialRailcar<T,I>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using F = ArbitraryDegreePolynomial<T,I>;
        return Railcar<T,F>(p.lo, p.hi, compose(p.content, q));
    }
    template<typename T, typename I>
    constexpr auto compose(const ArbitraryDegreePolynomialRailcar<T,I>& p, const Scaling<T> g)
    {
        using F = ArbitraryDegreePolynomial<T,I>;
        return Railcar<T,F>(p.lo*g.factor, p.hi*g.factor, compose(p.content, g));
    }
    template<typename T, typename I>
    constexpr auto compose(const ArbitraryDegreePolynomialRailcar<T,I>& p, const Shifting<T> g)
    {
        using F = ArbitraryDegreePolynomial<T,I>;
        return Railcar<T,F>(p.lo+g.offset, p.hi+g.offset, compose(p.content, g));
    }


    template<typename T, typename I>
    constexpr auto derivative(const ArbitraryDegreePolynomialRailcar<T,I>& p)
    {
        using F = Polynomial<T,std::min(0,Plo),Phi>;
        return Railcar<T,F>(p.lo, p.hi, derivative(p.content));
    }


    template<typename T, typename I>
    constexpr auto integral(const ArbitraryDegreePolynomialRailcar<T,I>& p)
    {
        using F = Polynomial<T,std::min(0,Plo),Phi>;
        const T oo = std::numeric_limits<T>::max();
        Railcar<T,F> g, gmax;
        g = Railcar<T,F>(p.lo, p.hi, integral(p.content) - integral(p.content, p.lo));
        gmax = Railcar<T,F>(g.hi, oo, F() );
        gmax.content[0] = g.content(g.hi);
        return Railyard<T,F>({g, gmax});
    }




}

