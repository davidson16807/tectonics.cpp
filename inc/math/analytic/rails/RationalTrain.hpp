#pragma once

// in-house libraries
#include "../Identity.hpp"
#include "../Scaling.hpp"
#include "../Shifting.hpp"
#include "../Rational.hpp"

namespace analytic {

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    using RationalRailyard = Railyard<T,Rational<T,Plo,Phi,Qlo,Qhi>>;


    // operations that are closed under trains

    template<typename T, 
        int P1lo, int P1hi, int Q1lo, int Q1hi, 
        int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator+(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        using R1 = Rational<T,P1lo,P1hi,Q1lo,Q1hi>;
        using R2 = Rational<T,P2lo,P2hi,Q2lo,Q2hi>;
        using R = Rational<T,std::min(P1lo,P2lo),std::max(P1hi,P2hi),std::min(Q1lo,Q2lo),std::max(Q1hi,Q2hi)>;
        const std::vector<T> couplers_ = couplers(r,s);
        std::vector<R> contents;
        for (std::size_t i=0; i<couplers_.size(); i++)
        {
            T sample = (couplers_[i-1] + couplers_[i]) / 2.0;
            R1 ri;
            R2 si;
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
            contents.push_back(ri+si);
        }
        return Train<T,R>(contents, couplers_);
    }

