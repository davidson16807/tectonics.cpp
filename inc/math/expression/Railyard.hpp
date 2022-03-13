#pragma once

// std libraries
#include <algorithm> // copy, back_inserter
#include <initializer_list>
#include <iostream>

// in-house libraries
#include "Identity.hpp"
#include "Scaling.hpp"
#include "Shifting.hpp"
#include "Railcar.hpp"
#include "Railcar_to_string.hpp"
#include "Polynomial.hpp"
#include "Polynomial_to_string.hpp"

namespace math {

    /*
    A `Railyard` is a sum of railcar functions multiplied by functions of an arbitrary type F. 
    See `Railcar` for more information.

    A railyard of a type of function F has the following properties:
    * If F is closed under multiplication with itself and scalars,
      then the railyard of F will be closed under addition, subtraction, and multiplication, with itself and scalars.
    * If F is closed under derivatives, then railyard of F will be closed under derivatives, 
      and a similar statement can be made for integrals.
    */
    template<typename T, typename F>
    struct Railyard {
        std::vector<F> terms; 
        Railyard() : terms() 
        {
        }
        Railyard(const Railyard<T,F>& p) : terms(p.terms) 
        {
        }
        explicit Railyard(const std::vector<F> terms_) : terms(terms_) 
        {
        }
        explicit Railyard(std::initializer_list<F> ts)
        {
            std::copy(ts.begin(), ts.end(), std::back_inserter(terms));
        }

        T operator()(const T x) const
        {
            T y(0.0f);
            for (std::size_t i=0; i<terms.size(); i++)
            {
                y += terms[i](x);
            }
            return y;
        }

        Railyard<T,F>& operator+=(const T k)
        {
            F f; f+=k;
            terms.push_back(f);
            return *this;
        }

        Railyard<T,F>& operator-=(const T k)
        {
            F f; f-=k;
            terms.push_back(f);
            return *this;
        }

        Railyard<T,F>& operator*=(const T k)
        {
            for (std::size_t i=0; i<terms.size(); i++)
            {
                terms[i].f *= k;
            }
            return *this;
        }

        Railyard<T,F>& operator/=(const T k)
        {
            for (std::size_t i=0; i<terms.size(); i++)
            {
                terms[i].f /= k;
            }
            return *this;
        }


        template<typename G>
        Railyard<T,F>& operator+=(const G& p)
        {
            terms.push_back(F(p));
            return *this;
        }

        template<typename G>
        Railyard<T,F>& operator-=(const G& p)
        {
            terms.push_back(F(-p));
            return *this;
        }

        template<typename G>
        Railyard<T,F>& operator+=(const Railyard<T,G>& q)
        {
            for (std::size_t i=0; i<q.terms.size(); i++)
            {
                terms.push_back(F(q.terms[i].f));
            }
            return *this;
        }

        template<typename G>
        Railyard<T,F>& operator-=(const Railyard<T,G>& q)
        {
            for (std::size_t i=0; i<q.terms.size(); i++)
            {
                terms.push_back(F(-q.terms[i].f));
            }
            return *this;
        }
    };




    template<typename T, typename F, typename G>
    constexpr auto operator-(const Railyard<T,F>& f)
    {
        Railyard<T,F> y(f);
        y *= -1;
        return y;
    }




