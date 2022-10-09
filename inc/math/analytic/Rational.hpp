#pragma once

// in-house libraries
#include "Identity.hpp"
#include "Scaling.hpp"
#include "Shifting.hpp"
#include "Polynomial.hpp"

namespace analytic {

    /* 
    `Rational<T,Plo,Phi,Qlo,Qhi>` is a class template that represents functions of the form f(x)=p/q,
    where p,q are polynomials. It is designed for high performance applications 
    where functions must be composed from other functions.
    To address data locality concerns, the data structure is stored on the heap, 
    and users may control size in memory by specifying the highest and lowest exponent of each polynomial.
    A side effect of this design is that it allows the user to specify negative exponents,
    meaning that p and q may constitute what are known as "Laurent polynomials".
    See documentation in Polynomial.hpp for additional explanation on this design.
    */
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    struct Rational
    {
        Polynomial<T,Plo,Phi> p;
        Polynomial<T,Qlo,Qhi> q;
        template <int P2lo, int P2hi, int Q2lo, int Q2hi> 
        constexpr explicit Rational(const Polynomial<T,P2lo,P2hi> p, const Polynomial<T,Q2lo,Q2hi> q): 
            p(p), 
            q(q)
        {}
        // zero constructor
        constexpr explicit Rational(): 
            p(), 
            q()
        {
            p[0] = T(0);
            q[0] = T(1);
        }
        // constant constructor
        constexpr explicit Rational(const T k): 
            p(), 
            q()
        {
            p[0] = k;
            q[0] = T(1);
        }
        // copy constructor
        template <int P2lo, int P2hi, int Q2lo, int Q2hi> 
        constexpr Rational(const Rational<T,P2lo,P2hi,Q2lo,Q2hi>& r): 
            p(r.p), 
            q(r.q)
        {}
        // cast constructors...
        constexpr explicit Rational(const Identity<T> e): 
            p(e), 
            q()
        {
            q[0] = T(1);
        }
        constexpr explicit Rational(const Scaling<T> f): 
            p(f), 
            q()
        {
            q[0] = T(1);
        }
        constexpr explicit Rational(const Shifting<T> f): 
            p(f), 
            q()
        {
            q[0] = T(1);
        }
        template<int Nlo, int Nhi>
        constexpr explicit Rational(const Polynomial<T,Nlo,Nhi> p): 
            p(p), 
            q()
        {
            q[0] = T(1);
        }
        constexpr T operator()(const T x) const
        {
            return p(x)/q(x);
        }
        constexpr Rational<T,Plo,Phi,Qlo,Qhi>& operator+=(const T k)
        {
            p += k*q;
            return *this;
        }
        constexpr Rational<T,Plo,Phi,Qlo,Qhi>& operator-=(const T k)
        {
            p -= k*q;
            return *this;
        }
        constexpr Rational<T,Plo,Phi,Qlo,Qhi>& operator*=(const T k)
        {
            p *= k;
            return *this;
        }
        constexpr Rational<T,Plo,Phi,Qlo,Qhi>& operator/=(const T k)
        {
            p /= k;
            return *this;
        }
    };


