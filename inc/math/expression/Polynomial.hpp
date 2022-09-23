#pragma once

#include <array>       // array
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
    `Polynomial<T,Nlo,Nhi>` is a class template that represents functions of the form f(x)=Σᵢaᵢxᵇⁱ where bᵢ∈ℤ.
    It is designed for high performance applications where a function must be composed from other functions.
    To address data locality concerns, the data structure is stored on the heap, 
    and users may control size in memory by specifying the highest and lowest exponent within the polynomial.

    A side effect of this design is that it allows the user to specify negative exponents.
    Introducing a negative exponent into the expression creates what is known as a "Laurent polynomial".
    A Laurent polynomial runs in the same amount of time as a classic polynomial that has the same number of terms,
    and support for Laurent polynomials places little to no performance penalties on classic polynomial behavior,
    so we support Laurent polynomials as a degenerate case to avoid creating a separate implementation.
    */
    template<typename T, int Nlo, int Nhi>
    struct Polynomial
    {
        std::array<T,Nhi+1-Nlo> k;
        // the zero polynomial
        constexpr Polynomial(): k()
        {
            std::fill(k.begin(), k.end(), T(0.0));
        }
        // constant constructor
        constexpr explicit Polynomial(const T k2): k()
        {
            std::fill(k.begin(), k.end(), T(0.0));
            k[0-Nlo] = k2;
        }
        // copy constructor
        constexpr Polynomial(const Polynomial& p): k()
        {
            std::copy(p.k.begin(), p.k.end(), k.begin());
        }
        // cast constructor
        template <typename T2, int Qlo, int Qhi, 
            typename = std::enable_if_t<(Nlo <= Qlo&&Qhi <= Nhi)>> 
        constexpr Polynomial(const Polynomial<T2,Qlo,Qhi>& p): k()
        {
            std::fill(k.begin(), k.end(), T(0.0));
            for (int i = Qlo; i <= Qhi; ++i)
            {
                k[i-Nlo] = T(p.k[i-Qlo]);
            }
        }
        constexpr explicit Polynomial(const Identity<T> e): k()
        {
            std::fill(k.begin(), k.end(), T(0.0));
            k[1-Nlo] = T(1.0);
        }
        constexpr explicit Polynomial(const Scaling<T> f): k()
        {
            std::fill(k.begin(), k.end(), T(0.0));
            k[1-Nlo] = f.factor;
        }
        constexpr explicit Polynomial(const Shifting<T> f): k()
        {
            std::fill(k.begin(), k.end(), T(0.0));
            k[0-Nlo] = f.offset;
            k[1-Nlo] = T(1.0);
        }
        constexpr explicit Polynomial(const std::array<T,Nhi+1-Nlo> k2): k(k2)
        {
        }
        template<typename TIterator>
        constexpr explicit Polynomial(TIterator first, TIterator last)
        {
            std::copy(first, last, k.begin());
        }
        constexpr Polynomial<T,Nlo,Nhi>& operator=(const Polynomial<T,Nlo,Nhi>& p)
        {
            std::copy(p.k.begin(), p.k.end(), k.begin());
            return *this;
        }
        constexpr T operator()(const T x) const
        {
            T y(0.0f);
            for (int i = Nlo; i <= -1; ++i)
            {
                // exponents are calculated using pow(), 
                // rather than repeated multiplication, to avoid precision errors
                y += k[i-Nlo] * std::pow(x, i);
            }
            int i0(std::max(Nlo,0));
            T xi(std::pow(x, i0));
            for (int i = i0; i <= Nhi; ++i)
            {
                y += k[i-Nlo] * xi;
                xi*= x;
            }
            return y;
        }
        constexpr T& operator[](const int i)
        {
            return k[i-Nlo];
        }
        constexpr T operator[](const int i) const
        {
            return Nlo<=i&&i<=Nhi? k[i-Nlo] : T(0.0);
        }
        template<int Qlo, int Qhi>
        constexpr Polynomial<T,Nlo,Nhi>& operator+=(const Polynomial<T,Qlo,Qhi>& p)
        {
            for (int i = Qlo; i <= Qhi; ++i)
            {
                k[i-Nlo] += p.k[i-Qlo];
            }
            return *this;
        }
        template<int Qlo, int Qhi>
        constexpr Polynomial<T,Nlo,Nhi>& operator-=(const Polynomial<T,Qlo,Qhi>& p)
        {
            for (int i = Qlo; i <= Qhi; ++i)
            {
                k[i-Nlo] -= p.k[i-Qlo];
            }
            return *this;
        }
        constexpr Polynomial<T,Nlo,Nhi>& operator+=(const T k2)
        {
            k[0-Nlo] += k2;
            return *this;
        }
        constexpr Polynomial<T,Nlo,Nhi>& operator-=(const T k2)
        {
            k[0-Nlo] -= k2;
            return *this;
        }
        constexpr Polynomial<T,Nlo,Nhi>& operator*=(const T k2)
        {
            for (int i = Nlo; i <= Nhi; ++i)
            {
                k[i-Nlo] *= k2;
            }
            return *this;
        }
        constexpr Polynomial<T,Nlo,Nhi>& operator/=(const T k2)
        {
            for (int i = Nlo; i <= Nhi; ++i)
            {
                k[i-Nlo] /= k2;
            }
            return *this;
        }
    };


    // operators with reals that are closed under Polynomial<T,Plo,Phi> 
    template<typename T, int Plo, int Phi>
    constexpr Polynomial<T,std::min(Plo,0),std::max(Phi,0)> operator+(const Polynomial<T,Plo,Phi>& p, const T& k)
    {
        Polynomial<T,std::min(Plo,0),std::max(Phi,0)> y(p);
        y[0] +=k;
        return y;
    }
    template<typename T, int Plo, int Phi>
    constexpr Polynomial<T,std::min(Plo,0),std::max(Phi,0)> operator+(const T k, const Polynomial<T,Plo,Phi>& p)
    {
        Polynomial<T,std::min(Plo,0),std::max(Phi,0)> y(p);
        y[0] +=k;
        return y;
    }
    template<typename T, int Plo, int Phi>
    constexpr Polynomial<T,std::min(Plo,0),std::max(Phi,0)> operator-(const Polynomial<T,Plo,Phi>& p, const T k)
    {
        Polynomial<T,std::min(Plo,0),std::max(Phi,0)> y(p);
        y[0] -=k;
        return y;
    }
    template<typename T, int Plo, int Phi>
    constexpr Polynomial<T,std::min(Plo,0),std::max(Phi,0)> operator-(const T k, const Polynomial<T,Plo,Phi>& p)
    {
        Polynomial<T,std::min(Plo,0),std::max(Phi,0)> y(p);
        y *=-1.0f;
        y[0] += k;
        return y;
    }
    template<typename T, int Plo, int Phi>
    constexpr Polynomial<T,Plo,Phi> operator*(const Polynomial<T,Plo,Phi>& p, const T k)
    {
        Polynomial<T,Plo,Phi> y(p);
        y *= k;
        return y;
    }
    template<typename T, int Plo, int Phi>
    constexpr Polynomial<T,Plo,Phi> operator*(const T k, const Polynomial<T,Plo,Phi>& p)
    {
        Polynomial<T,Plo,Phi> y(p);
        y *= k;
        return y;
    }
    template<typename T, int Plo, int Phi>
    constexpr Polynomial<T,Plo,Phi> operator/(const Polynomial<T,Plo,Phi>& p, const T k)
    {
        Polynomial<T,Plo,Phi> y(p);
        y /= k;
        return y;
    }
    template<typename T, int N>
    constexpr Polynomial<T,-N,-N> operator/(const T k, const Polynomial<T,N,N>&  p)
    {
        return Polynomial<T,-N,-N>(k/p[N]);
    }
    template<typename T, int Plo, int Phi>
    constexpr Polynomial<T,Plo,Phi> operator-(const Polynomial<T,Plo,Phi>& p)
    {
        Polynomial<T,Plo,Phi> y(p);
        y *=-1.0f;
        return y;
    }

    // operators that are not closed under Polynomial<T,Plo,Phi> relations, but produce Polynomial<T,> relations of some degree
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> operator+(const Polynomial<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> y(p);
        y += q;
        return y;
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> operator-(const Polynomial<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> y(p);
        y -= q;
        return y;
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr Polynomial<T,Plo+Qlo,Phi+Qhi> operator*(const Polynomial<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        Polynomial<T,Plo+Qlo,Phi+Qhi> y;
        for (int i = Plo; i <= Phi; ++i)
        {
            for (int j = Qlo; j <= Qhi; ++j)
            {
                y[i+j] += p[i]*q[j];
            }
        }
        return y;
    }
    template<typename T, int Plo, int Phi, int M>
    constexpr Polynomial<T,Plo-M,Phi-M> operator/(const Polynomial<T,Plo,Phi>& p, const Polynomial<T,M,M>& q)
    {
        Polynomial<T,Plo-M,Phi-M> y;
        for (int i = Plo; i <= Phi; ++i)
        {
            y[i-M] = p[i]/q[M];
        }
        return y;
    }
    template<typename T, int N>
    constexpr T operator/(const Polynomial<T,N,N>& p, const Polynomial<T,N,N>& q)
    {
        return p[N]/q[N];
    }





    // operators with reals that produce Polynomial<T,Plo,Phi> relations given other relations as input
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Polynomial<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p + Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Identity<T> e, const Polynomial<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(e) + p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Polynomial<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p - Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Identity<T> e, const Polynomial<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(e) - p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Polynomial<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p * Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Identity<T> e, const Polynomial<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(e) * p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator/(const Polynomial<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p / Polynomial<T,1,1>(e);
    }
    template<typename T, int N>
    constexpr auto operator/(const Identity<T> e, const Polynomial<T,N,N>& p)
    {
        return Polynomial<T,1,1>(e) / p;
    }



    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Polynomial<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p + Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Scaling<T> f, const Polynomial<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(f) + p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Polynomial<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p - Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Scaling<T> f, const Polynomial<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(f) - p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Polynomial<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p * Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Scaling<T> f, const Polynomial<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(f) * p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator/(const Polynomial<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p / Polynomial<T,1,1>(f);
    }
    template<typename T, int N>
    constexpr auto operator/(const Scaling<T> f, const Polynomial<T,N,N>& p)
    {
        return Polynomial<T,1,1>(f) / p;
    }



    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Polynomial<T,Plo,Phi>& p, const Shifting<T> f)
    {
        return p + Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Shifting<T> f, const Polynomial<T,Plo,Phi>& p)
    {
        return Polynomial<T,0,1>(f) + p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Polynomial<T,Plo,Phi>& p, const Shifting<T> f)
    {
        return p - Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Shifting<T> f, const Polynomial<T,Plo,Phi>& p)
    {
        return Polynomial<T,0,1>(f) - p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Polynomial<T,Plo,Phi>& p, const Shifting<T> f)
    {
        return p * Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Shifting<T> f, const Polynomial<T,Plo,Phi>& p)
    {
        return Polynomial<T,0,1>(f) * p;
    }
    template<typename T, int N>
    constexpr auto operator/(const Shifting<T> f, const Polynomial<T,N,N>& p)
    {
        return Polynomial<T,0,1>(f) / p;
    }


























    template<typename T, int N>
    constexpr auto operator-(const Shifting<T> f)
    {
        return -Polynomial<T,0,1>(f);
    }

    

    template<typename T>
    constexpr auto operator*(const Identity<T> e, const Scaling<T> f)
    {
        return Polynomial<T,1,1>(e) * Polynomial<T,1,1>(f);
    }
    template<typename T>
    constexpr auto operator*(const Scaling<T> f, const Identity<T> e)
    {
        return Polynomial<T,1,1>(f) * Polynomial<T,1,1>(e);
    }



    template<typename T>
    constexpr auto operator+(const Identity<T> e, const Shifting<T> f)
    {
        return Polynomial<T,1,1>(e) + Polynomial<T,0,1>(f);
    }
    template<typename T>
    constexpr auto operator+(const Shifting<T> f, const Identity<T> e)
    {
        return Polynomial<T,0,1>(f) + Polynomial<T,1,1>(e);
    }
    template<typename T>
    constexpr auto operator-(const Identity<T> e, const Shifting<T> f)
    {
        return Polynomial<T,1,1>(e) - Polynomial<T,0,1>(f);
    }
    template<typename T>
    constexpr auto operator-(const Shifting<T> f, const Identity<T> e)
    {
        return Polynomial<T,0,1>(f) - Polynomial<T,1,1>(e);
    }
    template<typename T>
    constexpr auto operator*(const Identity<T> e, const Shifting<T> f)
    {
        return Polynomial<T,1,1>(e) * Polynomial<T,0,1>(f);
    }
    template<typename T>
    constexpr auto operator*(const Shifting<T> f, const Identity<T> e)
    {
        return Polynomial<T,0,1>(f) * Polynomial<T,1,1>(e);
    }



    template<typename T>
    constexpr auto operator+(const Shifting<T> g, const Scaling<T> f)
    {
        return Polynomial<T,0,1>(g) + Polynomial<T,1,1>(f);
    }
    template<typename T>
    constexpr auto operator+(const Scaling<T> f, const Shifting<T> g)
    {
        return Polynomial<T,1,1>(f) + Polynomial<T,0,1>(g);
    }
    template<typename T>
    constexpr auto operator-(const Shifting<T> g, const Scaling<T> f)
    {
        return Polynomial<T,0,1>(g) - Polynomial<T,1,1>(f);
    }
    template<typename T>
    constexpr auto operator-(const Scaling<T> f, const Shifting<T> g)
    {
        return Polynomial<T,1,1>(f) - Polynomial<T,0,1>(g);
    }
    template<typename T>
    constexpr auto operator*(const Shifting<T> g, const Scaling<T> f)
    {
        return Polynomial<T,0,1>(g) * Polynomial<T,1,1>(f);
    }
    template<typename T>
    constexpr auto operator*(const Scaling<T> f, const Shifting<T> g)
    {
        return Polynomial<T,1,1>(f) * Polynomial<T,0,1>(g);
    }







    // template<typename T>
    // constexpr auto operator+(const T k, const Identity<T> e)
    // {
    //     return k + Polynomial<T,1,1>(e);
    // }
    // template<typename T>
    // constexpr auto operator+(const Identity<T> e, const T k)
    // {
    //     return Polynomial<T,1,1>(e) + k;
    // }
    template<typename T>
    constexpr auto operator-(const T k, const Identity<T> e)
    {
        return k - Polynomial<T,1,1>(e);
    }
    // template<typename T>
    // constexpr auto operator-(const Identity<T> e, const T k)
    // {
    //     return Polynomial<T,1,1>(e) - k;
    // }
    // template<typename T>
    // constexpr auto operator*(const T k, const Identity<T> e)
    // {
    //     return k * Polynomial<T,1,1>(e);
    // }
    // template<typename T>
    // constexpr auto operator*(const Identity<T> e, const T k)
    // {
    //     return Polynomial<T,1,1>(e) * k;
    // }
    template<typename T>
    constexpr auto operator/(const T k, const Identity<T> e)
    {
        return k / Polynomial<T,1,1>(e);
    }
    // template<typename T>
    // constexpr auto operator/(const Identity<T> e, const T k)
    // {
    //     return Polynomial<T,1,1>(e) / k;
    // }



    template<typename T>
    constexpr auto operator+(const T k, const Scaling<T> f)
    {
        return k + Polynomial<T,1,1>(f);
    }
    template<typename T>
    constexpr auto operator+(const Scaling<T> f, const T k)
    {
        return Polynomial<T,1,1>(f) + k;
    }
    template<typename T>
    constexpr auto operator-(const T k, const Scaling<T> f)
    {
        return k - Polynomial<T,1,1>(f);
    }
    template<typename T>
    constexpr auto operator-(const Scaling<T> f, const T k)
    {
        return Polynomial<T,1,1>(f) - k;
    }
    // template<typename T>
    // constexpr auto operator*(const T k, const Scaling<T> f)
    // {
    //     return k * Polynomial<T,1,1>(f);
    // }
    // template<typename T>
    // constexpr auto operator*(const Scaling<T> f, const T k)
    // {
    //     return Polynomial<T,1,1>(f) * k;
    // }
    template<typename T>
    constexpr auto operator/(const T k, const Scaling<T> f)
    {
        return k / Polynomial<T,1,1>(f);
    }
    // template<typename T>
    // constexpr auto operator/(const Scaling<T> f, const T k)
    // {
    //     return Polynomial<T,1,1>(f) / k;
    // }




    // template<typename T>
    // constexpr auto operator+(const T k, const Shifting<T> f)
    // {
    //     return k + Polynomial<T,0,1>(f);
    // }
    // template<typename T>
    // constexpr auto operator+(const Shifting<T> f, const T k)
    // {
    //     return Polynomial<T,0,1>(f) + k;
    // }
    template<typename T>
    constexpr auto operator-(const T k, const Shifting<T> f)
    {
        return k - Polynomial<T,0,1>(f);
    }
    // template<typename T>
    // constexpr auto operator-(const Shifting<T> f, const T k)
    // {
    //     return Polynomial<T,0,1>(f) - k;
    // }
    template<typename T>
    constexpr auto operator*(const T k, const Shifting<T> f)
    {
        return k * Polynomial<T,0,1>(f);
    }
    template<typename T>
    constexpr auto operator*(const Shifting<T> f, const T k)
    {
        return Polynomial<T,0,1>(f) * k;
    }
    // template<typename T>
    // constexpr auto operator/(const T k, const Shifting<T> f)
    // {
    //     return k / Polynomial<T,0,1>(f);
    // }
    template<typename T>
    constexpr auto operator/(const Shifting<T> f, const T k)
    {
        return Polynomial<T,0,1>(f) / k;
    }







    template<typename T, int N>
    constexpr auto operator+(const Scaling<T> f, const Scaling<T> g)
    {
        return Polynomial<T,1,1>(f) + Polynomial<T,1,1>(g);
    }
    template<typename T, int N>
    constexpr auto operator-(const Scaling<T> f, const Scaling<T> g)
    {
        return Polynomial<T,1,1>(f) - Polynomial<T,1,1>(g);
    }
    // template<typename T, int N>
    // constexpr auto operator*(const Scaling<T> f, const Scaling<T> g)
    // {
    //     return Polynomial<T,1,1>(f) * Polynomial<T,1,1>(g);
    // }
    // template<typename T, int N>
    // constexpr auto operator/(const Scaling<T> f, const Scaling<T> g)
    // {
    //     return Polynomial<T,1,1>(f) / Polynomial<T,1,1>(g);
    // }



    // template<typename T, int N>
    // constexpr auto operator+(const Shifting<T> f, const Shifting<T> g)
    // {
    //     return Polynomial<T,0,1>(f) + Polynomial<T,0,1>(g);
    // }
    // template<typename T, int N>
    // constexpr auto operator-(const Shifting<T> f, const Shifting<T> g)
    // {
    //     return Polynomial<T,0,1>(f) - Polynomial<T,0,1>(g);
    // }
    template<typename T, int N>
    constexpr auto operator*(const Shifting<T> f, const Shifting<T> g)
    {
        return Polynomial<T,0,1>(f) * Polynomial<T,0,1>(g);
    }
    // template<typename T, int N>
    // constexpr auto operator/(const Shifting<T> f, const Shifting<T> g)
    // {
    //     return Polynomial<T,0,1>(f) / Polynomial<T,0,1>(g);
    // }



    // template<typename T, int N>
    // constexpr auto operator+(const Identity<T> e1, const Identity<T> e2)
    // {
    //     return Polynomial<T,1,1>(e) + Polynomial<T,1,1>(e);
    // }
    // template<typename T, int N>
    // constexpr auto operator-(const Identity<T> e1, const Identity<T> e2)
    // {
    //     return Polynomial<T,1,1>(e) - Polynomial<T,1,1>(e);
    // }
    template<typename T, int N>
    constexpr auto operator*(const Identity<T> e1, const Identity<T> e2)
    {
        return Polynomial<T,1,1>(e1) * Polynomial<T,1,1>(e2);
    }
    // template<typename T, int N>
    // constexpr auto operator/(const Identity<T> e1, const Identity<T> e2)
    // {
    //     return Polynomial<T,1,1>(e1) / Polynomial<T,1,1>(e2);
    // }


    template<typename T>
    constexpr Polynomial<T,0,1> inverse(const Polynomial<T,0,1> p) 
    {
        // we know:           y=a+bx
        // therefore: -a/b+1/by=x
        return Polynomial<T,0,1>(-p[0]/p[1], T(1)/p[1]);
    }

    template<typename T>
    constexpr Polynomial<T,1,1> inverse(const Polynomial<T,1,1> p) 
    {
        // we know:           y=ax
        // therefore:      1/ay=x
        return Polynomial<T,1,1>(T(1)/p[1]);
    }


    /* 
    NOTE: we avoid expressing derivatives and integrals as methods,
    since not all equations are closed under derivatives and integrals,
    and such methods would complicate the order with which classes must be declared
    */
    template<typename T>
    constexpr T derivative(const Polynomial<T,0,1> p) 
    {
        return p[0];
    }
    template<typename T>
    constexpr T derivative(const Polynomial<T,1,1> p) 
    {
        return 0.0f;
    }
    template<typename T, int Plo, int Phi>
    Polynomial<T,Plo==0?0:Plo-1,Phi-1> derivative(const Polynomial<T,Plo,Phi>& p)
    {
        Polynomial<T,Plo==0?0:Plo-1,Phi-1> dpdx;
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
    template<typename T, int Plo, int Phi>
    T derivative(const Polynomial<T,Plo,Phi>& p, const T x)
    {
        T dydx(0.0f);
        T xi(std::pow(x, Plo-1));
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
    template<typename T, int Plo, int Phi, 
        typename = std::enable_if_t<!(Plo <= -1&&-1 <= Phi)>>
    Polynomial<T,Plo+1,Phi+1> integral(const Polynomial<T,Plo,Phi>& p)
    {
        Polynomial<T,Plo+1,Phi+1> dpdx;
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
    template<typename T, int Plo, int Phi>
    T integral(const Polynomial<T,Plo,Phi>& p, const T x)
    {
        T I(0.0f);

        for (int i = Plo; i < -1; ++i)
        {
            // exponents are calculated using pow(), 
            // rather than repeated multiplication, to avoid precision errors
            I += p[i] * std::pow(x, i+1) / (i+1);
        }

        I += p[-1] * (p[-1] != T(0.0f) && Plo < 0? 
            std::log(std::abs(x)) : T(0.0f));

        for (int i = 0; i <= Phi; ++i)
        {
            I += p[i] * std::pow(x, i+1) / (i+1);
        }
        return I;
    }

    /*
    `integral` returns the definite integral of a polynomial 
    without representing the integral as its own function object.
    This is meant to be used as a fallback in the event the function is a 
    Laurent polynomial with a coefficient of degree -1, 
    since the integral for such a function includes a logarithmic term.
    */
    template<typename T, int Plo, int Phi>
    T integral(const Polynomial<T,Plo,Phi>& p, const T lo, const T hi)
    {
        T I(0.0f);

        for (int i = Plo; i < -1; ++i)
        {
            I += p[i] *  (std::pow(hi, i+1) / (i+1) 
                        - std::pow(lo, i+1) / (i+1));
        }

        I += p[-1] * (p[-1] != T(0.0f) && Plo < 0? 
            std::log(std::abs(hi)) - std::log(std::abs(lo)) : T(0.0f));

        for (int i = 0; i <= Phi; ++i)
        {
            I += p[i] *  (std::pow(hi, i+1) / (i+1) 
                        - std::pow(lo, i+1) / (i+1));
        }
        return I;
    }


    template<typename T>
    constexpr Polynomial<T,2,2> integral(const Scaling<T> f) 
    {
        return Polynomial<T,2,2>(std::array<T,1>{f.factor/2.0});
    }

    template<typename T>
    constexpr Polynomial<T,1,2> integral(const Shifting<T> f) 
    {
        return Polynomial<T,1,2>(std::array<T,2>{f.offset, T(0.5)});
    }


    /*
    `solve()` provides all possible solutions to a polynomial, complex or otherwise.
    The number of solutions under these conditions is always known at compile time,
    so the output is stored either as a T (for linear functions) 
    or as an array of complex numbers (for any other polynomial).
    This can be useful if performance takes precedence over ease of implementation.
    */
    template<typename T, int Plo, 
        typename = std::enable_if_t<(Plo>0)>>
    constexpr std::vector<std::complex<T>> solve(const Polynomial<T,Plo,Plo+1> p, const T y) 
    {
        return solve(p/Identity<T>());
    }
    // template<typename T, int Plo, 
    //     typename = std::enable_if_t<(Plo<0)>>
    // constexpr T solve(const Polynomial<T,Plo,Plo+1> p, const T y) 
    // {
    //     return solve(p*Identity<T>());
    // }

    template<typename T, int Plo, 
        typename = std::enable_if_t<(Plo>0)>>
    constexpr std::vector<std::complex<T>> solve(const Polynomial<T,Plo,Plo+2> p, const T y) 
    {
        return solve(p/Identity<T>(),y);
    }
    // template<typename T, int Plo, 
    //     typename = std::enable_if_t<(Plo<0)>>
    // constexpr std::vector<std::complex<T>> solve(const Polynomial<T,Plo,Plo+2> p, const T y) 
    // {
    //     return solve(p*Identity<T>(),y);
    // }

    template<typename T, int Plo, 
        typename = std::enable_if_t<(Plo>0)>>
    constexpr std::vector<std::complex<T>> solve(const Polynomial<T,Plo,Plo+3> p, const T y) 
    {
        return solve(p/Identity<T>(),y);
    }
    // template<typename T, int Plo, 
    //     typename = std::enable_if_t<(Plo<0)>>
    // constexpr std::vector<std::complex<T>> solve(const Polynomial<T,Plo,Plo+3> p, const T y) 
    // {
    //     return solve(p*Identity<T>(),y);
    // }

    template<typename T>
    constexpr T solve(const Polynomial<T,0,1> p, const T y) 
    {
        return y-p[0] / p[1];
    }

    template<typename T>
    std::array<std::complex<T>, 2> solve(const Polynomial<T,0,2> p, const T y) 
    {
        // the quadratic formula
        const std::complex<T> sqrt_argument = p[1]*p[1] - 4.0f*(p[0]-y)*p[2];
        return std::array<std::complex<T>, 2>{
            -p[1]+std::sqrt(sqrt_argument) / p[2],
            -p[1]-std::sqrt(sqrt_argument) / p[2]
        };
    }

    template<typename T>
    std::array<std::complex<T>, 3> solve(const Polynomial<T,0,3> p, const T y) 
    {
        // the cubic formula

        const Polynomial<T,0,3> a = p/p[3]; // monic cubic polynomial

        const std::complex<T> q =  a[1]/3.0f - a[2]*a[2]/9.0f;
        const std::complex<T> r = (a[1]*a[2] - 3.0f*a[0])/6.0f - a[2]*a[2]*a[2]/27.0f;

        const std::complex<T> s1 = std::pow(r+std::sqrt(q*q*q+r*r), 1.0f/3.0f);
        const std::complex<T> s2 = std::pow(r-std::sqrt(q*q*q+r*r), 1.0f/3.0f);

        const std::complex<T> z1 =  (s1+s2) - a[2]/3.0f;
        const std::complex<T> z2 = -(s1+s2)/2.0f - a[2]/3.0f + (s1-s2)*std::cbrt(3.0f)/2.0f; // *i
        const std::complex<T> z3 = -(s1+s2)/2.0f - a[2]/3.0f - (s1-s2)*std::cbrt(3.0f)/2.0f; // *i

        return std::array<std::complex<T>, 3>{z1,z2,z3};
    }


    /*
    `reals()` is a convenience function that accepts iterators for a container of complex values 
    and returns a vector containing only those values which are strictly real.
    This can be useful when condensing solutions to polynomial equations.
    */
    template<typename T, typename TInputIterator, typename TOutputIterator>
    void reals(
        const TInputIterator first, 
        const TInputIterator last, 
        TOutputIterator result, 
        const T imaginary_precision
    ){
        std::complex<T> z;
        while(first != last) 
        {
            z = *first;
            if (std::abs(z.imag()) > imaginary_precision)
            {
                *result = z.real();
                ++result;
                ++first;
            }
        }
        return result;
    }

    template<typename T, int Plo, int Phi>
    constexpr auto extremum(const Polynomial<T,Plo,Phi>& p) 
    {
        return solve(derivative(p), 0.0f);
    }

    /*
    `max()` and `min()` returns the highest output of the function over the given range.
    These are used together when determining whether to include the function as a term in a larger equation.
    */
    template<typename T>
    constexpr T maximum(const Polynomial<T,0,1> p, const T lo, const T hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return p(hi) > p(lo)? hi : lo;
    }
    template<typename T>
    constexpr T minimum(const Polynomial<T,0,1> p, const T lo, const T hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return p(hi) < p(lo)? hi : lo;
    }

    template<typename T>
    constexpr T maximum(const Polynomial<T,0,2> p, const T lo, const T hi) 
    {
        T x = std::clamp(extremum(p), lo, hi);
        return p(x) > p(hi)? x : p(hi) > p(lo)? hi : lo;
    }
    template<typename T>
    constexpr T minimum(const Polynomial<T,0,2> p, const T lo, const T hi) 
    {
        T x = std::clamp(extremum(p), lo, hi);
        return p(x) < p(hi)? x : p(hi) < p(lo)? hi : lo;
    }

    template<typename T, int Plo, int Phi>
    T maximum(const Polynomial<T,Plo,Phi>& p, const T lo, const T hi) 
    {
        auto candidates = extremum(p);
        candidates.push_back(lo);
        candidates.push_back(hi);
        return *std::max_element(candidates.begin(), candidates.end(), 
            [&](T a, T b){ return p(std::clamp(a,lo,hi)) < p(std::clamp(b,lo,hi)); });
    }
    template<typename T, int Plo, int Phi>
    T minimum(const Polynomial<T,Plo,Phi>& p, const T lo, const T hi) 
    {
        auto candidates = extremum(p);
        candidates.push_back(lo);
        candidates.push_back(hi);
        return *std::min_element(candidates.begin(), candidates.end(), 
            [&](T a, T b){ return p(std::clamp(a,lo,hi)) < p(std::clamp(b,lo,hi)); });
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
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const Polynomial<T,Plo,Phi> p, const Polynomial<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return std::sqrt(integral((p-q)*(p-q), lo, hi)) / (hi-lo);
    }
    template<typename T, int Plo, int Phi>
    constexpr T distance(const Polynomial<T,Plo,Phi> p, const T k, const T lo, const T hi)
    {
        return std::sqrt(integral((p-k)*(p-k), lo, hi)) / (hi-lo);
    }
    template<typename T, int Plo, int Phi>
    constexpr T distance(const T k, const Polynomial<T,Plo, Phi> p, const T lo, const T hi)
    {
        return std::sqrt(integral((p-k)*(p-k), lo, hi)) / (hi-lo);
    }

    template<typename T, int Plo, int Phi>
    constexpr Polynomial<T,Plo,Phi> compose(const Polynomial<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p;
    }

    template<typename T, int Plo, int Phi>
    constexpr Polynomial<T,Plo,Phi> compose(const Polynomial<T,Plo,Phi>& p, const Scaling<T> g)
    {
        /*
        example:
        P(x)   = p+qx+rx²
        F(x)   = sx
        P∘F(x) = p+q(sx)+r(sx)²
        P∘F(x) = p+qsx+rs²x²
        */
        Polynomial<T,Plo,Phi> pg = p;
        for (int i = Plo; i <= Phi; ++i)
        {
            pg[i] *= std::pow(g.factor, T(i));
        }
        return pg;
    }

    template<typename T, int Plo, int Phi, 
        typename = std::enable_if_t<(Plo >= 0 && Phi >= 0)> >
    constexpr Polynomial<T,Plo,Phi> compose(const Polynomial<T,Plo,Phi>& p, const Shifting<T> g)
    {
        /*
        example:
        P(x)   = p+qx+rx²
        G(x)   = s+x
        P∘G(x) = p+q(s+x)+r(s+x)²
        P∘G(x) = p+q(s+x)+r(s²+2sx+x²)
        P∘G(x) = p+qs+qx+rs²+r2sx+rx²
        P∘G(x) = p+rs²+qs + qx+r2sx + rx²
        */
        Polynomial<T,Plo,Phi> pg;
        for (int i = Plo; i <= Phi; ++i)
        {
            for (int j = 0; j<= i; ++j)
            {
                pg[j] += p[i]*combination(i,j)*std::pow(g.offset, i-j);
            }
        }
        return pg;
    }

    template<typename T, int Qlo, int Qhi, 
        typename = std::enable_if_t<(Qlo >= 0 && Qhi >= 0)> >
    constexpr T compose(const Polynomial<T,0,0>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        return p[0];
    }

    template<typename T, int Qlo, int Qhi, 
        typename = std::enable_if_t<(Qlo >= 0 && Qhi >= 0)> >
    constexpr Polynomial<T,Qlo,Qhi> compose(const Polynomial<T,0,1> p, const Polynomial<T,Qlo,Qhi>& q)
    {
        return p[0] + p[1]*q;
    }

    // template<typename T, int Plo, int Qlo, int Qhi, 
    //     typename = std::enable_if_t<(Plo > 0 && Qlo >= 0 && Qhi >= 0)> >
    // constexpr Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)> compose(const Polynomial<T,Plo,Plo+1> p, const Polynomial<T,Qlo,Qhi>& q)
    // {
    //     return q*compose(Polynomial<T,Plo-1,Plo>(std::next((p.k).begin()), (p.k).end()));
    // }

    template<typename T, int Phi, int Qlo, int Qhi, 
        typename = std::enable_if_t<(Phi > 1 && Qlo >= 0 && Qhi >= 0)> >
    constexpr Polynomial<T,std::min(0,Phi*Qhi),std::max(0,Phi*Qhi)> compose(const Polynomial<T,0,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
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
        return p[0]+q*compose(Polynomial<T,0,Phi-1>(std::next((p.k).begin()), (p.k).end()), q);
    }

    // template<typename T, int Plo, int Phi, int Q>
    // constexpr Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)> compose(const Polynomial<T,0,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    // {
    //     if (Q<0)
    //     {
    //         std::reverse
    //     }
    //     return p[0]+q*compose(Polynomial<T,0,Phi-1>(std::next((p.k).begin()), (p.k).end()), q);
    // }





    template<typename T>
    constexpr Polynomial<T,0,1> linear_newton_polynomial(
        const T x1, 
        const T x2,
        const T y1, 
        const T y2
    ){
        const T dfdx = (y2-y1) / (x2-x1);
        return y1 + dfdx * Shifting<T>(-x1);
    }

    template<typename T, typename F>
    constexpr Polynomial<T,0,1> linear_newton_polynomial(
        const F f, 
        const T x1, 
        const T x2
    ){
        return linear_newton_polynomial(x1,x2, f(x1),f(x2));
    }

    template<typename T, typename F>
    constexpr Polynomial<T,0,1> linear_taylor_series(const F f, const T x, const T dx)
    {
        const T dx2 = dx*dx;
        return compose(
            Polynomial<T,0,1>(f(x), 
                central_finite_difference(f, x, dx, 1) / dx),
            Shifting<T>(-x)
        );
    }

    template<typename T>
    constexpr Polynomial<T,0,2> quadratic_newton_polynomial(
        const T x1, 
        const T x2, 
        const T x3,
        const T y1, 
        const T y2, 
        const T y3
    ){
        const T dydx_12  = (y2-y1) / (x2-x1);
        const T dydx_23  = (y3-y2) / (x3-x2);
        const T dy2dx2 = (dydx_23-dydx_12) / (x3-x1);
        return y1 
            + dydx_12 * Shifting<T>(-x1)
            + dy2dx2  * Shifting<T>(-x1) * Shifting<T>(-x2);
    }

    template<typename T, typename F>
    constexpr Polynomial<T,0,2> quadratic_newton_polynomial(
        const F f, 
        const T x1, 
        const T x2, 
        const T x3
    ){
        return quadratic_newton_polynomial(x1,x2,x3, f(x1),f(x2),f(x3));
    }

    template<typename T, typename F>
    constexpr Polynomial<T,0,2> quadratic_taylor_series(const F f, const T x, const T dx)
    {
        const T dx2 = dx*dx;
        return compose(
            Polynomial<T,0,2>{f(x), 
                central_finite_difference(f, x, dx, 1) / dx, 
                central_finite_difference(f, x, dx, 2) /(dx2*2.0f)},
            Shifting<T>(-x)
        );
    }

    template<typename T>
    constexpr Polynomial<T,0,3> cubic_newton_polynomial(
        const T x1, 
        const T x2, 
        const T x3,
        const T x4,
        const T y1, 
        const T y2, 
        const T y3,
        const T y4
    ){
        const T dydx_12 = (y2-y1) / (x2-x1);
        const T dydx_23 = (y3-y2) / (x3-x2);
        const T dydx_34 = (y4-y3) / (x4-x3);
        const T dy2dx2_13 = (dydx_23-dydx_12) / (x3-x1);
        const T dy2dx2_24 = (dydx_34-dydx_23) / (x4-x2);
        const T dy3dx3    = (dy2dx2_24-dy2dx2_13) / (x4-x1);
        return y1 
            + dydx_12   * Shifting<T>(-x1)
            + dy2dx2_13 * Shifting<T>(-x1) * Shifting<T>(-x2)
            + dy3dx3    * Shifting<T>(-x1) * Shifting<T>(-x2) * Shifting<T>(-x3);
    }

    template<typename T, typename F>
    constexpr Polynomial<T,0,3> cubic_newton_polynomial(
        const F f, 
        const T x1, 
        const T x2, 
        const T x3,
        const T x4
    ){
        return cubic_newton_polynomial(x1,x2,x3,x4, f(x1),f(x2),f(x3),f(x4));
    }

    template<typename T, typename F>
    constexpr Polynomial<T,0,3> cubic_taylor_series(const F f, const T x, const T dx)
    {
        const T dx2 = dx*dx;
        const T dx3 = dx2*dx;
        return 
            compose(
                Polynomial<T,0,3>(f(x), 
                    central_finite_difference(f, x, dx, 1) / dx, 
                    central_finite_difference(f, x, dx, 2) /(dx2*2.0f), 
                    central_finite_difference(f, x, dx, 3) /(dx3*6.0f)),
                Shifting<T>(-x)
            );
    }


}