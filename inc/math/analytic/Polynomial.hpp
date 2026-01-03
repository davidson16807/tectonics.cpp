#pragma once

// C libraries
#include <cmath>

// std libraries
#include <string>      // string
#include <ostream>     // operator<<
#include <vector>      // vector
#include <array>       // array
#include <algorithm>   // clamp, min_element, fill, copy
#include <complex>     // complex
#include <type_traits> // enable_if_t

// in-house libraries
#include "Identity.hpp"
#include "Scaling.hpp"
#include "Shifting.hpp"
#include "ScaledComplement.hpp"

#include <math/inspected/CentralFiniteDifference.hpp>
#include <math/combinatorics.hpp>

namespace analytic {

    /* 
    `Polynomial<T,Plo,Phi>` is a class template that represents functions of the form f(x)=Σᵢaᵢxᵇⁱ where bᵢ∈ℤ.
    It is designed for high performance applications where a function must be composed from other functions.
    To address data locality concerns, the data structure is stored on the stack, 
    and users may control size in memory by specifying the highest and lowest exponent within the polynomial.

    A side effect of this design is that it allows the user to specify negative exponents.
    Introducing a negative exponent into the expression creates what is known as a "Laurent polynomial".
    A Laurent polynomial runs in the same amount of time as a classic polynomial that has the same number of terms,
    and support for Laurent polynomials places little to no performance penalties on classic polynomial behavior,
    so we support Laurent polynomials as a degenerate case to avoid creating a separate implementation.
    */
    template<typename T, int Plo, int Phi>
    struct Polynomial
    {
        using value_type = T;

