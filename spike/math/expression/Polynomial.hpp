#pragma once

#include <algorithm>

// in-house libraries
#include "Identity.hpp"
#include "Scaling.hpp"
#include "Shifting.hpp"

#include <math/combinatorics.hpp>
#include <math/calculus.hpp>

namespace math {

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
            return 0<=i&&i<N? k[i] : 0.0f;
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
    constexpr auto operator-(const Shifting f)
    {
        return -Polynomial<1>(f);
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


}