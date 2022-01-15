#pragma once

// std libraries
#include <optional>          // optional

// in-house libraries
#include <units/si.hpp>

#include <models/compound/field/state/StateParameters.hpp>

namespace compound { 
namespace relation {

    /*
    `OptionalStateRelation` expresses a relation that may not necessarily be known 
    between pressure, temperature, and the property of a compound.
    It provides a convenience wrapper for dealing with `*Relation` instances 
    that have been wrapped inside a `std::optional<>`.

    `OptionalStateRelation` requires that the `F1` class that it wraps must 
    behave like a `*Relation`, namely that it have a `value_type` attribute 
    that is publically accessible and represents the type of compound property 
    being measured.
    */
    template<typename F1>
    class OptionalStateRelation
    {
        std::optional<F1> entry;
    public:
        using value_type = F1;
        using parameter_type = field::StateParameters;

        constexpr OptionalStateRelation(const F1 value)
        : entry(std::optional<F1>(value))
        {
        }

        constexpr OptionalStateRelation(const typename F1::value_type value)
        : entry(std::optional<F1>(F1(value)))
        {
        }

        constexpr OptionalStateRelation(const std::monostate entry)
        : entry(std::optional<F1>())
        {

        }

        constexpr OptionalStateRelation()
        : entry(std::optional<F1>())
        {

        }

        constexpr OptionalStateRelation<F1>& operator=(const F1 other)
        {
            entry = std::optional<F1>(other);
            return *this;
        }

        constexpr OptionalStateRelation<F1>& operator=(const std::monostate other)
        {
            entry = std::optional<F1>();
            return *this;
        }

        constexpr std::optional<typename F1::value_type> operator()(const si::pressure p, const si::temperature T) const
        {
            return entry.has_value()? entry.value()(p,T) : std::optional<typename F1::value_type>();
        }

        constexpr std::optional<typename F1::value_type> operator()(const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T) const
        {
            return entry.has_value()? entry.value()(p,T) : std::optional<typename F1::value_type>();
        }

        constexpr std::optional<typename F1::value_type> operator()(const field::StateParameters parameters) const
        {
            return entry.has_value()? entry.value()(parameters.pressure, parameters.temperature) : std::optional<typename F1::value_type>();
        }

