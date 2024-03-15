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



    // operations that are closed under trains

    template<typename T, 
        int P1lo, int P1hi,
        int P2lo, int P2hi>
    constexpr auto operator+(const PolynomialTrain<T,P1lo,P1hi>& p, const PolynomialTrain<T,P2lo,P2hi>& q)
    {
        using P1 = Polynomial<T,P1lo,P1hi>;
        using P2 = Polynomial<T,P2lo,P2hi>;
        using P = Polynomial<T,std::min(P1lo,P2lo),std::max(P1hi,P2hi)>;
        const std::vector<T> couplers_ = couplers(p,q);
        std::vector<P> contents;
        for (std::size_t i=0; i<=couplers_.size(); i++)
        {
            T sample = (couplers_[i] + couplers_[i+1]) / 2.0;
            P1 pi(0);
            P2 qi(0);
            // add together all contents that intersect the region from couplers_[i-1] to couplers_[i]
            for (std::size_t j=0; j<p.size(); j++)
            {
                auto car = p[j];
                if (car.lo <= sample && sample <= car.hi)
                {
                    pi = car.content;
                    break;
                }
            }
            // add together all contents that intersect the region from couplers_[i-1] to couplers_[i]
            for (std::size_t j=0; j<q.size(); j++)
            {
                auto car = q[j];
                if (car.lo <= sample && sample <= car.hi)
                {
                    qi = car.content;
                    break;
                }
            }
            contents.push_back(P(pi+qi));
        }
        Train<T,P> result(contents, couplers_);
        return result;
    }

    template<typename T, 
        int P1lo, int P1hi,
        int P2lo, int P2hi>
    constexpr auto operator-(const PolynomialTrain<T,P1lo,P1hi>& p, const PolynomialTrain<T,P2lo,P2hi>& q)
    {
        using P1 = Polynomial<T,P1lo,P1hi>;
        using P2 = Polynomial<T,P2lo,P2hi>;
        using P = Polynomial<T,std::min(P1lo,P2lo),std::max(P1hi,P2hi)>;
        const std::vector<T> couplers_ = couplers(p,q);
        std::vector<P> contents;
        for (std::size_t i=0; i<=couplers_.size(); i++)
        {
            T sample = (couplers_[i] + couplers_[i+1]) / 2.0;
            P1 pi(0);
            P2 qi(0);
            // add together all contents that intersect the region from couplers_[i-1] to couplers_[i]
            for (std::size_t j=0; j<p.size(); j++)
            {
                auto car = p[j];
                if (car.lo <= sample && sample <= car.hi)
                {
                    pi = car.content;
                    break;
                }
            }
            // add together all contents that intersect the region from couplers_[i-1] to couplers_[i]
            for (std::size_t j=0; j<q.size(); j++)
            {
                auto car = q[j];
                if (car.lo <= sample && sample <= car.hi)
                {
                    qi = car.content;
                    break;
                }
            }
            contents.push_back(P(pi-qi));
        }
        Train<T,P> result(contents, couplers_);
        return result;
    }

    template<typename T, 
        int P1lo, int P1hi,
        int P2lo, int P2hi>
    constexpr auto operator*(const PolynomialTrain<T,P1lo,P1hi>& p, const PolynomialTrain<T,P2lo,P2hi>& q)
    {
        using P1 = Polynomial<T,P1lo,P1hi>;
        using P2 = Polynomial<T,P2lo,P2hi>;
        using P = Polynomial<T,P1lo+P2lo,P1hi+P2hi>;
        const std::vector<T> couplers_ = couplers(p,q);
        std::vector<P> contents;
        for (std::size_t i=0; i<=couplers_.size(); i++)
        {
            T sample = (couplers_[i] + couplers_[i+1]) / 2.0;
            P1 pi(0);
            P2 qi(0);
            // add together all contents that intersect the region from couplers_[i-1] to couplers_[i]
            for (std::size_t j=0; j<p.size(); j++)
            {
                auto car = p[j];
                if (car.lo <= sample && sample <= car.hi)
                {
                    pi = car.content;
                    break;
                }
            }
            // add together all contents that intersect the region from couplers_[i-1] to couplers_[i]
            for (std::size_t j=0; j<q.size(); j++)
            {
                auto car = q[j];
                if (car.lo <= sample && sample <= car.hi)
                {
                    qi = car.content;
                    break;
                }
            }
            contents.push_back(P(pi*qi));
        }
        return Train<T,P>(contents, couplers_);
    }

    template<typename T, 
        int P1lo, int P1hi, 
        int P2x>
    constexpr auto operator/(const PolynomialTrain<T,P1lo,P1hi>& p, const PolynomialTrain<T,P2x,P2x>& q)
    {
        using P1 = Polynomial<T,P1lo,P1hi>;
        using P2 = Polynomial<T,P2x,P2x>;
        using P = Polynomial<T,P1lo-P2x,P1hi-P2x>;
        const std::vector<T> couplers_ = couplers(p,q);
        std::vector<P> contents;
        for (std::size_t i=0; i<=couplers_.size(); i++)
        {
            T sample = (couplers_[i] + couplers_[i+1]) / 2.0;
            P1 pi(0);
            P2 qi(0);
            // add together all contents that intersect the region from couplers_[i-1] to couplers_[i]
            for (std::size_t j=0; j<p.size(); j++)
            {
                auto car = p[j];
                if (car.lo <= sample && sample <= car.hi)
                {
                    pi = car.content;
                    break;
                }
            }
            // add together all contents that intersect the region from couplers_[i-1] to couplers_[i]
            for (std::size_t j=0; j<q.size(); j++)
            {
                auto car = q[j];
                if (car.lo <= sample && sample <= car.hi)
                {
                    qi = car.content;
                    break;
                }
            }
            contents.push_back(P(pi/qi));
        }
        return Train<T,P>(contents, couplers_);
    }









    // OPERATIONS WITH POLYNOMIAL YARDS
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator+(const PolynomialTrain<T,P1lo,P1hi>& p, const PolynomialRailyard<T,P2lo,P2hi> q)
    {
        return p + train(q);
    }
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator-(const PolynomialTrain<T,P1lo,P1hi>& p, const PolynomialRailyard<T,P2lo,P2hi> q)
    {
        return p - train(q);
    }
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator*(const PolynomialTrain<T,P1lo,P1hi>& p, const PolynomialRailyard<T,P2lo,P2hi> q)
    {
        return p * train(q);
    }
    // template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    // constexpr auto operator/(const PolynomialTrain<T,P1lo,P1hi>& p, const PolynomialRailyard<T,P2lo,P2hi> q)
    // {
    //     return p / train(q);
    // }

    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator+(const PolynomialRailyard<T,P2lo,P2hi> q, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return train(q) + p;
    }
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator-(const PolynomialRailyard<T,P2lo,P2hi> q, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return train(q) - p;
    }
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator*(const PolynomialRailyard<T,P2lo,P2hi> q, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return train(q) * p;
    }
    template<typename T, int P1, int P2lo, int P2hi>
    constexpr auto operator/(const PolynomialRailyard<T,P2lo,P2hi> q, const PolynomialTrain<T,P1,P1>& p)
    {
        return train(q) / p;
    }






    // OPERATIONS WITH POLYNOMIAL CARS
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator+(const PolynomialTrain<T,P1lo,P1hi>& p, const PolynomialRailcar<T,P2lo,P2hi> q)
    {
        return p + PolynomialTrain<T,P2lo,P2hi>(q);
    }
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator-(const PolynomialTrain<T,P1lo,P1hi>& p, const PolynomialRailcar<T,P2lo,P2hi> q)
    {
        return p - PolynomialTrain<T,P2lo,P2hi>(q);
    }
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator*(const PolynomialTrain<T,P1lo,P1hi>& p, const PolynomialRailcar<T,P2lo,P2hi> q)
    {
        return p * PolynomialTrain<T,P2lo,P2hi>(q);
    }
    // template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    // constexpr auto operator/(const PolynomialTrain<T,P1lo,P1hi>& p, const PolynomialRailcar<T,P2lo,P2hi> q)
    // {
    //     return p / PolynomialTrain<T,P2lo,P2hi>(q);
    // }
    /*
    NOTE: we cannot support division by railcars of any kind.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 

    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator+(const PolynomialRailcar<T,P2lo,P2hi> q, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return PolynomialTrain<T,P2lo,P2hi>(q) + p;
    }
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator-(const PolynomialRailcar<T,P2lo,P2hi> q, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return PolynomialTrain<T,P2lo,P2hi>(q) - p;
    }
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator*(const PolynomialRailcar<T,P2lo,P2hi> q, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return PolynomialTrain<T,P2lo,P2hi>(q) * p;
    }
    template<typename T, int P1, int P2lo, int P2hi>
    constexpr auto operator/(const PolynomialRailcar<T,P2lo,P2hi> q, const PolynomialTrain<T,P1,P1>& p)
    {
        return PolynomialTrain<T,P2lo,P2hi>(q) / p;
    }





    // OPERATIONS WITH POLYNOMIALS
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator+(const PolynomialTrain<T,P1lo,P1hi>& p, const Polynomial<T,P2lo,P2hi> q)
    {
        return p + PolynomialTrain<T,P2lo,P2hi>(q);
    }
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator-(const PolynomialTrain<T,P1lo,P1hi>& p, const Polynomial<T,P2lo,P2hi> q)
    {
        return p - PolynomialTrain<T,P2lo,P2hi>(q);
    }
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator*(const PolynomialTrain<T,P1lo,P1hi>& p, const Polynomial<T,P2lo,P2hi> q)
    {
        return p * PolynomialTrain<T,P2lo,P2hi>(q);
    }
    template<typename T, int P1lo, int P1hi, int P2>
    constexpr auto operator/(const PolynomialTrain<T,P1lo,P1hi>& p, const Polynomial<T,P2,P2> q)
    {
        return p / PolynomialTrain<T,P2,P2>(q);
    }

    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator+(const Polynomial<T,P2lo,P2hi> q, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return PolynomialTrain<T,P2lo,P2hi>(q) + p;
    }
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator-(const Polynomial<T,P2lo,P2hi> q, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return PolynomialTrain<T,P2lo,P2hi>(q) - p;
    }
    template<typename T, int P1lo, int P1hi, int P2lo, int P2hi>
    constexpr auto operator*(const Polynomial<T,P2lo,P2hi> q, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return PolynomialTrain<T,P2lo,P2hi>(q) * p;
    }
    template<typename T, int P1, int P2lo, int P2hi>
    constexpr auto operator/(const Polynomial<T,P2lo,P2hi> q, const PolynomialTrain<T,P1,P1>& p)
    {
        return PolynomialTrain<T,P2lo,P2hi>(q) / p;
    }


    // OPERATIONS WITH SCALINGS
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator+(const PolynomialTrain<T,P1lo,P1hi>& p, const Scaling<T> f)
    {
        return p + Polynomial<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator-(const PolynomialTrain<T,P1lo,P1hi>& p, const Scaling<T> f)
    {
        return p - Polynomial<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator*(const PolynomialTrain<T,P1lo,P1hi>& p, const Scaling<T> f)
    {
        return p * Polynomial<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator/(const PolynomialTrain<T,P1lo,P1hi>& p, const Scaling<T> f)
    {
        return p / Polynomial<T,1,1>(f);
    }

    template<typename T, int P1lo, int P1hi>
    constexpr auto operator+(const Scaling<T> f, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return Polynomial<T,1,1>(f) + p;
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator-(const Scaling<T> f, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return Polynomial<T,1,1>(f) - p;
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator*(const Scaling<T> f, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return Polynomial<T,1,1>(f) * p;
    }
    // template<typename T, int P1lo, int P1hi>
    // constexpr auto operator/(const Scaling<T> f, const PolynomialTrain<T,P1lo,P1hi>& p)
    // {
    //     return Polynomial<T,1,1>(f) / p;
    // }


    // OPERATIONS WITH SHIFTINGS
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator+(const PolynomialTrain<T,P1lo,P1hi>& p, const Shifting<T> f)
    {
        return p + Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator-(const PolynomialTrain<T,P1lo,P1hi>& p, const Shifting<T> f)
    {
        return p - Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator*(const PolynomialTrain<T,P1lo,P1hi>& p, const Shifting<T> f)
    {
        return p * Polynomial<T,0,1>(f);
    }
    // template<typename T, int P1lo, int P1hi>
    // constexpr auto operator/(const PolynomialTrain<T,P1lo,P1hi>& p, const Shifting<T> f)
    // {
    //     return p / Polynomial<T,0,1>(f);
    // }

    template<typename T, int P1lo, int P1hi>
    constexpr auto operator+(const Shifting<T> f, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return Polynomial<T,0,1>(f) + p;
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator-(const Shifting<T> f, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return Polynomial<T,0,1>(f) - p;
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator*(const Shifting<T> f, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return Polynomial<T,0,1>(f) * p;
    }
    // template<typename T, int P1lo, int P1hi>
    // constexpr auto operator/(const Shifting<T> f, const PolynomialTrain<T,P1lo,P1hi>& p)
    // {
    //     return Polynomial<T,0,1>(f) / p;
    // }



    // OPERATIONS WITH IDENTITY
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator+(const PolynomialTrain<T,P1lo,P1hi>& p, const Identity<T> e)
    {
        return p + Polynomial<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator-(const PolynomialTrain<T,P1lo,P1hi>& p, const Identity<T> e)
    {
        return p - Polynomial<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator*(const PolynomialTrain<T,P1lo,P1hi>& p, const Identity<T> e)
    {
        return p * Polynomial<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator/(const PolynomialTrain<T,P1lo,P1hi>& p, const Identity<T> e)
    {
        return p / Polynomial<T,1,1>(e);
    }

    template<typename T, int P1lo, int P1hi>
    constexpr auto operator+(const Identity<T> e, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return Polynomial<T,1,1>(e) + p;
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator-(const Identity<T> e, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return Polynomial<T,1,1>(e) - p;
    }
    template<typename T, int P1lo, int P1hi>
    constexpr auto operator*(const Identity<T> e, const PolynomialTrain<T,P1lo,P1hi>& p)
    {
        return Polynomial<T,1,1>(e) * p;
    }
    // template<typename T, int P1lo, int P1hi>
    // constexpr auto operator/(const Identity<T> e, const PolynomialTrain<T,P1lo,P1hi>& p)
    // {
    //     return Polynomial<T,1,1>(e) / p;
    // }


    // NOTE: already implemented in generic case for Trains

    // // OPERATIONS WITH SCALARS
    // template<typename T, int P1lo, int P1hi>
    // constexpr auto operator+(const PolynomialTrain<T,P1lo,P1hi>& p, const T k)
    // {
    //     return p + Polynomial<T,0,0>(k);
    // }
    // template<typename T, int P1lo, int P1hi>
    // constexpr auto operator-(const PolynomialTrain<T,P1lo,P1hi>& p, const T k)
    // {
    //     return p - Polynomial<T,0,0>(k);
    // }
    // template<typename T, int P1lo, int P1hi>
    // constexpr auto operator*(const PolynomialTrain<T,P1lo,P1hi>& p, const T k)
    // {
    //     return p * Polynomial<T,0,0>(k);
    // }
    // template<typename T, int P1lo, int P1hi>
    // constexpr auto operator/(const PolynomialTrain<T,P1lo,P1hi>& p, const T k)
    // {
    //     return p / Polynomial<T,0,0>(k);
    // }

    // template<typename T, int P1lo, int P1hi>
    // constexpr auto operator+(const T k, const PolynomialTrain<T,P1lo,P1hi>& p)
    // {
    //     return Polynomial<T,0,0>(k) + p;
    // }
    // template<typename T, int P1lo, int P1hi>
    // constexpr auto operator-(const T k, const PolynomialTrain<T,P1lo,P1hi>& p)
    // {
    //     return Polynomial<T,0,0>(k) - p;
    // }
    // template<typename T, int P1lo, int P1hi>
    // constexpr auto operator*(const T k, const PolynomialTrain<T,P1lo,P1hi>& p)
    // {
    //     return Polynomial<T,0,0>(k) * p;
    // }
    // // template<typename T, int P1lo, int P1hi>
    // // constexpr auto operator/(const T k, const PolynomialTrain<T,P1lo,P1hi>& p)
    // // {
    // //     return Polynomial<T,0,0>(k) / p;
    // // }








    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto compose(const PolynomialTrain<T,Plo,Phi>& p, const PolynomialTrain<T,Qlo,Qhi> q)
    {
        using F = Polynomial<T,Plo,Phi>;
        using G = Polynomial<T,Qlo,Qhi>;
        using FG = Polynomial<T,std::min(Plo*Qlo,Phi*Qhi),std::max(Plo*Qlo,Phi*Qhi)>;
        Railyard<T,FG> y;
        for (std::size_t i=0; i<p.contents.size(); i++)
        {
            for (std::size_t j=0; i<q.contents.size(); i++)
            {
                y += compose(
                    Railcar<T,F>(p.lo(i), p.hi(i), p.contents[i]), 
                    Railcar<T,G>(q.lo(j), q.hi(j), q.contents[j]));
            }
        }
        return y;
    }


    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto compose(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi> q)
    {
        return compose(train(p), train(q));
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto compose(const PolynomialTrain<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi> q)
    {
        return compose(p, train(q));
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto compose(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialTrain<T,Qlo,Qhi> q)
    {
        return compose(train(p), q);
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

    /*
    NOTE: 
    Definite integration of trains can be handled generically, so it is implemented in Train.hpp.
    Indefinite integration does not exist on trains of any kind - 
    since trains do not permit gaps between railcars,
    there must be a railcar that stretches from -∞ to some lower bound, 
    so unless some bound is indicated for the integral, 
    the lower bound must be the lower bound that can be represented by the data type T,
    and before integration even reaches the lower bound 
    the value for the integral will likely become too large to be represented by the data type T.
    */



    template<int N, typename T, int Plo, int Phi>
    constexpr PolynomialTrain<T,Plo*N,Phi*N> pow(const PolynomialTrain<T,Plo,Phi>& p){
        using P = Polynomial<T,Plo*N,Phi*N>;
        std::vector<P> contents;
        for (auto pi : p.contents)
        {
            contents.push_back(pow<N>(pi));
        }
        Train<T,P> result(contents, p.couplers);
        return result;
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
        return std::sqrt(std::max(T(0), integral(pow<2>(p-q), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const PolynomialTrain<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return distance((p), train(q), lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialTrain<T,Qlo,Qhi>& q, const T lo, const T hi)
    {
        return distance(train(p), (q), lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const PolynomialTrain<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi> q, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(pow<2>(p-q), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(const Polynomial<T,Plo,Phi> p, const PolynomialTrain<T,Qlo,Qhi>& q, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(pow<2>(p-q), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi>
    constexpr T distance(const PolynomialTrain<T,Plo,Phi>& p, const T k, const T lo, const T hi)
    {
        return std::sqrt(std::max(T(0), integral(pow<2>(p-k), lo, hi))) / (hi-lo);
    }
    template<typename T, int Plo, int Phi>
    constexpr T distance(const T k, const PolynomialTrain<T,Plo, Phi>& p, const T lo, const T hi)
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

    // a convenience function that converts an arbitrary function to a train
    template<typename T, typename F, typename DFDX>
    PolynomialTrain<T,0,3> cubic_spline(const F& f, const DFDX& dfdx, std::vector<T>& couplers)
    {
        std::vector<Polynomial<T,0,3>> contents;
        for (std::size_t i=1; i<couplers.size(); i++)
        {
            T a(couplers[i-1]);
            T b(couplers[i]);
            contents.push_back(
                cubic_spline(
                    a,       b,
                    f(a),    f(b),
                    dfdx(a), dfdx(b)));
            contents.push_back(f);
        }
        return PolynomialTrain<T,0,3>(contents, couplers);
    }

}
