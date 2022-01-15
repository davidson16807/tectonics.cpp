#pragma once

#include <iostream>
#include <algorithm>

// in-house libraries
#include <units/si.hpp>

namespace compound {
namespace relation {


    /* 
    `Identity` is a trivial class that represents the function f(x)=x,
    oddly enough, it was useful for something, so we implemented it.
    (it was useful as the base case for the recursive implementation of `compose()` for polynomials)
    */
    struct Identity {
        constexpr explicit Identity()
        {}
        constexpr float operator()(const float x) const
        {
            return x;
        }
    };
    
    // operators that cause cancelation
    constexpr float operator-(const Identity e1, const Identity e2)
    {
        return 0.0f;
    }
    constexpr float operator/(const Identity e1, const Identity e2)
    {
        return 0.0f;
    }

    /* 
    `Scaling` is a trivial class that represents the function f(x)=ax,
    it was needed to provide an idiomatic way to request that input to other functions be scaled using compose(),
    however it has since found other uses.
    */
    struct Scaling {
        float factor;
        constexpr explicit Scaling(const float factor):
            factor(factor)
        {}
        constexpr float operator()(const float x) const
        {
            return factor*x;
        }

        constexpr Scaling& operator*=(const float k)
        {
            factor *= k;
            return *this;
        }

        constexpr Scaling& operator/=(const float k)
        {
            factor /= k;
            return *this;
        }

    };

    // operators that are closed under Scaling relations
    constexpr Scaling operator-(const Scaling f)
    {
        return Scaling(-f.factor);
    }

    // operators with reals that are closed under Scaling relations
    constexpr Scaling operator*(const Scaling f, const float k)
    {
        return Scaling(f.factor * k);
    }
    constexpr Scaling operator*(const float k, const Scaling f)
    {
        return Scaling(k * f.factor);
    }
    constexpr Scaling operator/(const Scaling f, const float k)
    {
        return Scaling(f.factor / k);
    }

    // operators that produce Scaling relations when given other relations as input
    constexpr Scaling operator*(const Identity f, const float k)
    {
        return Scaling(k);
    }
    constexpr Scaling operator*(const float k, const Identity f)
    {
        return Scaling(k);
    }
    constexpr Scaling operator/(const Identity f, const float k)
    {
        return Scaling(1.0f / k);
    }
    constexpr Scaling operator-(const Identity e)
    {
        return Scaling(-1.0f);
    }

    // operators that cause cancelation
    constexpr float operator/(const Scaling f, const Scaling g)
    {
        return f.factor/g.factor;
    }
    constexpr float operator/(const Identity e, const Scaling f)
    {
        return 1.0f/f.factor;
    }
    constexpr float operator/(const Scaling f, const Identity e)
    {
        return f.factor;
    }

    /* 
    `Shifting` is a trivial class that represents the function f(x)=x+b,
    it was needed to provide an idiomatic way to generate taylor series using compose(),
    however it has since found other uses.
    */
    struct Shifting {
        float offset;
        constexpr explicit Shifting(const float offset):
            offset(offset)
        {}
        constexpr float operator()(const float x) const
        {
            return x+offset;
        }

        Shifting& operator+=(const float k)
        {
            offset += k;
            return *this;
        }

        Shifting& operator-=(const float k)
        {
            offset -= k;
            return *this;
        }
    };

    // operators with reals that are closed under Shifting relations
    constexpr Shifting operator+(const Shifting f, const float k)
    {
        return Shifting(f.offset + k);
    }
    constexpr Shifting operator+(const float k, const Shifting f)
    {
        return Shifting(f.offset + k);
    }
    constexpr Shifting operator-(const Shifting f, const float k)
    {
        return Shifting(f.offset - k);
    }

    // operators that produce Scaling relations when given other relations as input
    constexpr Shifting operator+(const Identity f, const float k)
    {
        return Shifting(k);
    }
    constexpr Shifting operator+(const float k, const Identity f)
    {
        return Shifting(k);
    }
    constexpr Shifting operator-(const Identity f, const float k)
    {
        return Shifting(-k);
    }

    // operators that cause cancelation
    constexpr float operator-(const Shifting f, const Shifting g)
    {
        return f.offset-g.offset;
    }
    
    /* 
    `Polynomial<N>` is a class template that represents functions of the form f(x)=k₀x⁰+k₁x¹+k₂x²+...+kₙxⁿ.
    N is the degree of the polynomial, or the highest exponent of any nonzero term within the polynomial,
    meaning that a polynomial of degree N has N+1 coeffiicents, which are denoted k[0] through k[N].
    */
    template<int N>
    struct Polynomial
    {
        std::array<float,N+1> k;
        constexpr Polynomial(const Polynomial& p): k()
        {
            std::copy(p.k.begin(), p.k.end(), k.begin());
        }
        constexpr explicit Polynomial(const float k2): k()
        {
            k[0] = k2;
        }
        constexpr explicit Polynomial(const Identity e): k()
        {
            k[1] = 1.0f;
        }
        constexpr explicit Polynomial(const Scaling f): k()
        {
            k[1] = f.factor;
        }
        constexpr explicit Polynomial(const Shifting f): k()
        {
            k[0] = f.offset;
            k[1] = 1.0f;
        }
        constexpr explicit Polynomial(const std::array<float,N+1> k2): k()
        {
            std::copy(k2.begin(), k2.end(), k.begin());
        }
        template <int M> 
        constexpr Polynomial(const Polynomial<M>& p): k()
        {
            std::copy(p.k.begin(), p.k.end(), k.begin());
        }
        template <typename... T> 
        constexpr explicit Polynomial(T... ts) : k{ts...}
        {} 
        template<typename TIterator>
        constexpr explicit Polynomial(TIterator first, TIterator last) : k(first, last)
        {}
        constexpr float operator()(const float x) const
        {
            /*
            we forego a common optimization technique here (multiplying then adding into a single variable), 
            since it would either require traversing k in the opposite order (which is inefficient),
            or it would require complicating the design of other functions 
            that assume lower indices denote coefficients of smaller degree.
            */
            float y(0.0f);
            float xi(1.0f);
            for (std::size_t i = 0; i < k.size(); ++i)
            {
                y  =k[i]*xi;
                xi*=x;
            }
            return y;
        }
        constexpr float& operator[](const int i)
        {
            return k[i];
        }
        constexpr float operator[](const int i) const
        {
            return 0<i&&i<N? k[i] : 0.0f;
        }
        template<int M>
        constexpr Polynomial<N>& operator+=(const Polynomial<M>& p)
        {
            for (std::size_t i = 0; i < k.size(); ++i)
            {
                k[i] += p[i];
            }
            return *this;
        }
        template<int M>
        constexpr Polynomial<N>& operator-=(const Polynomial<M>& p)
        {
            for (std::size_t i = 0; i < k.size(); ++i)
            {
                k[i] -= p[i];
            }
            return *this;
        }
        constexpr Polynomial<N>& operator*=(const float k2)
        {
            for (std::size_t i = 0; i < k.size(); ++i)
            {
                k[i] *= k2;
            }
            return *this;
        }
        constexpr Polynomial<N>& operator/=(const float k2)
        {
            for (std::size_t i = 0; i < k.size(); ++i)
            {
                k[i] /= k2;
            }
            return *this;
        }
        constexpr Polynomial<N>& operator+=(const float k2)
        {
            k[0] += k2;
            return *this;
        }
        constexpr Polynomial<N>& operator-=(const float k2)
        {
            k[0] -= k2;
            return *this;
        }
    };


