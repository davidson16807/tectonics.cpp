#pragma once

// std libraries
#include <algorithm> // copy, back_inserter
#include <initializer_list>
#include <iostream>

// in-house libraries
#include "../Identity.hpp"
#include "../Clamped.hpp"

#include "Railcar.hpp"

namespace math {

    /*
    A `Railyard` is a sum of railcar functions multiplied by functions of an arbitrary type F.
    See `Railcar` for more information.

    A railyard of function type F has the following properties:
    * It is closed under addition with itself, with instances of F, 
      and any other functions that can be represented by F. This applies regardless of the properties of F.
    * If F is closed under scalar multiplication, 
      then the railyard of F will be closed under subtraction with itself.
    * If F is closed under an operation, then the railyard of F will also be closed under that operation.
    */

    template<typename T, typename F>
    struct Railyard {
        using value_type = T;

        std::vector<Railcar<T,F>> cars; 

        // the zero railyard
        Railyard() : cars()
        {
        }

        // constant constructor
        explicit Railyard(const T k) : cars(1, Railcar<T,F>(k))
        {
        }

        // copy constructor
        Railyard(const Railyard<T,F>& yard) : cars(yard.cars) 
        {
        }

        // cast constructor
        template<typename T2, typename F2>
        explicit Railyard(const Railyard<T2,F2>& yard) : cars() 
        {
            for (auto car : yard.cars)
            {
                cars.push_back(Railcar<T,F>(car));
            }
        }

        // cast constructor
        explicit Railyard(const Clamped<T,F>& clamped) : cars()
        {
            using R = Railcar<T,F>;
            const T oo = std::numeric_limits<T>::max();
            cars.push_back(R(       -oo, clamped.lo, F(clamped(clamped.lo))));
            cars.push_back(R(clamped.lo, clamped.hi,   clamped.f));
            cars.push_back(R(clamped.hi, oo,         F(clamped(clamped.hi))));
        }

        Railyard(const std::vector<Railcar<T,F>> cars_) : cars(cars_) 
        {
        }

        Railyard(std::initializer_list<Railcar<T,F>> ts)
        {
            std::copy(ts.begin(), ts.end(), std::back_inserter(cars));
        }

        explicit Railyard(const F& f) : cars(1, Railcar<T,F>(f))
        {
        }

        explicit Railyard(const Railcar<T,F>& car) : cars(1, car)
        {
        }

        // explicit Railyard(const Train<T,F>& train) : cars(train.cars)
        // {
        // }

        constexpr std::vector<T> couplers() const
        {
            // gather all boundaries for all cars
            std::vector<T> bounds;
            for (auto car : cars)
            {
                bounds.push_back(car.lo);
                bounds.push_back(car.hi);
            }
            std::sort(bounds.begin(), bounds.end());
            auto last = std::unique(bounds.begin(), bounds.end());
            bounds.erase(last, bounds.end());
            return bounds;
        }



        T operator()(const T x) const
        {
            T y(0.0);
            for (auto car : cars)
            {
                y += car(x);
            }
            return y;
        }


        Railcar<T,F>& operator[](const std::size_t i)
        {
            return cars[i];
        }
        Railcar<T,F> operator[](const std::size_t i) const
        {
            const T oo = std::numeric_limits<double>::max();
            return i<0? 
                    Railcar<T,F>(-oo,oo,F(0)) 
              : i>=cars.size()? 
                    Railcar<T,F>(oo,oo,F(0)) 
              : 
                    cars[i];
        }
        std::size_t size() const
        {
            return cars.size();
        }

        Railyard<T,F>& operator=(const T k)
        {
            cars.clear();
            cars.emplace_back(k);
            return *this;
        }

        Railyard<T,F>& operator=(const F& f)
        {
            cars.clear();
            cars.emplace_back(f);
            return *this;
        }

        Railyard<T,F>& operator=(const Railcar<T,F>& car)
        {
            cars.clear();
            cars.emplace_back(car);
            return *this;
        }

        Railyard<T,F>& operator=(const Railyard<T,F>& yard)
        {
            cars = yard.cars;
            return *this;
        }



        Railyard<T,F>& operator+=(const T k)
        {
            cars.emplace_back(k);
            return *this;
        }

        Railyard<T,F>& operator+=(const F& f)
        {
            cars.emplace_back(f);
            return *this;
        }

        Railyard<T,F>& operator+=(const Railcar<T,F>& car)
        {
            cars.push_back(car);
            return *this;
        }

        Railyard<T,F>& operator+=(const Railyard<T,F>& yard)
        {
            cars.insert(cars.end(), yard.cars.begin(), yard.cars.end());
            return *this;
        }



        Railyard<T,F>& operator-=(const T k)
        {
            cars.emplace_back(-k);
            return *this;
        }

        Railyard<T,F>& operator-=(const F& f)
        {
            cars.emplace_back(-f);
            return *this;
        }

        Railyard<T,F>& operator-=(const Railcar<T,F>& car)
        {
            cars.push_back(-car);
            return *this;
        }

