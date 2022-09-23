#pragma once

namespace math {

    /* 
    `ScaledComplement<T,F>` is a trivial class that represents the function f∘g(x)=f(1-x/k), 
    where f is a template function object of type F.
    `ScaledComplement<T,F>` was needed to provide a reoccuring pattern in chemistry 
    when dealing with equations of reduced temperature and pressure.
    */
    template<typename T, typename F>
    struct ScaledComplement {
        F f;
        T scale;
        constexpr explicit ScaledComplement<T,F>(const T scale, const F f):
            f(f),
            scale(scale)
        {}
        constexpr ScaledComplement<T,F>(const ScaledComplement<T,F>& fg):
            f(fg.f),
            scale(fg.scale)
        {}
        // zero constructor
        constexpr ScaledComplement<T,F>():
            f(),
            scale()
        {}
        // constant constructor
        constexpr ScaledComplement<T,F>(T k):
            f(k),
            scale(T(1))
        {}
        constexpr T operator()(const T x) const
        {
            return f(T(1)-x/scale);
        }

        ScaledComplement<T,F>& operator+=(const T k)
        {
            f += k;
            return *this;
        }

        ScaledComplement<T,F>& operator-=(const T k)
        {
            f -= k;
            return *this;
        }

        ScaledComplement<T,F>& operator*=(const T k)
        {
            f *= k;
            return *this;
        }

        ScaledComplement<T,F>& operator/=(const T k)
        {
            f /= k;
            return *this;
        }
    };

    // operators with reals that are closed under ScaledComplement<T,F> relations
    template<typename T, typename F>
    constexpr ScaledComplement<T,F> operator+(const ScaledComplement<T,F>& fg, const T k)
    {
        ScaledComplement<T,F> y(fg);
        y.f += k;
        return y;
    }
    template<typename T, typename F>
    constexpr ScaledComplement<T,F> operator+(const T k, const ScaledComplement<T,F>& fg)
    {
        ScaledComplement<T,F> y(fg);
        y.f += k;
        return y;
    }
    template<typename T, typename F>
    constexpr ScaledComplement<T,F> operator-(const ScaledComplement<T,F>& fg, const T k)
    {
        ScaledComplement<T,F> y(fg);
        y.f -= k;
        return y;
    }
    template<typename T, typename F>
    constexpr ScaledComplement<T,F> operator*(const ScaledComplement<T,F>& fg, const T k)
    {
        ScaledComplement<T,F> y(fg);
        y.f *= k;
        return y;
    }
    template<typename T, typename F>
    constexpr ScaledComplement<T,F> operator*(const T k, const ScaledComplement<T,F>& fg)
    {
        ScaledComplement<T,F> y(fg);
        y.f *= k;
        return y;
    }
    template<typename T, typename F>
    constexpr ScaledComplement<T,F> operator/(const ScaledComplement<T,F>& fg, const T k)
    {
        ScaledComplement<T,F> y(fg);
        y.f /= k;
        return y;
    }
    template<typename T, typename F>
    constexpr ScaledComplement<T,F> operator-(const ScaledComplement<T,F>& fg)
    {
        ScaledComplement<T,F> y(fg);
        y.f *= -1;
        return y;
    }

    /*
    Given functions f and g, return the composite function f∘g.
    */
    template<typename T, typename F>
    constexpr ScaledComplement<T,F> compose(const F f, const ScaledComplement<T,math::Identity<T>>& g)
    {
        return ScaledComplement<T,F>(g.scale, f);
    }

    /*
    Given functions f∘g and h, return the composite function f∘g∘h.
    */
    template<typename T, typename F>
    constexpr ScaledComplement<T,F> compose(const ScaledComplement<T,F>& fg, const math::Scaling<T> h)
    {
        return ScaledComplement<T,F>(fg.scale/h.factor, fg.f);
    }

    /*
    Given functions f∘g and h, return the composite function h∘f∘g.
    */
    template<typename T, typename F, typename H>
    constexpr ScaledComplement<T,F> compose(const H h, const ScaledComplement<T,F>& fg)
    {
        return ScaledComplement<T,F>(fg.scale, compose(h, fg.f));
    }

}