        std::array<T,Phi+1-Plo> k;
        // the zero polynomial
        constexpr Polynomial(): k()
        {
            static_assert(Plo <= Phi, "Polynomial<Plo,Phi> must satisfy Plo≤Phi");
            std::fill(k.begin(), k.end(), T(0));
        }
        // constant constructor
        constexpr explicit Polynomial(const T k2): k()
        {
            static_assert(Plo==Phi || Plo<=0&&0<=Phi, "Polynomial<Plo,Phi> must satisfy Plo=Phi or Plo≤0≤Phi");
            // regardless of Plo or Phi, if there is only one term, the caller unambiguously means to set its coefficient
            if (Plo==Phi) 
            {
                k[Plo] = k2;
            }
            else
            {
                std::fill(k.begin(), k.end(), T(0));
                k[0-Plo] = k2;
            }
        }
        // cast constructor
        template <typename T2, int Qlo, int Qhi, 
            typename = std::enable_if_t<(Plo <= Qlo&&Qhi <= Phi)>> 
        constexpr Polynomial(const Polynomial<T2,Qlo,Qhi>& p): k()
        {
            static_assert(Plo <= Phi, "Polynomial<Plo,Phi> must satisfy Plo≤Phi");
            std::fill(k.begin(), k.end(), T(0));
            for (int i = Qlo; i <= Qhi; ++i)
            {
                k[i-Plo] = T(p.k[i-Qlo]);
            }
        }
        constexpr explicit Polynomial(const Identity<T> e): k()
        {
            static_assert(Plo <= Phi, "Polynomial<Plo,Phi> must satisfy Plo≤Phi");
            std::fill(k.begin(), k.end(), T(0));
            k[1-Plo] = T(1);
        }
        constexpr explicit Polynomial(const Scaling<T> f): k()
        {
            static_assert(Plo <= Phi, "Polynomial<Plo,Phi> must satisfy Plo≤Phi");
            std::fill(k.begin(), k.end(), T(0));
            k[1-Plo] = f.factor;
        }
        constexpr explicit Polynomial(const Shifting<T> f): k()
        {
            static_assert(Plo <= Phi, "Polynomial<Plo,Phi> must satisfy Plo≤Phi");
            std::fill(k.begin(), k.end(), T(0));
            k[0-Plo] = f.offset;
            k[1-Plo] = T(1);
        }
        constexpr explicit Polynomial(const ScaledComplement<T,Identity<T>> f): k()
        {
            static_assert(Plo <= Phi, "Polynomial<Plo,Phi> must satisfy Plo≤Phi");
            std::fill(k.begin(), k.end(), T(0));
            k[0-Plo] = T(1);
            k[1-Plo] = T(-1)/f.scale;
        }
        constexpr explicit Polynomial(const std::array<T,Phi+1-Plo> k2)
        {
            static_assert(Plo <= Phi, "Polynomial<Plo,Phi> must satisfy Plo≤Phi");
            // std::fill(k.begin(), k.end(), T(0));
            std::copy(k2.begin(), k2.end(), k.begin());
        }
        template<typename TIterator>
        constexpr explicit Polynomial(TIterator first, TIterator last)
        {
            static_assert(Plo <= Phi, "Polynomial<Plo,Phi> must satisfy Plo≤Phi");
            std::fill(k.begin(), k.end(), T(0));
            std::copy(first, last, k.begin());
        }
        constexpr Polynomial<T,Plo,Phi>& operator=(const Polynomial<T,Plo,Phi>& p)
        {
            std::fill(k.begin(), k.end(), T(0));
            std::copy(p.k.begin(), p.k.end(), k.begin());
            return *this;
        }
        constexpr T operator()(const T x) const
        {
            T y(0);
            for (int i = Plo; i <= -1; ++i)
            {
                // Exponents are calculated using pow(), 
                // rather than repeated multiplication, to avoid precision errors
                // We check for k[i-Plo] == T(0) as a sensible prevention of nans when dividing by 0.
                y += k[i-Plo] == T(0)? T(0) : k[i-Plo] * std::pow(x, i);
            }
            int i0(std::max(Plo,0));
            T xi(std::pow(x, i0));
            for (int i = i0; i <= Phi; ++i)
            {
                y += k[i-Plo] * xi;
                xi*= x;
            }
            return y;
        }
        constexpr T& operator[](const int i)
        {
            return k[i-Plo];
        }
        constexpr T operator[](const int i) const
        {
            return Plo<=i&&i<=Phi? k[i-Plo] : T(0);
        }
        template<int Qlo, int Qhi, typename = std::enable_if_t<(Plo <= Qlo&&Qhi <= Phi)>>
        constexpr Polynomial<T,Plo,Phi>& operator+=(const Polynomial<T,Qlo,Qhi>& p)
        {
            for (int i = Qlo; i <= Qhi; ++i)
            {
                k[i-Plo] += p.k[i-Qlo];
            }
            return *this;
        }
        template<int Qlo, int Qhi, typename = std::enable_if_t<(Plo <= Qlo&&Qhi <= Phi)>>
        constexpr Polynomial<T,Plo,Phi>& operator-=(const Polynomial<T,Qlo,Qhi>& p)
        {
            for (int i = Qlo; i <= Qhi; ++i)
            {
                k[i-Plo] -= p.k[i-Qlo];
            }
            return *this;
        }
        constexpr Polynomial<T,Plo,Phi>& operator+=(const T k2)
        {
            k[0-Plo] += k2;
            return *this;
        }
        constexpr Polynomial<T,Plo,Phi>& operator-=(const T k2)
        {
            k[0-Plo] -= k2;
            return *this;
        }
        constexpr Polynomial<T,Plo,Phi>& operator*=(const T k2)
        {
            for (int i = Plo; i <= Phi; ++i)
            {
                k[i-Plo] *= k2;
            }
            return *this;
        }
        constexpr Polynomial<T,Plo,Phi>& operator/=(const T k2)
        {
            for (int i = Plo; i <= Phi; ++i)
            {
                k[i-Plo] /= k2;
            }
            return *this;
        }
    };



    template <typename T>
    constexpr Polynomial<T,0,0> polynomial(const T k2)
    {
        return Polynomial<T,0,0>(k2);
    }

    template <typename T, int Plo, int Phi> 
    constexpr Polynomial<T,Plo,Phi> polynomial(const Polynomial<T,Plo,Phi>& p)
    {
        return Polynomial<T,Plo,Phi>(p);
    }

    template <typename T>
    constexpr Polynomial<T,1,1> polynomial(const Identity<T> e)
    {
        return Polynomial<T,1,1>(e);
    }

