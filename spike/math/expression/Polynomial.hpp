#pragma once

#include <algorithm>   // clamp, min_element, fill, copy
#include <complex>     // complex
#include <type_traits> // enable_if_t

// in-house libraries
#include "Identity.hpp"
#include "Scaling.hpp"
#include "Shifting.hpp"

#include <math/combinatorics.hpp>
#include <math/calculus.hpp>

namespace math {

    /* 
    `Polynomial<Nlo,Nhi>` is a class template that represents functions of the form f(x)=Σᵢaᵢxᵇⁱ where bᵢ∈ℤ.
    It is designed for high performance applications where a function must be composed from other functions.
    To address data locality concerns, the data structure is stored on the heap, 
    and users may control size in memory by specifying the highest and lowest exponent within the polynomial.

    A side effect of this design is that it allows the user to specify negative exponents.
    Introducing a negative exponent into the expression creates what is known as a "Laurent polynomial".
    A Laurent polynomial runs in the same amount of time as a classic polynomial that has the same number of terms,
    and support for Laurent polynomials places little to no performance penalties on classic polynomial behavior,
    so we support Laurent polynomials as a degenerate case to avoid creating a separate implementation.
    */
    template<int Nlo, int Nhi>
    struct Polynomial
    {
        std::array<float,Nhi+1-Nlo> k;
        constexpr Polynomial(): k()
        {
            std::fill(k.begin(), k.end(), 0.0f);
        }
        constexpr Polynomial(const Polynomial& p): k()
        {
            std::copy(p.k.begin(), p.k.end(), k.begin());
        }
        constexpr explicit Polynomial(const Identity e): k()
        {
            k[1-Nlo] = 1.0f;
        }
        constexpr explicit Polynomial(const Scaling f): k()
        {
            k[1-Nlo] = f.factor;
        }
        constexpr explicit Polynomial(const Shifting f): k()
        {
            k[0-Nlo] = f.offset;
            k[1-Nlo] = 1.0f;
        }
        constexpr explicit Polynomial(const std::array<float,Nhi+1-Nlo> k2): k()
        {
            std::copy(k2.begin(), k2.end(), k.begin());
        }
        template <int Qlo, int Qhi> 
        constexpr Polynomial(const Polynomial<Qlo,Qhi>& p): k()
        {
            for (int i = Qlo; i <= Qhi; ++i)
            {
                k[i-Nlo] = p.k[i-Qlo];
            }
        }
        template <typename... T> 
        constexpr explicit Polynomial(T... ts) : k{ts...}
        {} 
        template<typename TIterator>
        constexpr explicit Polynomial(TIterator first, TIterator last) : k(first, last)
        {}
        constexpr float operator()(const float x) const
        {
            float y(0.0f);
            for (int i = Nlo; i <= -1; ++i)
            {
                // exponents are calculated using pow(), 
                // rather than repeated multiplication, to avoid precision errors
                y += k[i-Nlo] * std::pow(x, i);
            }
            int i0(std::max(Nlo,0));
            float xi(std::pow(x, i0));
            for (int i = i0; i < Nhi; ++i)
            {
                y += k[i-Nlo] * xi;
                xi*= x;
            }
            return y;
        }
        constexpr float& operator[](const int i)
        {
            return k[i-Nlo];
        }
        constexpr float operator[](const int i) const
        {
            return Nlo<=i&&i<=Nhi? k[i-Nlo] : 0.0f;
        }
        template<int Qlo, int Qhi>
        constexpr Polynomial<Nlo,Nhi>& operator+=(const Polynomial<Qlo,Qhi>& p)
        {
            for (int i = Qlo; i <= Qhi; ++i)
            {
                k[i-Nlo] += p.k[i-Qlo];
            }
            return *this;
        }
        template<int Qlo, int Qhi>
        constexpr Polynomial<Nlo,Nhi>& operator-=(const Polynomial<Qlo,Qhi>& p)
        {
            for (int i = Qlo; i <= Qhi; ++i)
            {
                k[i-Nlo] -= p.k[i-Qlo];
            }
            return *this;
        }
        constexpr Polynomial<Nlo,Nhi>& operator*=(const float k2)
        {
            for (int i = Nlo; i <= Nhi; ++i)
            {
                k[i-Nlo] *= k2;
            }
            return *this;
        }
        constexpr Polynomial<Nlo,Nhi>& operator/=(const float k2)
        {
            for (int i = Nlo; i <= Nhi; ++i)
            {
                k[i-Nlo] /= k2;
            }
            return *this;
        }
    };