    // an operation that defines Rationals, 
    // implemented first to simplify implementation of other operations that are closed under Rationals
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const Polynomial<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        return Rational<T,Plo,Phi,Qlo,Qhi>(p,q);
    }


    // operations that are closed under Rationals
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator+(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        return (r.p*s.q + s.p*r.q)/(r.q*s.q);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        return (r.p*s.q - s.p*r.q)/(r.q*s.q);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        return (r.p*s.p)/(r.q*s.q);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator/(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        return (r.p*s.q)/(r.q*s.p);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return -r.p/r.q;
    }


    // Operations that produce Rationals when given other relations as input
    // Operations with monomials,
    // implemented separately from polynomials to exploit reductions in memory footprint of output.
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2>
    constexpr auto operator*(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2,P2> p)
    {
        return (r.p*p)/r.q;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2>
    constexpr auto operator/(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2,P2> p)
    {
        return (r.p/p)/r.q;
    }

    // Operations with polynomials
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return (r.p+p*r.q) / r.q;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return (r.p-p*r.q) / r.q;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return (r.p*p) / r.q;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return r.p / (r.q*p);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const Polynomial<T,P2lo,P2hi> p, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return (p*r.q+r.p) / r.q;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const Polynomial<T,P2lo,P2hi> p, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return (p*r.q-r.p) / r.q;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const Polynomial<T,P2lo,P2hi> p, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return (r.p*p) / r.q;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const Polynomial<T,P2lo,P2hi> p, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return (p*r.q)/r.p;
    }


    // Operations with scalings
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling<T> f)
    {
        return r + Polynomial<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling<T> f)
    {
        return r - Polynomial<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling<T> f)
    {
        return r * Polynomial<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling<T> f)
    {
        return r / Polynomial<T,1,1>(f);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Scaling<T> f, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(f) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Scaling<T> f, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(f) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Scaling<T> f, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(f) * r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Scaling<T> f, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(f) / r;
    }


    // Operations with shiftings
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting<T> f)
    {
        return r + Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting<T> f)
    {
        return r - Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting<T> f)
    {
        return r * Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting<T> f)
    {
        return r / Polynomial<T,0,1>(f);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Shifting<T> f, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,1>(f) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Shifting<T> f, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,1>(f) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Shifting<T> f, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,1>(f) * r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Shifting<T> f, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,1>(f) / r;
    }



    // Operations with identity
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Identity<T> e)
    {
        return r + Polynomial<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Identity<T> e)
    {
        return r - Polynomial<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Identity<T> e)
    {
        return r * Polynomial<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Identity<T> e)
    {
        return r / Polynomial<T,1,1>(e);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Identity<T> e, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(e) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Identity<T> e, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(e) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Identity<T> e, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(e) * r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Identity<T> e, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(e) / r;
    }



    // Operations with scalars
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const T k)
    {
        return r + Polynomial<T,0,0>(k);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const T k)
    {
        return r - Polynomial<T,0,0>(k);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const T k)
    {
        return r * Polynomial<T,0,0>(k);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const T k)
    {
        return r / Polynomial<T,0,0>(k);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const T k, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,0>(k) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const T k, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,0>(k) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const T k, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,0>(k) * r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const T k, const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,0>(k) / r;
    }


    // Divisions by shifting
    template<typename T>
    constexpr auto operator/(const T k, const Shifting<T> g)
    {
        return Polynomial<T,0,0>(k) / Polynomial<T,0,1>(g);
    }
    template<typename T>
    constexpr auto operator/(const Identity<T> e, const Shifting<T> g)
    {
        return Polynomial<T,1,1>(e) / Polynomial<T,0,1>(g);
    }
    template<typename T>
    constexpr auto operator/(const Scaling<T> f, const Shifting<T> g)
    {
        return Polynomial<T,1,1>(f) / Polynomial<T,0,1>(g);
    }
    template<typename T>
    constexpr auto operator/(const Shifting<T> f, const Shifting<T> g)
    {
        return Polynomial<T,0,1>(f) / Polynomial<T,0,1>(g);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator/(const Polynomial<T,Plo,Phi> p, const Shifting<T> g)
    {
        return Polynomial<T,0,1>(p) / Polynomial<T,0,1>(g);
    }


    // Divisions by polynomial
    template<typename T, int Qlo, int Qhi>
    constexpr auto operator/(const T k, const Polynomial<T,Qlo,Qhi> q)
    {
        return Polynomial<T,0,0>(k) / q;
    }
    template<typename T, int Qlo, int Qhi>
    constexpr auto operator/(const Identity<T> e, const Polynomial<T,Qlo,Qhi> q)
    {
        return Polynomial<T,1,1>(e) / q;
    }
    template<typename T, int Qlo, int Qhi>
    constexpr auto operator/(const Scaling<T> f, const Polynomial<T,Qlo,Qhi> q)
    {
        return Polynomial<T,1,1>(f) / q;
    }
    template<typename T, int Qlo, int Qhi>
    constexpr auto operator/(const Shifting<T> f, const Polynomial<T,Qlo,Qhi> q)
    {
        return Polynomial<T,0,1>(f) / q;
    }







    /* 
    NOTE: we avoid expressing derivatives and integrals as methods,
    since not all equations are closed under derivatives and integrals,
    and such methods would complicate the order with which classes must be declared
    */
    template<typename T>
    constexpr T derivative(const Rational<T,0,1,0,0> r) 
    {
        return r.p[0];
    }
    template<typename T>
    constexpr T derivative(const Rational<T,1,1,0,0> r) 
    {
        return T(0);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    auto derivative(const Rational<T,Plo,Phi,Qlo,Qhi>& r)
    {
        return (r.p.derivative()*r.q + r.q.derivative()*r.p) / (r.q*r.q);
    }





    template<typename T, int Plo, int Phi, int Qlo, int Qhi, typename F>
    constexpr Rational<T,Plo,Phi,Qlo,Qhi> compose(const Rational<T,Plo,Phi,Qlo,Qhi>& r, const F& f)
    {
        return compose(r.p, f) / compose(r.q, f);
    }



    /*
    `distance` is the distance between the polynomials that are created when cross-multiplying rationals.
    We wish to find a distance metric for the rationals so that we can determine when:
        p/q = r/s
    Or in other words:
        ps  = rq
    The distance metric for polynomials already exists and is defined so that in this case:
        d(ps,rq) = 0
    Whereas the distance metric of the rationals (denoted d') must satisfy:
        d'(p/q,r/s) = 0
    So we equate the two:
        d'(p/q,r/s) = d(ps,rq)
    Under this definition, we see that the distance for rationals inherits 
    most of the properties that are guaranteed by the distance for polynomials:
        d'(p/q,r/s) ≥ 0
        d'(p/q,r/s) = d'(r/s,p/q)
        d'(p/q,r/s) = 0 iff p/q = r/s
        d'(p/q,r/s) = 0 iff 2rs = r2s
    The remaining property of a metric is harder to demonstrate.
    We'll provide unit tests to see if this definition upholds properties 
    of a metric for a small set of examples, but will forego formal proof.
    We would need to demonstrate the triangle equality holds, which would imply:
        d'(p/q,r/s) ≤ d'(p/q,t/u) + d'(t/u,r/s)
    for all values p,q,r,s,t,u ∈ ℙ.
    This would imply:
        d(ps,rq)    ≤ d(pu,tq)    + d(ts,ru)  
    so that:
        ∫(ps-rq)²∂x ≤ ∫(pu-tq)²∂x + ∫(ts-ru)²∂x
        ∫(ps-rq)²∂x ≤ ∫(pu-tq)²+(ts-ru)²∂x
        ∫ p²s²-2psrq-r²q² ∂x ≤ ∫ p²u²-2putq-t²q² + t²s²-2tsru-r²u² ∂x
    */
    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Rlo, int Rhi, int Slo, int Shi>
    constexpr T distance(
        const Rational<T,Plo,Phi,Qlo,Qhi> r, 
        const Rational<T,Rlo,Rhi,Slo,Shi> s, 
        const T lo, 
        const T hi
    ){
        return distance(r.p*s.q, s.p*r.q, lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Nlo, int Nhi>
    constexpr T distance(
        const Rational<T,Plo,Phi,Qlo,Qhi> r, 
        const Polynomial<T,Nlo,Nhi> p, 
        const T lo, 
        const T hi
    ){
        return distance(r.p, p*r.q, lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Nlo, int Nhi>
    constexpr T distance(
        const Polynomial<T,Nlo,Nhi> p, 
        const Rational<T,Plo,Phi,Qlo,Qhi> r, 
        const T lo, 
        const T hi
    ){
        return distance(r.p, p*r.q, lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(
        const Rational<T,Plo,Phi,Qlo,Qhi> r, 
        const T k, 
        const T lo, 
        const T hi
    ){
        return distance(r.p, k*r.q, lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(
        const T k, 
        const Rational<T,Plo,Phi,Qlo,Qhi> r, 
        const T lo, 
        const T hi
    ){
        return distance(r.p, k*r.q, lo, hi);
    }
}