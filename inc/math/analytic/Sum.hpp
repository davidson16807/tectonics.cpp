#pragma once

// std libraries
#include <algorithm> // copy, back_inserter
#include <initializer_list>
#include <iostream>

// in-house libraries
#include "Identity.hpp"

namespace analytic {

    /*
    A `Sum` represents a sum of arbitrary functions.
    It places no assumptions on the closure of functions under addition or multiplication.
    Specialized classes such as Polynomial, Rational, or Railyard, are recommended 
    if such closure properties are available.
    */
    template<typename T, typename F>
    struct Sum {

        std::vector<F> terms; 

        // zero constructor
        Sum() : terms()
        {
        }

        // constant constructor
        explicit Sum(const T k) : terms(1, F(k))
        {
        }

        // copy constructor
        Sum(const Sum<T,F>& sum) : terms(sum.terms) 
        {
        }

        // cast constructor
        template<typename T2, typename F2>
        explicit Sum(const Sum<T2,F2>& sum) : terms() 
        {
            for (auto term : sum.terms)
            {
                terms.push_back(F(term));
            }
        }

        explicit Sum(const std::vector<F> terms_) : terms(terms_) 
        {
        }

        explicit Sum(std::initializer_list<F> ts)
        {
            std::copy(ts.begin(), ts.end(), std::back_inserter(terms));
        }

        explicit Sum(const F& term) : terms(1, term)
        {
        }

        T operator()(const T x) const
        {
            T y(0.0);
            for (auto term : terms)
            {
                y += term(x);
            }
            return y;
        }

        Sum<T,F>& operator=(const T k)
        {
            terms.clear();
            terms.emplace_back(k);
            return *this;
        }

        Sum<T,F>& operator=(const F& term)
        {
            terms.clear();
            terms.emplace_back(term);
            return *this;
        }

        Sum<T,F>& operator=(const Sum<T,F>& sum)
        {
            terms = sum.terms;
            return *this;
        }

        Sum<T,F>& operator+=(const T k)
        {
            terms.emplace_back(k);
            return *this;
        }

        Sum<T,F>& operator+=(const F& term)
        {
            terms.push_back(term);
            return *this;
        }

        Sum<T,F>& operator+=(const Sum<T,F>& sum)
        {
            terms.insert(terms.end(), sum.terms.begin(), sum.terms.end());
            return *this;
        }

        Sum<T,F>& operator-=(const T k)
        {
            terms.emplace_back(-k);
            return *this;
        }

        Sum<T,F>& operator-=(const F& term)
        {
            terms.push_back(-term);
            return *this;
        }

        Sum<T,F>& operator-=(const Sum<T,F>& sum)
        {
            for (auto term : sum.terms)
            {
                terms.push_back(-term);
            }
            return *this;
        }

        Sum<T,F>& operator*=(const T k)
        {
            for (auto& term : terms)
            {
                term *= k;
            }
            return *this;
        }

        Sum<T,F>& operator/=(const T k)
        {
            for (auto& term : terms)
            {
                term /= k;
            }
            return *this;
        }

    };