    // operators with reals that are closed under Polynomial<Plo,Phi> relations
    template<int Plo, int Phi>
    constexpr Polynomial<std::min(Plo,0),std::max(Phi,0)> operator+(const Polynomial<Plo,Phi>& p, const float& k)
    {
        Polynomial<std::min(Plo,0),std::max(Phi,0)> y(p);
        y[0] +=k;
        return y;
    }
    template<int Plo, int Phi>
    constexpr Polynomial<std::min(Plo,0),std::max(Phi,0)> operator+(const float k, const Polynomial<Plo,Phi>& p)
    {
        Polynomial<std::min(Plo,0),std::max(Phi,0)> y(p);
        y[0] +=k;
        return y;
    }
    template<int Plo, int Phi>
    constexpr Polynomial<std::min(Plo,0),std::max(Phi,0)> operator-(const Polynomial<Plo,Phi>& p, const float k)
    {
        Polynomial<std::min(Plo,0),std::max(Phi,0)> y(p);
        y[0] -=k;
        return y;
    }
    template<int Plo, int Phi>
    constexpr Polynomial<std::min(Plo,0),std::max(Phi,0)> operator-(const float k, const Polynomial<Plo,Phi>& p)
    {
        Polynomial<std::min(Plo,0),std::max(Phi,0)> y(p);
        y *=-1.0f;
        y[0] += k;
        return y;
    }
    template<int Plo, int Phi>
    constexpr Polynomial<Plo,Phi> operator*(const Polynomial<Plo,Phi>& p, const float k)
    {
        Polynomial<Plo,Phi> y(p);
        y *= k;
        return y;
    }
    template<int Plo, int Phi>
    constexpr Polynomial<Plo,Phi> operator*(const float k, const Polynomial<Plo,Phi>& p)
    {
        Polynomial<Plo,Phi> y(p);
        y *= k;
        return y;
    }
    template<int Plo, int Phi>
    constexpr Polynomial<Plo,Phi> operator/(const Polynomial<Plo,Phi>& p, const float k)
    {
        Polynomial<Plo,Phi> y(p);
        y /= k;
        return y;
    }
    template<int N>
    constexpr Polynomial<-N,-N> operator/(const float k, const Polynomial<N,N>&  p)
    {
        return Polynomial<-N,-N>(k/p[N]);
    }
    template<int Plo, int Phi>
    constexpr Polynomial<Plo,Phi> operator-(const Polynomial<Plo,Phi>& p)
    {
        Polynomial<Plo,Phi> y(p);
        y *=-1.0f;
        return y;
    }

    // operators that are not closed under Polynomial<Plo,Phi> relations, but produce Polynomial<> relations of some degree
    template<int Plo, int Phi, int Qlo, int Qhi>
    constexpr Polynomial<std::min(Plo,Qlo),std::max(Phi,Qhi)> operator+(const Polynomial<Plo,Phi>& p, const Polynomial<Qlo,Qhi>& q)
    {
        Polynomial<std::min(Plo,Qlo),std::max(Phi,Qhi)> y(p);
        y += q;
        return y;
    }
    template<int Plo, int Phi, int Qlo, int Qhi>
    constexpr Polynomial<std::min(Plo,Qlo),std::max(Phi,Qhi)> operator-(const Polynomial<Plo,Phi>& p, const Polynomial<Qlo,Qhi>& q)
    {
        Polynomial<std::min(Plo,Qlo),std::max(Phi,Qhi)> y(p);
        y -= q;
        return y;
    }
    template<int Plo, int Phi, int Qlo, int Qhi>
    constexpr Polynomial<Plo+Qlo,Phi+Qhi> operator*(const Polynomial<Plo,Phi>& p, const Polynomial<Qlo,Qhi>& q)
    {
        Polynomial<Plo+Qlo,Phi+Qhi> y;
        for (int i = Plo; i <= Phi; ++i)
        {
            for (int j = Qlo; j <= Qhi; ++j)
            {
                y[i+j] += p[i]*q[j];
            }
        }
        return y;
    }
    template<int Plo, int Phi, int M>
    constexpr Polynomial<Plo-M,Phi-M> operator/(const Polynomial<Plo,Phi>& p, const Polynomial<M,M>& q)
    {
        Polynomial<Plo-M,Phi-M> y;
        for (int i = Plo; i <= Phi; ++i)
        {
            y[i-M] = p[i]/q[M];
        }
        return y;
    }
    template<int N>
    constexpr float operator/(const Polynomial<N,N>& p, const Polynomial<N,N>& q)
    {
        return p[N]/q[N];
    }





