#pragma once

#include <map>         // map
#include <algorithm>   // clamp, min_element, fill, copy

// in-house libraries
#include "Identity.hpp"
#include "Scaling.hpp"
#include "Shifting.hpp"

#include <math/combinatorics.hpp>
#include <math/inspected/calculus.hpp>

namespace math {

    /* 
    `ArbitraryDegreePolynomial` is a class template that represents functions of the form f(x)=Σᵢaᵢxᵇⁱ where bᵢ∈ℤ.
    Unlike `Polynomial<T,Plo,Phi>`, it represents a polynomial whose exponents are not known at compile time.
    This situation crops up occasionally when dealing with polynomials.
    As a motivating example, calculating the `distance()` between two `RationalRailyard<T,Plo,Phi,Qlo,Qhi>` objects
    requires cross multiplying all `Rational` segments that shared an intersection, 
    but since segments were stored in a `std::vector<T>`, 
    the number of intersecting segments could not be known at compile time,
    and this meant the exponents resulting from cross multiplication could not be known at compile time, either.
    */
    template<typename T, typename I=std::int8_t>
    struct ArbitraryDegreePolynomial
    {
        std::map<I,T> k;
        constexpr ArbitraryDegreePolynomial(): k()
        {
        }
        constexpr ArbitraryDegreePolynomial(const ArbitraryDegreePolynomial& p): k(p.k)
        {
        }
        constexpr explicit ArbitraryDegreePolynomial(const std::map<I,T> k2): k(k2)
        {
        }
        constexpr explicit ArbitraryDegreePolynomial(const T k2): k{{I(0),k2}}
        {
        }
        constexpr explicit ArbitraryDegreePolynomial(const Shifting<T> f): k{{I(0),f.offset}, {I(1),T(1)}}
        {
        }
        constexpr explicit ArbitraryDegreePolynomial(const Scaling<T> f): k{{I(1),f.factor}}
        {
        }
        constexpr explicit ArbitraryDegreePolynomial(const Identity<T> e): k{{I(1),T(1)}}
        {
        }
        template<int Plo, int Phi>
        constexpr ArbitraryDegreePolynomial(const Polynomial<T,Plo,Phi>& p): k()
        {
            for (int i = Plo; i <= Phi; ++i)
            {
                k[I(i)] = p[i];
            }
        }
        // template <typename... T2> 
        // constexpr explicit ArbitraryDegreePolynomial(T2... ts) : k{ts...}
        // {}
        template<typename TIterator>
        constexpr explicit ArbitraryDegreePolynomial(TIterator first, TIterator last) : k(first, last)
        {}
        constexpr ArbitraryDegreePolynomial& operator=(const ArbitraryDegreePolynomial& p)
        {
            for (auto pair = p.k.cbegin(); pair != p.k.cend(); ++pair)
            {
                k[pair->first] = pair->second;
            }
            return *this;
        }
        template<int Plo, int Phi>
        constexpr ArbitraryDegreePolynomial& operator=(const Polynomial<T,Plo,Phi>& p)
        {
            return *this;
        }
        constexpr T operator()(const T x) const
        {
            T y(0);
            for (auto pair = k.cbegin(); pair != k.cend(); ++pair)
            {
                y += pair->second * std::pow(x, pair->first);
            }
            return y;
        }
        template<typename I2>
        constexpr T& operator[](const I2 i)
        {
            auto found = k.find(I(i));
            if (found == k.end())
            {
                found->second = T(0);
            }
            return found->second;
        }
        template<typename I2>
        constexpr T operator[](const I2 i) const
        {
            auto found = k.find(I(i));
            return found != k.cend()? found->second : T(0);
        }
        constexpr ArbitraryDegreePolynomial& operator+=(const ArbitraryDegreePolynomial<T,I>& p)
        {
            for (auto pair = p.k.cbegin(); pair != p.k.cend(); ++pair)
            {
                k[pair->first] = k[pair->first] + pair->second;
            }
            return *this;
        }
        constexpr ArbitraryDegreePolynomial& operator-=(const ArbitraryDegreePolynomial<T,I>& p)
        {
            for (auto pair = p.k.cbegin(); pair != p.k.cend(); ++pair)
            {
                k[pair->first] = k[pair->first] - pair->second;
            }
            return *this;
        }
        template<int Plo, int Phi>
        constexpr ArbitraryDegreePolynomial& operator+=(const Polynomial<T,Plo,Phi> p)
        {
            for (int i=Plo; i<=Phi; i++)
            {
                I i2(i);
                k[i2] = k.find(i2) != k.cend()? k[i2] + p.k[i] : p.k[i];
            }
            return *this;
        }
        template<int Plo, int Phi>
        constexpr ArbitraryDegreePolynomial& operator-=(const Polynomial<T,Plo,Phi> p)
        {
            for (int i=Plo; i<=Phi; i++)
            {
                I i2(i);
                k[i2] = k.find(i2) != k.cend()? k[i2] - p.k[i] : p.k[i];
            }
            return *this;
        }
        constexpr ArbitraryDegreePolynomial& operator+=(const T k2)
        {
            k[0] += k2;
            return *this;
        }
        constexpr ArbitraryDegreePolynomial& operator-=(const T k2)
        {
            k[0] -= k2;
            return *this;
        }
        constexpr ArbitraryDegreePolynomial& operator*=(const T k2)
        {
            for (auto pair = k.begin(); pair != k.cend(); ++pair)
            {
                k[pair->first] *= k2;
            }
            return *this;
        }
        constexpr ArbitraryDegreePolynomial& operator/=(const T k2)
        {
            for (auto pair = k.begin(); pair != k.cend(); ++pair)
            {
                k[pair->first] /= k2;
            }
            return *this;
        }

    };

