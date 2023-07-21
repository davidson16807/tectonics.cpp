#pragma once

namespace math {

    /* 
    `Exponentiated<T,F>` is a trivial class that represents the function f∘g(x)=argument(1-x/k), 
    where argument is a template function object of type F.
    `Exponentiated<T,F>` was needed to provide a reoccuring pattern in chemistry 
    when dealing with equations of reduced temperature and pressure.
    */
    template<typename T, typename F>
    struct Exponentiated {
        F argument;
        constexpr explicit Exponentiated<T,F>(const F argument):
            argument(argument)
        {}
        constexpr Exponentiated<T,F>(const Exponentiated<T,F>& expf):
            argument(expf.argument)
        {}
        // zero constructor
        constexpr Exponentiated<T,F>():
            argument()
        {}
        // constant constructor
        constexpr Exponentiated<T,F>(T k):
            argument(std::log(k))
        {}
        constexpr T operator()(const T x) const
        {
            return std::exp(argument(x));
        }

        Exponentiated<T,F>& operator*=(const T k)
        {
            argument += k;
            return *this;
        }

        Exponentiated<T,F>& operator/=(const T k)
        {
            argument -= k;
            return *this;
        }
    };

    // operators with reals that are closed under Exponentiated<T,F> relations
    template<typename T, typename F>
    constexpr Exponentiated<T,F> operator*(const Exponentiated<T,F>& expf, const T k)
    {
        return Exponentiated<T,F>(expf.argument+k);
    }
    template<typename T, typename F>
    constexpr Exponentiated<T,F> operator*(const T k, const Exponentiated<T,F>& expf)
    {
        return Exponentiated<T,F>(k+expf.argument);
    }
    template<typename T, typename F>
    constexpr Exponentiated<T,F> operator/(const Exponentiated<T,F>& expf, const T k)
    {
        return Exponentiated<T,F>(expf.argument-k);
    }
    template<typename T, typename F>
    constexpr Exponentiated<T,F> operator/(const T k, const Exponentiated<T,F>& expf)
    {
        return Exponentiated<T,F>(k-expf.argument);
    }

    // operators with reals that are closed under Exponentiated<T,F> relations
    template<typename T, typename F>
    constexpr Exponentiated<T,F> operator*(const Exponentiated<T,F>& expf, const Exponentiated<T,F>& expg)
    {
        return Exponentiated<T,F>(expf.argument+expg.argument);
    }
    template<typename T, typename F>
    constexpr Exponentiated<T,F> operator/(const Exponentiated<T,F>& expf, const Exponentiated<T,F>& expg)
    {
        return Exponentiated<T,F>(expf.argument-expg.argument);
    }

    /*
    Given functions exp and f, return the composite function exp∘f.
    */
    template<typename T, typename F>
    constexpr Exponentiated<T,F> compose(const Exponentiated<T,math::Identity<T>>& g, const F f)
    {
        return Exponentiated<T,F>(f);
    }

    /*
    Given functions exp∘f and g, return the composite function exp∘f∘g.
    */
    template<typename T, typename F, typename G, typename FG>
    constexpr Exponentiated<T,FG> compose(const Exponentiated<T,F>& expf, const G g)
    {
        return Exponentiated<T,FG>(compose(expf.argument, g));
    }

    /*
    Given functions g and exp∘f, return the composite function g∘exp∘f.
    */
    template<typename T, typename F>
    constexpr Exponentiated<T,F> compose(const math::Scaling<T> g, const Exponentiated<T,F>& expf)
    {
        return Exponentiated<T,F>(expf.argument + std::log(g.factor));
    }


    template<typename T, typename F>
    constexpr Exponentiated<T,F> exp(const F& f)
    {
        return Exponentiated<T,F>(f);
    }
}