    // operators with reals that produce Polynomial<Plo,Phi> relations given other relations as input
    template<int Plo, int Phi>
    constexpr auto operator+(const Polynomial<Plo,Phi>& p, const Identity e)
    {
        return p + Polynomial<1,1>(e);
    }
    template<int Plo, int Phi>
    constexpr auto operator+(const Identity e, const Polynomial<Plo,Phi>& p)
    {
        return Polynomial<1,1>(e) + p;
    }
    template<int Plo, int Phi>
    constexpr auto operator-(const Polynomial<Plo,Phi>& p, const Identity e)
    {
        return p - Polynomial<1,1>(e);
    }
    template<int Plo, int Phi>
    constexpr auto operator-(const Identity e, const Polynomial<Plo,Phi>& p)
    {
        return Polynomial<1,1>(e) - p;
    }
    template<int Plo, int Phi>
    constexpr auto operator*(const Polynomial<Plo,Phi>& p, const Identity e)
    {
        return p * Polynomial<1,1>(e);
    }
    template<int Plo, int Phi>
    constexpr auto operator*(const Identity e, const Polynomial<Plo,Phi>& p)
    {
        return Polynomial<1,1>(e) * p;
    }
    template<int Plo, int Phi>
    constexpr auto operator/(const Polynomial<Plo,Phi>& p, const Identity e)
    {
        return p / Polynomial<1,1>(e);
    }
    template<int N>
    constexpr auto operator/(const Identity e, const Polynomial<N,N>& p)
    {
        return Polynomial<1,1>(e) / p;
    }



    template<int Plo, int Phi>
    constexpr auto operator+(const Polynomial<Plo,Phi>& p, const Scaling f)
    {
        return p + Polynomial<1,1>(f);
    }
    template<int Plo, int Phi>
    constexpr auto operator+(const Scaling f, const Polynomial<Plo,Phi>& p)
    {
        return Polynomial<1,1>(f) + p;
    }
    template<int Plo, int Phi>
    constexpr auto operator-(const Polynomial<Plo,Phi>& p, const Scaling f)
    {
        return p - Polynomial<1,1>(f);
    }
    template<int Plo, int Phi>
    constexpr auto operator-(const Scaling f, const Polynomial<Plo,Phi>& p)
    {
        return Polynomial<1,1>(f) - p;
    }
    template<int Plo, int Phi>
    constexpr auto operator*(const Polynomial<Plo,Phi>& p, const Scaling f)
    {
        return p * Polynomial<1,1>(f);
    }
    template<int Plo, int Phi>
    constexpr auto operator*(const Scaling f, const Polynomial<Plo,Phi>& p)
    {
        return Polynomial<1,1>(f) * p;
    }
    template<int Plo, int Phi>
    constexpr auto operator/(const Polynomial<Plo,Phi>& p, const Scaling f)
    {
        return p / Polynomial<1,1>(f);
    }
    template<int N>
    constexpr auto operator/(const Scaling f, const Polynomial<N,N>& p)
    {
        return Polynomial<1,1>(f) / p;
    }



    template<int Plo, int Phi>
    constexpr auto operator+(const Polynomial<Plo,Phi>& p, const Shifting f)
    {
        return p + Polynomial<0,1>(f);
    }
    template<int Plo, int Phi>
    constexpr auto operator+(const Shifting f, const Polynomial<Plo,Phi>& p)
    {
        return Polynomial<0,1>(f) + p;
    }
    template<int Plo, int Phi>
    constexpr auto operator-(const Polynomial<Plo,Phi>& p, const Shifting f)
    {
        return p - Polynomial<0,1>(f);
    }
    template<int Plo, int Phi>
    constexpr auto operator-(const Shifting f, const Polynomial<Plo,Phi>& p)
    {
        return Polynomial<0,1>(f) - p;
    }
    template<int Plo, int Phi>
    constexpr auto operator*(const Polynomial<Plo,Phi>& p, const Shifting f)
    {
        return p * Polynomial<0,1>(f);
    }
    template<int Plo, int Phi>
    constexpr auto operator*(const Shifting f, const Polynomial<Plo,Phi>& p)
    {
        return Polynomial<0,1>(f) * p;
    }
    template<int N>
    constexpr auto operator/(const Shifting f, const Polynomial<N,N>& p)
    {
        return Polynomial<0,1>(f) / p;
    }


























