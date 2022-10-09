#pragma once

// std libraries
#include <algorithm> // copy, back_inserter
#include <initializer_list>
#include <iostream>

// in-house libraries
#include "Identity.hpp"
#include "Clamped.hpp"
#include "Railcar.hpp"
#include "Railcar_to_string.hpp"

namespace analytic {

    /*
    A `Train` is a sum of railcar functions where no overlaps exist.
    See `Railcar` for more information.

    A train has properties similar to a railyard, 
    but permits additional operations for certain kinds of contents.
    */

    template<typename T, typename F>
    struct Train {

        std::vector<F> content; 
        std::vector<T> couplers; 

        // zero constructor
        Train() : 
            content(1, F(0)),
            couplers()
        {
            const T oo = std::numeric_limits<T>::max();
            couplers.push_back(-oo);
            couplers.push_back(oo);
        }

        // constant constructor
        Train(const T k) : 
            content(1, F(k)),
            couplers()
        {
            const T oo = std::numeric_limits<T>::max();
            couplers.push_back(-oo);
            couplers.push_back(oo);
        }

        // copy constructor
        Train(const Train<T,F>& train) : 
            content(train.content),
            couplers(train.couplers)
        {
        }

        // cast constructor
        template<typename T2, typename F2>
        explicit Train(const Train<T2,F2>& train) : 
            content(),
            couplers()
        {
            for (auto car : train.content)
            {
                content.push_back(car);
            }
        }

        // cast constructor
        explicit Train(const Clamped<T,F>& clamped) : 
            content(),
            couplers()
        {
            using R = Railcar<T,F>;
            const T oo = std::numeric_limits<T>::max();
            couplers.push_back(-oo);
                                            content.push_back(clamped(clamped.lo));
            couplers.push_back(clamped.lo);
                                            content.push_back(clamped.f);
            couplers.push_back(clamped.hi);
                                            content.push_back(clamped(clamped.hi));
            couplers.push_back(oo);
        }

        explicit Train(const F& f) : 
            content(1, Railcar<T,F>(f)),
            couplers()
        {
            const T oo = std::numeric_limits<T>::max();
            couplers.push_back(-oo);
            couplers.push_back(oo);
        }

        T operator()(const T x) const
        {
            T y(0.0);
            for (std::size_t i=0; i<content.size(); i++)
            {
                if (couplers[i] < x&&x < couplers[i+1])
                {
                    return content[i](x);
                }
            }
            return y;
        }

        Train<T,F>& operator=(const T k)
        {
            content.clear();
            couplers.clear();
            const T oo = std::numeric_limits<T>::max();
            couplers.push_back(-oo);
            content.emplace_back(F(k));
            couplers.push_back(oo);
            return *this;
        }

        Train<T,F>& operator=(const F& f)
        {
            content.clear();
            couplers.clear();
            const T oo = std::numeric_limits<T>::max();
            couplers.push_back(-oo);
            content.emplace_back(f);
            couplers.push_back(oo);
            return *this;
        }

        Train<T,F>& operator=(const Railcar<T,F>& car)
        {
            content.clear();
            content.emplace_back(car);
            return *this;
        }

        Train<T,F>& operator=(const Train<T,F>& train)
        {
            content = train.content;
            couplers = train.couplers;
            return *this;
        }



        Train<T,F>& operator+=(const T k)
        {
            for (auto& car : content)
            {
                car += k;
            }
            return *this;
        }

        Train<T,F>& operator+=(const F& f)
        {
            for (auto& car : content)
            {
                car += f;
            }
            return *this;
        }



        Train<T,F>& operator-=(const T k)
        {
            for (auto& car : content)
            {
                car -= k;
            }
            return *this;
        }

        Train<T,F>& operator-=(const F& f)
        {
            for (auto& car : content)
            {
                car -= f;
            }
            return *this;
        }



