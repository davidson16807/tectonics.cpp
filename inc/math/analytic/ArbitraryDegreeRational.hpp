#pragma once

// in-house libraries
#include "Identity.hpp"
#include "Scaling.hpp"
#include "Shifting.hpp"
#include "Polynomial.hpp"
#include "Rational.hpp"
#include "ArbitraryDegreePolynomial.hpp"

namespace math {

    /* 
    `ArbitraryDegreeRational<T,I>` is a class template that represents functions of the form f(x)=p/q,
    where p,q are polynomials whose degree is not known at compile time.
    See ArbitraryDegreePolynomial.hpp for a discussion on motiviation.
    */
    template<typename T, typename I>
    struct ArbitraryDegreeRational
    {
        ArbitraryDegreePolynomial<T,I> p;
        ArbitraryDegreePolynomial<T,I> q;
        constexpr explicit ArbitraryDegreeRational(const ArbitraryDegreePolynomial<T,I>& p, const ArbitraryDegreePolynomial<T,I>& q): 
            p(p), 
            q(q)
        {}
        constexpr ArbitraryDegreeRational(const ArbitraryDegreeRational<T,I>& r): 
            p(r.p), 
            q(r.q)
        {}
        constexpr explicit ArbitraryDegreeRational(const T k): 
            p(T(k)), 
            q(T(1))
        {
        }
        constexpr explicit ArbitraryDegreeRational(const Identity<T> e): 
            p(e), 
            q(1)
        {
        }
        constexpr explicit ArbitraryDegreeRational(const Scaling<T> f): 
            p(f), 
            q(1)
        {
        }
        constexpr explicit ArbitraryDegreeRational(const Shifting<T> f): 
            p(f), 
            q(1)
        {
        }
        constexpr explicit ArbitraryDegreeRational(const ArbitraryDegreePolynomial<T,I>& p): 
            p(p), 
            q(1)
        {
        }
        template<int Plo, int Phi>
        constexpr explicit ArbitraryDegreeRational(const Polynomial<T,Plo,Phi> p): 
            p(p), 
            q(1)
        {
        }
        template<int Plo, int Phi, int Qlo, int Qhi>
        constexpr explicit ArbitraryDegreeRational(const Rational<T,Plo,Phi,Qlo,Qhi> r): 
            p(r.p), 
            q(r.q)
        {
        }
        constexpr T operator()(const T x) const
        {
            return p(x)/q(x);
        }

        ArbitraryDegreeRational<T,I>& operator+=(const T k)
        {
            p += k*q;
            return *this;
        }
        ArbitraryDegreeRational<T,I>& operator-=(const T k)
        {
            p -= k*q;
            return *this;
        }
        ArbitraryDegreeRational<T,I>& operator*=(const T k)
        {
            p *= k;
            return *this;
        }
        ArbitraryDegreeRational<T,I>& operator/=(const T k)
        {
            p /= k;
            return *this;
        }


        template<int Plo, int Phi>
        ArbitraryDegreeRational<T,I>& operator+=(const Polynomial<T,Plo,Phi>& p2)
        {
            p += p2*q;
            return *this;
        }
        template<int Plo, int Phi>
        ArbitraryDegreeRational<T,I>& operator-=(const Polynomial<T,Plo,Phi>& p2)
        {
            p -= p2*q;
            return *this;
        }
        template<int Plo, int Phi>
        ArbitraryDegreeRational<T,I>& operator*=(const Polynomial<T,Plo,Phi>& p2)
        {
            p = p * p2;
            return *this;
        }
        template<int Plo, int Phi>
        ArbitraryDegreeRational<T,I>& operator/=(const Polynomial<T,Plo,Phi>& p2)
        {
            q = q * p2;
            return *this;
        }
        template<int P>
        ArbitraryDegreeRational<T,I>& operator/=(const Polynomial<T,P,P>& p2)
        {
            p = p / p2;
            return *this;
        }