    template<int N>
    constexpr auto operator-(const Shifting f)
    {
        return -Polynomial<0,1>(f);
    }

    

    constexpr auto operator*(const Identity e, const Scaling f)
    {
        return Polynomial<1,1>(e) * Polynomial<1,1>(f);
    }
    constexpr auto operator*(const Scaling f, const Identity e)
    {
        return Polynomial<1,1>(f) * Polynomial<1,1>(e);
    }



    constexpr auto operator+(const Identity e, const Shifting f)
    {
        return Polynomial<1,1>(e) + Polynomial<0,1>(f);
    }
    constexpr auto operator+(const Shifting f, const Identity e)
    {
        return Polynomial<0,1>(f) + Polynomial<1,1>(e);
    }
    constexpr auto operator-(const Identity e, const Shifting f)
    {
        return Polynomial<1,1>(e) - Polynomial<0,1>(f);
    }
    constexpr auto operator-(const Shifting f, const Identity e)
    {
        return Polynomial<0,1>(f) - Polynomial<1,1>(e);
    }
    constexpr auto operator*(const Identity e, const Shifting f)
    {
        return Polynomial<1,1>(e) * Polynomial<0,1>(f);
    }
    constexpr auto operator*(const Shifting f, const Identity e)
    {
        return Polynomial<0,1>(f) * Polynomial<1,1>(e);
    }



    constexpr auto operator+(const Shifting g, const Scaling f)
    {
        return Polynomial<0,1>(g) + Polynomial<1,1>(f);
    }
    constexpr auto operator+(const Scaling f, const Shifting g)
    {
        return Polynomial<1,1>(f) + Polynomial<0,1>(g);
    }
    constexpr auto operator-(const Shifting g, const Scaling f)
    {
        return Polynomial<0,1>(g) - Polynomial<1,1>(f);
    }
    constexpr auto operator-(const Scaling f, const Shifting g)
    {
        return Polynomial<1,1>(f) - Polynomial<0,1>(g);
    }
    constexpr auto operator*(const Shifting g, const Scaling f)
    {
        return Polynomial<0,1>(g) * Polynomial<1,1>(f);
    }
    constexpr auto operator*(const Scaling f, const Shifting g)
    {
        return Polynomial<1,1>(f) * Polynomial<0,1>(g);
    }







    // constexpr auto operator+(const float k, const Identity e)
    // {
    //     return k + Polynomial<1,1>(e);
    // }
    // constexpr auto operator+(const Identity e, const float k)
    // {
    //     return Polynomial<1,1>(e) + k;
    // }
    constexpr auto operator-(const float k, const Identity e)
    {
        return k - Polynomial<1,1>(e);
    }
    // constexpr auto operator-(const Identity e, const float k)
    // {
    //     return Polynomial<1,1>(e) - k;
    // }
    // constexpr auto operator*(const float k, const Identity e)
    // {
    //     return k * Polynomial<1,1>(e);
    // }
    // constexpr auto operator*(const Identity e, const float k)
    // {
    //     return Polynomial<1,1>(e) * k;
    // }
    constexpr auto operator/(const float k, const Identity e)
    {
        return k / Polynomial<1,1>(e);
    }
    // constexpr auto operator/(const Identity e, const float k)
    // {
    //     return Polynomial<1,1>(e) / k;
    // }



    constexpr auto operator+(const float k, const Scaling f)
    {
        return k + Polynomial<1,1>(f);
    }
    constexpr auto operator+(const Scaling f, const float k)
    {
        return Polynomial<1,1>(f) + k;
    }
    constexpr auto operator-(const float k, const Scaling f)
    {
        return k - Polynomial<1,1>(f);
    }
    constexpr auto operator-(const Scaling f, const float k)
    {
        return Polynomial<1,1>(f) - k;
    }
    // constexpr auto operator*(const float k, const Scaling f)
    // {
    //     return k * Polynomial<1,1>(f);
    // }
    // constexpr auto operator*(const Scaling f, const float k)
    // {
    //     return Polynomial<1,1>(f) * k;
    // }
    constexpr auto operator/(const float k, const Scaling f)
    {
        return k / Polynomial<1,1>(f);
    }
    // constexpr auto operator/(const Scaling f, const float k)
    // {
    //     return Polynomial<1,1>(f) / k;
    // }




