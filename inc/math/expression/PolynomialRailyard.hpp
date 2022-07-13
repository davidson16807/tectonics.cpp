#pragma once

// std libraries
#include <algorithm> // copy, back_inserter
#include <iostream> 

// in-house libraries
#include "Identity.hpp"
#include "Scaling.hpp"
#include "Shifting.hpp"
#include "Railcar.hpp"
#include "Railyard.hpp"
#include "PolynomialRailcar.hpp"

namespace math {

    template<typename T, int Plo, int Phi>
    using PolynomialRailyard = Railyard<T,Polynomial<T,Plo,Phi>>;

    template<typename T, int Plo, int Phi>
    constexpr std::string to_string(const PolynomialRailyard<T,Plo,Phi>& yard)
    {
        std::string output("\r\n");
        for (std::size_t i=0; i<yard.cars.size(); i++)
        {
            output += to_string(yard.cars[i]);
            output += "\r\n";
        }
        return output;
    }

    // RAILYARDS

    // ADDITION AND SUBTRACTION

    // with polynomials

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const PolynomialRailyard<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y += F(q);
        return simplify(y);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const Polynomial<T,Qlo,Qhi>& q, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y += F(q);
        return simplify(y);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const PolynomialRailyard<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y += F(-q);
        return simplify(y);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const Polynomial<T,Qlo,Qhi>& q, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(q);
        y += F(-p);
        return simplify(y);
    }






    // with railcars

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailcar<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y += Railcar<T,F>(q);
        return simplify(y);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const PolynomialRailcar<T,Qlo,Qhi>& q, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y += Railcar<T,F>(q);
        return simplify(y);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailcar<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y += Railcar<T,F>(-q);
        return simplify(y);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const PolynomialRailcar<T,Qlo,Qhi>& q, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(q);
        y += Railcar<T,F>(-p);
        return simplify(y);
    }



    // with other railyards

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y += Railyard<T,F>(q);
        return simplify(y);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,std::min(Plo,Qlo),std::max(Phi,Qhi)>;
        Railyard<T,F> y(p);
        y -= Railyard<T,F>(q);
        return simplify(y);
    }




    // MULTIPLICATION

    // with polynomials

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialRailyard<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        Railyard<T,F> y;
        for (auto pi : p.cars)
        {
            y += pi*q;
        }
        return simplify(y);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Polynomial<T,Qlo,Qhi> q, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        Railyard<T,F> y;
        for (auto pi : p.cars)
        {
            y += pi*q;
        }
        return simplify(y);
    }

    template<typename T, int Plo, int Phi, int Q>
    constexpr auto operator/(const PolynomialRailyard<T,Plo,Phi>& p, const Polynomial<T,Q,Q> q)
    {
        using F = Polynomial<T,Plo-Q,Phi-Q>;
        Railyard<T,F> y;
        for (auto pi : p.cars)
        {
            y += pi/q;
        }
        return simplify(y);
    }

    /*
    NOTE: we cannot support division by railyards.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    We can only be reasonably confident this will not be the case for trains.
    */ 



    // with railcars

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailcar<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        Railyard<T,F> y;
        Railcar<T,F> car;
        for (auto pi : p.cars)
        {
            car = pi*q;
            if (car.lo < car.hi)
            {
                y += car;
            }
        }
        return simplify(y);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialRailcar<T,Qlo,Qhi> q, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        std::vector<Railcar<T,F>> cars;
        Railyard<T,F> y;
        Railcar<T,F> car;
        for (auto pi : p.cars)
        {
            car = q*pi;
            if (car.lo < car.hi)
            {
                y += car;
            }
        }
        return simplify(y);
    }



    // with other railyards

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi>& q)
    {
        using F = Polynomial<T,Plo+Qlo,Phi+Qhi>;
        std::vector<Railcar<T,F>> cars;
        Railyard<T,F> y;
        Railcar<T,F> car;
        for (auto pi : p.cars)
        {
            for (auto qj : q.cars)
            {
                car = (pi*qj);
                if (car.lo < car.hi)
                {
                    y += car;
                }
            }
        }
        return simplify(y);
    }

    /*
    NOTE: we cannot support division by railyards.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    We can only be reasonably confident this will not be the case for trains.
    */ 







    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const PolynomialRailyard<T,Plo,Phi>& p, const Shifting<T> f)
    {
        return p + Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Shifting<T> f, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,0,1>(f) + p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const PolynomialRailyard<T,Plo,Phi>& p, const Shifting<T> f)
    {
        return p - Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Shifting<T> f, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,0,1>(f) - p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const PolynomialRailyard<T,Plo,Phi>& p, const Shifting<T> f)
    {
        return p * Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Shifting<T> f, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,0,1>(f) * p;
    }



    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const PolynomialRailyard<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p + Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Scaling<T> f, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(f) + p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const PolynomialRailyard<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p - Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Scaling<T> f, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(f) - p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const PolynomialRailyard<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p * Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Scaling<T> f, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(f) * p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator/(const PolynomialRailyard<T,Plo,Phi>& p, const Scaling<T> f)
    {
        return p / Polynomial<T,1,1>(f);
    }



    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const PolynomialRailyard<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p + Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Identity<T> e, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(e) + p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const PolynomialRailyard<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p - Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Identity<T> e, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(e) - p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const PolynomialRailyard<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p * Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Identity<T> e, const PolynomialRailyard<T,Plo,Phi>& p)
    {
        return Polynomial<T,1,1>(e) * p;
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator/(const PolynomialRailyard<T,Plo,Phi>& p, const Identity<T> e)
    {
        return p / Polynomial<T,1,1>(e);
    }




    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto compose(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi> q)
    {
        const auto p2(simplify(p));
        const auto q2(simplify(q));

        std::vector<T> p_couplers(p2.couplers());
        std::vector<T> q_couplers(q2.couplers());
        std::vector<T> pq_couplers;
        pq_couplers.insert(pq_couplers.end(), p_couplers.begin(), p_couplers.end());
        pq_couplers.insert(pq_couplers.end(), q_couplers.begin(), q_couplers.end());
        std::sort(pq_couplers.begin(), pq_couplers.end());
        auto last = std::unique(pq_couplers.begin(), pq_couplers.end());
        pq_couplers.erase(last, pq_couplers.end());

        using F = Polynomial<T,Plo,Phi>;
        using G = Polynomial<T,Qlo,Qhi>;
        using FG = Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)>;
        std::vector<Railcar<T,F>> cars;
        for (std::size_t i=1; i<pq_couplers.size(); i++)
        {
            /*
            Create running totals for cars in p and q,
            add cars to their respective running total if they 
            intersect the region from pq_couplers[i-1] to pq_couplers[i]
            */
            bool is_f_nonzero(false);
            F f;
            G g;
            FG fg;
            for (auto car: p2.cars)
            {
                if (std::max(car.lo, pq_couplers[i-1]) < std::min(car.hi, pq_couplers[i]))
                {
                    is_f_nonzero = true;
                    f += car.content;
                }
            }
            for (auto car: q2.cars)
            {
                if (std::max(car.lo, pq_couplers[i-1]) < std::min(car.hi, pq_couplers[i]))
                {
                    g += car.content;
                }
            }
            /*
            If any cars were added to the running total for p,
            then add f∘g for the region [pq_couplers[i-1], pq_couplers[i]] 
            to the list of cars in p∘q
            */
            if (is_f_nonzero) {
                cars.push_back(Railcar<T,FG>(pq_couplers[i-1], pq_couplers[i], compose(f,g)));
            }
        }
        return Railyard<T,FG>(cars);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto compose(const PolynomialRailyard<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)>;
        std::vector<Railcar<T,F>> cars;
        for (auto pi : p.cars)
        {
            cars.push_back(compose(pi, q));
        }
        return Railyard<T,F>(cars);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto compose(const PolynomialRailyard<T,Plo,Phi>& p, const Scaling<T> g)
    {
        using F = Polynomial<T,Plo,Phi>;
        std::vector<Railcar<T,F>> cars;
        for (auto pi : p.cars)
        {
            cars.push_back(compose(pi, g));
        }
        return Railyard<T,F>(cars);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto compose(const PolynomialRailyard<T,Plo,Phi>& p, const Shifting<T> g)
    {
        using F = Polynomial<T,std::min(0,Plo),Phi>;
        std::vector<Railcar<T,F>> cars;
        for (auto pi : p.cars)
        {
            cars.push_back(compose(pi, g));
        }
        return Railyard<T,F>(cars);
    }



    template<typename T, int Plo, int Phi>
    constexpr auto derivative(const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,Plo-1,Phi-1>;
        Railyard<T,F> y;
        for (auto pi : p.cars)
        {
            y.cars.push_back(Railcar<T,F>(pi.lo, pi.hi, derivative(pi.content)));
            // y += derivative(pi.content);
        }
        return y;
    }

    template<typename T, int Plo, int Phi>
    constexpr auto integral(const PolynomialRailyard<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,Plo+1,Phi+1>;
        // const T oo(std::numeric_limits<T>::max());
        Railyard<T,F> y;
        Railcar<T,F> g, gmax;
        for (auto pi : p.cars)
        {
            y += integral(pi);
        }
        return y;
    }




    template<typename T, int Plo, int Phi>
    constexpr PolynomialRailyard<T,Plo*2,Phi*2> square(const PolynomialRailyard<T,Plo,Phi>& p){
        return p*p;
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
    We do not presume that this metric serves more naturally as a distance metric compared to others,
    however we address this function as `distance` since the alternative names
    are either unwieldy (`root_mean_square_difference`) or nonobvious (`rmsd`).
    */
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(
        const PolynomialRailyard<T,Plo,Phi>& p, 
        const PolynomialRailyard<T,Qlo,Qhi>& q, 
        const T lo, 
        const T hi
    ){
        const auto difference = p-q;
        return std::sqrt(std::max(T(0), integral(square(difference), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const PolynomialRailyard<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(square(p-q), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const Polynomial<T,Plo,Phi> p, const PolynomialRailyard<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(square(p-q), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi>
    constexpr T distance(const PolynomialRailyard<T,Plo,Phi>& p, const T k, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(square(p-k), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi>
    constexpr T distance(const T k, const PolynomialRailyard<T,Plo, Phi> p, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(square(p-k), lo, hi))) / (hi-lo);
    }


    template<typename T, int Plo, int Phi>
    constexpr T maximum(const PolynomialRailyard<T,Plo,Phi> p, const T lo, const T hi) 
    {
        auto p2 = simplify(p);
        T result = p.cars[0];
        T candidate;
        for (auto pi : p.cars)
        {
            candidate = maximum(pi, lo, hi);
            result = candidate > result? candidate : result;
        }
        return result;
    }
    template<typename T, int Plo, int Phi>
    constexpr T minimum(const PolynomialRailyard<T,Plo,Phi> p, const T lo, const T hi) 
    {
        auto p2 = simplify(p);
        T result = p.cars[0];
        T candidate;
        for (auto pi : p.cars)
        {
            candidate = minimum(pi, lo, hi);
            result = candidate < result? candidate : result;
        }
        return result;
    }

}