    template<typename T, 
        int P1lo, int P1hi, int Q1lo, int Q1hi, 
        int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        using R1 = Rational<T,P1lo,P1hi,Q1lo,Q1hi>;
        using R2 = Rational<T,P2lo,P2hi,Q2lo,Q2hi>;
        using R = Rational<T,std::min(P1lo,P2lo),std::max(P1hi,P2hi),std::min(Q1lo,Q2lo),std::max(Q1hi,Q2hi)>;
        const std::vector<T> couplers_ = couplers(r,s);
        std::vector<R> contents;
        for (std::size_t i=0; i<couplers_.size(); i++)
        {
            T sample = (couplers_[i-1] + couplers_[i]) / 2.0;
            R1 ri;
            R2 si;
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
            contents.push_back(ri-si);
        }
        return Train<T,R>(contents, couplers_);
    }

    template<typename T, 
        int P1lo, int P1hi, int Q1lo, int Q1hi, 
        int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        using R1 = Rational<T,P1lo,P1hi,Q1lo,Q1hi>;
        using R2 = Rational<T,P2lo,P2hi,Q2lo,Q2hi>;
        using R = Rational<T,P1lo+P2lo,P1hi+P2hi,Q1lo+Q2lo,Q1hi+Q2hi>;
        const std::vector<T> couplers_ = couplers(r,s);
        std::vector<R> contents;
        for (std::size_t i=0; i<couplers_.size(); i++)
        {
            T sample = (couplers_[i-1] + couplers_[i]) / 2.0;
            R1 ri;
            R2 si;
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
            contents.push_back(ri*si);
        }
        return Train<T,R>(contents, couplers_);
    }

    template<typename T, 
        int P1lo, int P1hi, int Q1lo, int Q1hi, 
        int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator/(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        using R1 = Rational<T,P1lo,P1hi,Q1lo,Q1hi>;
        using R2 = Rational<T,P2lo,P2hi,Q2lo,Q2hi>;
        using R = Rational<T,P1lo+Q2lo,P1hi+Q2hi,Q1lo+P2lo,Q1hi+P2hi>;
        const std::vector<T> couplers_ = couplers(r,s);
        std::vector<R> contents;
        for (std::size_t i=0; i<couplers_.size(); i++)
        {
            T sample = (couplers_[i-1] + couplers_[i]) / 2.0;
            R1 ri;
            R2 si;
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
            contents.push_back(ri/si);
        }
        return Train<T,R>(contents, couplers_);
    }




    // OPERATIONS BETWEEN POLYNOMIAL TRAINS THAT PRODUCE RATIONAL TRAINS
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const PolynomialTrain<T,Plo,Phi>& p, const PolynomialTrain<T,Qlo,Qhi>& q)
    {
        using P = Polynomial<T,Plo,Phi>;
        using Q = Polynomial<T,Qlo,Qhi>;
        using R = Rational<T,Plo,Phi,Qlo,Qhi>;
        const std::vector<T> couplers_ = couplers(p,q);
        std::vector<R> contents;
        for (std::size_t i=0; i<couplers_.size(); i++)
        {
            P pi;
            Q qi;
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
            contents.push_back(pi/qi);
        }
        return Train<T,R>(contents, couplers_);
    }


    // OPERATIONS WITH POLYNOMIAL TRAINS THAT PRODUCE RATIONAL TRAINS AS OUTPUT
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const PolynomialTrain<T,P1lo,P1hi>& r, const PolynomialRailyard<T,P2lo,P2hi> p)
    {
        return r / train(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const PolynomialRailyard<T,P2lo,P2hi> p, const PolynomialTrain<T,P1lo,P1hi>& r)
    {
        return train(p) / r;
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    // constexpr auto operator/(const PolynomialTrain<T,P1lo,P1hi>& r, const PolynomialRailcar<T,P2lo,P2hi> p)
    // {
    //     return r * PolynomialTrain<T,P2lo,P2hi>(p);
    // }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const PolynomialRailcar<T,P2lo,P2hi> p, const PolynomialTrain<T,P1lo,P1hi>& r)
    {
        return PolynomialTrain<T,P2lo,P2hi>(p) / r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const PolynomialTrain<T,P1lo,P1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return r * Polynomial<T,P2lo,P2hi>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const Polynomial<T,P2lo,P2hi> p, const PolynomialTrain<T,P1lo,P1hi>& r)
    {
        return Polynomial<T,P2lo,P2hi>(p) / r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Scaling<T> f, const PolynomialTrain<T,P1lo,P1hi>& r)
    {
        return Polynomial<T,1,1>(f) / r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const PolynomialTrain<T,P1lo,P1hi>& r, const Shifting<T> f)
    {
        return r / Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Shifting<T> f, const PolynomialTrain<T,P1lo,P1hi>& r)
    {
        return Polynomial<T,0,1>(f) / r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Identity<T> e, const PolynomialTrain<T,P1lo,P1hi>& r)
    {
        return Polynomial<T,1,1>(e) / r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const T k, const PolynomialTrain<T,P1lo,P1hi>& r)
    {
        return Polynomial<T,0,0>(k) / r;
    }

    // OPERATIONS WITH POLYNOMIAL RAILYARDS THAT PRODUCE RATIONAL TRAINS AS OUTPUT
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const PolynomialRailyard<T,P1lo,P1hi>& r, const PolynomialRailyard<T,P2lo,P2hi> p)
    {
        return train(r) / train(p);
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    // constexpr auto operator/(const PolynomialRailyard<T,P1lo,P1hi>& r, const PolynomialRailcar<T,P2lo,P2hi> p)
    // {
    //     return r / PolynomialRailyard<T,P2lo,P2hi>(p);
    // }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const PolynomialRailcar<T,P2lo,P2hi> p, const PolynomialRailyard<T,P1lo,P1hi>& r)
    {
        return PolynomialTrain<T,P2lo,P2hi>(p) / train(r);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const PolynomialRailyard<T,P1lo,P1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return train(r) / Polynomial<T,P2lo,P2hi>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const Polynomial<T,P2lo,P2hi> p, const PolynomialRailyard<T,P1lo,P1hi>& r)
    {
        return Polynomial<T,P2lo,P2hi>(p) / train(r);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Scaling<T> f, const PolynomialRailyard<T,P1lo,P1hi>& r)
    {
        return Polynomial<T,1,1>(f) / train(r);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const PolynomialRailyard<T,P1lo,P1hi>& r, const Shifting<T> f)
    {
        return train(r) / Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Shifting<T> f, const PolynomialRailyard<T,P1lo,P1hi>& r)
    {
        return Polynomial<T,0,1>(f) / train(r);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Identity<T> e, const PolynomialRailyard<T,P1lo,P1hi>& r)
    {
        return Polynomial<T,1,1>(e) / train(r);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const T k, const PolynomialRailyard<T,P1lo,P1hi>& r)
    {
        return Polynomial<T,0,0>(k) / train(r);
    }






    // OPERATIONS WITH RATIONAL YARDS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator+(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r + RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r - RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r * RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    // constexpr auto operator/(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi> s)
    // {
    //     return s / RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    // }
    /*
    NOTE: we cannot support division by rational railyards.
    This is because we cannot distibute division across the sum of rationals in the denominator.
    We can support division if we were able to convert railyards to trains, however if we convert a rational railyard to a train,
    then the degree of the rationals in the resulting train cannot be known at compile time,
    since adding two rationals produces a rational of a different degree
    */ 

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator+(const RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s) * r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator/(const RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s) / r;
    }



    // OPERATIONS WITH POLYNOMIAL YARDS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailcar<T,P2lo,P2hi> p)
    {
        return r + RationalRailcar<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailcar<T,P2lo,P2hi> p)
    {
        return r - RationalRailcar<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailcar<T,P2lo,P2hi> p)
    {
        return r * RationalRailcar<T,P2lo,P2hi,0,0>(p);
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    // constexpr auto operator/(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailcar<T,P2lo,P2hi> p)
    // {
    //     return r * RationalRailcar<T,P2lo,P2hi,0,0>(p);
    // }
    /*
    NOTE: we cannot support division by rational railyards.
    This is because we cannot distibute division across the sum of rationals in the denominator.
    We can support division if we were able to convert railyards to trains, however if we convert a rational railyard to a train,
    then the degree of the rationals in the resulting train cannot be known at compile time,
    since adding two rationals produces a rational of a different degree
    */ 

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const PolynomialRailcar<T,P2lo,P2hi> p, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailcar<T,P2lo,P2hi,0,0>(p) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const PolynomialRailcar<T,P2lo,P2hi> p, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailcar<T,P2lo,P2hi,0,0>(p) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const PolynomialRailcar<T,P2lo,P2hi> p, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailcar<T,P2lo,P2hi,0,0>(p) * r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const PolynomialRailcar<T,P2lo,P2hi> p, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailcar<T,P2lo,P2hi,0,0>(p) / r;
    }



    // OPERATIONS WITH RATIONAL CARS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator+(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r + RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r - RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r * RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    // constexpr auto operator/(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s)
    // {
    //     return s / RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    // }
    /*
    NOTE: we cannot support division by railcars of any kind.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator+(const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s) * r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator/(const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s) / r;
    }



    // OPERATIONS WITH POLYNOMIAL CARS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailcar<T,P2lo,P2hi> p)
    {
        return r + Rational<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailcar<T,P2lo,P2hi> p)
    {
        return r - Rational<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailcar<T,P2lo,P2hi> p)
    {
        return r * Rational<T,P2lo,P2hi,0,0>(p);
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    // constexpr auto operator/(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailcar<T,P2lo,P2hi> p)
    // {
    //     return r * Rational<T,P2lo,P2hi,0,0>(p);
    // }
    /*
    NOTE: we cannot support division by railcars of any kind.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const PolynomialRailcar<T,P2lo,P2hi> p, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Rational<T,P2lo,P2hi,0,0>(p) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const PolynomialRailcar<T,P2lo,P2hi> p, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Rational<T,P2lo,P2hi,0,0>(p) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const PolynomialRailcar<T,P2lo,P2hi> p, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Rational<T,P2lo,P2hi,0,0>(p) * r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const PolynomialRailcar<T,P2lo,P2hi> p, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Rational<T,P2lo,P2hi,0,0>(p) / r;
    }



    // OPERATIONS WITH RATIONALS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator+(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r + RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r - RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r * RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator/(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return s / RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator+(const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s) * r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator/(const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalTrain<T,P2lo,P2hi,Q2lo,Q2hi>(s) / r;
    }



    // OPERATIONS WITH POLYNOMIALS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return r + Rational<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return r - Rational<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return r * Rational<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return r * Rational<T,P2lo,P2hi,0,0>(p);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const Polynomial<T,P2lo,P2hi> p, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Rational<T,P2lo,P2hi,0,0>(p) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const Polynomial<T,P2lo,P2hi> p, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Rational<T,P2lo,P2hi,0,0>(p) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const Polynomial<T,P2lo,P2hi> p, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Rational<T,P2lo,P2hi,0,0>(p) * r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const Polynomial<T,P2lo,P2hi> p, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Rational<T,P2lo,P2hi,0,0>(p) / r;
    }


    // OPERATIONS WITH SCALINGS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling<T> f)
    {
        return r + Polynomial<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling<T> f)
    {
        return r - Polynomial<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling<T> f)
    {
        return r * Polynomial<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling<T> f)
    {
        return r / Polynomial<T,1,1>(f);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Scaling<T> f, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(f) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Scaling<T> f, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(f) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Scaling<T> f, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(f) * r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Scaling<T> f, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(f) / r;
    }


    // OPERATIONS WITH SHIFTINGS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting<T> f)
    {
        return r + Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting<T> f)
    {
        return r - Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting<T> f)
    {
        return r * Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting<T> f)
    {
        return r / Polynomial<T,0,1>(f);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Shifting<T> f, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,1>(f) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Shifting<T> f, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,1>(f) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Shifting<T> f, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,1>(f) * r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Shifting<T> f, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,1>(f) / r;
    }



    // OPERATIONS WITH IDENTITY
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Identity<T> e)
    {
        return r + Polynomial<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Identity<T> e)
    {
        return r - Polynomial<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Identity<T> e)
    {
        return r * Polynomial<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Identity<T> e)
    {
        return r / Polynomial<T,1,1>(e);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Identity<T> e, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(e) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Identity<T> e, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(e) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Identity<T> e, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(e) * r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Identity<T> e, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(e) / r;
    }



    // NOTE: already implemented in generic case for Trains
    
    // // OPERATIONS WITH SCALARS
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    // constexpr auto operator+(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const T k)
    // {
    //     return r + Polynomial<T,0,0>(k);
    // }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    // constexpr auto operator-(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const T k)
    // {
    //     return r - Polynomial<T,0,0>(k);
    // }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    // constexpr auto operator*(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const T k)
    // {
    //     return r * Polynomial<T,0,0>(k);
    // }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    // constexpr auto operator/(const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r, const T k)
    // {
    //     return r / Polynomial<T,0,0>(k);
    // }

    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    // constexpr auto operator+(const T k, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    // {
    //     return Polynomial<T,0,0>(k) + r;
    // }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    // constexpr auto operator-(const T k, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    // {
    //     return Polynomial<T,0,0>(k) - r;
    // }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    // constexpr auto operator*(const T k, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    // {
    //     return Polynomial<T,0,0>(k) * r;
    // }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    // constexpr auto operator/(const T k, const RationalTrain<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    // {
    //     return Polynomial<T,0,0>(k) / r;
    // }









    /* 
    NOTE: we avoid expressing derivatives and integrals as methods,
    since not all equations are closed under derivatives and integrals,
    and such methods would complicate the order with which classes must be declared
    */
    template<typename T>
    constexpr T derivative(const RationalTrain<T,0,1,0,0> r) 
    {
        return r.p[0];
    }
    template<typename T>
    constexpr T derivative(const RationalTrain<T,1,1,0,0> r) 
    {
        return T(0);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    auto derivative(const RationalTrain<T,Plo,Phi,Qlo,Qhi>& r)
    {
        return (r.p.derivative()*r.q + r.q.derivative()*r.p) / (r.q*r.q);
    }





    template<typename T, int Plo, int Phi, int Qlo, int Qhi, typename F>
    constexpr Rational<T,Plo,Phi,Qlo,Qhi> compose(const Rational<T,Plo,Phi,Qlo,Qhi>& r, const Identity<T> e)
    {
        return r;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, typename F>
    constexpr Rational<T,Plo,Phi,Qlo,Qhi> compose(const Identity<T> e, const Rational<T,Plo,Phi,Qlo,Qhi>& r)
    {
        return r;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, typename F>
    constexpr Rational<T,Plo,Phi,Qlo,Qhi> compose(const Rational<T,Plo,Phi,Qlo,Qhi>& r, const Shifting<T> f)
    {
        using R = Rational<T,0,Phi,0,Qhi>;
        Railyard<T,R> y;
        for (auto ri: r.cars)
        {
            y += compose(ri,f);
        }
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, typename F>
    constexpr Rational<T,Plo,Phi,Qlo,Qhi> compose(const Rational<T,Plo,Phi,Qlo,Qhi>& r, const Scaling<T> f)
    {
        using R = Rational<T,Plo,Phi,Qlo,Qhi>;
        Railyard<T,R> y;
        for (auto ri: r.cars)
        {
            y += compose(ri,f);
        }
        return y;
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr Rational<T,P1lo,P1hi,Q1lo,Q1hi> compose(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        using R = Rational<T,P1lo*P2lo,P1hi*P2hi,Q1lo+P2lo,Q1hi+P2hi>;
        Railyard<T,R> y;
        for (auto ri: r.cars)
        {
            y += compose(ri,p);
        }
        return y;
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr Rational<T,P1lo,P1hi,Q1lo,Q1hi> compose(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        using R = Rational<T,P1lo*P2lo,P1hi*P2hi,Q1lo*Q2lo,Q1hi*Q2hi>;
        Railyard<T,R> y;
        for (auto ri: r.cars)
        {
            y += compose(ri,s);
        }
        return y;
    }

}