    // operators with reals that are closed under Polynomial<N> relations
    template<int N>
    constexpr Polynomial<N> operator+(const Polynomial<N>& p, const float k)
    {
        Polynomial<N> y(p);
        y +=k;
        return y;
    }
    template<int N>
    constexpr Polynomial<N> operator+(const float k, const Polynomial<N>& p)
    {
        Polynomial<N> y(p);
        y +=k;
        return y;
    }
    template<int N>
    constexpr Polynomial<N> operator-(const Polynomial<N>& p, const float k)
    {
        Polynomial<N> y(p);
        y -=k;
        return y;
    }
    template<int N>
    constexpr Polynomial<N> operator-(const float k, const Polynomial<N>& p)
    {
        Polynomial<N> y(p);
        y *=-1.0f;
        y += k;
        return y;
    }
    template<int N>
    constexpr Polynomial<N> operator*(const Polynomial<N>& p, const float k)
    {
        Polynomial<N> y(p);
        y *= k;
        return y;
    }
    template<int N>
    constexpr Polynomial<N> operator*(const float k, const Polynomial<N>& p)
    {
        Polynomial<N> y(p);
        y *= k;
        return y;
    }
    template<int N>
    constexpr Polynomial<N> operator/(const Polynomial<N>& p, const float k)
    {
        Polynomial<N> y(p);
        y /= k;
        return y;
    }
    template<int N>
    constexpr Polynomial<N> operator-(const Polynomial<N>& p)
    {
        Polynomial<N> y(p);
        y *=-1.0f;
        return y;
    }

    // operators that are not closed under Polynomial<N> relations, but produce Polynomial<> relations of some degree
    template<int N, int M>
    constexpr Polynomial<std::max(N,M)> operator+(const Polynomial<N>& p, const Polynomial<M>& q)
    {
        Polynomial<std::max(N,M)> y(p);
        y += q;
        return y;
    }
    template<int N, int M>
    constexpr Polynomial<std::max(N,M)> operator-(const Polynomial<N>& p, const Polynomial<M>& q)
    {
        Polynomial<std::max(N,M)> y(p);
        y -= q;
        return y;
    }
    template<int N, int M>
    constexpr Polynomial<N+M> operator*(const Polynomial<N>& p, const Polynomial<M>& q)
    {
        Polynomial<N+M> y;
        for (std::size_t i = 0; i < p.k.size(); ++i)
        {
            for (std::size_t j = 0; j < q.k.size(); ++j)
            {
                y.k[i+j] += p.k[i]*q.k[j];
            }
        }
        return y;
    }




    // operators with reals that produce Polynomial<N> relations given other relations as input
    template<int N>
    constexpr auto operator+(const Polynomial<N>& p, const Identity e)
    {
        return p + Polynomial<1>(e);
    }
    template<int N>
    constexpr auto operator+(const Identity e, const Polynomial<N>& p)
    {
        return Polynomial<1>(e) + p;
    }
    template<int N>
    constexpr auto operator-(const Polynomial<N>& p, const Identity e)
    {
        return p - Polynomial<1>(e);
    }
    template<int N>
    constexpr auto operator-(const Identity e, const Polynomial<N>& p)
    {
        return Polynomial<1>(e) - p;
    }
    template<int N>
    constexpr auto operator*(const Polynomial<N>& p, const Identity e)
    {
        return p * Polynomial<1>(e);
    }
    template<int N>
    constexpr auto operator*(const Identity e, const Polynomial<N>& p)
    {
        return Polynomial<1>(e) * p;
    }
    template<int N>
    constexpr auto operator/(const Polynomial<N>& p, const Identity e)
    {
        return p / Polynomial<1>(e);
    }



    template<int N>
    constexpr auto operator+(const Polynomial<N>& p, const Scaling f)
    {
        return p + Polynomial<1>(f);
    }
    template<int N>
    constexpr auto operator+(const Scaling f, const Polynomial<N>& p)
    {
        return Polynomial<1>(f) + p;
    }
    template<int N>
    constexpr auto operator-(const Polynomial<N>& p, const Scaling f)
    {
        return p - Polynomial<1>(f);
    }
    template<int N>
    constexpr auto operator-(const Scaling f, const Polynomial<N>& p)
    {
        return Polynomial<1>(f) - p;
    }
    template<int N>
    constexpr auto operator*(const Polynomial<N>& p, const Scaling f)
    {
        return p * Polynomial<1>(f);
    }
    template<int N>
    constexpr auto operator*(const Scaling f, const Polynomial<N>& p)
    {
        return Polynomial<1>(f) * p;
    }
    template<int N>
    constexpr auto operator/(const Polynomial<N>& p, const Scaling f)
    {
        return p / Polynomial<1>(f);
    }
    template<int N>
    constexpr auto operator-(const Scaling f)
    {
        return -Polynomial<1>(f);
    }



    template<int N>
    constexpr auto operator+(const Polynomial<N>& p, const Shifting f)
    {
        return p + Polynomial<1>(f);
    }
    template<int N>
    constexpr auto operator+(const Shifting f, const Polynomial<N>& p)
    {
        return Polynomial<1>(f) + p;
    }
    template<int N>
    constexpr auto operator-(const Polynomial<N>& p, const Shifting f)
    {
        return p - Polynomial<1>(f);
    }
    template<int N>
    constexpr auto operator-(const Shifting f, const Polynomial<N>& p)
    {
        return Polynomial<1>(f) - p;
    }
    template<int N>
    constexpr auto operator*(const Polynomial<N>& p, const Shifting f)
    {
        return p * Polynomial<1>(f);
    }
    template<int N>
    constexpr auto operator*(const Shifting f, const Polynomial<N>& p)
    {
        return Polynomial<1>(f) * p;
    }
    template<int N>
    constexpr auto operator/(const Polynomial<N>& p, const Shifting f)
    {
        return p / Polynomial<1>(f);
    }
    template<int N>
    constexpr auto operator-(const Shifting f)
    {
        return -Polynomial<1>(f);
    }







    constexpr auto operator+(const Identity e, const Scaling f)
    {
        return Polynomial<1>(e) + Polynomial<1>(f);
    }
    constexpr auto operator+(const Scaling f, const Identity e)
    {
        return Polynomial<1>(f) + Polynomial<1>(e);
    }
    constexpr auto operator-(const Identity e, const Scaling f)
    {
        return Polynomial<1>(e) - Polynomial<1>(f);
    }
    constexpr auto operator-(const Scaling f, const Identity e)
    {
        return Polynomial<1>(f) - Polynomial<1>(e);
    }
    constexpr auto operator*(const Identity e, const Scaling f)
    {
        return Polynomial<1>(e) * Polynomial<1>(f);
    }
    constexpr auto operator*(const Scaling f, const Identity e)
    {
        return Polynomial<1>(f) * Polynomial<1>(e);
    }




