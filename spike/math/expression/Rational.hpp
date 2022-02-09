#pragma once

// in-house libraries
#include "Identity.hpp"
#include "Scaling.hpp"
#include "Shifting.hpp"
#include "Polynomial.hpp"

namespace math {

    /* 
    `Rational<Plo,Phi,Qlo,Qhi>` is a class template that represents functions of the form f(x)=p/q,
    where p,q are polynomials. It is designed for high performance applications 
    where functions must be composed from other functions.
    To address data locality concerns, the data structure is stored on the heap, 
    and users may control size in memory by specifying the highest and lowest exponent of each polynomial.
    A side effect of this design is that it allows the user to specify negative exponents,
    meaning that p and q may constitute what are known as "Laurent polynomials".
    See documentation in Polynomial.hpp for additional explanation on this design.
    */
    template<int Plo, int Phi, int Qlo, int Qhi>
    struct Rational
    {
        Polynomial<Plo,Phi> p;
        Polynomial<Qlo,Qhi> q;
        template <int P2lo, int P2hi, int Q2lo, int Q2hi> 
        constexpr explicit Rational(const Polynomial<P2lo,P2hi> p, const Polynomial<Q2lo,Q2hi> q): 
            p(p), 
            q(q)
        {}
        template <int P2lo, int P2hi, int Q2lo, int Q2hi> 
        constexpr Rational(const Rational<P2lo,P2hi,Q2lo,Q2hi>& r): 
            p(r.p), 
            q(r.q)
        {}
        constexpr explicit Rational(const Identity e): 
            p(e), 
            q()
        {
            q[0] = 1.0f;
        }
        constexpr explicit Rational(const Scaling f): 
            p(f), 
            q()
        {
            q[0] = 1.0f;
        }
        constexpr explicit Rational(const Shifting f): 
            p(f), 
            q()
        {
            q[0] = 1.0f;
        }
        template<int Nlo, int Nhi>
        constexpr explicit Rational(const Polynomial<Nlo,Nhi> p): 
            p(p), 
            q()
        {
            q[0] = 1.0f;
        }
        constexpr float operator()(const float x) const
        {
            return p(x)/q(x);
        }
        constexpr Rational<Plo,Phi,Qlo,Qhi>& operator+=(const float k)
        {
            p += k*q;
            return *this;
        }
        constexpr Rational<Plo,Phi,Qlo,Qhi>& operator-=(const float k)
        {
            p -= k*q;
            return *this;
        }
        constexpr Rational<Plo,Phi,Qlo,Qhi>& operator*=(const float k)
        {
            p *= k;
            return *this;
        }
        constexpr Rational<Plo,Phi,Qlo,Qhi>& operator/=(const float k)
        {
            p /= k;
            return *this;
        }
    };