        ArbitraryDegreeRational<T,I>& operator+=(const ArbitraryDegreePolynomial<T,I>& p2)
        {
            p += p2*q;
            return *this;
        }
        ArbitraryDegreeRational<T,I>& operator-=(const ArbitraryDegreePolynomial<T,I>& p2)
        {
            p -= p2*q;
            return *this;
        }
        ArbitraryDegreeRational<T,I>& operator*=(const ArbitraryDegreePolynomial<T,I>& p2)
        {
            p = p * p2;
            return *this;
        }
        ArbitraryDegreeRational<T,I>& operator/=(const ArbitraryDegreePolynomial<T,I>& p2)
        {
            q = q * p2;
            return *this;
        }


        template<int Plo, int Phi, int Qlo, int Qhi>
        ArbitraryDegreeRational<T,I>& operator+=(const Rational<T,Plo,Phi,Qlo,Qhi>& r)
        {
            p  = r.q*p + q*r.p;
            q  = q*r.q; 
            return *this;
        }
        template<int Plo, int Phi, int Qlo, int Qhi>
        ArbitraryDegreeRational<T,I>& operator-=(const Rational<T,Plo,Phi,Qlo,Qhi>& r)
        {
            p  = r.q*p - q*r.p;
            q  = q*r.q; 
            return *this;
        }
        template<int Plo, int Phi, int Qlo, int Qhi>
        ArbitraryDegreeRational<T,I>& operator*=(const Rational<T,Plo,Phi,Qlo,Qhi>& r)
        {
            p = p * r.p;
            q = q * r.q; 
            return *this;
        }
        template<int Plo, int Phi, int Qlo, int Qhi>
        ArbitraryDegreeRational<T,I>& operator/=(const Rational<T,Plo,Phi,Qlo,Qhi>& r)
        {
            p = p * r.q;
            q = q * r.p; 
            return *this;
        }


        ArbitraryDegreeRational<T,I>& operator+=(const ArbitraryDegreeRational<T,I>& r)
        {
            p  = r.q*p + q*r.p;
            q  = q*r.q; 
            return *this;
        }
        ArbitraryDegreeRational<T,I>& operator-=(const ArbitraryDegreeRational<T,I>& r)
        {
            p  = r.q*p - q*r.p;
            q  = q*r.q; 
            return *this;
        }
        ArbitraryDegreeRational<T,I>& operator*=(const ArbitraryDegreeRational<T,I>& r)
        {
            p = p * r.p;
            q = q * r.q; 
            return *this;
        }
        ArbitraryDegreeRational<T,I>& operator/=(const ArbitraryDegreeRational<T,I>& r)
        {
            p = p * r.q;
            q = q * r.p; 
            return *this;
        }

    };

    template<typename T, typename I>
    constexpr std::string to_string(const ArbitraryDegreeRational<T,I>& r)
    {
        std::string numerator   = to_string(r.p);
        std::string denominator = to_string(r.q);
        std::size_t line_length = std::max(numerator.size(), denominator.size());
        std::string output;
        output += "\r\n";
        output += numerator;
        output += "\r\n";
        for (std::size_t i = 0; i < line_length; ++i)
        {
            output += "-";
        }
        output += "\r\n";
        output += denominator;
        output += "\r\n";
        return output;
    }

    template<typename T, typename I>
    std::ostream& operator<<(std::ostream& os, const ArbitraryDegreeRational<T,I>& p) { 
        os << to_string(p);
        return os;
    }