    // constexpr auto operator+(const float k, const Shifting f)
    // {
    //     return k + Polynomial<0,1>(f);
    // }
    // constexpr auto operator+(const Shifting f, const float k)
    // {
    //     return Polynomial<0,1>(f) + k;
    // }
    constexpr auto operator-(const float k, const Shifting f)
    {
        return k - Polynomial<0,1>(f);
    }
    // constexpr auto operator-(const Shifting f, const float k)
    // {
    //     return Polynomial<0,1>(f) - k;
    // }
    constexpr auto operator*(const float k, const Shifting f)
    {
        return k * Polynomial<0,1>(f);
    }
    constexpr auto operator*(const Shifting f, const float k)
    {
        return Polynomial<0,1>(f) * k;
    }
    // constexpr auto operator/(const float k, const Shifting f)
    // {
    //     return k / Polynomial<0,1>(f);
    // }
    constexpr auto operator/(const Shifting f, const float k)
    {
        return Polynomial<0,1>(f) / k;
    }







    template<int N>
    constexpr auto operator+(const Scaling f, const Scaling g)
    {
        return Polynomial<1,1>(f) + Polynomial<1,1>(g);
    }
    template<int N>
    constexpr auto operator-(const Scaling f, const Scaling g)
    {
        return Polynomial<1,1>(f) - Polynomial<1,1>(g);
    }
    // template<int N>
    // constexpr auto operator*(const Scaling f, const Scaling g)
    // {
    //     return Polynomial<1,1>(f) * Polynomial<1,1>(g);
    // }
    // template<int N>
    // constexpr auto operator/(const Scaling f, const Scaling g)
    // {
    //     return Polynomial<1,1>(f) / Polynomial<1,1>(g);
    // }



    // template<int N>
    // constexpr auto operator+(const Shifting f, const Shifting g)
    // {
    //     return Polynomial<0,1>(f) + Polynomial<0,1>(g);
    // }
    // template<int N>
    // constexpr auto operator-(const Shifting f, const Shifting g)
    // {
    //     return Polynomial<0,1>(f) - Polynomial<0,1>(g);
    // }
    template<int N>
    constexpr auto operator*(const Shifting f, const Shifting g)
    {
        return Polynomial<0,1>(f) * Polynomial<0,1>(g);
    }
    // template<int N>
    // constexpr auto operator/(const Shifting f, const Shifting g)
    // {
    //     return Polynomial<0,1>(f) / Polynomial<0,1>(g);
    // }



    // template<int N>
    // constexpr auto operator+(const Identity e1, const Identity e2)
    // {
    //     return Polynomial<1,1>(e) + Polynomial<1,1>(e);
    // }
    // template<int N>
    // constexpr auto operator-(const Identity e1, const Identity e2)
    // {
    //     return Polynomial<1,1>(e) - Polynomial<1,1>(e);
    // }
    template<int N>
    constexpr auto operator*(const Identity e1, const Identity e2)
    {
        return Polynomial<1,1>(e1) * Polynomial<1,1>(e2);
    }
    // template<int N>
    // constexpr auto operator/(const Identity e1, const Identity e2)
    // {
    //     return Polynomial<1,1>(e1) / Polynomial<1,1>(e2);
    // }



    /* 
    NOTE: we avoid expressing derivatives and integrals as methods,
    since not all equations are closed under derivatives and integrals,
    and such methods would complicate the order with which classes must be declared
    */
    constexpr float derivative(const Polynomial<0,1> p) 
    {
        return p[0];
    }
    constexpr float derivative(const Polynomial<1,1> p) 
    {
        return 0.0f;
    }
    template<int Plo, int Phi>
    Polynomial<Plo==0?0:Plo-1,Phi-1> derivative(const Polynomial<Plo,Phi>& p)
    {
        Polynomial<Plo==0?0:Plo-1,Phi-1> dpdx;
        for (int i = Plo; i <=Phi; ++i)
        {
            dpdx[i-1] = i*p[i];
        }
        return dpdx;
    }

    /*
    `derivative` returns the derivative of a polynomial at a point.
    It is meant to compliment the method signature for integral(p, lo, hi)
    */
    template<int Plo, int Phi>
    float derivative(const Polynomial<Plo,Phi>& p, const float x)
    {
        float dydx(0.0f);
        float xi(std::pow(x, Plo-1));
        for (int i=Plo; i < Phi; ++i)
        {
            dydx += p[i] * xi * i;
            xi *= x;
        }
        return dydx;
    }