    template<typename T, typename I>
    constexpr std::string to_string(const ArbitraryDegreePolynomial<T,I>& p)
    {
        // const std::string exponents("⁰¹²³⁴⁵⁶⁷⁸⁹");
        std::string output;
        for (auto pair1 : p.k)
        {
            I i = pair1.first;
            output += std::to_string(p[i]);
            output += i==0?             "" : "x";
            output += 0<=i&&i<=1?       "" : "^" + std::to_string(i);
            output += pair1!=p.k.end()? "" : " + ";
        }
        return output;
    }

    template<typename T, typename I>
    std::ostream& operator<<(std::ostream& os, const ArbitraryDegreePolynomial<T,I>& p) { 
        os << to_string(p);
        return os;
    }

    // operators with reals that are closed under ArbitraryDegreePolynomial<T,I> expressions
    template<typename T, typename I>
    constexpr ArbitraryDegreePolynomial<T,I> operator+(const ArbitraryDegreePolynomial<T,I>& p, const T& k)
    {
        ArbitraryDegreePolynomial<T,I> y(p);
        y[0] +=k;
        return y;
    }
    template<typename T, typename I>
    constexpr ArbitraryDegreePolynomial<T,I> operator+(const T k, const ArbitraryDegreePolynomial<T,I>& p)
    {
        ArbitraryDegreePolynomial<T,I> y(p);
        y[0] +=k;
        return y;
    }
    template<typename T, typename I>
    constexpr ArbitraryDegreePolynomial<T,I> operator-(const ArbitraryDegreePolynomial<T,I>& p, const T k)
    {
        ArbitraryDegreePolynomial<T,I> y(p);
        y[0] -=k;
        return y;
    }
    template<typename T, typename I>
    constexpr ArbitraryDegreePolynomial<T,I> operator-(const T k, const ArbitraryDegreePolynomial<T,I>& p)
    {
        ArbitraryDegreePolynomial<T,I> y(p);
        y *=-1.0f;
        y[0] += k;
        return y;
    }
    template<typename T, typename I>
    constexpr ArbitraryDegreePolynomial<T,I> operator*(const ArbitraryDegreePolynomial<T,I>& p, const T k)
    {
        ArbitraryDegreePolynomial<T,I> y(p);
        y *= k;
        return y;
    }
    template<typename T, typename I>
    constexpr ArbitraryDegreePolynomial<T,I> operator*(const T k, const ArbitraryDegreePolynomial<T,I>& p)
    {
        ArbitraryDegreePolynomial<T,I> y(p);
        y *= k;
        return y;
    }
    template<typename T, typename I>
    constexpr ArbitraryDegreePolynomial<T,I> operator/(const ArbitraryDegreePolynomial<T,I>& p, const T k)
    {
        ArbitraryDegreePolynomial<T,I> y(p);
        y /= k;
        return y;
    }
    template<typename T, typename I>
    constexpr ArbitraryDegreePolynomial<T,I> operator-(const ArbitraryDegreePolynomial<T,I>& p)
    {
        ArbitraryDegreePolynomial<T,I> y(p);
        y *=-1.0f;
        return y;
    }