    // an operation that defines ArbitraryDegreeRationals, 
    // implemented first to simplify implementation of other operations that are closed under ArbitraryDegreeRationals
    template<typename T, typename I>
    constexpr auto operator/(const ArbitraryDegreePolynomial<T,I>& p, const ArbitraryDegreePolynomial<T,I>& q)
    {
        return ArbitraryDegreeRational<T,I>(p,q);
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator/(const ArbitraryDegreePolynomial<T,I>& p, const Polynomial<T,Plo,Phi>& q)
    {
        return ArbitraryDegreeRational<T,I>(p,ArbitraryDegreePolynomial<T,I>(q));
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator/(const Polynomial<T,Plo,Phi>& p, const ArbitraryDegreePolynomial<T,I>& q)
    {
        return ArbitraryDegreeRational<T,I>(ArbitraryDegreePolynomial<T,I>(p),q);
    }



    // operations that are closed under ArbitraryDegreeRationals
    template<typename T, typename I>
    constexpr auto operator-(const ArbitraryDegreeRational<T,I>& r)
    {
        return -r.p/r.q;
    }



    template<typename T, typename I>
    constexpr auto operator+(const ArbitraryDegreeRational<T,I>& r, const ArbitraryDegreeRational<T,I>& s)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y += s;
        return y;
    }
    template<typename T, typename I>
    constexpr auto operator-(const ArbitraryDegreeRational<T,I>& r, const ArbitraryDegreeRational<T,I>& s)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y -= s;
        return y;
    }
    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreeRational<T,I>& r, const ArbitraryDegreeRational<T,I>& s)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y *= s;
        return y;
    }
    template<typename T, typename I>
    constexpr auto operator/(const ArbitraryDegreeRational<T,I>& r, const ArbitraryDegreeRational<T,I>& s)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y /= s;
        return y;
    }





    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const Rational<T,Plo,Phi,Qlo,Qhi>& r, const ArbitraryDegreePolynomial<T,I>& p)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y += p;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const Rational<T,Plo,Phi,Qlo,Qhi>& r, const ArbitraryDegreePolynomial<T,I>& p)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y -= p;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Rational<T,Plo,Phi,Qlo,Qhi>& r, const ArbitraryDegreePolynomial<T,I>& p)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y *= p;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const Rational<T,Plo,Phi,Qlo,Qhi>& r, const ArbitraryDegreePolynomial<T,I>& p)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y /= p;
        return y;
    }



    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const ArbitraryDegreePolynomial<T,I>& p, const Rational<T,Plo,Phi,Qlo,Qhi>& r)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y += p;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const ArbitraryDegreePolynomial<T,I>& p, const Rational<T,Plo,Phi,Qlo,Qhi>& r)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y *= T(-1);
        y += p;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const ArbitraryDegreePolynomial<T,I>& p, const Rational<T,Plo,Phi,Qlo,Qhi>& r)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y *= p;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const ArbitraryDegreePolynomial<T,I>& p, const Rational<T,Plo,Phi,Qlo,Qhi>& r)
    {
        return (p*r.q)/r.p;
    }





    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator+(const ArbitraryDegreeRational<T,I>& r, const Polynomial<T,Plo,Phi>& p)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y += p;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator-(const ArbitraryDegreeRational<T,I>& r, const Polynomial<T,Plo,Phi>& p)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y -= p;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator*(const ArbitraryDegreeRational<T,I>& r, const Polynomial<T,Plo,Phi>& p)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y *= p;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator/(const ArbitraryDegreeRational<T,I>& r, const Polynomial<T,Plo,Phi>& p)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y /= p;
        return y;
    }



    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator+(const Polynomial<T,Plo,Phi>& p, const ArbitraryDegreeRational<T,I>& r)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y += p;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator-(const Polynomial<T,Plo,Phi>& p, const ArbitraryDegreeRational<T,I>& r)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y *= T(-1);
        y += p;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator*(const Polynomial<T,Plo,Phi>& p, const ArbitraryDegreeRational<T,I>& r)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y *= p;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator/(const Polynomial<T,Plo,Phi>& p, const ArbitraryDegreeRational<T,I>& r)
    {
        return (p*r.q)/r.p;
    }


    // Operations that produce ArbitraryDegreeRationals when given other relations as input
    // Operations with monomials,
    // implemented separately from polynomials to exploit reductions in memory footprint of output,
    // as well as to reduce likelyhood of overlow from calculating large exponents in the rational.
    template<typename T, typename I, int P2>
    constexpr auto operator*(const ArbitraryDegreeRational<T,I>& r, const Polynomial<T,P2,P2> p)
    {
        return r.p/(r.q/p);
    }
    template<typename T, typename I, int P2>
    constexpr auto operator/(const ArbitraryDegreeRational<T,I>& r, const Polynomial<T,P2,P2> p)
    {
        return (r.p/p)/r.q;
    }

    template<typename T, typename I, int P2>
    constexpr auto operator*(const Polynomial<T,P2,P2> p, const ArbitraryDegreeRational<T,I>& r)
    {
        return r.q/(r.p/p);
    }
    template<typename T, typename I, int P2>
    constexpr auto operator/(const Polynomial<T,P2,P2> p, const ArbitraryDegreeRational<T,I>& r)
    {
        return (r.q/p)/r.p;
    }











    template<typename T, typename I>
    constexpr auto operator+(const ArbitraryDegreeRational<T,I>& r, const ArbitraryDegreePolynomial<T,I>& p)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y += p;
        return y;
    }
    template<typename T, typename I>
    constexpr auto operator-(const ArbitraryDegreeRational<T,I>& r, const ArbitraryDegreePolynomial<T,I>& p)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y -= p;
        return y;
    }
    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreeRational<T,I>& r, const ArbitraryDegreePolynomial<T,I>& p)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y *= p;
        return y;
    }
    template<typename T, typename I>
    constexpr auto operator/(const ArbitraryDegreeRational<T,I>& r, const ArbitraryDegreePolynomial<T,I>& p)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y /= p;
        return y;
    }



    template<typename T, typename I>
    constexpr auto operator+(const ArbitraryDegreePolynomial<T,I>& p, const ArbitraryDegreeRational<T,I>& r)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y += p;
        return y;
    }
    template<typename T, typename I>
    constexpr auto operator-(const ArbitraryDegreePolynomial<T,I>& p, const ArbitraryDegreeRational<T,I>& r)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y *= T(-1);
        y += p;
        return y;
    }
    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreePolynomial<T,I>& p, const ArbitraryDegreeRational<T,I>& r)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y *= p;
        return y;
    }
    template<typename T, typename I>
    constexpr auto operator/(const ArbitraryDegreePolynomial<T,I>& p, const ArbitraryDegreeRational<T,I>& r)
    {
        return (p*r.q)/r.p;
    }







    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const ArbitraryDegreeRational<T,I>& r, const Rational<T,Plo,Phi,Qlo,Qhi>& s)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y += s;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const ArbitraryDegreeRational<T,I>& r, const Rational<T,Plo,Phi,Qlo,Qhi>& s)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y -= s;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const ArbitraryDegreeRational<T,I>& r, const Rational<T,Plo,Phi,Qlo,Qhi>& s)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y *= s;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const ArbitraryDegreeRational<T,I>& r, const Rational<T,Plo,Phi,Qlo,Qhi>& s)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y /= s;
        return y;
    }



    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const Rational<T,Plo,Phi,Qlo,Qhi>& s, const ArbitraryDegreeRational<T,I>& r)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y += s;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const Rational<T,Plo,Phi,Qlo,Qhi>& s, const ArbitraryDegreeRational<T,I>& r)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y *= T(-1);
        y += s;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Rational<T,Plo,Phi,Qlo,Qhi>& s, const ArbitraryDegreeRational<T,I>& r)
    {
        ArbitraryDegreeRational<T,I> y(r);
        y *= s;
        return y;
    }
    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const Rational<T,Plo,Phi,Qlo,Qhi>& s, const ArbitraryDegreeRational<T,I>& r)
    {
        return (r.p*r.q)/(s.q*r.p);
    }










    // Operations with scalings
    template<typename T, typename I>
    constexpr auto operator+(const ArbitraryDegreeRational<T,I>& r, const Scaling<T> f)
    {
        return r + Polynomial<T,1,1>(f);
    }
    template<typename T, typename I>
    constexpr auto operator-(const ArbitraryDegreeRational<T,I>& r, const Scaling<T> f)
    {
        return r - Polynomial<T,1,1>(f);
    }
    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreeRational<T,I>& r, const Scaling<T> f)
    {
        return r * Polynomial<T,1,1>(f);
    }
    template<typename T, typename I>
    constexpr auto operator/(const ArbitraryDegreeRational<T,I>& r, const Scaling<T> f)
    {
        return r / Polynomial<T,1,1>(f);
    }

    template<typename T, typename I>
    constexpr auto operator+(const Scaling<T> f, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,1,1>(f) + r;
    }
    template<typename T, typename I>
    constexpr auto operator-(const Scaling<T> f, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,1,1>(f) - r;
    }
    template<typename T, typename I>
    constexpr auto operator*(const Scaling<T> f, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,1,1>(f) * r;
    }
    template<typename T, typename I>
    constexpr auto operator/(const Scaling<T> f, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,1,1>(f) / r;
    }


    // Operations with shiftings
    template<typename T, typename I>
    constexpr auto operator+(const ArbitraryDegreeRational<T,I>& r, const Shifting<T> f)
    {
        return r + Polynomial<T,0,1>(f);
    }
    template<typename T, typename I>
    constexpr auto operator-(const ArbitraryDegreeRational<T,I>& r, const Shifting<T> f)
    {
        return r - Polynomial<T,0,1>(f);
    }
    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreeRational<T,I>& r, const Shifting<T> f)
    {
        return r * Polynomial<T,0,1>(f);
    }
    template<typename T, typename I>
    constexpr auto operator/(const ArbitraryDegreeRational<T,I>& r, const Shifting<T> f)
    {
        return r / Polynomial<T,0,1>(f);
    }

    template<typename T, typename I>
    constexpr auto operator+(const Shifting<T> f, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,0,1>(f) + r;
    }
    template<typename T, typename I>
    constexpr auto operator-(const Shifting<T> f, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,0,1>(f) - r;
    }
    template<typename T, typename I>
    constexpr auto operator*(const Shifting<T> f, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,0,1>(f) * r;
    }
    template<typename T, typename I>
    constexpr auto operator/(const Shifting<T> f, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,0,1>(f) / r;
    }



    // Operations with identity
    template<typename T, typename I>
    constexpr auto operator+(const ArbitraryDegreeRational<T,I>& r, const Identity<T> e)
    {
        return r + Polynomial<T,1,1>(e);
    }
    template<typename T, typename I>
    constexpr auto operator-(const ArbitraryDegreeRational<T,I>& r, const Identity<T> e)
    {
        return r - Polynomial<T,1,1>(e);
    }
    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreeRational<T,I>& r, const Identity<T> e)
    {
        return r * Polynomial<T,1,1>(e);
    }
    template<typename T, typename I>
    constexpr auto operator/(const ArbitraryDegreeRational<T,I>& r, const Identity<T> e)
    {
        return r / Polynomial<T,1,1>(e);
    }

    template<typename T, typename I>
    constexpr auto operator+(const Identity<T> e, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,1,1>(e) + r;
    }
    template<typename T, typename I>
    constexpr auto operator-(const Identity<T> e, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,1,1>(e) - r;
    }
    template<typename T, typename I>
    constexpr auto operator*(const Identity<T> e, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,1,1>(e) * r;
    }
    template<typename T, typename I>
    constexpr auto operator/(const Identity<T> e, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,1,1>(e) / r;
    }



    // Operations with scalars
    template<typename T, typename I>
    constexpr auto operator+(const ArbitraryDegreeRational<T,I>& r, const T k)
    {
        return r + Polynomial<T,0,0>(k);
    }
    template<typename T, typename I>
    constexpr auto operator-(const ArbitraryDegreeRational<T,I>& r, const T k)
    {
        return r - Polynomial<T,0,0>(k);
    }
    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreeRational<T,I>& r, const T k)
    {
        return r * Polynomial<T,0,0>(k);
    }
    template<typename T, typename I>
    constexpr auto operator/(const ArbitraryDegreeRational<T,I>& r, const T k)
    {
        return r / Polynomial<T,0,0>(k);
    }

    template<typename T, typename I>
    constexpr auto operator+(const T k, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,0,0>(k) + r;
    }
    template<typename T, typename I>
    constexpr auto operator-(const T k, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,0,0>(k) - r;
    }
    template<typename T, typename I>
    constexpr auto operator*(const T k, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,0,0>(k) * r;
    }
    template<typename T, typename I>
    constexpr auto operator/(const T k, const ArbitraryDegreeRational<T,I>& r)
    {
        return Polynomial<T,0,0>(k) / r;
    }


    // Divisions by shifting
    template<typename T, typename I>
    constexpr auto operator/(const ArbitraryDegreePolynomial<T,I>& p, const Shifting<T> g)
    {
        return Polynomial<T,0,1>(p) / Polynomial<T,0,1>(g);
    }


    // Divisions by polynomial
    template<typename T, typename I>
    constexpr auto operator/(const T k, const ArbitraryDegreePolynomial<T,I>& q)
    {
        return Polynomial<T,0,0>(k) / q;
    }
    template<typename T, typename I>
    constexpr auto operator/(const Identity<T> e, const ArbitraryDegreePolynomial<T,I>& q)
    {
        return Polynomial<T,1,1>(e) / q;
    }
    template<typename T, typename I>
    constexpr auto operator/(const Scaling<T> f, const ArbitraryDegreePolynomial<T,I>& q)
    {
        return Polynomial<T,1,1>(f) / q;
    }
    template<typename T, typename I>
    constexpr auto operator/(const Shifting<T> f, const ArbitraryDegreePolynomial<T,I>& q)
    {
        return Polynomial<T,0,1>(f) / q;
    }







    /* 
    NOTE: we avoid expressing derivatives and integrals as methods,
    since not all equations are closed under derivatives and integrals,
    and such methods would complicate the order with which classes must be declared
    */
    template<typename T, typename I>
    auto derivative(const ArbitraryDegreeRational<T,I>& r)
    {
        return (r.p.derivative()*r.q + r.q.derivative()*r.p) / (r.q*r.q);
    }





    template<typename T, typename I, typename F>
    constexpr ArbitraryDegreeRational<T,I> compose(const ArbitraryDegreeRational<T,I>& r, const F& f)
    {
        return compose(r.p, f) / compose(r.q, f);
    }



    /*
    `distance` is the distance between the polynomials that are created when cross-multiplying ArbitraryDegreerationals.
    We wish to find a distance metric for the ArbitraryDegreerationals so that we can determine when:
        p/q = r/s
    Or in other words:
        ps  = rq
    The distance metric for polynomials already exists and is defined so that in this case:
        d(ps,rq) = 0
    Whereas the distance metric of the ArbitraryDegreerationals (denoted d') must satisfy:
        d'(p/q,r/s) = 0
    So we equate the two:
        d'(p/q,r/s) = d(ps,rq)
    Under this definition, we see that the distance for ArbitraryDegreerationals inherits 
    most of the properties that are guaranteed by the distance for polynomials:
        d'(p/q,r/s) ∈ ℝ⁺ 
        d'(p/q,r/s) = d'(r/s,p/q)
    Other properties of a metric are harder to demonstrate.
    We'll provide unit tests to see if this definition upholds properties 
    of a metric for a small set of examples, but will forego formal proof.
    */
    template<typename T, typename I>
    constexpr T distance(
        const ArbitraryDegreeRational<T,I> r, 
        const ArbitraryDegreeRational<T,I> s, 
        const T lo, 
        const T hi
    ){
        return distance(r.p*s.q, s.p*r.q, lo, hi);
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr T distance(
        const ArbitraryDegreeRational<T,I> r, 
        const Polynomial<T,Plo,Phi> p, 
        const T lo, 
        const T hi
    ){
        return distance(r.p, p*r.q, lo, hi);
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr T distance(
        const Polynomial<T,Plo,Phi> p, 
        const ArbitraryDegreeRational<T,I> r, 
        const T lo, 
        const T hi
    ){
        return distance(r.p, p*r.q, lo, hi);
    }
    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(
        const ArbitraryDegreeRational<T,I> r, 
        const Rational<T,Plo,Phi,Qlo,Qhi> p, 
        const T lo, 
        const T hi
    ){
        return distance(r.p, p*r.q, lo, hi);
    }
    template<typename T, typename I, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(
        const Rational<T,Plo,Phi,Qlo,Qhi> p, 
        const ArbitraryDegreeRational<T,I> r, 
        const T lo, 
        const T hi
    ){
        return distance(r.p, p*r.q, lo, hi);
    }
    template<typename T, typename I>
    constexpr T distance(
        const ArbitraryDegreeRational<T,I> r, 
        const T k, 
        const T lo, 
        const T hi
    ){
        return distance(r.p, k*r.q, lo, hi);
    }
    template<typename T, typename I>
    constexpr T distance(
        const T k, 
        const ArbitraryDegreeRational<T,I> r, 
        const T lo, 
        const T hi
    ){
        return distance(r.p, k*r.q, lo, hi);
    }
}