    template<typename T, typename F>
    constexpr auto operator+(const Sum<T,F>& f, const T k)
    {
        Sum<T,F> y(f);
        y += k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator+(const T k, const Sum<T,F>& f)
    {
        Sum<T,F> y(f);
        y += k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator-(const Sum<T,F>& f, const T k)
    {
        Sum<T,F> y(f);
        y -= k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator-(const T k, const Sum<T,F>& f)
    {
        Sum<T,F> y(f);
        y *= T(-1);
        y += k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator*(const Sum<T,F>& f, const T k)
    {
        Sum<T,F> y(f);
        y *= k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator*(const T k, const Sum<T,F>& f)
    {
        Sum<T,F> y(f);
        y *= k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator/(const Sum<T,F>& f, const T k)
    {
        Sum<T,F> y(f);
        y /= k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator-(const Sum<T,F>& f)
    {
        Sum<T,F> y(f);
        y *= T(-1);
        return y;
    }




    /*
    NOTE: we cannot support multiplication between arbitrary sums and their function types.
    This is because we cannot determine the return type in the general case, 
    and specializations that can determine the return type will collide with templates for the general case
    */ 






    template<typename T, typename F>
    constexpr auto operator+(const Sum<T,F>& f, const F& g)
    {
        Sum<T,F> y(f);
        y += g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator+(const F& g, const Sum<T,F>& f)
    {
        Sum<T,F> y(f);
        y += g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator-(const Sum<T,F>& f, const F& g)
    {
        Sum<T,F> y(f);
        y -= g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator-(const F& g, const Sum<T,F>& f)
    {
        Sum<T,F> y(g);
        y -= f;
        return y;
    }




    /*
    NOTE: we cannot support multiplication between arbitrary railyards, even of the same type.
    This is because we cannot determine the return type in the general case, 
    and specializations that can determine the return type will collide with templates for the general case
    */ 













    template<typename T, typename F>
    constexpr Sum<T,F> compose(const Sum<T,F>& f, const Identity<T> e)
    {
        return f;
    }

    template<typename T, typename F, typename G>
    constexpr Sum<T,F> compose(const Sum<T,F>& f, const G& g)
    {
        std::vector<Railcar<T,F>> terms;
        for (auto term: f.terms)
        {
            terms.push_back( compose(term, g) );
        }
        return Sum<T,F>(terms);
    }


    /*
    `derivative` returns the derivative of a railyard at a point.
    It is meant to compliment the method signature for integral(sum, lo, hi)
    */
    template<typename T, typename F>
    T derivative(const Sum<T,F>& sum, const T x)
    {
        T dydx(0.0f);
        for (std::size_t i=0; i<sum.terms.size(); i++)
        {
            dydx += derivative(sum.terms[i], x);
        }
        return dydx;
    }

    /*
    `integral` returns the definite integral of a railyard 
    without representing the integral as its own function object.
    This is meant to be used as a fallback in the event the function is 
    not closed under integration. 
    */
    template<typename T, typename F>
    T integral(const Sum<T,F>& sum, const T x)
    {
        T I(0.0f);
        for (std::size_t i=0; i<sum.terms.size(); i++)
        {
            I += integral(sum.terms[i], x); 
        }
        return I;
    }

    /*
    `integral` returns the definite integral of a railyard 
    without representing the integral as its own function object.
    This is meant to be used as a fallback in the event the function is 
    not closed under integration. 
    */
    template<typename T, typename F>
    T integral(const Sum<T,F>& sum, const T lo, const T hi)
    {
        T I(0.0f);
        for (auto term : sum.terms)
        {
            I += integral(term, lo, hi);
        }
        return I;
    }


    template<typename T, typename F>
    constexpr Sum<T,F> restriction(const Sum<T,F>& sum, const T lo, const T hi)
    {
        std::vector<F> terms;
        const T oo(std::numeric_limits<T>::max());
        for (auto term: sum.terms)
        {
            if (lo < term.lo || term.hi <= hi)
            {
                if (terms.size() < 1)
                {
                    terms.push_back( F(-oo, term.lo, F( sum(term.lo) ) ));
                }
                terms.push_back(term);
            }
        }
        terms.push_back( F( 
            terms[terms.size()-1].hi, std::numeric_limits<T>::max(), 
            F( sum( terms[terms.size()-1].hi ) ) ));

        return Sum(terms);
    }


    // TODO: refactor to return Train<T,F> so that continuity is guaranteed downstream
    template<typename T, typename F>
    constexpr Sum<T,F> simplify(const Sum<T,F>& sum)
    {
        const std::vector<T> couplers(sum.couplers());
        std::vector<F> terms;
        for (std::size_t i=1; i<couplers.size(); i++)
        {
            F f;
            // add together all terms that intersect the region from couplers[i-1] to couplers[i]
            for (auto term: sum.terms)
            {
                if (std::max(term.lo, couplers[i-1]) < std::min(term.hi, couplers[i]))
                {
                    f += term.content;
                }
            }
            terms.push_back(F(couplers[i-1], couplers[i], f));
        }
        return Sum<T,F>(terms);
    }


    /*
    `dot` is the integral of the product between the output of two functions.
    It is analogous to the dot product of vectors
    however it treats functions as vectors in a function space with infinitely many dimensions.
    */
    template<typename T, typename F>
    constexpr T dot(
        const Sum<T,F>& yard1, 
        const Sum<T,F>& yard2, 
        const T lo, 
        const T hi
    ){
        return integral(yard1*yard2, lo, hi);
    }

    /*
    `length` is the root of the dot product of a function with itself.
    It is analogous to the Euclidean length of a vector, 
    however it treats functions as vectors in a function space with infinitely many dimensions.
    */
    template<typename T, typename F>
    constexpr T length(
        const Sum<T,F>& sum, 
        const T lo, 
        const T hi
    ){
        return std::sqrt(integral(sum*sum, lo, hi));
    }

    /*
    `similarity` is dot product of two functions divided by their lengths.
    It is analogous to the cosine similarity between two vectors,
    however it treats functions as vectors in a function space with infinitely many dimensions.
    */
    template<typename T, typename F>
    constexpr T similarity(
        const Sum<T,F>& sum1, 
        const Sum<T,F>& sum2, 
        const T lo, 
        const T hi
    ){
        return dot(sum1,sum2,lo,hi) / (length(sum1,lo,hi)*length(sum2,lo,hi));
    }

}