    /*
    `integral` returns a polynomial that represents the indefinite integral of `this`.
    */
    template<int Plo, int Phi, 
        typename = std::enable_if_t<!(Plo <= -1&&-1 <= Phi)>>
    Polynomial<Plo+1,Phi+1> integral(const Polynomial<Plo,Phi>& p)
    {
        Polynomial<Plo+1,Phi+1> dpdx;
        for (int i = Plo; i <=Phi; ++i)
        {
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
    template<int Plo, int Phi>
    float integral(const Polynomial<Plo,Phi>& p, const float lo, const float hi)
    {
        float I_hi(0.0f);
        float I_lo(0.0f);

        for (int i = Plo; i < -1; ++i)
        {
            // exponents are calculated using pow(), 
            // rather than repeated multiplication, to avoid precision errors
            I_hi += p[i] * std::pow(lo, i+1) / (i+1);
            I_lo += p[i] * std::pow(hi, i+1) / (i+1);
        }

        I_hi += p[-1] * std::log(std::abs(lo));
        I_lo += p[-1] * std::log(std::abs(hi));

        float lo_i  = 1.0f;
        float hi_i  = 1.0f;
        for (int i = 0; i <= Phi; ++i)
        {
            I_hi += p[i] * lo_i / (i+1);
            I_lo += p[i] * hi_i / (i+1);
            lo_i *= lo;
            hi_i *= hi;
        }
        return I_hi - I_lo;
    }




    template<int Plo, 
        typename = std::enable_if_t<(Plo>0)>>
    constexpr float solve(const Polynomial<Plo,Plo+1> p, const float y) 
    {
        return solve(p/Identity());
    }
    // template<int Plo, 
    //     typename = std::enable_if_t<(Plo<0)>>
    // constexpr float solve(const Polynomial<Plo,Plo+1> p, const float y) 
    // {
    //     return solve(p*Identity());
    // }

    template<int Plo, 
        typename = std::enable_if_t<(Plo>0)>>
    constexpr std::vector<std::complex<float>> solve(const Polynomial<Plo,Plo+2> p, const float y) 
    {
        return solve(p/Identity(),y);
    }
    // template<int Plo, 
    //     typename = std::enable_if_t<(Plo<0)>>
    // constexpr std::vector<std::complex<float>> solve(const Polynomial<Plo,Plo+2> p, const float y) 
    // {
    //     return solve(p*Identity(),y);
    // }

    template<int Plo, 
        typename = std::enable_if_t<(Plo>0)>>
    constexpr std::vector<std::complex<float>> solve(const Polynomial<Plo,Plo+3> p, const float y) 
    {
        return solve(p/Identity(),y);
    }
    // template<int Plo, 
    //     typename = std::enable_if_t<(Plo<0)>>
    // constexpr std::vector<std::complex<float>> solve(const Polynomial<Plo,Plo+3> p, const float y) 
    // {
    //     return solve(p*Identity(),y);
    // }

    constexpr float solve(const Polynomial<0,1> p, const float y) 
    {
        return y-p[0] / p[1];
    }

    std::array<std::complex<float>, 2> solve(const Polynomial<0,2> p, const float y) 
    {
        // the quadratic formula
        const std::complex<float> sqrt_argument = p[1]*p[1] - 4.0f*(p[0]-y)*p[2];
        return std::array<std::complex<float>, 2>{
            -p[1]+std::sqrt(sqrt_argument) / p[2],
            -p[1]-std::sqrt(sqrt_argument) / p[2]
        };
    }

    std::array<std::complex<float>, 3> solve(const Polynomial<0,3> p, const float y) 
    {
        // the cubic formula

        const Polynomial<0,3> a = p/p[3]; // monic cubic polynomial

        const std::complex<float> q =  a[1]/3.0f - a[2]*a[2]/9.0f;
        const std::complex<float> r = (a[1]*a[2] - 3.0f*a[0])/6.0f - a[2]*a[2]*a[2]/27.0f;

        const std::complex<float> s1 = std::pow(r+std::sqrt(q*q*q+r*r), 1.0f/3.0f);
        const std::complex<float> s2 = std::pow(r-std::sqrt(q*q*q+r*r), 1.0f/3.0f);

        const std::complex<float> z1 =  (s1+s2) - a[2]/3.0f;
        const std::complex<float> z2 = -(s1+s2)/2.0f - a[2]/3.0f + (s1-s2)*std::cbrt(3.0f)/2.0f; // *i
        const std::complex<float> z3 = -(s1+s2)/2.0f - a[2]/3.0f - (s1-s2)*std::cbrt(3.0f)/2.0f; // *i

        return std::array<std::complex<float>, 3>{z1,z2,z3};
    }

    template<int Plo, int Phi>
    constexpr auto extremum(const Polynomial<Plo,Phi>& p) 
    {
        return solve(derivative(p), 0.0f);
    }

    /*
    `max()` and `min()` returns the highest output of the function over the given range.
    These are used together when determining whether to include the function as a term in a larger equation.
    */
    constexpr float maximum(const Polynomial<0,1> p, const float lo, const float hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return p(hi) > p(lo)? hi : lo;
    }
    constexpr float minimum(const Polynomial<0,1> p, const float lo, const float hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return p(hi) < p(lo)? hi : lo;
    }

    constexpr float maximum(const Polynomial<0,2> p, const float lo, const float hi) 
    {
        float x = std::clamp(extremum(p), lo, hi);
        return p(x) > p(hi)? x : p(hi) > p(lo)? hi : lo;
    }
    constexpr float minimum(const Polynomial<0,2> p, const float lo, const float hi) 
    {
        float x = std::clamp(extremum(p), lo, hi);
        return p(x) < p(hi)? x : p(hi) < p(lo)? hi : lo;
    }

    template<int Plo, int Phi>
    float maximum(const Polynomial<Plo,Phi>& p, const float lo, const float hi) 
    {
        auto candidates = extremum(p);
        candidates.push_back(lo);
        candidates.push_back(hi);
        return *std::max_element(candidates.begin(), candidates.end(), 
            [&](float a, float b){ return p(std::clamp(a,lo,hi)) < p(std::clamp(b,lo,hi)); });
    }
    template<int Plo, int Phi>
    float minimum(const Polynomial<Plo,Phi>& p, const float lo, const float hi) 
    {
        auto candidates = extremum(p);
        candidates.push_back(lo);
        candidates.push_back(hi);
        return *std::min_element(candidates.begin(), candidates.end(), 
            [&](float a, float b){ return p(std::clamp(a,lo,hi)) < p(std::clamp(b,lo,hi)); });
    }



    template<int Plo, int Phi>
    constexpr float max_square_difference(const Polynomial<Plo,Phi> p, const Polynomial<Plo,Phi> q, const float lo, const float hi)
    {
        return maximum((p-q)*(p-q), lo, hi);
    }

    template<int Plo, int Phi, int Qlo, int Qhi>
    constexpr float mean_square_difference(const Polynomial<Plo,Phi> p, const Polynomial<Qlo,Qhi> q, const float lo, const float hi)
    {
        return integral((p-q)*(p-q), lo, hi) / (hi-lo);
    }
    template<int Plo, int Phi>
    constexpr float mean_square_difference(const Polynomial<Plo,Phi> p, const float k, const float lo, const float hi)
    {
        return integral((p-k)*(p-k), lo, hi) / (hi-lo);
    }
    template<int Plo, int Phi>
    constexpr float mean_square_difference(const float k, const Polynomial<Plo, Phi> p, const float lo, const float hi)
    {
        return integral((p-k)*(p-k), lo, hi) / (hi-lo);
    }

    template<int Plo, int Phi>
    constexpr Polynomial<Plo,Phi> compose(const Polynomial<Plo,Phi>& p, const Identity e)
    {
        return p;
    }

    template<int Plo, int Phi>
    constexpr Polynomial<Plo,Phi> compose(const Polynomial<Plo,Phi>& p, const Scaling g)
    {
        Polynomial<Plo,Phi> pq = p;
        for (int i = Plo; i <= Phi; ++i)
        {
            pq[i] *= std::pow(g.factor, float(i));
        }
        return pq;
    }

    template<int Plo, int Phi>
    constexpr Polynomial<Plo,Phi> compose(const Polynomial<Plo,Phi>& p, const Shifting g)
    {
        Polynomial<Plo,Phi> pq;
        for (int i = Plo; i <= Phi; ++i)
        {
            for (int j = Plo; j<= Phi; ++j)
            {
                pq[i] += p[j]*combination(j,j-i)*std::pow(g.offset, j-i);
            }
        }
        return pq;
    }

    template<int Qlo, int Qhi, 
        typename = std::enable_if_t<(Qlo >= 0 && Qhi >= 0)> >
    constexpr float compose(const Polynomial<0,0>& p, const Polynomial<Qlo,Qhi>& q)
    {
        return p[0];
    }

    template<int Plo, int Qlo, int Qhi, 
        typename = std::enable_if_t<(Plo >= 0 && Qlo >= 0 && Qhi >= 0)> >
    constexpr Polynomial<Qlo,Qhi> compose(const Polynomial<Plo,Plo+1> p, const Polynomial<Qlo,Qhi>& q)
    {
        return p[Plo] + p[Plo+1]*q;
    }

    template<int Plo, int Phi, int Qlo, int Qhi, 
        typename = std::enable_if_t<(Plo >= 0 && Phi >= 0 && Qlo >= 0 && Qhi >= 0)> >
    constexpr Polynomial<std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)> compose(const Polynomial<Plo,Phi>& p, const Polynomial<Qlo,Qhi>& q)
    {
        /*
        we rely on the fact that, e.g.
            p(x) = k₀x⁰+k₁x¹+k₂x²+k₃x³ = k₀+(k₁+(k₂+k₃x)x)x
        so there is a p₀(u) and pₙ₋₁(u) such that 
            p(x) = p₀∘pₙ₋₁.
        and it turns out that, in this case:
            p₀(u) = k₀+xu 
            pₙ₋₁(u) = k₁+(k₂+k₃u)u = k₁u⁰+k₂u¹+k₃u²
        */
        return p[0]+Identity()*compose(Polynomial<Plo,Phi-1>(p.k.begin()+1, p.k.end()), q);
    }





    template<typename F>
    constexpr Polynomial<0,1> linear_newton_polynomial(
        const F f, 
        const float x1, 
        const float x2
    ){
        const float dfdx = (f(x2)-f(x1)) / (x2-x1);
        return f(x1) + dfdx * Shifting(-x1);
    }

    template<typename F>
    constexpr Polynomial<0,1> linear_taylor_series(const F f, const float x, const float dx)
    {
        const float dx2 = dx*dx;
        return compose(
            Polynomial<0,1>(f(x), 
                central_finite_difference(f, x, dx, 1) / dx),
            Shifting(-x)
        );
    }

    template<typename F>
    constexpr Polynomial<0,2> quadratic_newton_polynomial(
        const F f, 
        const float x1, 
        const float x2, 
        const float x3
    ){
        const float dfdx_12  = (f(x2)-f(x1)) / (x2-x1);
        const float dfdx_23  = (f(x3)-f(x2)) / (x3-x2);
        const float df2dx2 = (dfdx_23-dfdx_12) / (x3-x1);
        return f(x1) 
            + dfdx_12 * Shifting(-x1)
            + df2dx2  * Shifting(-x1) * Shifting(-x2);
    }

    template<typename F>
    constexpr Polynomial<0,2> quadratic_taylor_series(const F f, const float x, const float dx)
    {
        const float dx2 = dx*dx;
        return compose(
            Polynomial<0,2>(f(x), 
                central_finite_difference(f, x, dx, 1) / dx, 
                central_finite_difference(f, x, dx, 2) /(dx2*2.0f)),
            Shifting(-x)
        );
    }

    template<typename F>
    constexpr Polynomial<0,3> cubic_newton_polynomial(
        const F f, 
        const float x1, 
        const float x2, 
        const float x3,
        const float x4
    ){
        const float dfdx_12 = (f(x2)-f(x1)) / (x2-x1);
        const float dfdx_23 = (f(x3)-f(x2)) / (x3-x2);
        const float dfdx_34 = (f(x4)-f(x3)) / (x4-x3);
        const float df2dx2_13 = (dfdx_23-dfdx_12) / (x3-x1);
        const float df2dx2_24 = (dfdx_34-dfdx_23) / (x4-x2);
        const float df3dx3    = (df2dx2_24-df2dx2_13) / (x4-x1);
        return f(x1) 
            + dfdx_12   * Shifting(-x1)
            + df2dx2_13 * Shifting(-x1) * Shifting(-x2)
            + df3dx3    * Shifting(-x1) * Shifting(-x2) * Shifting(-x3);
    }

    template<typename F>
    constexpr Polynomial<0,3> cubic_taylor_series(const F f, const float x, const float dx)
    {
        const float dx2 = dx*dx;
        const float dx3 = dx2*dx;
        return 
            compose(
                Polynomial<0,3>(f(x), 
                    central_finite_difference(f, x, dx, 1) / dx, 
                    central_finite_difference(f, x, dx, 2) /(dx2*2.0f), 
                    central_finite_difference(f, x, dx, 3) /(dx3*6.0f)),
                Shifting(-x)
            );
    }


}