    constexpr auto operator+(const Identity e, const Shifting f)
    {
        return Polynomial<1>(e) + Polynomial<1>(f);
    }
    constexpr auto operator+(const Shifting f, const Identity e)
    {
        return Polynomial<1>(f) + Polynomial<1>(e);
    }
    constexpr auto operator-(const Identity e, const Shifting f)
    {
        return Polynomial<1>(e) - Polynomial<1>(f);
    }
    constexpr auto operator-(const Shifting f, const Identity e)
    {
        return Polynomial<1>(f) - Polynomial<1>(e);
    }
    constexpr auto operator*(const Identity e, const Shifting f)
    {
        return Polynomial<1>(e) * Polynomial<1>(f);
    }
    constexpr auto operator*(const Shifting f, const Identity e)
    {
        return Polynomial<1>(f) * Polynomial<1>(e);
    }



    constexpr auto operator+(const Shifting g, const Scaling f)
    {
        return Polynomial<1>(g) + Polynomial<1>(f);
    }
    constexpr auto operator+(const Scaling f, const Shifting g)
    {
        return Polynomial<1>(f) + Polynomial<1>(g);
    }
    constexpr auto operator-(const Shifting g, const Scaling f)
    {
        return Polynomial<1>(g) - Polynomial<1>(f);
    }
    constexpr auto operator-(const Scaling f, const Shifting g)
    {
        return Polynomial<1>(f) - Polynomial<1>(g);
    }
    constexpr auto operator*(const Shifting g, const Scaling f)
    {
        return Polynomial<1>(g) * Polynomial<1>(f);
    }
    constexpr auto operator*(const Scaling f, const Shifting g)
    {
        return Polynomial<1>(f) * Polynomial<1>(g);
    }


















    // constexpr auto operator+(const float k, const Identity e)
    // {
    //     return k + Polynomial<1>(e);
    // }
    // constexpr auto operator+(const Identity e, const float k)
    // {
    //     return Polynomial<1>(e) + k;
    // }
    constexpr auto operator-(const float k, const Identity e)
    {
        return k - Polynomial<1>(e);
    }
    // constexpr auto operator-(const Identity e, const float k)
    // {
    //     return Polynomial<1>(e) - k;
    // }
    // constexpr auto operator*(const float k, const Identity e)
    // {
    //     return k * Polynomial<1>(e);
    // }
    // constexpr auto operator*(const Identity e, const float k)
    // {
    //     return Polynomial<1>(e) * k;
    // }



    constexpr auto operator+(const float k, const Scaling f)
    {
        return k + Polynomial<1>(f);
    }
    constexpr auto operator+(const Scaling f, const float k)
    {
        return Polynomial<1>(f) + k;
    }
    constexpr auto operator-(const float k, const Scaling f)
    {
        return k - Polynomial<1>(f);
    }
    constexpr auto operator-(const Scaling f, const float k)
    {
        return Polynomial<1>(f) - k;
    }
    // constexpr auto operator*(const float k, const Scaling f)
    // {
    //     return k * Polynomial<1>(f);
    // }
    // constexpr auto operator*(const Scaling f, const float k)
    // {
    //     return Polynomial<1>(f) * k;
    // }




    // constexpr auto operator+(const float k, const Shifting f)
    // {
    //     return k + Polynomial<1>(f);
    // }
    // constexpr auto operator+(const Shifting f, const float k)
    // {
    //     return Polynomial<1>(f) + k;
    // }
    constexpr auto operator-(const float k, const Shifting f)
    {
        return k - Polynomial<1>(f);
    }
    // constexpr auto operator-(const Shifting f, const float k)
    // {
    //     return Polynomial<1>(f) - k;
    // }
    constexpr auto operator*(const float k, const Shifting f)
    {
        return k * Polynomial<1>(f);
    }
    constexpr auto operator*(const Shifting f, const float k)
    {
        return Polynomial<1>(f) * k;
    }








    template<int N>
    constexpr auto operator+(const Scaling f, const Scaling g)
    {
        return Polynomial<1>(f) + Polynomial<1>(g);
    }
    template<int N>
    constexpr auto operator-(const Scaling f, const Scaling g)
    {
        return Polynomial<1>(f) - Polynomial<1>(g);
    }
    // template<int N>
    // constexpr auto operator*(const Scaling f, const Scaling g)
    // {
    //     return Polynomial<1>(f) * Polynomial<1>(g);
    // }



    // template<int N>
    // constexpr auto operator+(const Shifting f, const Shifting g)
    // {
    //     return Polynomial<1>(f) + Polynomial<1>(e);
    // }
    // template<int N>
    // constexpr auto operator-(const Shifting f, const Shifting g)
    // {
    //     return Polynomial<1>(f) - Polynomial<1>(e);
    // }
    template<int N>
    constexpr auto operator*(const Shifting f, const Shifting g)
    {
        return Polynomial<1>(f) * Polynomial<1>(g);
    }



    // template<int N>
    // constexpr auto operator+(const Identity e1, const Identity e2)
    // {
    //     return Polynomial<1>(e) + Polynomial<1>(e);
    // }
    // template<int N>
    // constexpr auto operator-(const Identity e1, const Identity e2)
    // {
    //     return Polynomial<1>(e) - Polynomial<1>(e);
    // }
    template<int N>
    constexpr auto operator*(const Identity e1, const Identity e2)
    {
        return Polynomial<1>(e1) * Polynomial<1>(e2);
    }




    constexpr int factorial(const int n)
    {
        int y = 1;
        for (int i = 1; i < n; ++i)
        {
            y *= i;
        }
        return y;
    }
    constexpr int combination(const int n, const int r)
    {
        return factorial(n) / (factorial(r) * factorial(n - r));
    }

    /*
    `central_finite_difference()` is used to approximate the derivative of a function for the parameter value, x.
    It does so using the finite difference method. 
    The finite difference method approximates the nth order derivative as dⁿf/dxⁿ, 
    where dⁿf is the return value of `central_finite_difference(f,x,dx,n)`.
    */
    template<typename F>
    constexpr float central_finite_difference(const F& f, const float x, const float dx, const int order)
    {
        float df = 0.0f;
        float weight = 1.0f;
        for (int i = 0; i < order; ++i)
        {
            df += weight * combination(order, i) * f(x+(order/2.0f - i)*dx);
            weight *= -1;
        }
        return df;
    }


    /* 
    NOTE: we avoid expressing derivatives and integrals as methods,
    since not all equations are closed under derivatives and integrals,
    and such methods would complicate the order with which classes must be declared
    */
    constexpr float derivative(const float k) 
    {
        return 0.0f;
    }
    constexpr float derivative(const Polynomial<1> p) 
    {
        return p[0];
    }
    template<int N>
    Polynomial<N-1> derivative(const Polynomial<N>& p)
    {
        Polynomial<N-1> dpdx;
        for (std::size_t i = 1; i < p.k.size(); ++i)
        {
            dpdx.k[i-1] = i*p.k[i];
        }
        return dpdx;
    }