    template <typename T>
    constexpr Polynomial<T,1,1> polynomial(const Scaling<T> f)
    {
        return Polynomial<T,1,1>(f);
    }

    template <typename T>
    constexpr Polynomial<T,0,1> polynomial(const Shifting<T> f)
    {
        return Polynomial<T,0,1>(f);
    }

    template <typename T>
    constexpr Polynomial<T,0,1> polynomial(const ScaledComplement<T,Identity<T>> f)
    {
        return Polynomial<T,0,1>(f);
    }


    template<typename T, int Plo, int Phi>
    constexpr std::string to_string(const Polynomial<T,Plo,Phi>& p)
    {
        // const std::string exponents("⁰¹²³⁴⁵⁶⁷⁸⁹");
        std::string output;
        for (int i = Plo; i <= Phi; ++i)
        {
            output += std::to_string(p[i]);
            output += i==0?       "" : "x";
            output += 0<=i&&i<=1? "" : "^" + std::to_string(i);
            output += i>=Phi?     "" : " + ";
        }
        return output;
    }

    template<typename T, int Plo, int Phi>
    std::ostream& operator<<(std::ostream& os, const Polynomial<T,Plo,Phi>& p) { 
        os << to_string(p);
        return os;
    }

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
        y *=T(-1);
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
        Polynomial<T,-N,-N> y;
        y[-N] = k/p[N];
        return y;
    }

    template<typename T, int Plo, int Phi>
    constexpr Polynomial<T,Plo,Phi> operator-(const Polynomial<T,Plo,Phi>& p)
    {
        Polynomial<T,Plo,Phi> y(p);
        y *=T(-1);
        return y;
    }

    // operators that are not closed under Polynomial<T,Plo,Phi> relations, but produce Polynomial<T,> relations of some degree
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const Polynomial<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> y(p);
        y += q;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const Polynomial<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> y(p);
        y -= q;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Polynomial<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
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
    constexpr auto operator/(const Polynomial<T,Plo,Phi>& p, const Polynomial<T,M,M>& q)
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


























    template<typename T>
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







    // template<typename T>
    // constexpr auto operator+(const Scaling<T> f, const Scaling<T> g)
    // {
    //     return Polynomial<T,1,1>(f) + Polynomial<T,1,1>(g);
    // }

    // template<typename T>
    // constexpr auto operator-(const Scaling<T> f, const Scaling<T> g)
    // {
    //     return Polynomial<T,1,1>(f) - Polynomial<T,1,1>(g);
    // }
    template<typename T>
    constexpr auto operator*(const Scaling<T> f, const Scaling<T> g)
    {
        return Polynomial<T,1,1>(f) * Polynomial<T,1,1>(g);
    }
    // template<typename T>
    // constexpr auto operator/(const Scaling<T> f, const Scaling<T> g)
    // {
    //     return Polynomial<T,1,1>(f) / Polynomial<T,1,1>(g);
    // }



    template<typename T>
    constexpr auto operator+(const Shifting<T> f, const Shifting<T> g)
    {
        return Polynomial<T,0,1>(f) + Polynomial<T,0,1>(g);
    }
    // template<typename T>
    // constexpr auto operator-(const Shifting<T> f, const Shifting<T> g)
    // {
    //     return Polynomial<T,0,1>(f) - Polynomial<T,0,1>(g);
    // }

    template<typename T>
    constexpr auto operator*(const Shifting<T> f, const Shifting<T> g)
    {
        return Polynomial<T,0,1>(f) * Polynomial<T,0,1>(g);
    }
    // template<typename T>
    // constexpr auto operator/(const Shifting<T> f, const Shifting<T> g)
    // {
    //     return Polynomial<T,0,1>(f) / Polynomial<T,0,1>(g);
    // }



    // template<typename T>
    // constexpr auto operator+(const Identity<T> e1, const Identity<T> e2)
    // {
    //     return Polynomial<T,1,1>(e) + Polynomial<T,1,1>(e);
    // }
    // template<typename T>
    // constexpr auto operator-(const Identity<T> e1, const Identity<T> e2)
    // {
    //     return Polynomial<T,1,1>(e) - Polynomial<T,1,1>(e);
    // }

    template<typename T>
    constexpr auto operator*(const Identity<T> e1, const Identity<T> e2)
    {
        return Polynomial<T,1,1>(e1) * Polynomial<T,1,1>(e2);
    }
    // template<typename T>
    // constexpr auto operator/(const Identity<T> e1, const Identity<T> e2)
    // {
    //     return Polynomial<T,1,1>(e1) / Polynomial<T,1,1>(e2);
    // }


    template<typename T>
    constexpr Polynomial<T,0,1> inverse(const Polynomial<T,0,1>& y) 
    {
        // we know:           y=a+bx
        // therefore: -a/b+1/by=x
        Polynomial<T,0,1> x;
        x[0] = -y[0]/y[1];
        x[1] = T(1)/y[1];
        return x;
    }

    template<typename T>
    constexpr Polynomial<T,1,1> inverse(const Polynomial<T,1,1>& y) 
    {
        // we know:        y=ax
        // therefore:      y/a=x
        Polynomial<T,1,1> x;
        x[1] = T(1)/y[1];
        return x;
    }


    /* 
    NOTE: we avoid expressing derivatives and integrals as methods,
    since not all equations are closed under derivatives and integrals,
    and such methods would complicate the order with which classes must be declared
    */
    template<typename T>
    constexpr T derivative(const Polynomial<T,0,1> p) 
    {
        return p[1];
    }

    template<typename T>
    constexpr T derivative(const Polynomial<T,1,1> p) 
    {
        return p[1];
    }

    template<typename T, int Plo, int Phi>
    Polynomial<T,Plo==0?0:Plo-1,Phi-1> derivative(const Polynomial<T,Plo,Phi>& p)
    {
        Polynomial<T,Plo==0?0:Plo-1,Phi-1> dpdx;
        for (int i = std::max(1,Plo); i<=Phi; ++i)
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
        T dydx(0);
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
        T I(0);

        for (int i = Plo; i < -1; ++i)
        {
            // exponents are calculated using pow(), 
            // rather than repeated multiplication, to avoid precision errors
            I += p[i] * std::pow(x, i+1) / (i+1);
        }

        I += p[-1] * (p[-1] != T(0) && Plo < 0? 
            std::log(std::abs(x)) : T(0));

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
        T I(0);

        for (int i = Plo; i < -1; ++i)
        {
            I += p[i] *  (std::pow(hi, i+1) / (i+1) 
                        - std::pow(lo, i+1) / (i+1));
        }

        I += p[-1] * (p[-1] != T(0) && Plo < 0? 
            std::log(std::abs(hi)) - std::log(std::abs(lo)) : T(0));

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

    template<int N, typename T, int Plo, int Phi, 
        typename = std::enable_if_t<(N==0)>>
    constexpr Polynomial<T,0,0> pow(const Polynomial<T,Plo,Phi>& p){
        return Polynomial<T,0,0>(T(1));
    }

    template<int N, typename T, int Plo, int Phi, 
        typename = std::enable_if_t<(N==1)>>
    constexpr Polynomial<T,Plo,Phi> pow(const Polynomial<T,Plo,Phi>& p){
        return p;
    }

    template<int N, typename T, int Plo, int Phi, 
        typename = std::enable_if_t<(N>1)>>
    constexpr Polynomial<T,Plo*N,Phi*N> pow(const Polynomial<T,Plo,Phi>& p){
        return p*pow<N-1>(p);
    }

    template<int N, typename T, int P, 
        typename = std::enable_if_t<(N<0)>>
    constexpr Polynomial<T,P*N,P*N> pow(const Polynomial<T,P,P>& p){
        return pow<N>(1/p);
    }


    template<int N, typename T,
        typename = std::enable_if_t<(N>=0)>>
    constexpr auto pow(const Identity<T>& e){
        return pow<N>(Polynomial<T,1,1>(e));
    }

    template<int N, typename T,
        typename = std::enable_if_t<(N>=0)>>
    constexpr auto pow(const Shifting<T>& f){
        return pow<N>(Polynomial<T,0,1>(f));
    }

    template<int N, typename T,
        typename = std::enable_if_t<(N>=0)>>
    constexpr auto pow(const Scaling<T>& f){
        return pow<N>(Polynomial<T,1,1>(f));
    }

    template<int N, typename T,
        typename = std::enable_if_t<(N>=0)>>
    constexpr auto pow(const ScaledComplement<T,Identity<T>>& f){
        return pow<N>(Polynomial<T,0,1>(f));
    }


    /*
    `solution()` provides the single real valued solution to a function
    where only one solution exists
    */
    template<typename T>
    constexpr T solution(const Polynomial<T,0,1> p, const T y) 
    {
        return (y-p[0]) / p[1];
    }

    /*
    `solutions()` provides all possible solutions to a polynomial, complex or otherwise.
    The number of solutions under these conditions is always known at compile time,
    so the output is stored either as a T (for linear functions) 
    or as an array of complex numbers (for any other polynomial).
    This can be useful if performance takes precedence over ease of implementation.
    */
    template<typename T>
    std::array<std::complex<T>, 1> solutions(const Polynomial<T,0,1> p, const T y) 
    {
        return std::array<std::complex<T>, 1>{(y-p[0]) / p[1]};
    }

    template<typename T>
    std::array<std::complex<T>, 2> solutions(const Polynomial<T,0,2> p, const T y) 
    {
        // the quadratic formula
        const std::complex<T> sqrt_argument = p[1]*p[1] - T(4)*(p[0]-y)*p[2];
        return std::array<std::complex<T>, 2>{
            (-p[1]+std::sqrt(sqrt_argument)) / (T(2)*p[2]),
            (-p[1]-std::sqrt(sqrt_argument)) / (T(2)*p[2])
        };
    }

    template<typename T>
    std::array<std::complex<T>, 3> solutions(const Polynomial<T,0,3> p, const T y) 
    {
        // the cubic formula
        const Polynomial<T,0,3> a = p/p[3]; // monic cubic polynomial

        const std::complex<T> q =  a[1]/T(3) - a[2]*a[2]/T(9);
        const std::complex<T> r = (a[1]*a[2] - T(3)*a[0])/T(6) - a[2]*a[2]*a[2]/T(27);

        const std::complex<T> s1 = std::pow(r+std::sqrt(q*q*q+r*r), T(1)/T(3));
        const std::complex<T> s2 = std::pow(r-std::sqrt(q*q*q+r*r), T(1)/T(3));

        const std::complex<T> z1 = {(s1+s2) - a[2]/T(3), T(0)};
        const std::complex<T> z2 = {T(0), -(s1+s2)/T(2) - a[2]/T(3) + (s1-s2)*std::cbrt(T(3))/T(2)}; // i
        const std::complex<T> z3 = {T(0), -(s1+s2)/T(2) - a[2]/T(3) - (s1-s2)*std::cbrt(T(3))/T(2)}; // i

        return std::array<std::complex<T>, 3>{z1,z2,z3};
    }

    /*
    `reals()` is a convenience function that accepts iterators for a container of complex values 
    and returns an iterator containing only those values that are strictly real.
    This can be useful when condensing solutions to polynomial equations.
    */
    template<typename TInputIterator, typename TOutputIterator>
    void reals(
        TInputIterator first, 
        TInputIterator last, 
        TOutputIterator result
    ){
        while(first != last)
        {
            auto z = *first;
            if (std::abs(z.imag()) > 0*z.imag())
            {
                *result = z.real();
                ++result;
                ++first;
            }
        }
    }

    template<typename T, int Plo>
    constexpr auto extremum(const Polynomial<T,Plo,2>& p) 
    {
        return solution(derivative(p), T(0));
    }

    template<typename T, int Plo, int Phi>
    constexpr auto extrema(const Polynomial<T,Plo,Phi>& p) 
    {
        return solutions(derivative(p), T(0));
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
        auto candidates = extrema(p);
        candidates.push_back(lo);
        candidates.push_back(hi);
        return *std::max_element(candidates.begin(), candidates.end(), 
            [&](T a, T b){ return p(std::clamp(a,lo,hi)) < p(std::clamp(b,lo,hi)); });
    }

    template<typename T, int Plo, int Phi>
    T minimum(const Polynomial<T,Plo,Phi>& p, const T lo, const T hi) 
    {
        auto candidates = extrema(p);
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
        return std::sqrt(integral((p-q)*(p-q), lo, hi) / (hi-lo));
    }

    template<typename T, int Plo, int Phi>
    constexpr T distance(const Polynomial<T,Plo,Phi> p, const T k, const T lo, const T hi)
    {
        return std::sqrt(integral((p-k)*(p-k), lo, hi) / (hi-lo));
    }

    template<typename T, int Plo, int Phi>
    constexpr T distance(const T k, const Polynomial<T,Plo, Phi> p, const T lo, const T hi)
    {
        return std::sqrt(integral((p-k)*(p-k), lo, hi) / (hi-lo));
    }

    template<typename T, int Plo, int Phi>
    constexpr T distance(const Polynomial<T,Plo,Phi> p, const Scaling<T> f, const T lo, const T hi)
    {
        return distance(p, Polynomial<T,1,1>(f), lo, hi);
    }

    template<typename T, int Plo, int Phi>
    constexpr T distance(const Scaling<T> f, const Polynomial<T,Plo, Phi> p, const T lo, const T hi)
    {
        return distance(Polynomial<T,1,1>(f), p, lo, hi);
    }

    template<typename T>
    constexpr T distance(const Scaling<T> f, const Scaling<T> g, const T lo, const T hi)
    {
        return distance(Polynomial<T,1,1>(f), Polynomial<T,1,1>(g), lo, hi);
    }

    template<typename T, int Plo, int Phi>
    constexpr T distance(const Polynomial<T,Plo,Phi> p, const Shifting<T> f, const T lo, const T hi)
    {
        return distance(p, Polynomial<T,0,1>(f), lo, hi);
    }

    template<typename T, int Plo, int Phi>
    constexpr T distance(const Shifting<T> f, const Polynomial<T,Plo, Phi> p, const T lo, const T hi)
    {
        return distance(Polynomial<T,0,1>(f), p, lo, hi);
    }

    template<typename T>
    constexpr T distance(const Shifting<T> f, const Shifting<T> g, const T lo, const T hi)
    {
        return distance(Polynomial<T,0,1>(f), Polynomial<T,0,1>(g), lo, hi);
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
            pg[i] *= std::pow(g.factor, i);
        }
        return pg;
    }

    template<typename T, int Plo, int Phi>
    constexpr Polynomial<T,Plo,Phi> compose(const Scaling<T> g, const Polynomial<T,Plo, Phi>& p)
    {
        return g.factor*p;
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
                pg[j] += p[i] * combinatoric::combination(i,j)*std::pow(g.offset, i-j);
            }
        }
        return pg;
    }

    template<typename T, int Plo, int Phi>
    constexpr auto compose(const Shifting<T> g, const Polynomial<T,Plo, Phi>& p)
    {
        return g.offset+p;
    }


    template<typename T, 
        typename Expression> constexpr T compose(const Polynomial<T,0,0>& p, const Expression& f)        { return p[0]; }
    template<typename T>     constexpr T compose(const Polynomial<T,0,0>& p, const int f)                { return p[0]; }
    template<typename T>     constexpr T compose(const Polynomial<T,0,0>& p, const float f)              { return p[0]; }
    template<typename T>     constexpr T compose(const Polynomial<T,0,0>& p, const double f)             { return p[0]; }
    template<typename T>     constexpr T compose(const Polynomial<T,0,0>& p, const Identity<T>& f)       { return p[0]; }
    template<typename T>     constexpr T compose(const Polynomial<T,0,0>& p, const Scaling<T>& f)        { return p[0]; }
    template<typename T>     constexpr T compose(const Polynomial<T,0,0>& p, const Shifting<T>& f)       { return p[0]; }
    template<typename T>     constexpr T compose(const Polynomial<T,0,0>& p, const Polynomial<T,0,0>& q) { return p[0]; }

    template<typename T, int Qlo, int Qhi, 
        typename = std::enable_if_t<(Qlo >= 0 && Qhi >= 0)> >
    constexpr auto compose(const Polynomial<T,0,1> p, const Polynomial<T,Qlo,Qhi>& q)
    {
        return p[0] + p[1]*q;
    }

    template<typename T, int Qlo, int Qhi, 
        typename = std::enable_if_t<(Qlo >= 0 && Qhi >= 0)> >
    constexpr auto compose(const Polynomial<T,1,1> p, const Polynomial<T,Qlo,Qhi>& q)
    {
        return p[1]*q;
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

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, 
        typename = std::enable_if_t<(Plo > 0 && Phi > 1 && Qlo >= 0 && Qhi >= 0)> >
    constexpr Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)> compose(const Polynomial<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        const auto x = Identity<T>();
        return q*compose(p/x, q);
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
    constexpr Polynomial<T,0,1> linear_taylor_series(const F f, const T x, const T dx)
    {
        const T dx2 = dx*dx;
        return compose(
            Polynomial<T,0,1>(f(x), 
                inspected::central_finite_difference(f, x, dx, 1) / dx),
            Shifting<T>(-x)
        );
    }

    template<typename T, typename F>
    constexpr Polynomial<T,0,2> quadratic_taylor_series(const F f, const T x, const T dx)
    {
        const T dx2 = dx*dx;
        return compose(
            Polynomial<T,0,2>{f(x), 
                inspected::central_finite_difference(f, x, dx, 1) / dx, 
                inspected::central_finite_difference(f, x, dx, 2) /(dx2*T(2))},
            Shifting<T>(-x)
        );
    }

    template<typename T, typename F>
    constexpr Polynomial<T,0,3> cubic_taylor_series(const F f, const T x, const T dx)
    {
        const T dx2 = dx*dx;
        const T dx3 = dx2*dx;
        return 
            compose(
                Polynomial<T,0,3>(f(x), 
                    inspected::central_finite_difference(f, x, dx, 1) / dx, 
                    inspected::central_finite_difference(f, x, dx, 2) /(dx2*T(2)), 
                    inspected::central_finite_difference(f, x, dx, 3) /(dx3*T(6))),
                Shifting<T>(-x)
            );
    }


    /* 
    NOTE: The following is an alternate implementation to "cubic_spline()".
    It requires no familiarity with linear algebra, it places no dependency on the glm library,
    and it provides a good way to verify other spline functions, 
    however its implementation is more complex and cannot be easily adapted to higher order splines.
    */
    template<typename T, typename F>
    constexpr Polynomial<T,0,3> cubic_algebraic_spline(
        const T x0, // lower bound of spline
        const T x1, // upper bound of spline
        const T y0, // value of y at x1
        const T y1, // value of y at x1
        const T d0, // derivative of y at x0
        const T d1  // derivative of y at x1
    ){
        /*
        First, we construct a new coordinate system based around (x0,y0) as the origin. 
        In this coordinate system, the coordinates (x1,y1) are denoted (X,Y).
        */
        const T Y = y1-y0;
        const T X = x1-x0;
        /*
        in the new coordinate system, we know that x0=0 and y0=0, 
        so we can adopt a simpler system of equations:
           Y =  aX² +  cX³
          d1 = 2aX  + 3cX²
        we divide Y and d1 by X² and X respectively to get:
           Y =  a +  cX
          d1 = 2a + 3cX
        */
        const T u = (Y-d0)/(X*X);
        const T v = (d1-d0)/X;
        /*
        We then solve this system of equations, which results in the following code:
        */
        const Identity<T> x;
        const auto x2 = x*x;
        const auto x3 = x*x*x;
        return y0 + 
            compose(
                d0*x + (T(3)*u-v)*x2 + ((v-T(2)*u)/X)*x3,
                x-x0
            );
    }

    /*
    `legendre_polynomial<T,N>` is the nth standard Legendre polynomial:

        pₙ(z) = dⁿ/dzⁿ (z²-1)ⁿ/(2ⁿl!) 
    */

    template<typename T, int N>
    constexpr auto legendre_polynomial()
    {
        return 
            higher_order_derivative<N>(pow<N>(Identity<T>()*Identity<T>()-T(1))) / 
            (std::pow(T(2),T(N)) * combinatoric::factorial(N));
    }

}