    // an operation that defines Rationals, 
    // implemented first to simplify implementation of other operations that are closed under Rationals
    template<int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const Polynomial<Plo,Phi>& p, const Polynomial<Qlo,Qhi>& q)
    {
        return Rational<Plo,Phi,Qlo,Qhi>(p,q);
    }


    // operations that are closed under Rationals
    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator+(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        return (r.p*s.q + s.p*r.q)/(r.q*s.q);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        return (r.p*s.q - s.p*r.q)/(r.q*s.q);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        return (r.p*s.p)/(r.q*s.q);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator/(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        return (r.p*s.q)/(r.q*s.p);
    }

    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return -r.p/r.q;
    }


    // Operations that produce Rationals when given other relations as input
    // Operations with monomials,
    // implemented separately from polynomials to exploit reductions in memory footprint of output.
    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2>
    constexpr auto operator*(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, Polynomial<P2,P2> p)
    {
        return (r.p*p)/r.q;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2>
    constexpr auto operator/(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, Polynomial<P2,P2> p)
    {
        return (r.p/p)/r.q;
    }

    // Operations with polynomials
    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<P2lo,P2hi> p)
    {
        return (r.p+p*r.q) / r.q;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<P2lo,P2hi> p)
    {
        return (r.p-p*r.q) / r.q;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<P2lo,P2hi> p)
    {
        return (r.p*p) / r.q;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<P2lo,P2hi> p)
    {
        return r.p / (r.q*p);
    }

    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const Polynomial<P2lo,P2hi> p, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return (p*r.q+r.p) / r.q;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const Polynomial<P2lo,P2hi> p, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return (p*r.q-r.p) / r.q;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const Polynomial<P2lo,P2hi> p, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return (r.p*p) / r.q;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const Polynomial<P2lo,P2hi> p, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return (p*r.q)/r.p;
    }


    // Operations with scalings
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling f)
    {
        return r + Polynomial<1,1>(f);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling f)
    {
        return r - Polynomial<1,1>(f);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling f)
    {
        return r * Polynomial<1,1>(f);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling f)
    {
        return r / Polynomial<1,1>(f);
    }

    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Scaling f, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<1,1>(f) + r;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Scaling f, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<1,1>(f) - r;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Scaling f, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<1,1>(f) * r;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Scaling f, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<1,1>(f) / r;
    }


    // Operations with shiftings
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting f)
    {
        return r + Polynomial<0,1>(f);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting f)
    {
        return r - Polynomial<0,1>(f);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting f)
    {
        return r * Polynomial<0,1>(f);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting f)
    {
        return r / Polynomial<0,1>(f);
    }

    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Shifting f, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<0,1>(f) + r;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Shifting f, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<0,1>(f) - r;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Shifting f, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<0,1>(f) * r;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Shifting f, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<0,1>(f) / r;
    }



    // Operations with identity
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Identity e)
    {
        return r + Polynomial<1,1>(e);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Identity e)
    {
        return r - Polynomial<1,1>(e);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Identity e)
    {
        return r * Polynomial<1,1>(e);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const Identity e)
    {
        return r / Polynomial<1,1>(e);
    }

    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Identity e, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<1,1>(e) + r;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Identity e, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<1,1>(e) - r;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Identity e, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<1,1>(e) * r;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Identity e, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<1,1>(e) / r;
    }



    // Operations with scalars
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const float k)
    {
        return r + Polynomial<0,0>(k);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const float k)
    {
        return r - Polynomial<0,0>(k);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const float k)
    {
        return r * Polynomial<0,0>(k);
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Rational<P1lo,P1hi,Q1lo,Q1hi>& r, const float k)
    {
        return r / Polynomial<0,0>(k);
    }

    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const float k, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<0,0>(k) + r;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const float k, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<0,0>(k) - r;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const float k, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<0,0>(k) * r;
    }
    template<int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const float k, const Rational<P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<0,0>(k) / r;
    }


    // Divisions by shifting
    constexpr auto operator/(const float k, const Shifting g)
    {
        return Polynomial<0,0>(k) / Polynomial<0,1>(g);
    }
    constexpr auto operator/(const Identity e, const Shifting g)
    {
        return Polynomial<1,1>(e) / Polynomial<0,1>(g);
    }
    constexpr auto operator/(const Scaling f, const Shifting g)
    {
        return Polynomial<1,1>(f) / Polynomial<0,1>(g);
    }
    constexpr auto operator/(const Shifting f, const Shifting g)
    {
        return Polynomial<0,1>(f) / Polynomial<0,1>(g);
    }
    template<int Plo, int Phi>
    constexpr auto operator/(const Polynomial<Plo,Phi> p, const Shifting g)
    {
        return Polynomial<0,1>(p) / Polynomial<0,1>(g);
    }


    // Divisions by polynomial
    template<int Qlo, int Qhi>
    constexpr auto operator/(const float k, const Polynomial<Qlo,Qhi> q)
    {
        return Polynomial<0,0>(k) / q;
    }
    template<int Qlo, int Qhi>
    constexpr auto operator/(const Identity e, const Polynomial<Qlo,Qhi> q)
    {
        return Polynomial<1,1>(e) / q;
    }
    template<int Qlo, int Qhi>
    constexpr auto operator/(const Scaling f, const Polynomial<Qlo,Qhi> q)
    {
        return Polynomial<1,1>(f) / q;
    }
    template<int Qlo, int Qhi>
    constexpr auto operator/(const Shifting f, const Polynomial<Qlo,Qhi> q)
    {
        return Polynomial<0,1>(f) / q;
    }







    /* 
    NOTE: we avoid expressing derivatives and integrals as methods,
    since not all equations are closed under derivatives and integrals,
    and such methods would complicate the order with which classes must be declared
    */
    constexpr float derivative(const Rational<0,1,0,0> r) 
    {
        return r.p[0];
    }
    constexpr float derivative(const Rational<1,1,0,0> r) 
    {
        return 0.0f;
    }
    template<int Plo, int Phi, int Qlo, int Qhi>
    auto derivative(const Rational<Plo,Phi,Qlo,Qhi>& r)
    {
        return (r.p.derivative()*r.q + r.q.derivative()*r.p) / (r.q*r.q);
    }





    template<int Plo, int Phi, int Qlo, int Qhi, typename F>
    constexpr Rational<Plo,Phi,Qlo,Qhi> compose(const Rational<Plo,Phi,Qlo,Qhi>& r, const F& f)
    {
        return compose(r.p, f) / compose(r.q, f);
    }



    /*
    `distance` is the distance between the polynomials created when cross-multiplying rationals.
    We wish to find a distance metric for the rationals so that we can determine when:
        p/q = r/s
    Or in other words:
        ps  = rq
    The distance metric for polynomials already exists and is defined so that in this case:
        d(ps,rq) = 0
    Whereas the distance metric of the rationals must satisfy:
        d(p/q,r/s) = 0
    So we equate the two:
        d(p/q,r/s) = d(ps,rq)
    Under this definition, we immediately see that the distance for rationals inherits 
    most of the properties that are guaranteed by the distance for polynomials:
        d(p/q,r/s) ∈ ℝ⁺ 
        d(p/q,r/s) = d(r/s,p/q)
    Other properties of a metric are harder to demonstrate.
    We'll provide unit tests to see if this definition upholds properties 
    of a metric for a small set of examples, but will forego formal proof.
    */
    template<int Plo, int Phi, int Qlo, int Qhi, int Rlo, int Rhi, int Slo, int Shi>
    constexpr float distance(
        const Rational<Plo,Phi,Qlo,Qhi> r, 
        const Rational<Rlo,Rhi,Slo,Shi> s, 
        const float lo, 
        const float hi
    ){
        return distance(r.p*s.q, s.p*r.q, lo, hi);
    }
    template<int Plo, int Phi, int Qlo, int Qhi, int Nlo, int Nhi>
    constexpr float distance(
        const Rational<Plo,Phi,Qlo,Qhi> r, 
        const Polynomial<Nlo,Nhi> p, 
        const float lo, 
        const float hi
    ){
        return distance(r.p, p*r.q, lo, hi);
    }
    template<int Plo, int Phi, int Qlo, int Qhi, int Nlo, int Nhi>
    constexpr float distance(
        const Polynomial<Nlo,Nhi> p, 
        const Rational<Plo,Phi,Qlo,Qhi> r, 
        const float lo, 
        const float hi
    ){
        return distance(r.p, p*r.q, lo, hi);
    }
    template<int Plo, int Phi, int Qlo, int Qhi>
    constexpr float distance(
        const Rational<Plo,Phi,Qlo,Qhi> r, 
        const float k, 
        const float lo, 
        const float hi
    ){
        return distance(r.p, k*r.q, lo, hi);
    }
    template<int Plo, int Phi, int Qlo, int Qhi>
    constexpr float distance(
        const float k, 
        const Rational<Plo,Phi,Qlo,Qhi> r, 
        const float lo, 
        const float hi
    ){
        return distance(r.p, k*r.q, lo, hi);
    }
}