    constexpr Scaling integral(const float k) 
    {
        return Scaling(k);
    }
    template<int N>
    Polynomial<N+1> integral(const Polynomial<N>& p)
    {
        Polynomial<N+1> dpdx;
        for (std::size_t i = 0; i < p.k.size(); ++i)
        {
            dpdx.k[i+1] = p.k[i]/(i+1);
        }
        return dpdx;
    }

    constexpr float solve(const Polynomial<1> p, const float y) 
    {
        return y-p[0] / p[1];
    }
    std::vector<float> solve(const Polynomial<2> p, const float y) 
    {
        // the quadratic formula
        std::vector<float> solutions;
        const float float_precision = 1e-6;
        const float sqrt_argument = p[1]*p[1] - 4.0f*(p[0]-y)*p[2];
        if (sqrt_argument > float_precision)
        {
            solutions.push_back(-p[1] / p[2]);
        }
        else if (sqrt_argument > 0.0f)
        {
            solutions.push_back(-p[1]+std::sqrt(sqrt_argument) / p[2]); 
            solutions.push_back(-p[1]-std::sqrt(sqrt_argument) / p[2]); 
        }
        return solutions;
    }

    template<int N>
    constexpr auto extremum(const Polynomial<N>& p) 
    {
        return solve(derivative(p), 0.0f);
    }

    /*
    `max()` and `min()` returns the highest output of the function over the given range.
    These are used together when determining whether to include the function as a term in a larger equation.
    */
    constexpr float maximum(const Polynomial<1> p, const float lo, const float hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return p(hi) > p(lo)? hi : lo;
    }
    constexpr float minimum(const Polynomial<1> p, const float lo, const float hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return p(hi) < p(lo)? hi : lo;
    }

    constexpr float maximum(const Polynomial<2> p, const float lo, const float hi) 
    {
        float x = std::clamp(extremum(p), lo, hi);
        return p(x) > p(hi)? x : p(hi) > p(lo)? hi : lo;
    }
    constexpr float minimum(const Polynomial<2> p, const float lo, const float hi) 
    {
        float x = std::clamp(extremum(p), lo, hi);
        return p(x) < p(hi)? x : p(hi) < p(lo)? hi : lo;
    }

    template<int N>
    float maximum(const Polynomial<N>& p, const float lo, const float hi) 
    {
        auto candidates = extremum(p);
        candidates.push_back(lo);
        candidates.push_back(hi);
        return *std::max_element(candidates.begin(), candidates.end(), 
            [&](float a, float b){ return p(std::clamp(a,lo,hi)) < p(std::clamp(b,lo,hi)); });
    }
    template<int N>
    float minimum(const Polynomial<N>& p, const float lo, const float hi) 
    {
        auto candidates = extremum(p);
        candidates.push_back(lo);
        candidates.push_back(hi);
        return *std::min_element(candidates.begin(), candidates.end(), 
            [&](float a, float b){ return p(std::clamp(a,lo,hi)) < p(std::clamp(b,lo,hi)); });
    }

    template<int N>
    constexpr Polynomial<N> compose(const Polynomial<N>& p, const Identity e)
    {
        return p;
    }

    template<int N>
    constexpr Polynomial<N> compose(const Polynomial<N>& p, const Scaling g)
    {
        Polynomial<N+1> pq = p;
        for (std::size_t i = 1; i < p.k.size(); ++i)
        {
            pq.k[i] *= std::pow(g.factor, float(i));
        }
        return pq;
    }

    template<int N>
    constexpr Polynomial<N> compose(const Polynomial<N>& p, const Shifting g)
    {
        Polynomial<N> pq;
        for (std::size_t i = 0; i < p.k.size(); ++i)
        {
            for (std::size_t j = i; j < p.k.size(); ++j)
            {
                pq[i] += p[j]*combination(j,j-i)*std::pow(g.offset, j-i);
            }
        }
        return pq;
    }

    template<int M>
    constexpr float compose(const Polynomial<0>& p, const Polynomial<M>& q)
    {
        return p[0];
    }

    template<int M>
    constexpr Polynomial<M> compose(const Polynomial<1> p, const Polynomial<M>& q)
    {
        return p[0] + p[1]*q;
    }

    template<int N, int M>
    constexpr Polynomial<N*M> compose(const Polynomial<N>& p, const Polynomial<M>& q)
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
        return p[0]+Identity()*compose(Polynomial<N-1>(p.k.begin()+1, p.k.end()), q);
    }



    template<typename F>
    constexpr Polynomial<1> linear_newton_polynomial(
        const F f, 
        const float x1, 
        const float x2
    ){
        const float dfdx = (f(x2)-f(x1)) / (x2-x1);
        return f(x1) + dfdx * Shifting(-x1);
    }

    template<typename F>
    constexpr Polynomial<1> linear_taylor_series(const F f, const float x, const float dx)
    {
        const float dx2 = dx*dx;
        return compose(
            Polynomial<1>(f(x), 
                central_finite_difference(f, x, dx, 1) / dx),
            Shifting(-x)
        );
    }