        constexpr F1 complete(const F1 fallback) const 
        {
            return entry.has_value()? entry.value() : fallback;
        }

        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        constexpr OptionalStateRelation<F1> value_or(const OptionalStateRelation<F1> other) const
        {
            return entry.has_value()? *this : other;
        }

        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameter `a`
        */
        template<typename F2>
        constexpr OptionalStateRelation<F1> value_or(
            const std::function<F1(F2)> f, 
            const OptionalStateRelation<F2> a) const
        {
            if(has_value()) // no substitute needed
            {
                return *this;
            }
            else if(!a.has_value()) // any monostates
            {
                return std::optional<F1>();
            }
            else
            {
                return f(a.entry.value());
            }
        }

        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename F2, typename F3>
        constexpr OptionalStateRelation<F1> value_or(
            const std::function<F1(F2, F3)> f, 
            const OptionalStateRelation<F2> a, 
            const OptionalStateRelation<F3> b) const
        {
            if(has_value()) // no substitute needed
            {
                return *this;
            }
            else if(!a.has_value() || !b.has_value()) // any monostates
            {
                return std::monostate();
            }
            else
            {
                return f(a.entry.value(), b.entry.value());
            }
        }

        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename F2, typename F3, typename F4>
        constexpr OptionalStateRelation<F1> value_or(
            const std::function<F1(F2, F3, F4)> f, 
            const OptionalStateRelation<F2> a, 
            const OptionalStateRelation<F3> b, 
            const OptionalStateRelation<F4> c) const
        {
            if(has_value())
            {
                return *this;
            }
            else if(!a.has_value() || !b.has_value() || !c.has_value()) // any monostates
            {
                return std::monostate();
            }
            else
            {
                return f(a.entry.value(), b.entry.value(), c.entry.value());
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename F2, typename F3, typename F4, typename F5>
        constexpr OptionalStateRelation<F1> value_or(
            const std::function<F1(F2, F3, F4, const F5)> f, 
            const OptionalStateRelation<F2> a, 
            const OptionalStateRelation<F3> b, 
            const OptionalStateRelation<F4> c, 
            const OptionalStateRelation<F5> d) const
        {
            if(has_value())
            {
                return *this;
            }
            else if(!a.has_value() || !b.has_value() || !c.has_value() || !d.has_value()) // any monostates
            {
                return std::monostate();
            }
            else
            {
                return f(a.entry.value(), b.entry.value(), c.entry.value(), d.entry.value());
            }
        }

        /*
		Return whether a entry exists within the field
        */
        constexpr bool has_value() const
        {
            return entry.has_value();
        }

        template<typename F2>
        OptionalStateRelation<F1> composition(const F2 relation) const                                                                   
        {                                                                                                                                   
            return entry.has_value()? entry.value().composition(relation) : OptionalStateRelation<F1>();
        } 

        OptionalStateRelation<F1>& operator*=(const float scalar) 
        {
            if (entry.has_value())
            {
                entry.value() *= scalar;
            }
            return *this;
        }

        OptionalStateRelation<F1> operator/=(const float scalar) 
        {
            if (entry.has_value())
            {
                entry.value() /= scalar;
            }
            return *this;
        }

        OptionalStateRelation<F1>& operator+=(const OptionalStateRelation<F1> other) 
        {
            if (entry.has_value() && other.has_value())
            {
                entry.value() += other.value();
            } 
            else 
            {
                entry = std::optional<F1>();
            }
            return *this;
        }

        OptionalStateRelation<F1>& operator-=(const OptionalStateRelation<F1> other) 
        {
            if (entry.has_value() && other.has_value())
            {
                entry.value() -= other.value();
            } 
            else 
            {
                entry = std::optional<F1>();
            }
            return *this;
        }

        template<typename F2>
		friend class OptionalStateRelation;
    };

    template<typename F1>
    OptionalStateRelation<F1> operator+(const OptionalStateRelation<F1> relation, const OptionalStateRelation<F1> other) 
    {
        OptionalStateRelation<F1> result = relation;
        result += other;
        return result;
    }

    template<typename F1>
    OptionalStateRelation<F1> operator-(const OptionalStateRelation<F1> relation, const OptionalStateRelation<F1> other) 
    {
        OptionalStateRelation<F1> result = relation;
        result -= other;
        return result;
    }

    template<typename F1>
    OptionalStateRelation<F1> operator+(const OptionalStateRelation<F1> relation, const typename F1::value_type offset) 
    {
        OptionalStateRelation<F1> result = relation;
        result += offset;
        return result;
    }

    template<typename F1>
    OptionalStateRelation<F1> operator-(const OptionalStateRelation<F1> relation, const typename F1::value_type offset) 
    {
        OptionalStateRelation<F1> result = relation;
        result -= offset;
        return result;
    }

    template<typename F1>
    OptionalStateRelation<F1> operator*(const OptionalStateRelation<F1> relation, const float scalar) 
    {
        OptionalStateRelation<F1> result = relation;
        result *= scalar;
        return result;
    }

    template<typename F1>
    OptionalStateRelation<F1> operator/(const OptionalStateRelation<F1> relation, const float scalar) 
    {
        OptionalStateRelation<F1> result = relation;
        result /= scalar;
        return result;
    }

    template<typename F1>
    OptionalStateRelation<F1> operator+(const typename F1::value_type offset, const OptionalStateRelation<F1> relation) 
    {
        OptionalStateRelation<F1> result = relation;
        result += offset;
        return result;
    }

    template<typename F1>
    OptionalStateRelation<F1> operator-(const typename F1::value_type offset, const OptionalStateRelation<F1> relation) 
    {
        OptionalStateRelation<F1> result = relation;
        result *= -1.0f;
        result += offset;
        return result;
    }

    template<typename F1>
    OptionalStateRelation<F1> operator*(const float scalar, const OptionalStateRelation<F1> relation) 
    {
        OptionalStateRelation<F1> result = relation;
        result *= scalar;
        return result;
    }

    template<typename F1>
    OptionalStateRelation<F1> operator-(const OptionalStateRelation<F1> relation) 
    {
        OptionalStateRelation<F1> result = relation;
        result *= -1.0f;
        return result;
    }
}}