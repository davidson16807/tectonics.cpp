#pragma once

// std libraries
#include <algorithm> // copy, back_inserter
#include <iostream> 

// in-house libraries
#include "../Identity.hpp"
#include "../Scaling.hpp"
#include "../Shifting.hpp"
#include "../Polynomial.hpp"

#include "Railcar.hpp"
#include "Train.hpp"
#include "PolynomialRailyard.hpp"

namespace analytic {

    template<typename T, int Plo, int Phi>
    using PolynomialTrain = Train<T,Polynomial<T,Plo,Phi>>;

    // RAILYARDS

    // ADDITION AND SUBTRACTION

    // with polynomials

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const PolynomialTrain<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        return railyard(p) + q;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const Polynomial<T,Qlo,Qhi>& q, const PolynomialTrain<T,Plo,Phi>& p)
    {
        return q + railyard(p);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const PolynomialTrain<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        return railyard(p) - q;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const Polynomial<T,Qlo,Qhi>& q, const PolynomialTrain<T,Plo,Phi>& p)
    {
        return q - railyard(p);
    }






    // // with railcars

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const PolynomialTrain<T,Plo,Phi>& p, const PolynomialRailcar<T,Qlo,Qhi>& q)
    {
        return railyard(p) + q;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const PolynomialRailcar<T,Qlo,Qhi>& q, const PolynomialTrain<T,Plo,Phi>& p)
    {
        return q + railyard(p);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const PolynomialTrain<T,Plo,Phi>& p, const PolynomialRailcar<T,Qlo,Qhi>& q)
    {
        return railyard(p) - q;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const PolynomialRailcar<T,Qlo,Qhi>& q, const PolynomialTrain<T,Plo,Phi>& p)
    {
        return q - railyard(p);
    }

    // // with railyards

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const PolynomialTrain<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi>& q)
    {
        return railyard(p) + q;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const PolynomialRailyard<T,Qlo,Qhi>& q, const PolynomialTrain<T,Plo,Phi>& p)
    {
        return q + railyard(p);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const PolynomialTrain<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi>& q)
    {
        return railyard(p) - q;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const PolynomialRailyard<T,Qlo,Qhi>& q, const PolynomialTrain<T,Plo,Phi>& p)
    {
        return q - railyard(p);
    }



    // // with other trains

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(
        const PolynomialTrain<T,Plo,Phi>& p, const PolynomialTrain<T,Qlo,Qhi>& q)
    {
        return railyard(p) + railyard(q);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(
        const PolynomialTrain<T,Plo,Phi>& p, const PolynomialTrain<T,Qlo,Qhi>& q)
    {
        return railyard(p) - railyard(q);
    }




    // // MULTIPLICATION

    // // with polynomials

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialTrain<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        std::vector<Polynomial<T,Plo+Qlo,Phi+Qhi>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pi*q);
        }
        return PolynomialTrain<T,Plo+Qlo,Phi+Qhi>(contents, p.couplers);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Polynomial<T,Qlo,Qhi> q, const PolynomialTrain<T,Plo,Phi>& p)
    {
        std::vector<Polynomial<T,Plo+Qlo,Phi+Qhi>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(q*pi);
        }
        return PolynomialTrain<T,Plo+Qlo,Phi+Qhi>(contents, p.couplers);
    }

    template<typename T, int Plo, int Phi, int Q>
    constexpr auto operator/(const PolynomialTrain<T,Plo,Phi>& p, const Polynomial<T,Q,Q> q)
    {
        std::vector<Polynomial<T,Plo-Q,Phi-Q>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pi/q);
        }
        return PolynomialTrain<T,Plo-Q,Phi-Q>(contents, p.couplers);
    }

    // /*
    // NOTE: we cannot support division by railcars.
    // This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    // We can only be reasonably confident this will not be the case for trains.
    // */ 



    // // with railcars

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialTrain<T,Plo,Phi>& p, const PolynomialRailcar<T,Qlo,Qhi> q)
    {
        return railyard(p) * q;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialRailcar<T,Qlo,Qhi> q, const PolynomialTrain<T,Plo,Phi>& p)
    {
        return q * railyard(p);
    }

    // // with railyards

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialTrain<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi> q)
    {
        return railyard(p) * q;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialRailyard<T,Qlo,Qhi> q, const PolynomialTrain<T,Plo,Phi>& p)
    {
        return q * railyard(p);
    }



    // // with other trains

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const PolynomialTrain<T,Plo,Phi>& p, const PolynomialTrain<T,Qlo,Qhi>& q)
    {
        return railyard(p) * railyard(q);
    }

    // /*
    // NOTE: we cannot support division by railyards.
    // This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    // We can only be reasonably confident this will not be the case for trains.
    // */ 







    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const PolynomialTrain<T,Plo,Phi>& p, const Shifting<T> f)
    {
        std::vector<Polynomial<T,std::min(Plo,0),std::max(1,Phi)>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pi+f);
        }
        return PolynomialTrain<T,std::min(Plo,0),std::max(1,Phi)>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Shifting<T> f, const PolynomialTrain<T,Plo,Phi>& p)
    {
        std::vector<Polynomial<T,std::min(Plo,0),std::max(1,Phi)>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(f+pi);
        }
        return PolynomialTrain<T,std::min(Plo,0),std::max(1,Phi)>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const PolynomialTrain<T,Plo,Phi>& p, const Shifting<T> f)
    {
        std::vector<Polynomial<T,std::min(Plo,0),std::max(1,Phi)>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pi-f);
        }
        return PolynomialTrain<T,std::min(Plo,0),std::max(1,Phi)>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Shifting<T> f, const PolynomialTrain<T,Plo,Phi>& p)
    {
        std::vector<Polynomial<T,std::min(Plo,0),std::max(1,Phi)>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(f-pi);
        }
        return PolynomialTrain<T,std::min(Plo,0),std::max(1,Phi)>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const PolynomialTrain<T,Plo,Phi>& p, const Shifting<T> f)
    {
        std::vector<Polynomial<T,Plo+0,Phi+1>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pi*f);
        }
        return PolynomialTrain<T,Plo+0,Phi+1>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Shifting<T> f, const PolynomialTrain<T,Plo,Phi>& p)
    {
        std::vector<Polynomial<T,Plo+0,Phi+1>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(f*pi);
        }
        return PolynomialTrain<T,Plo+0,Phi+1>(contents, p.couplers);
    }



    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const PolynomialTrain<T,Plo,Phi>& p, const Scaling<T> f)
    {
        std::vector<Polynomial<T,std::min(Plo,1),std::max(1,Phi)>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pi+f);
        }
        return PolynomialTrain<T,std::min(Plo,1),std::max(1,Phi)>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Scaling<T> f, const PolynomialTrain<T,Plo,Phi>& p)
    {
        std::vector<Polynomial<T,std::min(Plo,1),std::max(1,Phi)>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(f+pi);
        }
        return PolynomialTrain<T,std::min(Plo,1),std::max(1,Phi)>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const PolynomialTrain<T,Plo,Phi>& p, const Scaling<T> f)
    {
        std::vector<Polynomial<T,std::min(Plo,1),std::max(1,Phi)>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pi-f);
        }
        return PolynomialTrain<T,std::min(Plo,1),std::max(1,Phi)>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Scaling<T> f, const PolynomialTrain<T,Plo,Phi>& p)
    {
        std::vector<Polynomial<T,std::min(Plo,1),std::max(1,Phi)>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(f-pi);
        }
        return PolynomialTrain<T,std::min(Plo,1),std::max(1,Phi)>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const PolynomialTrain<T,Plo,Phi>& p, const Scaling<T> f)
    {
        std::vector<Polynomial<T,Plo+1,Phi+1>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pi*f);
        }
        return PolynomialTrain<T,Plo+1,Phi+1>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Scaling<T> f, const PolynomialTrain<T,Plo,Phi>& p)
    {
        std::vector<Polynomial<T,Plo+1,Phi+1>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(f*pi);
        }
        return PolynomialTrain<T,Plo+1,Phi+1>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator/(const PolynomialTrain<T,Plo,Phi>& p, const Scaling<T> f)
    {
        std::vector<Polynomial<T,Plo-1,Phi-1>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pi/f);
        }
        return PolynomialTrain<T,Plo-1,Phi-1>(contents, p.couplers);
    }



    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const PolynomialTrain<T,Plo,Phi>& p, const Identity<T> e)
    {
        std::vector<Polynomial<T,Plo+1,Phi+1>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pi+e);
        }
        return PolynomialTrain<T,std::min(Plo,1),std::max(1,Phi)>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator+(const Identity<T> e, const PolynomialTrain<T,Plo,Phi>& p)
    {
        std::vector<Polynomial<T,Plo+1,Phi+1>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(e+pi);
        }
        return PolynomialTrain<T,std::min(Plo,1),std::max(1,Phi)>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const PolynomialTrain<T,Plo,Phi>& p, const Identity<T> e)
    {
        std::vector<Polynomial<T,Plo+1,Phi+1>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pi-e);
        }
        return PolynomialTrain<T,std::min(Plo,1),std::max(1,Phi)>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator-(const Identity<T> e, const PolynomialTrain<T,Plo,Phi>& p)
    {
        std::vector<Polynomial<T,Plo+1,Phi+1>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(e-pi);
        }
        return PolynomialTrain<T,std::min(Plo,1),std::max(1,Phi)>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const PolynomialTrain<T,Plo,Phi>& p, const Identity<T> e)
    {
        std::vector<Polynomial<T,Plo+1,Phi+1>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pi*e);
        }
        return PolynomialTrain<T,Plo+1,Phi+1>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator*(const Identity<T> e, const PolynomialTrain<T,Plo,Phi>& p)
    {
        std::vector<Polynomial<T,Plo+1,Phi+1>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pi*e);
        }
        return PolynomialTrain<T,Plo+1,Phi+1>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto operator/(const PolynomialTrain<T,Plo,Phi>& p, const Identity<T> e)
    {
        std::vector<Polynomial<T,Plo-1,Phi-1>> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pi/e);
        }
        return PolynomialTrain<T,Plo+1,Phi+1>(contents, p.couplers);
    }



    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto compose(const PolynomialTrain<T,Plo,Phi>& p, const PolynomialTrain<T,Qlo,Qhi> q)
    {
        std::vector<T> pq_couplers = couplers(p,q);
        using F = Polynomial<T,Plo,Phi>;
        using G = Polynomial<T,Qlo,Qhi>;
        using FG = Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)>;
        std::vector<Railcar<T,F>> contents;
        for (std::size_t i=1; i<pq_couplers.size(); i++)
        {
            /*
            Create running totals for contents in p and q,
            add contents to their respective running total if they 
            intersect the region from pq_couplers[i-1] to pq_couplers[i]
            */
            bool is_f_nonzero(false);
            F f;
            G g;
            FG fg;
            for (auto car: p.contents)
            {
                if (std::max(car.lo, pq_couplers[i-1]) < std::min(car.hi, pq_couplers[i]))
                {
                    is_f_nonzero = true;
                    f += car.content;
                }
            }
            for (auto car: q.contents)
            {
                if (std::max(car.lo, pq_couplers[i-1]) < std::min(car.hi, pq_couplers[i]))
                {
                    g += car.content;
                }
            }
            /*
            If any contents were added to the running total for p,
            then add f∘g for the region [pq_couplers[i-1], pq_couplers[i]] 
            to the list of contents in p∘q
            */
            if (is_f_nonzero) {
                contents.push_back(Railcar<T,FG>(pq_couplers[i-1], pq_couplers[i], compose(f,g)));
            }
        }
        return Railyard<T,FG>(contents);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto compose(const PolynomialTrain<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)>;
        std::vector<F> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(compose(pi, q));
        }
        return Train<T,F>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto compose(const PolynomialTrain<T,Plo,Phi>& p, const Scaling<T> g)
    {
        using F = Polynomial<T,Plo,Phi>;
        std::vector<F> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(compose(pi, g));
        }
        return Train<T,F>(contents, p.couplers);
    }
    template<typename T, int Plo, int Phi>
    constexpr auto compose(const PolynomialTrain<T,Plo,Phi>& p, const Shifting<T> g)
    {
        using F = Polynomial<T,std::min(0,Plo),Phi>;
        std::vector<F> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(compose(pi, g));
        }
        return Train<T,F>(contents, p.couplers);
    }



    template<typename T, int Plo, int Phi>
    constexpr auto derivative(const PolynomialTrain<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,Plo-1,Phi-1>;
        std::vector<F> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(derivative(pi));
        }
        return Train<T,F>(contents, p.couplers);
    }

    template<typename T, int Plo, int Phi>
    constexpr auto integral(const PolynomialTrain<T,Plo,Phi>& p)
    {
        using F = Polynomial<T,Plo+1,Phi+1>;
        // const T oo(std::numeric_limits<T>::max());
        std::vector<F> contents;
        Railcar<T,F> g, gmax;
        for (auto pi : p.contents)
        {
            contents.push_back(integral(pi));
        }
        return Train<T,F>(contents, p.couplers);
    }



    template<int N, typename T, int Plo, int Phi, 
        typename = std::enable_if_t<(N==0)>>
    constexpr PolynomialTrain<T,0,0> pow(const PolynomialTrain<T,Plo,Phi>& p){
        return PolynomialTrain<T,0,0>(T(1));
    }

    template<int N, typename T, int Plo, int Phi, 
        typename = std::enable_if_t<(N>0)>>
    constexpr PolynomialTrain<T,Plo*N,Phi*N> pow(const PolynomialTrain<T,Plo,Phi>& p){
        return p*pow<N-1>(p);
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
        const PolynomialTrain<T,Plo,Phi>& p, 
        const PolynomialTrain<T,Qlo,Qhi>& q, 
        const T lo, 
        const T hi
    ){
        const auto difference = p-q;
        return std::sqrt(std::max(T(0), integral(pow<2>(difference), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const PolynomialTrain<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return distance((p), train(q), lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const PolynomialRailyard<T,Plo,Phi> p, const PolynomialTrain<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return distance(train(p), (q), lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const PolynomialTrain<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(pow<2>(p-q), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const Polynomial<T,Plo,Phi> p, const PolynomialTrain<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(pow<2>(p-q), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi>
    constexpr T distance(const PolynomialTrain<T,Plo,Phi>& p, const T k, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(pow<2>(p-k), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi>
    constexpr T distance(const T k, const PolynomialTrain<T,Plo, Phi> p, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(pow<2>(p-k), lo, hi))) / (hi-lo);
    }



    template<typename T, int Plo, int Phi>
    constexpr T maximum(const PolynomialTrain<T,Plo,Phi> p, const T lo, const T hi) 
    {
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
    constexpr T minimum(const PolynomialTrain<T,Plo,Phi> p, const T lo, const T hi) 
    {
        T result = p.cars[0];
        T candidate;
        for (auto pi : p.cars)
        {
            candidate = minimum(pi, lo, hi);
            result = candidate < result? candidate : result;
        }
        return result;
    }



    template<typename T, typename F>
    PolynomialTrain<T,0,1> linear_newton_polynomial(
        const Train<T,F>& train
    ){
        std::vector<Polynomial<T,0,1>> contents;
        for (auto car : train.cars)
        {
            contents.push_back(linear_newton_polynomial(car));
        }
        return PolynomialTrain<T,0,1>(contents, train.couplers);
    }

    template<typename T, typename F>
    PolynomialTrain<T,0,2> quadratic_newton_polynomial(
        const Train<T,F>& train
    ){
        std::vector<Polynomial<T,0,2>> contents;
        for (auto car : train.cars)
        {
            contents.push_back(quadratic_newton_polynomial(car));
        }
        return PolynomialTrain<T,0,2>(contents, train.couplers);
    }

    template<typename T, typename F>
    PolynomialTrain<T,0,3> cubic_newton_polynomial(
        const Train<T,F>& train
    ){
        std::vector<Polynomial<T,0,3>> contents;
        for (auto car : train.cars)
        {
            contents.push_back(cubic_newton_polynomial(car));
        }
        return PolynomialTrain<T,0,3>(contents, train.couplers);
    }

    template<typename T, typename F>
    PolynomialTrain<T,0,3> cubic_spline(
        const Train<T,F>& train
    ){
        std::vector<Polynomial<T,0,3>> contents;
        for (auto car : train.cars)
        {
            contents.push_back(cubic_spline(car));
        }
        return PolynomialTrain<T,0,3>(contents, train.couplers);
    }


}