    template<typename F>
    constexpr Polynomial<2> quadratic_newton_polynomial(
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
    constexpr Polynomial<2> quadratic_taylor_series(const F f, const float x, const float dx)
    {
        const float dx2 = dx*dx;
        return compose(
            Polynomial<2>(f(x), 
                central_finite_difference(f, x, dx, 1) / dx, 
                central_finite_difference(f, x, dx, 2) /(dx2*2.0f)),
            Shifting(-x)
        );
    }

    template<typename F>
    constexpr Polynomial<3> cubic_newton_polynomial(
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
    constexpr Polynomial<3> cubic_taylor_series(const F f, const float x, const float dx)
    {
        const float dx2 = dx*dx;
        const float dx3 = dx2*dx;
        return 
            compose(
                Polynomial<3>(f(x), 
                    central_finite_difference(f, x, dx, 1) / dx, 
                    central_finite_difference(f, x, dx, 2) /(dx2*2.0f), 
                    central_finite_difference(f, x, dx, 3) /(dx3*6.0f)),
                Shifting(-x)
            );
    }


    struct ClampedExponent {
        float lo;
        float hi;
        float weight;
        float exponent;
        constexpr explicit ClampedExponent(const float lo, const float hi, const float weight, const float exponent):
            lo(lo),
            hi(hi),
            weight(weight),
            exponent(exponent)
        {}
        constexpr explicit ClampedExponent():
            lo(0.0f),
            hi(0.0f),
            weight(0.0f),
            exponent(1.0f)
        {}
        constexpr float operator()(const float x) const
        {
            return weight*std::pow(std::clamp(x,lo,hi), exponent);
        }
        constexpr ClampedExponent& operator*=(const float k)
        {
            weight *= k;
            return *this;
        }
        constexpr ClampedExponent& operator/=(const float k)
        {
            weight /= k;
            return *this;
        }
    };

    constexpr ClampedExponent operator*(const ClampedExponent f, const float k)
    {
        return ClampedExponent(f.lo, f.hi, f.weight*k, f.exponent);
    }
    constexpr ClampedExponent operator*(const float k, const ClampedExponent f)
    {
        return ClampedExponent(f.lo, f.hi, f.weight*k, f.exponent);
    }
    constexpr ClampedExponent operator/(const ClampedExponent f, const float k)
    {
        return ClampedExponent(f.lo, f.hi, f.weight/k, f.exponent);
    }
    constexpr ClampedExponent operator/(const float k, const ClampedExponent f)
    {
        return ClampedExponent(f.lo, f.hi, k/f.weight, -f.exponent);
    }

    constexpr ClampedExponent compose(const ClampedExponent f, const Scaling g)
    {
        return ClampedExponent(
            f.lo*g.factor,
            f.hi*g.factor,
            f.weight*std::pow(g.factor, f.exponent),
            f.exponent
        );
    }

    struct ClampedSigmoid {
        float xlo;
        float xhi;
        float xscale;
        float xoffset;
        float ymax;
        constexpr explicit ClampedSigmoid(const float xlo, const float xhi, const float xscale, const float xoffset, const float ymax):
            xlo(xlo),
            xhi(xhi),
            xscale(xscale),
            xoffset(xoffset),
            ymax(ymax)
        {}
        constexpr explicit ClampedSigmoid():
            xlo(0.0f),
            xhi(0.0f),
            xscale(0.0f),
            xoffset(0.0f),
            ymax(0.0f)
        {}
        constexpr float operator()(const float x) const
        {
            float u = xscale * std::clamp(x, xlo, xhi) + xoffset;
            return ymax * u / std::sqrt(1.0f + u*u);
        }
        constexpr ClampedSigmoid& operator*=(const float scalar)
        {
            ymax *= scalar;
            return *this;
        }
        constexpr ClampedSigmoid& operator/=(const float scalar)
        {
            ymax /= scalar;
            return *this;
        }
    };
    constexpr ClampedSigmoid operator*(const ClampedSigmoid relation, const float scalar)
    {
        return ClampedSigmoid(relation.xlo, relation.xhi, relation.xscale, relation.xoffset, relation.ymax*scalar);
    }
    constexpr ClampedSigmoid operator*(const float scalar, const ClampedSigmoid relation)
    {
        return ClampedSigmoid(relation.xlo, relation.xhi, relation.xscale, relation.xoffset, relation.ymax*scalar);
    }
    constexpr ClampedSigmoid operator/(const ClampedSigmoid relation, const float scalar)
    {
        return ClampedSigmoid(relation.xlo, relation.xhi, relation.xscale, relation.xoffset, relation.ymax/scalar);
    }


    constexpr ClampedSigmoid compose(const ClampedSigmoid f, const Scaling g)
    {
        return ClampedSigmoid(
            g.factor > 0.0f? f.xlo * g.factor : f.xhi * g.factor,
            g.factor > 0.0f? f.xhi * g.factor : f.xlo * g.factor,
            f.xscale * g.factor,
            f.xoffset,
            f.ymax
        );
    }

    struct Dippr102 {
        float lo;
        float hi;
        float c1;
        float c2;
        float c3;
        float c4;
        constexpr explicit Dippr102(
            const float lo,
            const float hi,
            const float c1,
            const float c2,
            const float c3,
            const float c4
        ):
            lo(lo),
            hi(hi),
            c1(c1),
            c2(c2),
            c3(c3),
            c4(c4)
        {}
        constexpr explicit Dippr102():
            lo(0.0f),
            hi(0.0f),
            c1(0.0f),
            c2(1.0f),
            c3(0.0f),
            c4(0.0f)
        {}
        constexpr float operator()(const float x) const
        {
            const float u = std::clamp(x, lo, hi);
            return c1 * std::pow(u, c2) / (1.0f + c3/u + c4/(u*u));
        }
        constexpr Dippr102& operator*=(const float scalar)
        {
            c1 *= scalar;
            return *this;
        }
        constexpr Dippr102& operator/=(const float scalar)
        {
            c1 /= scalar;
            return *this;
        }
    };
    constexpr Dippr102 operator*(const Dippr102 relation, const float scalar)
    {
        return Dippr102(relation.lo, relation.hi, relation.c1 * scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr102 operator*(const float scalar, const Dippr102 relation)
    {
        return Dippr102(relation.lo, relation.hi, relation.c1 * scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr102 operator/(const Dippr102 relation, const float scalar)
    {
        return Dippr102(relation.lo, relation.hi, relation.c1 / scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr102 operator-(const Dippr102 relation)
    {
        return Dippr102(relation.lo, relation.hi, relation.c1 * -1.0f, relation.c2, relation.c3, relation.c4);
    }


    constexpr Dippr102 compose(Dippr102 f, const Scaling g)
    {
        return Dippr102(
            g.factor > 0.0f? f.lo * g.factor : f.hi * g.factor,
            g.factor > 0.0f? f.hi * g.factor : f.lo * g.factor,
            f.c1 * std::pow(g.factor, f.c2),
            f.c2,
            f.c3 / g.factor,
            f.c4 / (g.factor*g.factor)
        );
    }

    constexpr float maximum(const ClampedExponent& f, const float lo, const float hi)
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) > f(lo)? hi : lo;
    }

    constexpr float minimum(const ClampedExponent& f, const float lo, const float hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) < f(lo)? hi : lo;
    }

    constexpr float maximum(const ClampedSigmoid& f, const float lo, const float hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) > f(lo)? hi : lo;
    }

    constexpr float minimum(const ClampedSigmoid& f, const float lo, const float hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) < f(lo)? hi : lo;
    }

    template<typename F>
    float maximum(const F& f, float lo, float hi, const int iteration_count) 
    {
        /*
        Start with an initial estimate of lo or hi, whichever produces greater output.
        Approximate the function using a second order Taylor series based around the estimate, then solve for the extremum. 
        Accept extremum as an updated estimate if it is higher than the previous estimate, then repeat.
        */
        const float dx = (hi-lo)/1000.0f;
        const float dx2 = dx*dx;
        Polynomial<2> approximation(0.0f, 0.0f, 0.0f);
        float x = f(lo) > f(hi)? lo : hi;
        float x2;
        for (int i = 0; i < iteration_count; ++i)
        {
            approximation = compose(
                Polynomial<2>(float(f(x)), 
                    central_finite_difference(f, x, dx, 1)/(dx * factorial(1.0)), 
                    central_finite_difference(f, x, dx, 2)/(dx2* factorial(2.0))),
                Shifting(-x)
            );
            x2 = maximum(derivative(approximation), lo, hi);
            x = f(x2) > f(x)? x2 : x;
        }
        return f(x);
    }

    template<typename F>
    float minimum(const F& f, float lo, float hi, const int iteration_count) 
    {
        // same as max(), but for negated output
        return maximum(-f, lo, hi, iteration_count);
    }

    /*
    `max_distance` measures the largest absolute difference between the output of two functions over the given range.
    This is useful when determining whether two functions should be consolidated when used in a larger equation.
    */
    constexpr float max_distance(const ClampedExponent relation1, const ClampedExponent relation2, const float lo, const float hi)
    {
        return std::max(
            std::abs(relation1(hi)-relation2(hi)),
            std::abs(relation1(lo)-relation2(lo))
        );
    }

    constexpr float max_distance(const ClampedSigmoid relation1, const ClampedSigmoid relation2, const float lo, const float hi)
    {
        return std::max(
            std::abs(relation1(hi)-relation2(hi)),
            std::abs(relation1(lo)-relation2(lo))
        );
    }


    /*
    `GasPropertyStateRelation` consolidates many kinds of expressions
    that are commonly used to represent pressure/temperature relations for compounds in their gas phase.
    These properties are namely the isobaric specific heat capacity, thermal conductivity, and dynamic viscosity.

    More broadly, GasPropertyStateRelation should be capable of representing any smooth pressure/temperature relation
    in which pressure and temperature interactions are negligible.
    ClampedExponents can be used to represent arbitrary polynomials,
    and ClampedSigmoids can be used to represent arbitrary lerp functions.
    */
    template<typename Ty>
    class GasPropertyStateRelation
    {
        std::vector<ClampedExponent> pexponents;
        std::vector<ClampedExponent> Texponents;
        std::vector<ClampedSigmoid>  Tsigmoids;
        std::vector<Dippr102> Tdippr102s;

        si::pressure    punits;
        si::temperature Tunits;
        Ty              yunits;

        float intercept;

    public:
        float known_max_fractional_error;
        using value_type = Ty;
        using parameter_type = field::StateParameters;

        constexpr GasPropertyStateRelation(
            const std::vector<ClampedExponent> pexponents,
            const std::vector<ClampedExponent> Texponents,
            const std::vector<ClampedSigmoid>  Tsigmoids,
            const std::vector<Dippr102> Tdippr102s,

            const si::pressure    punits,
            const si::temperature Tunits,
            const Ty              yunits,

            const float intercept,
            const float known_max_fractional_error
        ):
            pexponents(pexponents),
            Texponents(Texponents),
            Tsigmoids(Tsigmoids),
            Tdippr102s(Tdippr102s),

            punits(punits),
            Tunits(Tunits),
            yunits(yunits),

            intercept(intercept),
            known_max_fractional_error(known_max_fractional_error)
        {
        }

        constexpr GasPropertyStateRelation(const Ty intercept):
            pexponents(0),
            Texponents(0),
            Tsigmoids(0),
            Tdippr102s(0),

            punits(si::standard_pressure),
            Tunits(si::standard_temperature),
            yunits(Ty(1.0)),

            intercept(intercept/Ty(1.0)),
            known_max_fractional_error(0.0)
        {
        }

        constexpr GasPropertyStateRelation<Ty>& operator=(const GasPropertyStateRelation<Ty> other)
        {
            pexponents = other.pexponents;
            Texponents = other.Texponents;
            Tsigmoids  = other.Tsigmoids;
            Tdippr102s = other.Tdippr102s;

            punits = other.punits;
            Tunits = other.Tunits;
            yunits = other.yunits;

            intercept = other.intercept;
            known_max_fractional_error = other.known_max_fractional_error;

            return *this;
        }

        Ty operator()(const si::pressure pressure, const si::temperature temperature) const
        {
            const float p = float(pressure/punits);
            const float T = float(temperature/Tunits);
            ClampedExponent exponent;
            ClampedSigmoid sigmoid;
            Dippr102 dippr102;
            float u = 0.0f;
            float y = intercept;
            for (std::size_t i = 0; i < pexponents.size(); ++i)
            {
                exponent = pexponents[i];
                y += exponent.weight * std::pow(std::clamp(p, exponent.lo, exponent.hi), exponent.exponent);
            }
            for (std::size_t i = 0; i < Texponents.size(); ++i)
            {
                exponent = Texponents[i];
                y += exponent.weight * std::pow(std::clamp(T, exponent.lo, exponent.hi), exponent.exponent);
            }
            for (std::size_t i = 0; i < Tsigmoids.size(); ++i)
            {
                sigmoid = Tsigmoids[i];
                u = sigmoid.xscale * std::clamp(T, sigmoid.xlo, sigmoid.xhi) + sigmoid.xoffset;
                y += sigmoid.ymax * u / std::sqrt(1.0f + u*u);
            }
            for (std::size_t i = 0; i < Tdippr102s.size(); ++i)
            {
                dippr102 = Tdippr102s[i];
                u = std::clamp(T, dippr102.lo, dippr102.hi);
                y += dippr102.c1 * std::pow(u, dippr102.c2) / (1.0f + dippr102.c3/u + dippr102.c4/(u*u));
            }

            // return intercept * yunits;
            return y * yunits;
        }

        Ty operator()(const field::StateParameters parameters) const
        {
            return (*this)(parameters.pressure, parameters.temperature);
        }

        GasPropertyStateRelation<Ty> restriction(
            const si::pressure min_pressure, const si::pressure max_pressure,
            const si::temperature min_temperature, const si::temperature max_temperature,
            const float known_max_fractional_error
        ) const
        {
            const float plo = float(min_pressure/punits);
            const float Tlo = float(min_temperature/Tunits);
            const float phi = float(max_pressure/punits);
            const float Thi = float(max_temperature/Tunits);
            GasPropertyStateRelation<Ty> restricted(*this);
            bool is_removal_occurring(true);
            float float_precision_of_error = 1e-6;
            float least_useful_term_error;
            while(restricted.known_max_fractional_error < known_max_fractional_error) {
                auto least_useful_pexponent = std::min_element(restricted.pexponents.begin(), restricted.pexponents.end(),
                    [&](const ClampedExponent& f1, const ClampedExponent& f2){
                        (maximum(f1,plo,phi)-minimum(f1,plo,phi)) < (maximum(f2,plo,phi)-minimum(f2,plo,phi));
                    });
                float least_useful_pexponent_error = maximum(least_useful_pexponent, plo,phi) - minimum(least_useful_pexponent, Tlo,Thi);
                auto least_useful_Texponent = std::min_element(
                    restricted.Texponents.begin(), restricted.Texponents.end(),
                    [&](const ClampedExponent& f1, const ClampedExponent& f2){
                        (maximum(f1,Tlo,Thi)-minimum(f1,Tlo,Thi)) < (maximum(f2,Tlo,Thi)-minimum(f2,Tlo,Thi));
                    });
                float least_useful_Texponent_error = maximum(least_useful_Texponent, plo,phi) - minimum(least_useful_Texponent, Tlo,Thi);
                auto least_useful_Tsigmoid = std::min_element(
                    restricted.Tsigmoids.begin(), restricted.Tsigmoids.end(),
                    [&](const ClampedExponent& f1, const ClampedExponent& f2){
                        (maximum(f1,Tlo,Thi)-minimum(f1,Tlo,Thi)) < (maximum(f2,Tlo,Thi)-minimum(f2,Tlo,Thi));
                    });
                float least_useful_Tsigmoid_error = maximum(least_useful_Tsigmoid, plo,phi) - minimum(least_useful_Tsigmoid, Tlo,Thi);
                auto least_useful_Tdippr102 = std::min_element(
                    restricted.Tdippr102s.begin(), restricted.Tdippr102s.end(),
                    [&](const ClampedExponent& f1, const ClampedExponent& f2){
                        (maximum(f1,Tlo,Thi)-minimum(f1,Tlo,Thi)) < (maximum(f2,Tlo,Thi)-minimum(f2,Tlo,Thi));
                    });
                float least_useful_Tdippr102_error = maximum(least_useful_Tdippr102, plo,phi) - minimum(least_useful_Tdippr102, Tlo,Thi);

                least_useful_term_error = std::min(least_useful_pexponent_error, least_useful_Texponent_error);
                least_useful_term_error = std::min(least_useful_term_error,      least_useful_Tsigmoid_error );
                least_useful_term_error = std::min(least_useful_term_error,      least_useful_Tdippr102_error);

                if (restricted.known_max_fractional_error + least_useful_term_error < known_max_fractional_error)
                {
                    if (std::abs(least_useful_pexponent_error - least_useful_term_error) < float_precision_of_error)
                    {
                        restricted.intercept += (maximum(least_useful_pexponent, plo,phi) - minimum(least_useful_pexponent, Tlo,Thi)) / 2.0;
                        restricted.known_max_fractional_error += least_useful_pexponent_error;
                        restricted.pexponents.erase(least_useful_pexponent);
                        continue;
                    }
                    if (std::abs(least_useful_Texponent_error - least_useful_term_error) < float_precision_of_error)
                    {
                        restricted.intercept += (maximum(least_useful_Texponent, plo,phi) - minimum(least_useful_Texponent, Tlo,Thi)) / 2.0;
                        restricted.known_max_fractional_error += least_useful_Texponent_error;
                        restricted.Texponents.erase(least_useful_Texponent);
                        continue;
                    }
                    if (std::abs(least_useful_Tsigmoid_error - least_useful_term_error) < float_precision_of_error)
                    {
                        restricted.intercept += (maximum(least_useful_Tsigmoid, plo,phi) - minimum(least_useful_Tsigmoid, Tlo,Thi)) / 2.0;
                        restricted.known_max_fractional_error += least_useful_Tsigmoid_error;
                        restricted.Tsigmoids.erase(least_useful_Tsigmoid);
                        continue;
                    }
                    if (std::abs(least_useful_Tdippr102_error - least_useful_term_error) < float_precision_of_error)
                    {
                        restricted.intercept += (maximum(least_useful_Tdippr102, plo,phi) - minimum(least_useful_Tdippr102, Tlo,Thi)) / 2.0;
                        restricted.known_max_fractional_error += least_useful_Tdippr102_error;
                        restricted.Tdippr102s.erase(least_useful_Tdippr102);
                        continue;
                    }
                }
            }
            return restricted;
        }

        GasPropertyStateRelation<Ty>& operator+=(const Ty offset)
        {
            intercept += offset/yunits;
            return *this;
        }

        GasPropertyStateRelation<Ty>& operator-=(const Ty offset)
        {
            intercept -= offset/yunits;
            return *this;
        }

        GasPropertyStateRelation<Ty>& operator*=(const float scalar)
        {
            for (std::size_t i = 0; i < pexponents.size(); ++i)
            {
                pexponents[i] *= scalar;
            }
            for (std::size_t i = 0; i < Texponents.size(); ++i)
            {
                Texponents[i] *= scalar;
            }
            for (std::size_t i = 0; i < Tdippr102s.size(); ++i)
            {
                Tdippr102s[i] *= scalar;
            }
            for (std::size_t i = 0; i < Tsigmoids.size(); ++i)
            {
                Tsigmoids[i] *= scalar;
            }
            intercept *= scalar;
            return *this;
        }

        GasPropertyStateRelation<Ty> operator/=(const float scalar)
        {
            for (std::size_t i = 0; i < pexponents.size(); ++i)
            {
                pexponents[i] /= scalar;
            }
            for (std::size_t i = 0; i < Texponents.size(); ++i)
            {
                Texponents[i] /= scalar;
            }
            for (std::size_t i = 0; i < Tdippr102s.size(); ++i)
            {
                Tdippr102s[i] /= scalar;
            }
            for (std::size_t i = 0; i < Tsigmoids.size(); ++i)
            {
                Tsigmoids[i] /= scalar;
            }
            intercept /= scalar;
            return *this;
        }

        GasPropertyStateRelation<Ty>& operator+=(const GasPropertyStateRelation<Ty> other)
        {
            const float pscale = float(other.punits / punits);
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            for (std::size_t i = 0; i < other.pexponents.size(); ++i)
            {
                pexponents.push_back(yscale * compose(other.pexponents[i], Scaling(pscale)));
            }
            for (std::size_t i = 0; i < other.Texponents.size(); ++i)
            {
                Texponents.push_back(yscale * compose(other.Texponents[i], Scaling(Tscale)));
            }
            for (std::size_t i = 0; i < other.Tdippr102s.size(); ++i)
            {
                Tdippr102s.push_back(yscale * compose(other.Tdippr102s[i], Scaling(Tscale)));
            }
            for (std::size_t i = 0; i < other.Tsigmoids.size(); ++i)
            {
                Tsigmoids.push_back(yscale * compose(other.Tsigmoids[i], Scaling(Tscale)));
            }
            intercept += yscale * other.intercept;
            return *this;
        }

        GasPropertyStateRelation<Ty>& operator-=(const GasPropertyStateRelation<Ty> other)
        {
            const float pscale = float(other.punits / punits);
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = -float(other.yunits / yunits);
            for (std::size_t i = 0; i < other.pexponents.size(); ++i)
            {
                pexponents.push_back(yscale * compose(other.pexponents[i], Scaling(pscale)));
            }
            for (std::size_t i = 0; i < other.Texponents.size(); ++i)
            {
                Texponents.push_back(yscale * compose(other.Texponents[i], Scaling(Tscale)));
            }
            for (std::size_t i = 0; i < other.Tdippr102s.size(); ++i)
            {
                Tdippr102s.push_back(yscale * compose(other.Tdippr102s[i], Scaling(Tscale)));
            }
            for (std::size_t i = 0; i < other.Tsigmoids.size(); ++i)
            {
                Tsigmoids.push_back(yscale * compose(other.Tsigmoids[i], Scaling(Tscale)));
            }
            intercept += yscale * other.intercept;
            return *this;
        }

    };

    // Custom parameterization, offers high accuracy for most pure compounds
    // 36 uses, for dynamic_viscosity, isobaric_specific_heat_capacity, and thermal_conductivity of gases
    template<typename Ty>
    constexpr GasPropertyStateRelation<Ty> get_sigmoid_exponent_pressure_temperature_relation(
        const si::temperature Tunits, const si::pressure punits, const Ty yunits,
        const float pslope, const float pexponent,
        const float Tslope, const float Texponent,
        const float Tsigmoid_max, const float Tsigmoid_scale, const float Tsigmoid_center,
        const float intercept,
        const float Tmin, const float Tmax,
        const float pmin, const float pmax,
        const float known_max_fractional_error
    ) {
        return GasPropertyStateRelation<Ty>(
            std::vector<ClampedExponent>{ClampedExponent(pmin, pmax, pslope, pexponent)},
            std::vector<ClampedExponent>{ClampedExponent(Tmin, Tmax, Tslope, Texponent)},
            std::vector<ClampedSigmoid>{ClampedSigmoid(Tmin, Tmax, 1.0f/Tsigmoid_scale, -Tsigmoid_center/Tsigmoid_scale, Tsigmoid_max)},
            std::vector<Dippr102>(),

            punits, Tunits, yunits,

            intercept,
            known_max_fractional_error
        );
    }

    // Direct port from the DIPPR 102 equation
    // 9 uses, for viscosity and thermal conductivity of gas
    template<typename Ty>
    constexpr GasPropertyStateRelation<Ty> get_dippr_temperature_relation_102(
        const si::temperature Tunits, const Ty yunits,
        const float c1, const float c2, const float c3, const float c4,
        const float Tmin, const float Tmax
    ) {
        return GasPropertyStateRelation<Ty>(
            std::vector<ClampedExponent>(0),
            std::vector<ClampedExponent>(0),
            std::vector<ClampedSigmoid>(0),
            std::vector<Dippr102>{Dippr102(Tmin, Tmax, c1, c2, c3, c4)},

            si::pressure(1.0), Tunits, yunits,

            0.0, // intercept
            0.0 // known_max_fractional_error
        );
    }

    /*
    `approx_inferred_pressure_temperature_gas_relation()` attempts to encode a bivariate function f(p,T)
    into an equation that can be expressed using GasPropertyStateRelation. 
    It is intended that the function represents a property whose values were inferred from other properties.
    The inference methods tend to be error prone, and are only used as a last resort,
    so underfitting is not a significant concern, 
    and is in fact desireable to err on the side of a conservative approximation
    that experiences no sudden changes throughout the relationship.
    For this reason, we encode the function as a bivariate quadratic Newton polynomial 
    over the ranges defined by [pmin,pmax] and [Tmin,Tmax]. 
    Interaction terms within the polynomial (such as pT, pT², and p²T) are ignored,
    since GasPropertyStateRelation does not represent properties that exhibit strong pressure/temperature interaction.
    After fitting, the maximum error for the approximation is estimated using Newton's method.

    Newton polynomials explode past the range of their observed values, 
    but GasPropertyStateRelation already clamps input to the valid range to prevent this from happening.
    Newton polynomials also have a tendency to overfit,
    but this is not a significant issue when the degree of the polynomial is small.
    Underfitting may occur if the degree is small and the approximated function has sudden jumps,
    but as mentioned, a conservative smooth approximation is desireable. 
    Furthermore, observed relationships for GasPropertyStateRelation do not tend to to have sudden changes,
    and if any jump does occur it will be at a temperature close to absolute zero.
    More importantly, the quadratic newton polynomial provides a conservative approximation 
    that is guaranteed to be found in constant time without using iterative optimization. 
    Error estimation is iterative, but the iterative method converges fast and can be done in inconsequential time.
    */
    template<typename Ty, typename F>
    constexpr GasPropertyStateRelation<Ty> approx_inferred_pressure_temperature_gas_relation(
        const si::temperature Tunits, const si::pressure punits, const Ty yunits,
        const F& f,
        const float Tmin, const float Tmax,
        const float pmin, const float pmax,
        const float known_max_fractional_error
    ) {
        //pick STP as the middle sample point if it falls within the valid range, otherwise pick the halfway point.
        const float Tmid = Tmin*Tunits < si::standard_temperature && si::standard_temperature < Tmax*Tunits? 
            si::standard_temperature/Tunits : (Tmin+Tmax)/2.0f;
        const float pmid = pmin*punits < si::standard_pressure    && si::standard_pressure    < pmax*punits? 
            si::standard_pressure   /punits : (pmin+pmax)/2.0f;
        auto fp   = quadratic_newton_polynomial([&](float p) -> float{ return f(p*punits,   Tmid*Tunits) / yunits; }, pmin, pmid, pmax);
        auto fT   = quadratic_newton_polynomial([&](float T) -> float{ return f(pmin*punits,T*Tunits   ) / yunits; }, Tmin, Tmid, Tmax);
        auto fhat = GasPropertyStateRelation<Ty>(
            std::vector<ClampedExponent>{
                ClampedExponent(pmin, pmax, fp[1], 1.0f),
                ClampedExponent(pmin, pmax, fp[2], 2.0f)
            },
            std::vector<ClampedExponent>{
                ClampedExponent(Tmin, Tmax, fT[1], 1.0f),
                ClampedExponent(Tmin, Tmax, fT[2], 2.0f)
            },
            std::vector<ClampedSigmoid>(),
            std::vector<Dippr102>(),

            si::pressure(1.0), Tunits, yunits,

            fT[0], 
            known_max_fractional_error
        );
        // Estimate max fractional error by iteratively solving for the maximum of a loss function
        // Do this once independantly for each parameter.
        auto fractional_error = [&](float p, float T) -> float
        {
            auto fpT    = f(p*punits,T*Tunits)/yunits;
            auto fpThat = f(p*punits,T*Tunits)/yunits;
            return std::abs(fpThat-fpT) / std::abs(fpT); 
        };
        float Tworst = maximum([&](float T){ return fractional_error(pmid,T);   }, Tmin, Tmax, 10);
        float pworst = maximum([&](float p){ return fractional_error(p,Tworst); }, pmin, pmax, 10);
        fhat.known_max_fractional_error += fractional_error(pworst, Tworst);
        return fhat;
    }

    // Direct port from the Perry equation used for some gas properties
    // 1 use, for heat capacity of nitric oxide
    template<typename Ty>
    constexpr GasPropertyStateRelation<Ty> get_perry_temperature_gas_relation(
        const si::temperature Tunits, const Ty yunits,
        const float intercept, const float linear, const float inverse_square, const float square,
        const float Tmin, const float Tmax
    ) {
        return GasPropertyStateRelation<Ty>(
            std::vector<ClampedExponent>(),
            std::vector<ClampedExponent>{
                ClampedExponent(Tmin, Tmax, linear, 1.0f),
                ClampedExponent(Tmin, Tmax, inverse_square, -2.0f),
                ClampedExponent(Tmin, Tmax, square, 2.0f)
            },
            std::vector<ClampedSigmoid>(),
            std::vector<Dippr102>(),

            si::pressure(1.0), Tunits, yunits,

            intercept,
            0.0 // known_max_fractional_error
        );
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator+(const GasPropertyStateRelation<Ty> relation, const GasPropertyStateRelation<Ty> other)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result += other;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator-(const GasPropertyStateRelation<Ty> relation, const GasPropertyStateRelation<Ty> other)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result -= other;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator+(const GasPropertyStateRelation<Ty> relation, const Ty offset)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result += offset;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator-(const GasPropertyStateRelation<Ty> relation, const Ty offset)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result -= offset;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator*(const GasPropertyStateRelation<Ty> relation, const float scalar)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result *= scalar;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator/(const GasPropertyStateRelation<Ty> relation, const float scalar)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result /= scalar;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator+(const Ty offset, const GasPropertyStateRelation<Ty> relation)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result += offset;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator-(const Ty offset, const GasPropertyStateRelation<Ty> relation)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result *= 1.0f;
        result += offset;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator-(const GasPropertyStateRelation<Ty> relation)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result *= 1.0f;
        return result;
    }

    template<typename Ty>
    GasPropertyStateRelation<Ty> operator*(const float scalar, const GasPropertyStateRelation<Ty> relation)
    {
        GasPropertyStateRelation<Ty> result = relation;
        result *= scalar;
        return result;
    }
}}

