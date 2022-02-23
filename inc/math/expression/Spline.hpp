#pragma once

// std libraries
#include <algorithm> // copy, back_inserter
#include <initializer_list>
#include <iostream>

// in-house libraries
#include "Identity.hpp"
#include "Scaling.hpp"
#include "Shifting.hpp"
#include "Piecewise.hpp"
#include "Piecewise_to_string.hpp"
#include "Polynomial.hpp"
#include "Polynomial_to_string.hpp"

namespace math {

    /*
    A `Spline` in this code base represents a sum of piecewise polynomials.
    The above definition for a spline is broader than most. Unlike Ahlberg (1967),
    it does not place restrictions on the degree of the polynomial, 
    the continuity of its derivatives, or even the continuity of the function itself,
    and it allows instances to be represented by the sum of overlapping polynomial pieces.
    We adopt this definition over others since we value closure - 
    a spline defined as such is closed under addition, subtraction, multiplication, derivation, and integration,
    making it a very attractive structure to implement on a computer.
    Earlier versions of code called this entity `SumOfPiecewisePolynomials`,
    however the name was changed after its widespread usage brought about the need for brevity.
    */
    template<typename T, int Plo, int Phi>
    struct Spline {
        std::vector<Piecewise<T,Polynomial<T,Plo,Phi>>> pieces; 
        Spline(const Spline<T,Plo,Phi>& p) : pieces(p.pieces) 
        {
        }
        explicit Spline(const std::vector<Piecewise<T,Polynomial<T,Plo,Phi>>> pieces_) : pieces(pieces_) 
        {
        }
        explicit Spline(std::initializer_list<Piecewise<T,Polynomial<T,Plo,Phi>>> ts)
        {
            std::copy(ts.begin(), ts.end(), std::back_inserter(pieces));
        }
        T operator()(const T x) const
        {
            T y(0.0f);
            for (std::size_t i=0; i<pieces.size(); i++)
            {
                y += pieces[i](x);
            }
            return y;
        }

        Spline<T,Plo,Phi>& operator+=(const T k)
        {
            for (std::size_t i=0; i<pieces.size(); i++)
            {
                pieces[i].f += k;
            }
            return *this;
        }
        Spline<T,Plo,Phi>& operator-=(const T k)
        {
            for (std::size_t i=0; i<pieces.size(); i++)
            {
                pieces[i].f -= k;
            }
            return *this;
        }
        Spline<T,Plo,Phi>& operator*=(const T k)
        {
            for (std::size_t i=0; i<pieces.size(); i++)
            {
                pieces[i].f *= k;
            }
            return *this;
        }
        Spline<T,Plo,Phi>& operator/=(const T k)
        {
            for (std::size_t i=0; i<pieces.size(); i++)
            {
                pieces[i].f /= k;
            }
            return *this;
        }

        template<int Qlo, int Qhi>
        Spline<T,Plo,Phi>& operator+=(const Spline<T,Qlo,Qhi>& q)
        {
            using F = Polynomial<T,Plo,Phi>;
            using G = Piecewise<T,F>;
            for (std::size_t i=0; i<q.pieces.size(); i++)
            {
                pieces[i].push_back(G(q.pieces[i].lo, q.pieces[i].hi, F(q.pieces[i])));
            }
            return *this;
        }
        template<int Qlo, int Qhi>
        Spline<T,Plo,Phi>& operator-=(const Spline<T,Qlo,Qhi>& q)
        {
            using F = Polynomial<T,Plo,Phi>;
            using G = Piecewise<T,F>;
            for (std::size_t i=0; i<q.pieces.size(); i++)
            {
                pieces[i].push_back(G(q.pieces[i].lo, q.pieces[i].hi, F(q.pieces[i])));
            }
            return *this;
        }
    };