    template<typename T, typename F>
    constexpr auto operator+(const Railyard<T,F>& f, const T k)
    {
        Railyard<T,F> y(f);
        y += g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator+(const T k, const Railyard<T,F>& f)
    {
        Railyard<T,F> y(f);
        y += g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator-(const Railyard<T,F>& f, const T k)
    {
        Railyard<T,F> y(f);
        y -= g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator-(const T k, const Railyard<T,F>& f)
    {
        Railyard<T,F> y(f);
        y *= T(-1);
        y += g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator*(const Railyard<T,F>& f, const T k)
    {
        Railyard<T,F> y(f);
        y *= k;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator*(const T k, const Railyard<T,F>& f)
    {
        Railyard<T,F> y(f);
        y *= k;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator/(const Railyard<T,F>& f, const T k)
    {
        Railyard<T,F> y(f);
        y /= k;
        return y;
    }





    template<typename T, typename F, typename G>
    constexpr auto operator+(const Railyard<T,F>& f, const G g)
    {
        Railyard<T,F> y(f);
        y += F(g);
        return y;
    }

    template<typename T, typename F, typename G>
    constexpr auto operator+(const G g, const Railyard<T,F>& f)
    {
        Railyard<T,F> y(f);
        y += F(g);
        return y;
    }

    template<typename T, typename F, typename G>
    constexpr auto operator-(const Railyard<T,F>& f, const G g)
    {
        Railyard<T,F> y(f);
        y -= F(g);
        return y;
    }

    template<typename T, typename F, typename G>
    constexpr auto operator-(const G g, const Railyard<T,F>& f)
    {
        Railyard<T,F> y(f);
        y *= T(-1);
        y += g;
        return y;
    }

    template<typename T, typename F, typename G>
    constexpr auto operator*(const Railyard<T,F>& f, const G g)
    {
        Railyard<T,F> y(f);
        y *= F(g);
        return y;
    }

    template<typename T, typename F, typename G>
    constexpr auto operator*(const G g, const Railyard<T,F>& f)
    {
        Railyard<T,F> y(f);
        y *= F(g);
        return y;
    }

    template<typename T, typename F, typename G>
    constexpr auto operator/(const Railyard<T,F>& f, const G g)
    {
        Railyard<T,F> y(f);
        y /= F(g);
        return y;
    }





    template<typename T, typename F>
    constexpr auto operator+(const Railyard<T,F>& f, const Railyard<T,F>& g)
    {
        Railyard<T,F> y(f);
        y += g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator-(const Railyard<T,F>& f, const Railyard<T,F>& g)
    {
        Railyard<T,F> y(f);
        y -= g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator-(const Railyard<T,F>& g, const Railyard<T,F>& f)
    {
        Railyard<T,F> y(f);
        y *= T(-1);
        y += g;
        return y;
    }





















    template<typename T, typename F, int Qlo, int Qhi>
    constexpr auto operator+(const Railyard<T,F>& p, const Railyard<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        using G = Railcar<T,F>;
        std::vector<G> terms;
        for (std::size_t i=0; i<p.terms.size(); i++)
        {
            terms.push_back(G(p.terms[i].lo, p.terms[i].hi, F(p.terms[i].f)));
        }
        for (std::size_t i=0; i<q.terms.size(); i++)
        {
            terms.push_back(G(q.terms[i].lo, q.terms[i].hi, F(q.terms[i].f)));
        }
        return simplify(Railyard<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>(terms));
    }

    template<typename T, typename F, int Qlo, int Qhi>
    constexpr auto operator-(const Railyard<T,F>& p, const Railyard<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        using G = Railcar<T,F>;
        std::vector<G> terms;
        for (std::size_t i=0; i<p.terms.size(); i++)
        {
            terms.push_back(G(p.terms[i].lo, p.terms[i].hi, F(p.terms[i].f)));
        }
        for (std::size_t i=0; i<q.terms.size(); i++)
        {
            terms.push_back(G(q.terms[i].lo, q.terms[i].hi, F(-q.terms[i].f)));
        }
        return simplify(Railyard<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>(terms));
    }

    template<typename T, typename F, int Qlo, int Qhi>
    constexpr auto operator*(const Railyard<T,F>& p, const Railyard<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        using G = Railcar<T,F>;
        std::vector<G> terms;
        T lo, hi;
        for (std::size_t i=0; i<p.terms.size(); i++)
        {
            for (std::size_t j=0; j<q.terms.size(); j++)
            {
                lo = std::max(p.terms[i].lo, q.terms[j].lo);
                hi = std::min(p.terms[i].hi, q.terms[j].hi);
                if (lo<hi)
                {
                    terms.push_back(G(lo, hi, p.terms[i].f * q.terms[j].f));
                }
            }
        }
        return simplify(Railyard<T,Plo+Qlo,Phi+Qhi>(terms));
    }













    template<typename T, typename F>
    constexpr auto operator+(const Railyard<T,F>& p, const Shifting<T> f)
    {
        return p + Polynomial<T,0,1>(f);
    }
    template<typename T, typename F>
    constexpr auto operator+(const Shifting<T> f, const Railyard<T,F>& p)
    {
        return Polynomial<T,0,1>(f) + p;
    }
    template<typename T, typename F>
    constexpr auto operator-(const Railyard<T,F>& p, const Shifting<T> f)
    {
        return p - Polynomial<T,0,1>(f);
    }
    template<typename T, typename F>
    constexpr auto operator-(const Shifting<T> f, const Railyard<T,F>& p)
    {
        return Polynomial<T,0,1>(f) - p;
    }
    template<typename T, typename F>
    constexpr auto operator*(const Railyard<T,F>& p, const Shifting<T> f)
    {
        return p * Polynomial<T,0,1>(f);
    }
    template<typename T, typename F>
    constexpr auto operator*(const Shifting<T> f, const Railyard<T,F>& p)
    {
        return Polynomial<T,0,1>(f) * p;
    }











    template<typename T, typename F>
    constexpr auto operator+(const Railyard<T,F>& p, const Scaling<T> f)
    {
        return p + Polynomial<T,1,1>(f);
    }
    template<typename T, typename F>
    constexpr auto operator+(const Scaling<T> f, const Railyard<T,F>& p)
    {
        return Polynomial<T,1,1>(f) + p;
    }
    template<typename T, typename F>
    constexpr auto operator-(const Railyard<T,F>& p, const Scaling<T> f)
    {
        return p - Polynomial<T,1,1>(f);
    }
    template<typename T, typename F>
    constexpr auto operator-(const Scaling<T> f, const Railyard<T,F>& p)
    {
        return Polynomial<T,1,1>(f) - p;
    }
    template<typename T, typename F>
    constexpr auto operator*(const Railyard<T,F>& p, const Scaling<T> f)
    {
        return p * Polynomial<T,1,1>(f);
    }
    template<typename T, typename F>
    constexpr auto operator*(const Scaling<T> f, const Railyard<T,F>& p)
    {
        return Polynomial<T,1,1>(f) * p;
    }
    template<typename T, typename F>
    constexpr auto operator/(const Railyard<T,F>& p, const Scaling<T> f)
    {
        return p / Polynomial<T,1,1>(f);
    }












    template<typename T, typename F>
    constexpr auto operator+(const Railyard<T,F>& p, const Identity<T> e)
    {
        return p + Polynomial<T,1,1>(e);
    }
    template<typename T, typename F>
    constexpr auto operator+(const Identity<T> e, const Railyard<T,F>& p)
    {
        return Polynomial<T,1,1>(e) + p;
    }
    template<typename T, typename F>
    constexpr auto operator-(const Railyard<T,F>& p, const Identity<T> e)
    {
        return p - Polynomial<T,1,1>(e);
    }
    template<typename T, typename F>
    constexpr auto operator-(const Identity<T> e, const Railyard<T,F>& p)
    {
        return Polynomial<T,1,1>(e) - p;
    }
    template<typename T, typename F>
    constexpr auto operator*(const Railyard<T,F>& p, const Identity<T> e)
    {
        return p * Polynomial<T,1,1>(e);
    }
    template<typename T, typename F>
    constexpr auto operator*(const Identity<T> e, const Railyard<T,F>& p)
    {
        return Polynomial<T,1,1>(e) * p;
    }
    template<typename T, typename F>
    constexpr auto operator/(const Railyard<T,F>& p, const Identity<T> e)
    {
        return p / Polynomial<T,1,1>(e);
    }












    template<typename T, typename F, int Qlo, int Qhi>
    constexpr Railyard<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)> compose(const Railyard<T,F>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using PiecewisePolynomial = Railcar<T,Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)>>;
        std::vector<PiecewisePolynomial> terms;
        for (std::size_t i=0; i<p.terms.size(); i++)
        {
            terms.push_back(PiecewisePolynomial(p.terms[i].lo, p.terms[i].hi, compose(p.terms[i].f, q)));
        }
        return Railyard(terms);
    }
    template<typename T, typename F>
    constexpr Railyard<T,F> compose(const Railyard<T,F>& p, const Scaling<T> g)
    {
        using PiecewisePolynomial = F;
        std::vector<PiecewisePolynomial> terms;
        for (std::size_t i=0; i<p.terms.size(); i++)
        {
            terms.push_back(PiecewisePolynomial(p.terms[i].lo, p.terms[i].hi, compose(p.terms[i].f, g)));
        }
        return Railyard(terms);
    }
    template<typename T, typename F>
    constexpr Railyard<T,F> compose(const Railyard<T,F>& p, const Shifting<T> g)
    {
        using PiecewisePolynomial = F;
        std::vector<PiecewisePolynomial> terms;
        for (std::size_t i=0; i<p.terms.size(); i++)
        {
            terms.push_back(PiecewisePolynomial(p.terms[i].lo, p.terms[i].hi, compose(p.terms[i].f, g)));
        }
        return Railyard(terms);
    }
    template<typename T, typename F>
    constexpr Railyard<T,F> compose(const Railyard<T,F>& p, const Identity<T> e)
    {
        return p;
    }


    template<typename T, typename F>
    constexpr Railyard<T,Plo-1,Phi-1> derivative(const Railyard<T,F>& p)
    {
        using PiecewisePolynomial = Railcar<T,Polynomial<T,Plo-1,Phi-1>>;
        std::vector<PiecewisePolynomial> terms;
        for (std::size_t i=0; i<p.terms.size(); i++)
        {
            terms.push_back(PiecewisePolynomial(p.terms[i].lo, p.terms[i].hi, derivative(p.terms[i].f)));
        }
        return Railyard(terms);
    }

    /*
    `derivative` returns the derivative of a polynomial at a point.
    It is meant to compliment the method signature for integral(p, lo, hi)
    */
    template<typename T, typename F>
    T derivative(const Railyard<T,F>& p, const T x)
    {
        T dydx(0.0f);
        for (std::size_t i=0; i<p.terms.size(); i++)
        {
            dydx += derivative(p.terms[i], x);
        }
        return dydx;
    }

    /*
    `integral` returns the definite integral of a polynomial 
    without representing the integral as its own function object.
    This is meant to be used as a fallback in the event the function is a 
    Laurent polynomial with a coefficient of degree -1, 
    since the integral for such a function includes a logarithmic term.
    */
    template<typename T, typename F>
    T integral(const Railyard<T,F>& p, const T x)
    {
        T I(0.0f);
        for (std::size_t i=0; i<p.terms.size(); i++)
        {
            if (p.terms[i].lo < x)
            {
                I += integral(p.terms[i].f, std::min(x, p.terms[i].hi)) 
                   - integral(p.terms[i].f, p.terms[i].lo);
            }
        }
        return I;
    }

    template<typename T, typename F>
    T integral(const Railyard<T,F>& p, const T lo, const T hi)
    {
        T I(0.0f);
        for (std::size_t i=0; i<p.terms.size(); i++)
        {
            /*
            Q: Why do we check for lo < p.terms[i].hi?
            A: If p.terms[i].hi < lo, then p.terms[i].hi < hi as well, 
               so we know the difference in the integral between lo and hi is 0.
               However the result of the integral may still be big,
               so we could be trying to find a nonexistant difference between two big numbers.
               To avoid destructive cancellation we do not calculate.
               Similar statements could be made for p.terms[i].lo < hi.
            */
            if (lo < p.terms[i].hi && p.terms[i].lo < hi)
            {
                I += integral(p.terms[i].f, std::min(hi, p.terms[i].hi)) 
                   - integral(p.terms[i].f, std::max(lo, p.terms[i].lo));
            }
        }
        return I;
    }

    template<typename T, typename F>
    constexpr Railyard<T,Plo+1,Phi+1> integral(const Railyard<T,F>& p)
    {
        using F = Polynomial<T,Plo+1,Phi+1>;
        using G = Railcar<T,F>;
        const T oo = std::numeric_limits<T>::max();
        std::vector<G> terms;
        G g, gmax;
        for (std::size_t i=0; i<p.terms.size(); i++)
        {
            g = G(p.terms[i].lo, p.terms[i].hi, 
                    integral(p.terms[i].f) - integral(p.terms[i].f, p.terms[i].lo));
            gmax = G(g.hi, oo, F() );
            gmax.f[0] = g.f(g.hi);
            terms.push_back(g);
            terms.push_back(gmax);
        }
        return Railyard(terms);
    }




    template<typename T, typename F>
    constexpr Railyard<T,F> restriction(const Railyard<T,F>& p, const T lo, const T hi)
    {
        using F = Polynomial<T,Plo,Phi>;
        using G = Railcar<T,F>;
        std::vector<G> terms;
        const T oo = std::numeric_limits<T>::max();
        for (std::size_t i=0; i<p.terms.size(); i++)
        {
            if (lo < p.terms[i].lo || p.terms[i].hi <= hi)
            {
                if (terms.size() < 1)
                {
                    terms.push_back( G(oo, p.terms[i].lo, F( p.terms[i].f(p.terms[i].lo) ) ));
                }
                terms.push_back(p.terms[i]);
            }
        }
        terms.push_back(
            F( 
                terms[terms.size()-1].hi, std::numeric_limits<T>::max(), 
                Polynomial<T,Plo,Phi>( terms[terms.size()-1].f( terms[terms.size()-1].hi ) )
            ));
        return Railyard(terms);

    }


    template<typename T, typename F>
    constexpr Railyard<T,Plo*2,Phi*2> square(const Railyard<T,F>& p){
        return p*p;
        // Railyard<T,F> q = simplify(p);
        // using F = Polynomial<T,Plo*2,Phi*2>;
        // using G = Railcar<T,F>;
        // std::vector<G> terms;
        // for (std::size_t i = 0; i < q.terms.size(); ++i)
        // {
        //     terms.push_back(G(q.terms[i].lo, q.terms[i].hi, q.terms[i].f*q.terms[i].f));
        // }
        // return Railyard<T,Plo*2,Phi*2>(terms);
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
    template<typename T, typename F, int Qlo, int Qhi>
    constexpr T distance(
        const Railyard<T,F>& p, 
        const Railyard<T,Qlo,Qhi>& q, 
        const T lo, 
        const T hi
    ){
        const auto difference = p-q;
        return std::sqrt(std::max(T(0), integral(square(difference), lo, hi))) / (hi-lo);
    }
    template<typename T, typename F, int Qlo, int Qhi>
    constexpr T distance(const Railyard<T,F>& p, const Polynomial<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(square(p-q), lo, hi))) / (hi-lo);
    }
    template<typename T, typename F, int Qlo, int Qhi>
    constexpr T distance(const Polynomial<T,Plo,Phi> p, const Railyard<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(square(p-q), lo, hi))) / (hi-lo);
    }
    template<typename T, typename F>
    constexpr T distance(const Railyard<T,F>& p, const T k, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(square(p-k), lo, hi))) / (hi-lo);
    }
    template<typename T, typename F>
    constexpr T distance(const T k, const Railyard<T,Plo, Phi> p, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(square(p-k), lo, hi))) / (hi-lo);
    }

    /*
    `dot` is the integral of the product between the output of two functions.
    It is analogous to the dot product of vectors
    however it treats functions as vectors in a function space with infinitely many dimensions.
    */
    template<typename T, typename F, int Qlo, int Qhi>
    constexpr T dot(
        const Railyard<T,F>& p, 
        const Railyard<T,Qlo,Qhi>& q, 
        const T lo, 
        const T hi
    ){
        return integral(p*q, lo, hi);
    }
    template<typename T, typename F, int Qlo, int Qhi>
    constexpr T dot(const Railyard<T,F>& p, const Polynomial<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return integral(p*q, lo, hi);
    }
    template<typename T, typename F, int Qlo, int Qhi>
    constexpr T dot(const Polynomial<T,Plo,Phi> p, const Railyard<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return integral(p*q, lo, hi);
    }
    template<typename T, typename F>
    constexpr T dot(const Railyard<T,F>& p, const T k, const T lo, const T hi)
    {
        return integral(p*k, lo, hi);
    }
    template<typename T, typename F>
    constexpr T dot(const T k, const Railyard<T,Plo, Phi> p, const T lo, const T hi)
    {
        return integral(p*k, lo, hi);
    }

    /*
    `length` is the root of the dot product of a function with itself.
    It is analogous to the Euclidean length of a vector, 
    however it treats functions as vectors in a function space with infinitely many dimensions.
    */
    template<typename T, typename F, int Qlo, int Qhi>
    constexpr T length(
        const Railyard<T,F>& p, 
        const Railyard<T,Qlo,Qhi>& q, 
        const T lo, 
        const T hi
    ){
        return std::sqrt(integral(p*p, lo, hi));
    }

    /*
    `similarity` is dot product of two functions divided by their lengths.
    It is analogous to the cosine similarity between two vectors,
    however it treats functions as vectors in a function space with infinitely many dimensions.
    */
    template<typename T, typename F, int Qlo, int Qhi>
    constexpr T similarity(
        const Railyard<T,F>& p, 
        const Railyard<T,Qlo,Qhi>& q, 
        const T lo, 
        const T hi
    ){
        return dot(p,q,lo,hi) / (length(p,lo,hi)*length(q,lo,hi));
    }
    template<typename T, typename F, int Qlo, int Qhi>
    constexpr T similarity(const Railyard<T,F>& p, const Polynomial<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return dot(p,q,lo,hi) / (length(p,lo,hi)*length(q,lo,hi));
    }
    template<typename T, typename F, int Qlo, int Qhi>
    constexpr T similarity(const Polynomial<T,Plo,Phi> p, const Railyard<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return dot(p,q,lo,hi) / (length(p,lo,hi)*length(q,lo,hi));
    }
    template<typename T, typename F>
    constexpr T similarity(const Railyard<T,F>& p, const T k, const T lo, const T hi)
    {
        return dot(p,k,lo,hi) / (length(p,lo,hi)*length(k,lo,hi));
    }
    template<typename T, typename F>
    constexpr T similarity(const T k, const Railyard<T,Plo, Phi> p, const T lo, const T hi)
    {
        return dot(p,k,lo,hi) / (length(p,lo,hi)*length(k,lo,hi));
    }

    /*
    template<typename T, typename F, int Qlo, int Qhi>
    constexpr Railyard<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> max(const Polynomial<T,Plo,Phi> p, const Polynomial<T,Qlo,Qhi> q)
    {
        auto candidates = solve(p-q, 0.0f);
        std::vector<T> solutions;
        reals(candidates.begin(), candidates.end(), std::back_inserter(solutions), 1e-7);
        std::sort(solutions.begin(), solutions.end());

        T lo, hi, x;
        Railyard<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> spline;
        for(int i=0; i<=solutions.size(); i++)
        {
            lo = i<1?                 solutions[i-1] : std::numeric_limits<T>::min();
            hi = i==solutions.size()? solutions[i]   : std::numeric_limits<T>::max();
            x = (lo+hi) / 2.0f;
            spline.push_back(G(lo, hi, p(x) > q(x)? p : q));
        }
        return spline;
    }

    template<typename T, typename F, int Qlo, int Qhi>
    constexpr Railyard<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> min(const Polynomial<T,Plo,Phi> p, const Polynomial<T,Qlo,Qhi> q)
    {
        auto candidates = solve(p-q, 0.0f);
        std::vector<T> solutions;
        reals(candidates.begin(), candidates.end(), std::back_inserter(solutions), 1e-7);
        std::sort(solutions.begin(), solutions.end());

        T lo, hi, x;
        Railyard<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> spline;
        for(int i=0; i<=solutions.size(); i++)
        {
            lo = i<1?                 solutions[i-1] : std::numeric_limits<T>::min();
            hi = i==solutions.size()? solutions[i]   : std::numeric_limits<T>::max();
            x = (lo+hi) / T(2.0);
            spline.push_back(G(lo, hi, p(x) < q(x)? p : q));
        }
        return spline;
    }

    template<typename T, typename F>
    constexpr Railyard<T,std::min(Plo,0),std::max(Phi,0)> max(const Polynomial<T,Plo,Phi> p, const T k)
    {
        return max(p,Polynomial<T,0,0>{k});
    }

    template<typename T, typename F>
    constexpr Railyard<T,std::min(Plo,0),std::max(Phi,0)> max(const T k, const Polynomial<T,Plo, Phi> p)
    {
        return max(p,Polynomial<T,0,0>{k});
    }

    template<typename T, typename F>
    constexpr Railyard<T,std::min(Plo,0),std::max(Phi,0)> max(const Polynomial<T,Plo,Phi> p, const T k)
    {
        return max(p,Polynomial<T,0,0>{k});
    }

    template<typename T, typename F>
    constexpr Railyard<T,std::min(Plo,0),std::max(Phi,0)> max(const T k, const Polynomial<T,Plo, Phi> p)
    {
        return max(p,Polynomial<T,0,0>{k});
    }
    */
}
