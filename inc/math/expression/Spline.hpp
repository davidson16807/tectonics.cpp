#pragma once

// std libraries
#include <algorithm> // copy, back_inserter
#include <initializer_list>
#include <iostream>

// in-house libraries
#include "Identity.hpp"
#include "Scaling.hpp"
#include "Shifting.hpp"
#include "Boxed.hpp"
#include "Boxed_to_string.hpp"
#include "Polynomial.hpp"
#include "Polynomial_to_string.hpp"

namespace math {

    /*
    A `Spline` in this code base represents a sum of piecewise polynomials.
    The above definition for a spline is broader than most. Unlike Ahlberg (1967),
    it does not place restrictions on the degree of the polynomial, 
    the continuity of its derivatives, or even the continuity of the function itself,
    and it allows instances to be represented by the sum of overlapping boxed polynomials.
    We adopt this definition over others since we value closure - 
    a spline defined as such is closed under addition, subtraction, multiplication, derivation, and integration,
    making it a very attractive structure to implement on a computer.
    Earlier versions of code called this entity `SumOfPiecewisePolynomials`,
    however the name was changed after its widespread usage brought about the need for brevity.
    */
    template<typename T, int Plo, int Phi>
    struct Spline {
        std::vector<Boxed<T,Polynomial<T,Plo,Phi>>> boxes; 
        Spline() : boxes() 
        {
        }
        Spline(const Spline<T,Plo,Phi>& p) : boxes(p.boxes) 
        {
        }
        explicit Spline(const std::vector<Boxed<T,Polynomial<T,Plo,Phi>>> pieces_) : boxes(pieces_) 
        {
        }
        explicit Spline(std::initializer_list<Boxed<T,Polynomial<T,Plo,Phi>>> ts)
        {
            std::copy(ts.begin(), ts.end(), std::back_inserter(boxes));
        }
        T operator()(const T x) const
        {
            T y(0.0f);
            for (std::size_t i=0; i<boxes.size(); i++)
            {
                y += boxes[i](x);
            }
            return y;
        }

        Spline<T,Plo,Phi>& operator+=(const T k)
        {
            using F = Polynomial<T,Plo,Phi>;
            using G = Boxed<T,F>;
            const T oo = std::numeric_limits<T>::max();
            F f; f[0] = k;
            boxes.push_back(G(-oo, oo, f));
            return *this;
        }
        Spline<T,Plo,Phi>& operator-=(const T k)
        {
            using F = Polynomial<T,Plo,Phi>;
            using G = Boxed<T,F>;
            const T oo = std::numeric_limits<T>::max();
            F f; f[0] = -k;
            boxes.push_back(G(-oo, oo, f));
            return *this;
        }
        Spline<T,Plo,Phi>& operator*=(const T k)
        {
            for (std::size_t i=0; i<boxes.size(); i++)
            {
                boxes[i].f *= k;
            }
            return *this;
        }
        Spline<T,Plo,Phi>& operator/=(const T k)
        {
            for (std::size_t i=0; i<boxes.size(); i++)
            {
                boxes[i].f /= k;
            }
            return *this;
        }

        Spline<T,Plo,Phi>& operator+=(const Polynomial<T,Plo,Phi>& p)
        {
            using F = Polynomial<T,Plo,Phi>;
            using G = Boxed<T,F>;
            const T oo = std::numeric_limits<T>::max();
            F f(p);
            boxes.push_back(G(-oo, oo, p));
            return *this;
        }
        Spline<T,Plo,Phi>& operator-=(const Polynomial<T,Plo,Phi>& p)
        {
            using F = Polynomial<T,Plo,Phi>;
            using G = Boxed<T,F>;
            const T oo = std::numeric_limits<T>::max();
            F f(p);
            boxes.push_back(G(-oo, oo, p));
            return *this;
        }