        Railyard<T,F>& operator-=(const Railyard<T,F>& yard)
        {
            for (auto car : yard.cars)
            {
                cars.push_back(-car);
            }
            return *this;
        }



        Railyard<T,F>& operator*=(const T k)
        {
            for (auto& car : cars)
            {
                car *= k;
            }
            return *this;
        }

        Railyard<T,F>& operator/=(const T k)
        {
            for (auto& car : cars)
            {
                car /= k;
            }
            return *this;
        }

    };

    template<typename T, typename F>
    constexpr std::string to_string(const Railyard<T,F>& yard)
    {
        std::string output("\r\n");
        for (std::size_t i=0; i<yard.cars.size(); i++)
        {
            output += to_string(yard.cars[i]);
            output += "\r\n";
        }
        return output;
    }

    template<typename T, typename F>
    std::ostream& operator<<(std::ostream& os, const Railyard<T,F>& yard) { 
        os << to_string(yard);
        return os;
    }

    // a convenience function that returns a sorted list of all couplers found within two railyards
    template<typename T, typename F, typename G>
    constexpr auto couplers(const Railyard<T,F>& p, const Railyard<T,G>& q)
    {
        std::vector<T> p_couplers(p.couplers());
        std::vector<T> q_couplers(q.couplers());
        std::vector<T> pq_couplers;
        pq_couplers.insert(pq_couplers.end(), p_couplers.begin(), p_couplers.end());
        pq_couplers.insert(pq_couplers.end(), q_couplers.begin(), q_couplers.end());
        std::sort(pq_couplers.begin(), pq_couplers.end());
        auto last = std::unique(pq_couplers.begin(), pq_couplers.end());
        pq_couplers.erase(last, pq_couplers.end());
    }


    template<typename T, typename F>
    constexpr auto operator-(const Railyard<T,F>& f)
    {
        Railyard<T,F> y(f);
        y *= T(-1);
        return y;
    }