    template<typename T, int Plo, int Phi>
    constexpr std::string to_string(const Spline<T,Plo,Phi>& s)
    {
        std::string output("\r\n");
        for (std::size_t i=0; i<s.pieces.size(); i++)
        {
            output += std::to_string(i);
            output += ": ";
            output += to_string(s.pieces[i]);
            output += "\r\n";
        }
        return output;
    }


    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Spline<T,Plo,Phi>& f, const T k)
    {
        Spline<T,Plo,Phi> y(f);
        y += k;
        return y;
    }

    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const T k, const Spline<T,Plo,Phi>& f)
    {
        Spline<T,Plo,Phi> y(f);
        y += k;
        return y;
    }


    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Spline<T,Plo,Phi>& f, const T k)
    {
        Spline<T,Plo,Phi> y(f);
        y -= k;
        return y;
    }

    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const T k, const Spline<T,Plo,Phi>& f)
    {
        Spline<T,Plo,Phi> y(f);
        y -= k;
        return y;
    }


    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Spline<T,Plo,Phi>& f, const T k)
    {
        Spline<T,Plo,Phi> y(f);
        y *= k;
        return y;
    }

    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const T k, const Spline<T,Plo,Phi>& f)
    {
        Spline<T,Plo,Phi> y(f);
        y *= k;
        return y;
    }


    template<typename T, int Plo, int Phi>
    constexpr auto operator/(const Spline<T,Plo,Phi>& f, const T k)
    {
        Spline<T,Plo,Phi> y(f);
        y /= k;
        return y;
    }

    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Spline<T,Plo,Phi>& f)
    {
        Spline<T,Plo,Phi> y(f);
        y *= -1;
        return y;
    }








    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const Spline<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        using G = Piecewise<T,F>;

        std::vector<G> pieces;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            pieces.push_back(G(p.pieces[i].lo, p.pieces[i].hi, F(p.pieces[i].f+q)));
        }
        return Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>(pieces);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const Spline<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        using G = Piecewise<T,F>;
        std::vector<G> pieces;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            pieces.push_back(G(p.pieces[i].lo, p.pieces[i].hi, F(p.pieces[i].f-q)));
        }
        return Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>(pieces);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Spline<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        using G = Piecewise<T,F>;
        std::vector<G> pieces;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            pieces.push_back(G(p.pieces[i].lo, p.pieces[i].hi, F(p.pieces[i].f*q)));
        }
        return Spline<T,Plo+Qlo,Phi+Qhi>(pieces);
    }

    template<typename T, int Plo, int Phi, int Q>
    constexpr auto operator/(const Spline<T,Plo,Phi>& p, const Polynomial<T,Q,Q> q)
    {
        using F = Polynomial<T,Plo-Q,Phi-Q>;
        using G = Piecewise<T,F>;
        std::vector<G> pieces;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            pieces.push_back(G(p.pieces[i].lo, p.pieces[i].hi, F(p.pieces[i].f/q)));
        }
        return Spline<T,Plo-Q,Phi-Q>(pieces);
    }









    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const Polynomial<T,Qlo,Qhi> q, const Spline<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        using G = Piecewise<T,F>;
        std::vector<G> pieces;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            pieces.push_back(G(p.pieces[i].lo, p.pieces[i].hi, F(p.pieces[i].f+q)));
        }
        return Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>(pieces);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const Polynomial<T,Qlo,Qhi> q, const Spline<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        using G = Piecewise<T,F>;
        std::vector<G> pieces;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            pieces.push_back(G(p.pieces[i].lo, p.pieces[i].hi, F(p.pieces[i].f-q)));
        }
        return Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>(pieces);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Polynomial<T,Qlo,Qhi> q, const Spline<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        using G = Piecewise<T,F>;
        std::vector<G> pieces;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            pieces.push_back(G(p.pieces[i].lo, p.pieces[i].hi, F(p.pieces[i].f*q)));
        }
        return Spline<T,Plo+Qlo,Phi+Qhi>(pieces);
    }







    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const Spline<T,Plo,Phi>& p, const Spline<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        using G = Piecewise<T,F>;
        std::vector<G> pieces;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            pieces.push_back(G(p.pieces[i].lo, p.pieces[i].hi, F(p.pieces[i].f)));
        }
        for (std::size_t i=0; i<q.pieces.size(); i++)
        {
            pieces.push_back(G(q.pieces[i].lo, q.pieces[i].hi, F(q.pieces[i].f)));
        }
        return Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>(pieces);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const Spline<T,Plo,Phi>& p, const Spline<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        using G = Piecewise<T,F>;
        std::vector<G> pieces;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            pieces.push_back(G(p.pieces[i].lo, p.pieces[i].hi, F(p.pieces[i].f)));
        }
        for (std::size_t i=0; i<q.pieces.size(); i++)
        {
            pieces.push_back(G(q.pieces[i].lo, q.pieces[i].hi, F(-q.pieces[i].f)));
        }
        return Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>(pieces);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Spline<T,Plo,Phi>& p, const Spline<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        using G = Piecewise<T,F>;
        std::vector<G> pieces;
        T lo, hi;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            for (std::size_t j=0; j<q.pieces.size(); j++)
            {
                lo = std::max(p.pieces[i].lo, q.pieces[j].lo);
                hi = std::min(p.pieces[i].hi, q.pieces[j].hi);
                if (lo<hi)
                {
                    pieces.push_back(G(lo, hi, p.pieces[i].f * q.pieces[j].f));
                }
            }
        }
        return Spline<T,Plo+Qlo,Phi+Qhi>(pieces);
    }













    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Spline<T,Plo,Phi>& p, const Shifting<T> f)
    {
        return p + Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Shifting<T> f, const Spline<T,Plo,Phi>& p)
    {
        return Polynomial<T,0,1>(f) + p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Spline<T,Plo,Phi>& p, const Shifting<T> f)
    {
        return p - Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Shifting<T> f, const Spline<T,Plo,Phi>& p)
    {
        return Polynomial<T,0,1>(f) - p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Spline<T,Plo,Phi>& p, const Shifting<T> f)
    {
        return p * Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Shifting<T> f, const Spline<T,Plo,Phi>& p)
    {
        return Polynomial<T,0,1>(f) * p;
    }











    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Spline<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p + Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Scaling<T> f, const Spline<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(f) + p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Spline<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p - Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Scaling<T> f, const Spline<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(f) - p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Spline<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p * Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Scaling<T> f, const Spline<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(f) * p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator/(const Spline<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p / Polynomial<T,1,1>(f);
    }












    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Spline<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p + Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Identity<T> e, const Spline<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(e) + p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Spline<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p - Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Identity<T> e, const Spline<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(e) - p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Spline<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p * Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Identity<T> e, const Spline<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(e) * p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator/(const Spline<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p / Polynomial<T,1,1>(e);
    }












    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr Spline<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)> compose(const Spline<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using PiecewisePolynomial = Piecewise<T,Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)>>;
        std::vector<PiecewisePolynomial> pieces;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            pieces.push_back(PiecewisePolynomial(p.pieces[i].lo, p.pieces[i].hi, compose(p.pieces[i].f, q)));
        }
        return Spline(pieces);
    }
    template<typename T, int Plo, int Phi>
    constexpr Spline<T,Plo,Phi> compose(const Spline<T,Plo,Phi>& p, const Scaling<T> g)
    {
        using PiecewisePolynomial = Piecewise<T,Polynomial<T,Plo,Phi>>;
        std::vector<PiecewisePolynomial> pieces;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            pieces.push_back(PiecewisePolynomial(p.pieces[i].lo, p.pieces[i].hi, compose(p.pieces[i].f, g)));
        }
        return Spline(pieces);
    }
    template<typename T, int Plo, int Phi>
    constexpr Spline<T,Plo,Phi> compose(const Spline<T,Plo,Phi>& p, const Shifting<T> g)
    {
        using PiecewisePolynomial = Piecewise<T,Polynomial<T,Plo,Phi>>;
        std::vector<PiecewisePolynomial> pieces;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            pieces.push_back(PiecewisePolynomial(p.pieces[i].lo, p.pieces[i].hi, compose(p.pieces[i].f, g)));
        }
        return Spline(pieces);
    }
    template<typename T, int Plo, int Phi>
    constexpr Spline<T,Plo,Phi> compose(const Spline<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p;
    }


    template<typename T, int Plo, int Phi>
    constexpr Spline<T,Plo-1,Phi-1> derivative(const Spline<T,Plo,Phi>& p)
    {
        using PiecewisePolynomial = Piecewise<T,Polynomial<T,Plo-1,Phi-1>>;
        std::vector<PiecewisePolynomial> pieces;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            pieces.push_back(PiecewisePolynomial(p.pieces[i].lo, p.pieces[i].hi, derivative(p.pieces[i].f)));
        }
        return Spline(pieces);
    }

    /*
    `derivative` returns the derivative of a polynomial at a point.
    It is meant to compliment the method signature for integral(p, lo, hi)
    */
    template<typename T, int Plo, int Phi>
    T derivative(const Spline<T,Plo,Phi>& p, const T x)
    {
        T dydx(0.0f);
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            dydx += derivative(p.pieces[i], x);
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
    template<typename T, int Plo, int Phi>
    T integral(const Spline<T,Plo,Phi>& p, const T x)
    {
        T I(0.0f);
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            I += integral(p.pieces[i].f, std::clamp(x, p.pieces[i].hi, p.pieces[i].hi));
            I -= integral(p.pieces[i].f, p.pieces[i].lo);
        }
        return I;
    }

    template<typename T, int Plo, int Phi>
    T integral(const Spline<T,Plo,Phi>& p, const T hi, const T lo)
    {
        T I(0.0f);
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            I += integral(p.pieces[i].f, std::clamp(hi, p.pieces[i].hi, p.pieces[i].hi));
            I -= integral(p.pieces[i].f, std::clamp(lo, p.pieces[i].hi, p.pieces[i].hi));
        }
        return I;
    }

    template<typename T, int Plo, int Phi>
    constexpr Spline<T,Plo+1,Phi+1> integral(const Spline<T,Plo,Phi>& p)
    {
        using PiecewisePolynomial = Piecewise<T,Polynomial<T,Plo+1,Phi+1>>;
        std::vector<PiecewisePolynomial> pieces;
        PiecewisePolynomial f;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            f = PiecewisePolynomial(p.pieces[i].lo, p.pieces[i].hi, 
                    integral(p.pieces[i].f) - integral(p.pieces[i].f, p.pieces[i].lo));
            pieces.push_back(f);
            pieces.push_back(
                PiecewisePolynomial( 
                    f.hi, std::numeric_limits<T>::max(), Polynomial<T,Plo+1,Phi+1>( f.f( f.hi ) ) )
            );
        }
        return Spline(pieces);
    }




    template<typename T, int Plo, int Phi>
    constexpr Spline<T,Plo,Phi> restriction(const Spline<T,Plo,Phi>& p, const T lo, const T hi)
    {
        std::vector<Piecewise<T,Polynomial<T,Plo,Phi>>> pieces;
        for (std::size_t i=0; i<p.pieces.size(); i++)
        {
            if (lo < p.pieces[i].lo || p.pieces[i].hi <= hi)
            {
                if (pieces.size() < 1)
                {
                    pieces.push_back(
                        Piecewise<T,Polynomial<T,Plo,Phi>>( 
                            std::numeric_limits<T>::max(), p.pieces[i].lo, 
                            Polynomial<T,Plo,Phi>( p.pieces[i].f(p.pieces[i].lo) ) 
                        ));
                }
                pieces.push_back(p.pieces[i]);
            }
        }
        pieces.push_back(
            Piecewise<T,Polynomial<T,Plo,Phi>>( 
                pieces[pieces.size()-1].hi, std::numeric_limits<T>::max(), 
                Polynomial<T,Plo,Phi>( pieces[pieces.size()-1].f( pieces[pieces.size()-1].hi ) )
            ));
        return Spline(pieces);

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
        const Spline<T,Plo,Phi>& p, 
        const Spline<T,Qlo,Qhi>& q, 
        const T lo, 
        const T hi
    ){
        const auto difference = p-q;
        return std::sqrt(integral(difference*difference, lo, hi)) / (hi-lo);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const Spline<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return std::sqrt(integral((p-q)*(p-q), lo, hi)) / (hi-lo);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const Polynomial<T,Plo,Phi> p, const Spline<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return std::sqrt(integral((p-q)*(p-q), lo, hi)) / (hi-lo);
    }
    template<typename T, int Plo, int Phi>
    constexpr T distance(const Spline<T,Plo,Phi>& p, const T k, const T lo, const T hi)
    {
        return std::sqrt(integral((p-k)*(p-k), lo, hi)) / (hi-lo);
    }
    template<typename T, int Plo, int Phi>
    constexpr T distance(const T k, const Spline<T,Plo, Phi> p, const T lo, const T hi)
    {
        return std::sqrt(integral((p-k)*(p-k), lo, hi)) / (hi-lo);
    }

    /*
    `dot` is the integral of the product between the output of two functions.
    It is analogous to the dot product of vectors
    however it treats functions as vectors in a function space with infinitely many dimensions.
    */
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T dot(
        const Spline<T,Plo,Phi>& p, 
        const Spline<T,Qlo,Qhi>& q, 
        const T lo, 
        const T hi
    ){
        return integral(p*q, lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T dot(const Spline<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return integral(p*q, lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T dot(const Polynomial<T,Plo,Phi> p, const Spline<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return integral(p*q, lo, hi);
    }
    template<typename T, int Plo, int Phi>
    constexpr T dot(const Spline<T,Plo,Phi>& p, const T k, const T lo, const T hi)
    {
        return integral(p*k, lo, hi);
    }
    template<typename T, int Plo, int Phi>
    constexpr T dot(const T k, const Spline<T,Plo, Phi> p, const T lo, const T hi)
    {
        return integral(p*k, lo, hi);
    }

    /*
    `length` is the root of the dot product of a function with itself.
    It is analogous to the Euclidean length of a vector, 
    however it treats functions as vectors in a function space with infinitely many dimensions.
    */
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T length(
        const Spline<T,Plo,Phi>& p, 
        const Spline<T,Qlo,Qhi>& q, 
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
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T similarity(
        const Spline<T,Plo,Phi>& p, 
        const Spline<T,Qlo,Qhi>& q, 
        const T lo, 
        const T hi
    ){
        return dot(p,q,lo,hi) / (length(p,lo,hi)*length(q,lo,hi));
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T similarity(const Spline<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return dot(p,q,lo,hi) / (length(p,lo,hi)*length(q,lo,hi));
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T similarity(const Polynomial<T,Plo,Phi> p, const Spline<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return dot(p,q,lo,hi) / (length(p,lo,hi)*length(q,lo,hi));
    }
    template<typename T, int Plo, int Phi>
    constexpr T similarity(const Spline<T,Plo,Phi>& p, const T k, const T lo, const T hi)
    {
        return dot(p,k,lo,hi) / (length(p,lo,hi)*length(k,lo,hi));
    }
    template<typename T, int Plo, int Phi>
    constexpr T similarity(const T k, const Spline<T,Plo, Phi> p, const T lo, const T hi)
    {
        return dot(p,k,lo,hi) / (length(p,lo,hi)*length(k,lo,hi));
    }

    /*
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> max(const Polynomial<T,Plo,Phi> p, const Polynomial<T,Qlo,Qhi> q)
    {
        auto candidates = solve(p-q, 0.0f);
        std::vector<T> solutions;
        reals(candidates.begin(), candidates.end(), std::back_inserter(solutions), 1e-7);
        std::sort(solutions.begin(), solutions.end());

        T lo, hi, x;
        Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> spline;
        for(int i=0; i<=solutions.size(); i++)
        {
            lo = i<1?                 solutions[i-1] : std::numeric_limits<T>::min();
            hi = i==solutions.size()? solutions[i]   : std::numeric_limits<T>::max();
            x = (lo+hi) / 2.0f;
            spline.push_back(G(lo, hi, p(x) > q(x)? p : q));
        }
        return spline;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> min(const Polynomial<T,Plo,Phi> p, const Polynomial<T,Qlo,Qhi> q)
    {
        auto candidates = solve(p-q, 0.0f);
        std::vector<T> solutions;
        reals(candidates.begin(), candidates.end(), std::back_inserter(solutions), 1e-7);
        std::sort(solutions.begin(), solutions.end());

        T lo, hi, x;
        Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> spline;
        for(int i=0; i<=solutions.size(); i++)
        {
            lo = i<1?                 solutions[i-1] : std::numeric_limits<T>::min();
            hi = i==solutions.size()? solutions[i]   : std::numeric_limits<T>::max();
            x = (lo+hi) / T(2.0);
            spline.push_back(G(lo, hi, p(x) < q(x)? p : q));
        }
        return spline;
    }

    template<typename T, int Plo, int Phi>
    constexpr Spline<T,std::min(Plo,0),std::max(Phi,0)> max(const Polynomial<T,Plo,Phi> p, const T k)
    {
        return max(p,Polynomial<T,0,0>{k});
    }

    template<typename T, int Plo, int Phi>
    constexpr Spline<T,std::min(Plo,0),std::max(Phi,0)> max(const T k, const Polynomial<T,Plo, Phi> p)
    {
        return max(p,Polynomial<T,0,0>{k});
    }

    template<typename T, int Plo, int Phi>
    constexpr Spline<T,std::min(Plo,0),std::max(Phi,0)> max(const Polynomial<T,Plo,Phi> p, const T k)
    {
        return max(p,Polynomial<T,0,0>{k});
    }

    template<typename T, int Plo, int Phi>
    constexpr Spline<T,std::min(Plo,0),std::max(Phi,0)> max(const T k, const Polynomial<T,Plo, Phi> p)
    {
        return max(p,Polynomial<T,0,0>{k});
    }
    */
}