    // operators that are not closed under ArbitraryDegreePolynomial<T,I> relations, but produce ArbitraryDegreePolynomial<T,> relations of some degree
    template<typename T, typename I>
    constexpr ArbitraryDegreePolynomial<T,I> operator+(const ArbitraryDegreePolynomial<T,I>& p, const ArbitraryDegreePolynomial<T,I>& q)
    {
        ArbitraryDegreePolynomial<T,I> y(p);
        y += q;
        return y;
    }
    template<typename T, typename I>
    constexpr ArbitraryDegreePolynomial<T,I> operator-(const ArbitraryDegreePolynomial<T,I>& p, const ArbitraryDegreePolynomial<T,I>& q)
    {
        ArbitraryDegreePolynomial<T,I> y(p);
        y -= q;
        return y;
    }
    template<typename T, typename I>
    constexpr ArbitraryDegreePolynomial<T,I> operator*(const ArbitraryDegreePolynomial<T,I>& p, const ArbitraryDegreePolynomial<T,I>& q)
    {
        ArbitraryDegreePolynomial<T,I> y;
        for (auto pair1 = p.k.cbegin(); pair1 != p.k.cend(); ++pair1)
        {
            I i = pair1->first;
            for (auto pair2 = q.k.cbegin(); pair2 != q.k.cend(); ++pair2)
            {
                I j = pair2->first;
                y[i+j] += p[i]*q[j];
            }
        }
        return y;
    }



    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator+(const ArbitraryDegreePolynomial<T,I>& p, const Polynomial<T,Plo,Phi>& q)
    {
        return p + ArbitraryDegreePolynomial<T,I>(q);
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator+(const Polynomial<T,Plo,Phi>& q, const ArbitraryDegreePolynomial<T,I>& p)
    {
        return ArbitraryDegreePolynomial<T,I>(q) + p;
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator-(const ArbitraryDegreePolynomial<T,I>& p, const Polynomial<T,Plo,Phi>& q)
    {
        return p - ArbitraryDegreePolynomial<T,I>(q);
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator-(const Polynomial<T,Plo,Phi>& q, const ArbitraryDegreePolynomial<T,I>& p)
    {
        return ArbitraryDegreePolynomial<T,I>(q) - p;
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator*(const ArbitraryDegreePolynomial<T,I>& p, const Polynomial<T,Plo,Phi>& q)
    {
        return p * ArbitraryDegreePolynomial<T,I>(q);
    }
    template<typename T, typename I, int Plo, int Phi>
    constexpr auto operator*(const Polynomial<T,Plo,Phi>& q, const ArbitraryDegreePolynomial<T,I>& p)
    {
        return ArbitraryDegreePolynomial<T,I>(q) * p;
    }
    template<typename T, typename I, int P>
    constexpr auto operator/(const ArbitraryDegreePolynomial<T,I>& p, const Polynomial<T,P,P> q)
    {
        ArbitraryDegreePolynomial<T,I> y;
        for (auto pair1 = p.k.cbegin(); pair1 != p.k.cend(); ++pair1)
        {
            I i = pair1->first;
            y[i-P] = p[i];
        }
        return y;
    }




    // operators with reals that produce ArbitraryDegreePolynomial<T,I> relations given other relations as input
    template<typename T, typename I>
    constexpr auto operator+(const ArbitraryDegreePolynomial<T,I>& p, const Identity<T> e)
    {
        return p + ArbitraryDegreePolynomial<T,I>(e);
    }
    template<typename T, typename I>
    constexpr auto operator+(const Identity<T> e, const ArbitraryDegreePolynomial<T,I>& p)
    {
        return ArbitraryDegreePolynomial<T,I>(e) + p;
    }
    template<typename T, typename I>
    constexpr auto operator-(const ArbitraryDegreePolynomial<T,I>& p, const Identity<T> e)
    {
        return p - ArbitraryDegreePolynomial<T,I>(e);
    }
    template<typename T, typename I>
    constexpr auto operator-(const Identity<T> e, const ArbitraryDegreePolynomial<T,I>& p)
    {
        return ArbitraryDegreePolynomial<T,I>(e) - p;
    }
    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreePolynomial<T,I>& p, const Identity<T> e)
    {
        return p * ArbitraryDegreePolynomial<T,I>(e);
    }
    template<typename T, typename I>
    constexpr auto operator*(const Identity<T> e, const ArbitraryDegreePolynomial<T,I>& p)
    {
        return ArbitraryDegreePolynomial<T,I>(e) * p;
    }
    template<typename T, typename I>
    constexpr auto operator/(const ArbitraryDegreePolynomial<T,I>& p, const Identity<T> e)
    {
        return p / Polynomial<T,1,1>(e);
    }
    


    template<typename T, typename I>
    constexpr auto operator+(const ArbitraryDegreePolynomial<T,I>& p, const Scaling<T> f)
    {
        return p + ArbitraryDegreePolynomial<T,I>(f);
    }
    template<typename T, typename I>
    constexpr auto operator+(const Scaling<T> f, const ArbitraryDegreePolynomial<T,I>& p)
    {
        return ArbitraryDegreePolynomial<T,I>(f) + p;
    }
    template<typename T, typename I>
    constexpr auto operator-(const ArbitraryDegreePolynomial<T,I>& p, const Scaling<T> f)
    {
        return p - ArbitraryDegreePolynomial<T,I>(f);
    }
    template<typename T, typename I>
    constexpr auto operator-(const Scaling<T> f, const ArbitraryDegreePolynomial<T,I>& p)
    {
        return ArbitraryDegreePolynomial<T,I>(f) - p;
    }
    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreePolynomial<T,I>& p, const Scaling<T> f)
    {
        return p * ArbitraryDegreePolynomial<T,I>(f);
    }
    template<typename T, typename I>
    constexpr auto operator*(const Scaling<T> f, const ArbitraryDegreePolynomial<T,I>& p)
    {
        return ArbitraryDegreePolynomial<T,I>(f) * p;
    }
    template<typename T, typename I>
    constexpr auto operator/(const ArbitraryDegreePolynomial<T,I>& p, const Scaling<T> q)
    {
        return p / Polynomial<T,1,1>(q);
    }



    template<typename T, typename I>
    constexpr auto operator+(const ArbitraryDegreePolynomial<T,I>& p, const Shifting<T> f)
    {
        return p + ArbitraryDegreePolynomial<T,I>(f);
    }
    template<typename T, typename I>
    constexpr auto operator+(const Shifting<T> f, const ArbitraryDegreePolynomial<T,I>& p)
    {
        return ArbitraryDegreePolynomial<T,I>(f) + p;
    }
    template<typename T, typename I>
    constexpr auto operator-(const ArbitraryDegreePolynomial<T,I>& p, const Shifting<T> f)
    {
        return p - ArbitraryDegreePolynomial<T,I>(f);
    }
    template<typename T, typename I>
    constexpr auto operator-(const Shifting<T> f, const ArbitraryDegreePolynomial<T,I>& p)
    {
        return ArbitraryDegreePolynomial<T,I>(f) - p;
    }
    template<typename T, typename I>
    constexpr auto operator*(const ArbitraryDegreePolynomial<T,I>& p, const Shifting<T> f)
    {
        return p * ArbitraryDegreePolynomial<T,I>(f);
    }
    template<typename T, typename I>
    constexpr auto operator*(const Shifting<T> f, const ArbitraryDegreePolynomial<T,I>& p)
    {
        return ArbitraryDegreePolynomial<T,I>(f) * p;
    }






















    /* 
    NOTE: we avoid expressing derivatives and integrals as methods,
    since not all equations are closed under derivatives and integrals,
    and such methods would complicate the order with which classes must be declared
    */
    template<typename T, typename I>
    ArbitraryDegreePolynomial<T,I> derivative(const ArbitraryDegreePolynomial<T,I>& p)
    {
        ArbitraryDegreePolynomial<T,I> dpdx;
        for (auto pair = p.k.cbegin(); pair != p.k.cend(); ++pair)
        {
            I i = pair->first;
            dpdx[i-1] = i!=I(1)? i*p[i] : T(0);
        }
        return dpdx;
    }

    /*
    `derivative` returns the derivative of a polynomial at a point.
    It is meant to compliment the method signature for integral(p, lo, hi)
    */
    template<typename T, typename I>
    T derivative(const ArbitraryDegreePolynomial<T,I>& p, const T x)
    {
        T dydx(0);
        for (auto pair = p.k.cbegin(); pair != p.k.cend(); ++pair)
        {
            I i = pair->first;
            dydx += p[i] * std::pow(x, i-1) * i;
        }
        return dydx;
    }

    /*
    `integral` returns a polynomial that represents the indefinite integral of `this`.
    */
    template<typename T, typename I>
    ArbitraryDegreePolynomial<T,I> integral(const ArbitraryDegreePolynomial<T,I>& p)
    {
        ArbitraryDegreePolynomial<T,I> dpdx;
        for (auto pair = p.k.cbegin(); pair != p.k.cend(); ++pair)
        {
            I i = pair->first;
            dpdx[i+1] = p[i]/(i+1);
        }
        return dpdx;
    }

    /*
    `integral` returns the definite integral of a polynomial 
    without representing the integral as its own function object.
    This is meant to be used as a fallback in the event the function is a 
    Laurent polynomial with a coefficient of degree -1, 
    since the integral for such a function includes a logarithmic term.
    */
    template<typename T, typename I>
    T integral(const ArbitraryDegreePolynomial<T,I>& p, const T x)
    {
        T integral(0);
        for (auto pair = p.k.cbegin(); pair != p.k.cend(); ++pair)
        {
            I i = pair->first;
            // exponents are calculated using pow(), 
            // rather than repeated multiplication, to avoid precision errors
            integral += i != -1? 
                p[i] * std::pow(x, i+1) / (i+1)
              : p[i] * (p[i] != T(0.0)? std::log(std::abs(x)) : T(0));
        }
        return integral;
    }

    /*
    `integral` returns the definite integral of a polynomial 
    without representing the integral as its own function object.
    This is meant to be used as a fallback in the event the function is a 
    Laurent polynomial with a coefficient of degree -1, 
    since the integral for such a function includes a logarithmic term.
    */
    template<typename T, typename I>
    T integral(const ArbitraryDegreePolynomial<T,I>& p, const T lo, const T hi)
    {
        T integral(0);

        for (auto pair = p.k.cbegin(); pair != p.k.cend(); ++pair)
        {
            I i = pair->first;
            // exponents are calculated using pow(), 
            // rather than repeated multiplication, to avoid precision errors
            integral += i != -1? 
                p[i] * (std::pow(hi, i+1) / (i+1) 
                      - std::pow(lo, i+1) / (i+1))
              : p[i] * (p[i] != T(0.0)? std::log(std::abs(hi)) - std::log(std::abs(lo)) : T(0));
        }

        return integral;
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
    Other functions satisfy these criteria and may also serve as distance metrics
    (for instance, maximum of the squared difference, or the mean of the absolute difference),
    however those we could find have no closed-form, non-piecewise solution for arbitrary polynomials 
    We do not presume that this metric serves more naturally as a distance metric compared to others,
    however we address this function as `distance` since the alternative names
    are either unwieldy (`root_mean_square_difference`) or nonobvious (`rmsd`).
    */
    template<typename T, typename I>
    constexpr T distance(const ArbitraryDegreePolynomial<T,I> p, const ArbitraryDegreePolynomial<T,I> q, const T lo, const T hi)
    {
        return std::sqrt(integral((p-q)*(p-q), lo, hi)) / (hi-lo);
    }
    template<typename T, typename I>
    constexpr T distance(const ArbitraryDegreePolynomial<T,I> p, const T k, const T lo, const T hi)
    {
        return std::sqrt(integral((p-k)*(p-k), lo, hi)) / (hi-lo);
    }
    template<typename T, typename I>
    constexpr T distance(const T k, const ArbitraryDegreePolynomial<T,I> p, const T lo, const T hi)
    {
        return std::sqrt(integral((p-k)*(p-k), lo, hi)) / (hi-lo);
    }

    template<typename T, typename I>
    constexpr ArbitraryDegreePolynomial<T,I> compose(const ArbitraryDegreePolynomial<T,I>& p, const Identity<T> e)
    {
        return p;
    }

    template<typename T, typename I>
    constexpr ArbitraryDegreePolynomial<T,I> compose(const ArbitraryDegreePolynomial<T,I>& p, const Scaling<T> g)
    {
        ArbitraryDegreePolynomial<T,I> pq = p;
        for (auto pair = p.k.cbegin(); pair != p.k.cend(); ++pair)
        {
            I i = pair->first;
            pq[i] *= std::pow(g.factor, T(i));
        }
        return pq;
    }

    template<typename T, typename I>
    constexpr ArbitraryDegreePolynomial<T,I> compose(const ArbitraryDegreePolynomial<T,I>& p, const Shifting<T> g)
    {
        ArbitraryDegreePolynomial<T,I> pq;
        for (auto pair1 = p.k.cbegin(); pair1 != p.k.cend(); ++pair1)
        {
            I i = pair1->first;
            for (auto pair2 = p.k.cbegin(); pair2 != p.k.cend(); ++pair2)
            {
                I j = pair2->first;
                pq[i] += p[j]*combination(j,j-i)*std::pow(g.offset, j-i);
            }
        }
        return pq;
    }






}