    template<typename T, typename F>
    constexpr auto operator+(const Railyard<T,F>& f, const T k)
    {
        Railyard<T,F> y(f);
        y += k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator+(const T k, const Railyard<T,F>& f)
    {
        Railyard<T,F> y(f);
        y += k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator-(const Railyard<T,F>& f, const T k)
    {
        Railyard<T,F> y(f);
        y -= k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator-(const T k, const Railyard<T,F>& f)
    {
        Railyard<T,F> y(f);
        y *= T(-1);
        y += k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator*(const Railyard<T,F>& f, const T k)
    {
        Railyard<T,F> y(f);
        y *= k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator*(const T k, const Railyard<T,F>& f)
    {
        Railyard<T,F> y(f);
        y *= k;
        return (y);
    }

    template<typename T, typename F>
    constexpr auto operator/(const Railyard<T,F>& f, const T k)
    {
        Railyard<T,F> y(f);
        y /= k;
        return (y);
    }





    // template<typename T, typename F>
    // constexpr auto operator+(const Railyard<T,F>& f, const F& g)
    // {
    //     Railyard<T,F> y(f);
    //     y += g;
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator+(const F& g, const Railyard<T,F>& f)
    // {
    //     Railyard<T,F> y(f);
    //     y += g;
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator-(const Railyard<T,F>& f, const F& g)
    // {
    //     Railyard<T,F> y(f);
    //     y -= g;
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator-(const F& g, const Railyard<T,F>& f)
    // {
    //     Railyard<T,F> y(g);
    //     y -= f;
    //     return y;
    // }

    /*
    NOTE: we cannot support multiplication between arbitrary railyards and their function types.
    This is because we cannot determine the return type in the general case, 
    and specializations that can determine the return type will collide with templates for the general case
    */ 





    template<typename T, typename F>
    constexpr auto operator+(const Clamped<T,F>& f, const Railcar<T,F>& g)
    {
        Railyard<T,F> y(f);
        y += g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator+(const Railcar<T,F>& g, const Clamped<T,F>& f)
    {
        Railyard<T,F> y(f);
        y += g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator-(const Clamped<T,F>& f, const Railcar<T,F>& g)
    {
        Railyard<T,F> y(f);
        y -= g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator-(const Railcar<T,F>& g, const Clamped<T,F>& f)
    {
        Railyard<T,F> y(g);
        y -= Railyard<T,F>(f);
        return y;
    }





    template<typename T, typename F>
    constexpr auto operator+(const Clamped<T,F>& f, const Railyard<T,F>& g)
    {
        Railyard<T,F> y(f);
        y += g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator+(const Railyard<T,F>& g, const Clamped<T,F>& f)
    {
        Railyard<T,F> y(f);
        y += g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator-(const Clamped<T,F>& f, const Railyard<T,F>& g)
    {
        Railyard<T,F> y(f);
        y -= g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator-(const Railyard<T,F>& g, const Clamped<T,F>& f)
    {
        Railyard<T,F> y(g);
        y -= Railyard<T,F>(f);
        return y;
    }





    template<typename T, typename F>
    constexpr auto operator+(const Railyard<T,F>& f, const Railcar<T,F>& g)
    {
        Railyard<T,F> y(f);
        y += g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator+(const Railcar<T,F>& g, const Railyard<T,F>& f)
    {
        Railyard<T,F> y(f);
        y += g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator-(const Railyard<T,F>& f, const Railcar<T,F>& g)
    {
        Railyard<T,F> y(f);
        y -= g;
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator-(const Railcar<T,F>& g, const Railyard<T,F>& f)
    {
        Railyard<T,F> y(g);
        y -= f;
        return y;
    }




    template<typename T, typename F>
    constexpr auto operator+(const Railcar<T,F>& f, const Railcar<T,F>& g)
    {
        Railyard<T,F> y(f);
        y += Railyard<T,F>(g);
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator-(const Railcar<T,F>& f, const Railcar<T,F>& g)
    {
        Railyard<T,F> y(f);
        y -= Railyard<T,F>(g);
        return y;
    }


    template<typename T, typename F>
    constexpr auto operator+(const Clamped<T,F>& f, const Clamped<T,F>& g)
    {
        Railyard<T,F> y(f);
        y += Railyard<T,F>(g);
        return y;
    }

    template<typename T, typename F>
    constexpr auto operator-(const Clamped<T,F>& f, const Clamped<T,F>& g)
    {
        Railyard<T,F> y(f);
        y -= Railyard<T,F>(g);
        return y;
    }



    /*
    NOTE: we cannot support multiplication between arbitrary railyards, even of the same type.
    This is because we cannot determine the return type in the general case (particularly for Rationals), 
    and specializations that can determine the return type will collide with templates for the general case
    */ 

    // template<typename T, typename F>
    // constexpr auto operator+(const Railyard<T,F>& f, const Railyard<T,F>& g)
    // {
    //     Railyard<T,F> y(f);
    //     y += g;
    //     return y;
    // }

    // template<typename T, typename F>
    // constexpr auto operator-(const Railyard<T,F>& f, const Railyard<T,F>& g)
    // {
    //     Railyard<T,F> y(f);
    //     y -= g;
    //     return y;
    // }


    /*
    NOTE: we cannot support multiplication between arbitrary railyards, even of the same type.
    This is because we cannot determine the return type in the general case, 
    and specializations that can determine the return type will collide with templates for the general case
    */ 





    template<typename T, typename F>
    constexpr Railyard<T,F> compose(const Railyard<T,F>& f, const Identity<T> e)
    {
        return f;
    }









    /*
    `derivative` returns the derivative of a railyard at a point.
    It is meant to compliment the method signature for integral(yard, lo, hi)
    */
    template<typename T, typename F>
    T derivative(const Railyard<T,F>& yard, const T x)
    {
        T dydx(0.0f);
        for (std::size_t i=0; i<yard.cars.size(); i++)
        {
            dydx += derivative(yard.cars[i], x);
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
    T integral(const Railyard<T,F>& yard, const T x)
    {
        T I(0.0f);
        for (std::size_t i=0; i<yard.cars.size(); i++)
        {
            I += integral(yard.cars[i], x); 
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
    T integral(const Railyard<T,F>& yard, const T lo, const T hi)
    {
        T I(0.0f);
        for (std::size_t i=0; i<yard.cars.size(); i++)
        {
            I += integral(yard.cars[i], lo, hi);
        }
        return I;
    }



    template<typename T, typename F>
    constexpr Railyard<T,F> restriction(const Railyard<T,F>& yard, const T lo, const T hi)
    {
        std::vector<Railcar<T,F>> cars;
        const T oo(std::numeric_limits<T>::max());
        for (auto car: yard.cars)
        {
            if (lo < car.lo || car.hi <= hi)
            {
                if (cars.size() < 1)
                {
                    cars.push_back( Railcar<T,F>(-oo, car.lo, F( yard(car.lo) ) ));
                }
                cars.push_back(car);
            }
        }
        cars.push_back( Railcar<T,F>( 
            cars[cars.size()-1].hi, std::numeric_limits<T>::max(), 
            F( yard( cars[cars.size()-1].hi ) ) ));

        return Railyard(cars);
    }






    /*
    `dot` is the integral of the product between the output of two functions.
    It is analogous to the dot product of vectors
    however it treats functions as vectors in a function space with infinitely many dimensions.
    */
    template<typename T, typename F>
    constexpr T dot(
        const Railyard<T,F>& yard1, 
        const Railyard<T,F>& yard2, 
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
        const Railyard<T,F>& yard, 
        const T lo, 
        const T hi
    ){
        return std::sqrt(integral(yard*yard, lo, hi));
    }

    /*
    `similarity` is dot product of two functions divided by their lengths.
    It is analogous to the cosine similarity between two vectors,
    however it treats functions as vectors in a function space with infinitely many dimensions.
    */
    template<typename T, typename F>
    constexpr T similarity(
        const Railyard<T,F>& yard, 
        const Railyard<T,F>& q, 
        const T lo, 
        const T hi
    ){
        auto length_ = length(yard,lo,hi);
        return dot(yard,q,lo,hi) / (length_*length_);
    }

}