        template<int Qlo, int Qhi>
        Spline<T,Plo,Phi>& operator+=(const Spline<T,Qlo,Qhi>& q)
        {
            using F = Polynomial<T,Plo,Phi>;
            using G = Boxed<T,F>;
            for (std::size_t i=0; i<q.boxes.size(); i++)
            {
                boxes.push_back(G(q.boxes[i].lo, q.boxes[i].hi, F(q.boxes[i].f)));
            }
            return *this;
        }
        template<int Qlo, int Qhi>
        Spline<T,Plo,Phi>& operator-=(const Spline<T,Qlo,Qhi>& q)
        {
            using F = Polynomial<T,Plo,Phi>;
            using G = Boxed<T,F>;
            for (std::size_t i=0; i<q.boxes.size(); i++)
            {
                boxes.push_back(G(q.boxes[i].lo, q.boxes[i].hi, F(-q.boxes[i].f)));
            }
            return *this;
        }
    };

    template<typename T, int Plo, int Phi>
    constexpr std::string to_string(const Spline<T,Plo,Phi>& s)
    {
        std::string output("\r\n");
        for (std::size_t i=0; i<s.boxes.size(); i++)
        {
            output += to_string(s.boxes[i]);
            output += "\r\n";
        }
        return output;
    }


    template<typename T, int Plo, int Phi>
    constexpr Spline<T,Plo,Phi> simplify(const Spline<T,Plo,Phi>& s)
    {
        // gather all boundaries for all boxes
        std::vector<T> bounds;
        for (std::size_t i=0; i<s.boxes.size(); i++)
        {
            bounds.push_back(s.boxes[i].lo);
            bounds.push_back(s.boxes[i].hi);
        }
        std::sort(bounds.begin(), bounds.end());
        auto last = std::unique(bounds.begin(), bounds.end());
        bounds.erase(last, bounds.end());

        using F = Polynomial<T,Plo,Phi>;
        using G = Boxed<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=1; i<bounds.size(); i++)
        {
            F f;
            // add together all boxes that intersect the region from bounds[i-1] to bounds[i]
            for (std::size_t j=0; j<s.boxes.size(); j++)
            {
                if (std::max(s.boxes[j].lo, bounds[i-1]) < std::min(s.boxes[j].hi, bounds[i]))
                {
                    f += s.boxes[j].f;
                }
            }
            boxes.push_back(G(bounds[i-1], bounds[i], f));
        }
        return Spline<T,Plo,Phi>(boxes);
    }




    template<typename T, int Plo, int Phi>
    constexpr Spline<T,std::min(0,Plo),std::max(0,Phi)> operator+(const Spline<T,Plo,Phi>& f, const T k)
    {
        Spline<T,std::min(0,Plo),std::max(0,Phi)> y;
        y += f;
        y += k;
        return simplify(y);
    }

    template<typename T, int Plo, int Phi>
    constexpr Spline<T,std::min(0,Plo),std::max(0,Phi)> operator+(const T k, const Spline<T,Plo,Phi>& f)
    {
        Spline<T,std::min(0,Plo),std::max(0,Phi)> y;
        y += f;
        y += k;
        return simplify(y);
    }


    template<typename T, int Plo, int Phi>
    constexpr Spline<T,std::min(0,Plo),std::max(0,Phi)> operator-(const Spline<T,Plo,Phi>& f, const T k)
    {
        Spline<T,std::min(0,Plo),std::max(0,Phi)> y;
        y += f;
        y -= k;
        return simplify(y);
    }

    template<typename T, int Plo, int Phi>
    constexpr Spline<T,std::min(0,Plo),std::max(0,Phi)> operator-(const T k, const Spline<T,Plo,Phi>& f)
    {
        Spline<T,std::min(0,Plo),std::max(0,Phi)> y;
        y -= f;
        y += k;
        return simplify(y);
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
    constexpr Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> operator+(const Spline<T,Plo,Phi>& s, const Polynomial<T,Qlo,Qhi> p)
    {
        Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> y;
        y += s;
        y += p;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> operator-(const Spline<T,Plo,Phi>& s, const Polynomial<T,Qlo,Qhi> p)
    {
        Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> y;
        y += s;
        y -= p;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Spline<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        using G = Boxed<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f*q)));
        }
        return Spline<T,Plo+Qlo,Phi+Qhi>(boxes);
    }

    template<typename T, int Plo, int Phi, int Q>
    constexpr auto operator/(const Spline<T,Plo,Phi>& p, const Polynomial<T,Q,Q> q)
    {
        using F = Polynomial<T,Plo-Q,Phi-Q>;
        using G = Boxed<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f/q)));
        }
        return Spline<T,Plo-Q,Phi-Q>(boxes);
    }









    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> operator+(const Polynomial<T,Qlo,Qhi> p, const Spline<T,Plo,Phi>& s)
    {
        Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> y;
        y += s;
        y += p;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> operator-(const Polynomial<T,Qlo,Qhi> p, const Spline<T,Plo,Phi>& s)
    {
        Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> y;
        y += p;
        y -= s;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Polynomial<T,Qlo,Qhi> p, const Spline<T,Plo,Phi>& s)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        using G = Boxed<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<s.boxes.size(); i++)
        {
            boxes.push_back(G(s.boxes[i].lo, s.boxes[i].hi, F(s.boxes[i].f*p)));
        }
        return Spline<T,Plo+Qlo,Phi+Qhi>(boxes);
    }







    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const Spline<T,Plo,Phi>& p, const Spline<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        using G = Boxed<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f)));
        }
        for (std::size_t i=0; i<q.boxes.size(); i++)
        {
            boxes.push_back(G(q.boxes[i].lo, q.boxes[i].hi, F(q.boxes[i].f)));
        }
        return simplify(Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>(boxes));
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const Spline<T,Plo,Phi>& p, const Spline<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        using G = Boxed<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f)));
        }
        for (std::size_t i=0; i<q.boxes.size(); i++)
        {
            boxes.push_back(G(q.boxes[i].lo, q.boxes[i].hi, F(-q.boxes[i].f)));
        }
        return simplify(Spline<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>(boxes));
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Spline<T,Plo,Phi>& p, const Spline<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        using G = Boxed<T,F>;
        std::vector<G> boxes;
        T lo, hi;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            for (std::size_t j=0; j<q.boxes.size(); j++)
            {
                lo = std::max(p.boxes[i].lo, q.boxes[j].lo);
                hi = std::min(p.boxes[i].hi, q.boxes[j].hi);
                if (lo<hi)
                {
                    boxes.push_back(G(lo, hi, p.boxes[i].f * q.boxes[j].f));
                }
            }
        }
        return simplify(Spline<T,Plo+Qlo,Phi+Qhi>(boxes));
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
        using PiecewisePolynomial = Boxed<T,Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)>>;
        std::vector<PiecewisePolynomial> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(PiecewisePolynomial(p.boxes[i].lo, p.boxes[i].hi, compose(p.boxes[i].f, q)));
        }
        return Spline(boxes);
    }
    template<typename T, int Plo, int Phi>
    constexpr Spline<T,Plo,Phi> compose(const Spline<T,Plo,Phi>& p, const Scaling<T> g)
    {
        using PiecewisePolynomial = Boxed<T,Polynomial<T,Plo,Phi>>;
        std::vector<PiecewisePolynomial> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(PiecewisePolynomial(p.boxes[i].lo, p.boxes[i].hi, compose(p.boxes[i].f, g)));
        }
        return Spline(boxes);
    }
    template<typename T, int Plo, int Phi>
    constexpr Spline<T,Plo,Phi> compose(const Spline<T,Plo,Phi>& p, const Shifting<T> g)
    {
        using PiecewisePolynomial = Boxed<T,Polynomial<T,Plo,Phi>>;
        std::vector<PiecewisePolynomial> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(PiecewisePolynomial(p.boxes[i].lo, p.boxes[i].hi, compose(p.boxes[i].f, g)));
        }
        return Spline(boxes);
    }
    template<typename T, int Plo, int Phi>
    constexpr Spline<T,Plo,Phi> compose(const Spline<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p;
    }


    template<typename T, int Plo, int Phi>
    constexpr Spline<T,Plo-1,Phi-1> derivative(const Spline<T,Plo,Phi>& p)
    {
        using PiecewisePolynomial = Boxed<T,Polynomial<T,Plo-1,Phi-1>>;
        std::vector<PiecewisePolynomial> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(PiecewisePolynomial(p.boxes[i].lo, p.boxes[i].hi, derivative(p.boxes[i].f)));
        }
        return Spline(boxes);
    }

    /*
    `derivative` returns the derivative of a polynomial at a point.
    It is meant to compliment the method signature for integral(p, lo, hi)
    */
    template<typename T, int Plo, int Phi>
    T derivative(const Spline<T,Plo,Phi>& p, const T x)
    {
        T dydx(0.0f);
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            dydx += derivative(p.boxes[i], x);
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
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            if (p.boxes[i].lo < x)
            {
                I += integral(p.boxes[i].f, std::min(x, p.boxes[i].hi)) 
                   - integral(p.boxes[i].f, p.boxes[i].lo);
            }
        }
        return I;
    }

    template<typename T, int Plo, int Phi>
    T integral(const Spline<T,Plo,Phi>& p, const T lo, const T hi)
    {
        T I(0.0f);
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            /*
            Q: Why do we check for lo < p.boxes[i].hi?
            A: If p.boxes[i].hi < lo, then p.boxes[i].hi < hi as well, 
               so we know the difference in the integral between lo and hi is 0.
               However the result of the integral may still be big,
               so we could be trying to find a nonexistant difference between two big numbers.
               To avoid destructive cancellation we do not calculate.
               Similar statements could be made for p.boxes[i].lo < hi.
            */
            if (lo < p.boxes[i].hi && p.boxes[i].lo < hi)
            {
                I += integral(p.boxes[i].f, std::min(hi, p.boxes[i].hi)) 
                   - integral(p.boxes[i].f, std::max(lo, p.boxes[i].lo));
            }
        }
        return I;
    }

    template<typename T, int Plo, int Phi>
    constexpr Spline<T,Plo+1,Phi+1> integral(const Spline<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,Plo+1,Phi+1>;
        using G = Boxed<T,F>;
        const T oo = std::numeric_limits<T>::max();
        std::vector<G> boxes;
        G g, gmax;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            g = G(p.boxes[i].lo, p.boxes[i].hi, 
                    integral(p.boxes[i].f) - integral(p.boxes[i].f, p.boxes[i].lo));
            gmax = G(g.hi, oo, F() );
            gmax.f[0] = g.f(g.hi);
            boxes.push_back(g);
            boxes.push_back(gmax);
        }
        return Spline(boxes);
    }




    template<typename T, int Plo, int Phi>
    constexpr Spline<T,Plo,Phi> restriction(const Spline<T,Plo,Phi>& p, const T lo, const T hi)
    {
        using F = Polynomial<T,Plo,Phi>;
        using G = Boxed<T,F>;
        std::vector<G> boxes;
        const T oo = std::numeric_limits<T>::max();
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            if (lo < p.boxes[i].lo || p.boxes[i].hi <= hi)
            {
                if (boxes.size() < 1)
                {
                    boxes.push_back( G(oo, p.boxes[i].lo, F( p.boxes[i].f(p.boxes[i].lo) ) ));
                }
                boxes.push_back(p.boxes[i]);
            }
        }
        boxes.push_back(
            Boxed<T,Polynomial<T,Plo,Phi>>( 
                boxes[boxes.size()-1].hi, std::numeric_limits<T>::max(), 
                Polynomial<T,Plo,Phi>( boxes[boxes.size()-1].f( boxes[boxes.size()-1].hi ) )
            ));
        return Spline(boxes);

    }


    template<typename T, int Plo, int Phi>
    constexpr Spline<T,Plo*2,Phi*2> square(const Spline<T,Plo,Phi>& p){
        return p*p;
        // Spline<T,Plo,Phi> q = simplify(p);
        // using F = Polynomial<T,Plo*2,Phi*2>;
        // using G = Boxed<T,F>;
        // std::vector<G> boxes;
        // for (std::size_t i = 0; i < q.boxes.size(); ++i)
        // {
        //     boxes.push_back(G(q.boxes[i].lo, q.boxes[i].hi, q.boxes[i].f*q.boxes[i].f));
        // }
        // return Spline<T,Plo*2,Phi*2>(boxes);
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
        return std::sqrt(std::max(T(0), integral(square(difference), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const Spline<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(square(p-q), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const Polynomial<T,Plo,Phi> p, const Spline<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(square(p-q), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi>
    constexpr T distance(const Spline<T,Plo,Phi>& p, const T k, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(square(p-k), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi>
    constexpr T distance(const T k, const Spline<T,Plo, Phi> p, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(square(p-k), lo, hi))) / (hi-lo);
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