        Train<T,F>& operator*=(const T k)
        {
            for (auto& car : content)
            {
                car *= k;
            }
            return *this;
        }

        Train<T,F>& operator/=(const T k)
        {
            for (auto& car : content)
            {
                car /= k;
            }
            return *this;
        }

    };




    template<typename T, typename F>
    constexpr auto operator-(const Train<T,F>& f)
    {
        Train<T,F> y(f);
        y *= T(-1);
        return y;
    }




    template<typename T, typename F>
    constexpr auto operator+(const Train<T,F>& f, const T k)
    {
        Train<T,F> y(f);
        y += k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator+(const T k, const Train<T,F>& f)
    {
        Train<T,F> y(f);
        y += k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator-(const Train<T,F>& f, const T k)
    {
        Train<T,F> y(f);
        y -= k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator-(const T k, const Train<T,F>& f)
    {
        Train<T,F> y(f);
        y *= T(-1);
        y += k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator*(const Train<T,F>& f, const T k)
    {
        Train<T,F> y(f);
        y *= k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator*(const T k, const Train<T,F>& f)
    {
        Train<T,F> y(f);
        y *= k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator/(const Train<T,F>& f, const T k)
    {
        Train<T,F> y(f);
        y /= k;
        return (y);
    }




    // a convenience function that returns a sorted list of all couplers found within two trains
    template<typename T, typename F, typename G>
    constexpr auto couplers(const Train<T,F>& f, const Train<T,G>& g)
    {
        std::vector<T> fg_couplers;
        fg_couplers.insert(fg_couplers.end(), f.couplers.begin(), f.couplers.end());
        fg_couplers.insert(fg_couplers.end(), g.couplers.begin(), g.couplers.end());
        std::sort(fg_couplers.begin(), fg_couplers.end());
        auto last = std::unique(fg_couplers.begin(), fg_couplers.end());
        fg_couplers.erase(last, fg_couplers.end());
    }


    template<typename T, typename F>
    constexpr auto operator+(const Train<T,F>& f, const F& g)
    {
        std::vector<F> contents;
        for (auto fi : f.contents)
        {
            contents.push_back(fi+g);
        }
        return Train<T,F>(contents, f.couplers);
    }

    template<typename T, typename F>
    constexpr auto operator+(const F& g, const Train<T,F>& f)
    {
        std::vector<F> contents;
        for (auto fi : f.contents)
        {
            contents.push_back(g+fi);
        }
        return Train<T,F>(contents, f.couplers);
    }

    template<typename T, typename F>
    constexpr auto operator-(const Train<T,F>& f, const F& g)
    {
        std::vector<F> contents;
        for (auto fi : f.contents)
        {
            contents.push_back(fi-g);
        }
        return Train<T,F>(contents, f.couplers);
    }

    template<typename T, typename F>
    constexpr auto operator-(const F& g, const Train<T,F>& f)
    {
        std::vector<F> contents;
        for (auto fi : f.contents)
        {
            contents.push_back(g-fi);
        }
        return Train<T,F>(contents, f.couplers);
    }

    /*
    NOTE: we cannot support multiplication between arbitrary trains and their function types.
    This is because we cannot determine the return type in the general case, 
    and specializations that can determine the return type will collide with templates for the general case
    */ 





    // template<typename T, typename F>
    // constexpr auto operator+(const Clamped<T,F>& f, const Railcar<T,F>& g)
    // {
    //     Train<T,F> y(f);
    //     y += g;
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator+(const Railcar<T,F>& g, const Clamped<T,F>& f)
    // {
    //     Train<T,F> y(f);
    //     y += g;
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator-(const Clamped<T,F>& f, const Railcar<T,F>& g)
    // {
    //     Train<T,F> y(f);
    //     y -= g;
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator-(const Railcar<T,F>& g, const Clamped<T,F>& f)
    // {
    //     Train<T,F> y(g);
    //     y -= Train<T,F>(f);
    //     return y;
    // }





    // template<typename T, typename F>
    // constexpr auto operator+(const Clamped<T,F>& f, const Train<T,F>& g)
    // {
    //     Train<T,F> y(f);
    //     y += g;
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator+(const Train<T,F>& g, const Clamped<T,F>& f)
    // {
    //     Train<T,F> y(f);
    //     y += g;
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator-(const Clamped<T,F>& f, const Train<T,F>& g)
    // {
    //     Train<T,F> y(f);
    //     y -= g;
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator-(const Train<T,F>& g, const Clamped<T,F>& f)
    // {
    //     Train<T,F> y(g);
    //     y -= Train<T,F>(f);
    //     return y;
    // }





    // template<typename T, typename F>
    // constexpr auto operator+(const Train<T,F>& f, const Railcar<T,F>& g)
    // {
    //     Train<T,F> y(f);
    //     y += g;
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator+(const Railcar<T,F>& g, const Train<T,F>& f)
    // {
    //     Train<T,F> y(f);
    //     y += g;
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator-(const Train<T,F>& f, const Railcar<T,F>& g)
    // {
    //     Train<T,F> y(f);
    //     y -= g;
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator-(const Railcar<T,F>& g, const Train<T,F>& f)
    // {
    //     Train<T,F> y(g);
    //     y -= f;
    //     return y;
    // }




    // template<typename T, typename F>
    // constexpr auto operator+(const Railcar<T,F>& f, const Railcar<T,F>& g)
    // {
    //     Train<T,F> y(f);
    //     y += Train<T,F>(g);
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator-(const Railcar<T,F>& f, const Railcar<T,F>& g)
    // {
    //     Train<T,F> y(f);
    //     y -= Train<T,F>(g);
    //     return y;
    // }


    // template<typename T, typename F>
    // constexpr auto operator+(const Clamped<T,F>& f, const Clamped<T,F>& g)
    // {
    //     Train<T,F> y(f);
    //     y += Train<T,F>(g);
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator-(const Clamped<T,F>& f, const Clamped<T,F>& g)
    // {
    //     Train<T,F> y(f);
    //     y -= Train<T,F>(g);
    //     return y;
    // }



    // template<typename T, typename F>
    // constexpr auto operator+(const Train<T,F>& f, const Train<T,F>& g)
    // {
    //     Train<T,F> y(f);
    //     y += g;
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator-(const Train<T,F>& f, const Train<T,F>& g)
    // {
    //     Train<T,F> y(f);
    //     y -= g;
    //     return y;
    // }


    /*
    NOTE: we cannot support multiplication between arbitrary trains, even of the same type.
    This is because we cannot determine the return type in the general case, 
    and specializations that can determine the return type will collide with templates for the general case
    */ 





    template<typename T, typename F>
    constexpr Train<T,F> compose(const Train<T,F>& f, const Identity<T> e)
    {
        return f;
    }

    template<typename T, typename F, typename G>
    constexpr Train<T,F> compose(const Train<T,F>& f, const G& g)
    {
        std::vector<F> content;
        for (auto car: f.content)
        {
            content.push_back( compose(car, g) );
        }
        return Train(content, f.couplers);
    }

    template<typename FG, typename T, typename F, typename G>
    constexpr Railyard<T,FG> compose(const Train<T,F>& f, const Train<T,G>& g)
    {
        Railyard<T,FG> y;
        for (auto fi: f2.content)
        {
            for (auto gi: g2.content)
            {
                y += compose(fi, gi);
            }
        }
        return y;
    }










    /*
    `derivative` returns the derivative of a train at a point.
    It is meant to compliment the method signature for integral(train, lo, hi)
    */
    template<typename T, typename F>
    T derivative(const Train<T,F>& train, const T x)
    {
        for (std::size_t i=0; i<content.size(); i++)
        {
            if (couplers[i] < x&&x < couplers[i+1])
            {
                return derivative(train.content[i], x);
            }
        }
        return 0.0;
    }

    /*
    `integral` returns the definite integral of a train 
    without representing the integral as its own function object.
    This is meant to be used as a fallback in the event the function is 
    not closed under integration. 
    */
    template<typename T, typename F>
    T integral(const Train<T,F>& train, const T x)
    {
        T I(0.0f);
        for (std::size_t i=0; i<content.size(); i++)
        {
            I += integral(train.content[i], std::min(x, train.couplers[i+1])); 
            if (couplers[i] < x&&x < couplers[i+1])
            {
                return I;
            } 
        }
        return I;
    }

    /*
    `integral` returns the definite integral of a train 
    without representing the integral as its own function object.
    This is meant to be used as a fallback in the event the function is 
    not closed under integration. 
    */
    template<typename T, typename F>
    T integral(const Train<T,F>& train, const T lo, const T hi)
    {
        T I(0.0f);
        for (std::size_t i=0; i<content.size(); i++)
        {
            if (lo < couplers[i])
            {
                I += integral(train.content[i], std::max(lo, train.couplers[i]), std::min(hi, train.couplers[i+1])); 
            }
            if (couplers[i] < hi&&hi < couplers[i+1])
            {
                return I;
            }
        }
        return I;
    }




    template<typename T, typename F>
    constexpr Train<T,F> restriction(const Train<T,F>& train, const T lo, const T hi)
    {
        std::vector<F> content;
        std::vector<T> couplers;
        const T oo(std::numeric_limits<T>::max());
        for (std::size_t i=0; i<train.content.size(); i++)
        {
            if (lo < train.couplers[i] || train.couplers[i+1] <= hi)
            {
                if (content.size() < 1)
                {
                    couplers.push_back(-oo);
                    content.push_back(train.content[i](train.couplers[i]));
                    couplers.push_back(train.couplers[i]);
                }
                content.push_back(train.content[i]);
                couplers.push_back(train.couplers[i+1]);
            }
        }
        content.push_back(train.content[train.content.size()-1](train.couplers[train.couplers.size()-1]));
        couplers.push_back(oo);
        return Train(content, couplers);
    }

    // TODO: refactor to return Train<T,F> so that continuity is guaranteed downstream
    template<typename T, typename F>
    constexpr Train<T,F> simplify(const Railyard<T,F>& yard)
    {
        const std::vector<T> couplers(yard.couplers());
        std::vector<F> cars;
        for (std::size_t i=1; i<couplers.size(); i++)
        {
            F f;
            // add together all cars that intersect the region from couplers[i-1] to couplers[i]
            for (auto car: yard.cars)
            {
                if (std::max(car.lo, couplers[i-1]) < std::min(car.hi, couplers[i]))
                {
                    f += car.content;
                }
            }
            cars.push_back(f);
        }
        return Train<T,F>(contents, couplers);
    }



    /*
    `dot` is the integral of the product between the output of two functions.
    It is analogous to the dot product of vectors
    however it treats functions as vectors in a function space with infinitely many dimensions.
    */
    template<typename T, typename F>
    constexpr T dot(
        const Train<T,F>& yard1, 
        const Train<T,F>& yard2, 
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
        const Train<T,F>& train, 
        const T lo, 
        const T hi
    ){
        return std::sqrt(integral(train*train, lo, hi));
    }

    /*
    `similarity` is dot product of two functions divided by their lengths.
    It is analogous to the cosine similarity between two vectors,
    however it treats functions as vectors in a function space with infinitely many dimensions.
    */
    template<typename T, typename F>
    constexpr T similarity(
        const Train<T,F>& train, 
        const Train<T,F>& q, 
        const T lo, 
        const T hi
    ){
        auto length_ = length(train,lo,hi);
        return dot(train,q,lo,hi) / (length_*length_);
    }

}
