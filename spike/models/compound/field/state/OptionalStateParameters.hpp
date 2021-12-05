#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include "StateParameters.hpp"

namespace compound { 
namespace field {

    class OptionalStateParameters
    {
        typedef std::variant<std::monostate, StateParameters, StateParametersAggregate> OptionalStateParametersVariant;

        class OptionalStateParametersCompletionVisitor
        {
            StateParameters fallback;
        public:
            constexpr OptionalStateParametersCompletionVisitor(const StateParameters fallback) : fallback(fallback)
            {

            }
            StateParameters operator()(const std::monostate a) const {
                return fallback;
            }
            StateParameters operator()(const StateParameters a) const {
                return a;
            }
            StateParameters operator()(const StateParametersAggregate a ) const {
                return fallback;
            }
        };

        OptionalStateParametersVariant entry;
    public:
        OptionalStateParameters(const OptionalStateParametersVariant entry)
        : entry(entry)
        {

        }
        constexpr OptionalStateParameters(const std::monostate entry)
        : entry(entry)
        {

        }
        constexpr OptionalStateParameters(const StateParameters entry)
        : entry(entry)
        {

        }
        constexpr OptionalStateParameters(const StateParametersAggregate entry)
        : entry(entry)
        {

        }
        constexpr OptionalStateParameters()
        : entry(std::monostate())
        {

        }
        template<typename T2>
        constexpr OptionalStateParameters& operator=(const T2& other)
        {
            entry = OptionalStateParametersVariant(other);
            return *this;
        }
        constexpr bool operator==(const OptionalStateParameters other) const
        {
            return entry == other.entry;
        }
        constexpr bool operator!=(const OptionalStateParameters other) const
        {
            return entry != other.entry;
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        OptionalStateParameters value_or(const OptionalStateParameters other) const
        {
            return entry.index() > 0? *this : other;
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        constexpr StateParameters complete(const StateParameters fallback) const 
        {
            return std::visit(OptionalStateParametersCompletionVisitor(fallback), entry);
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a`, `b`, and `c`
        */
        template<typename Tfield2, typename Tfield3, typename Tfield4>
        constexpr OptionalStateParameters value_or(
            const std::function<OptionalStateParameters(const typename Tfield2::value_type)> f, 
            const Tfield2 a) const
        {
            if(entry.index() > 0) // no substitute needed
            {
                return *this;
            }
            else if(!a.has_value()) // any monostates
            {
                return std::monostate();
            }
            else // constant
            {
                return f(a(a.parameters()).value());
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a`, `b`, and `c`
        */
        template<typename Tfield2, typename Tfield3, typename Tfield4>
        constexpr OptionalStateParameters value_or(
            const std::function<OptionalStateParameters(const typename Tfield2::value_type, const typename Tfield3::value_type)> f, 
            const Tfield2 a, 
            const Tfield3 b) const
        {
            if(entry.index() > 0) // no substitute needed
            {
                return *this;
            }
            else if(!a.has_value() || !b.has_value()) // any monostates
            {
                return std::monostate();
            }
            else // constant
            {
                return f(
                    a(a.parameters()).value(),
                    b(b.parameters()).value()
                );
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a`, `b`, and `c`
        */
        template<typename Tfield2, typename Tfield3, typename Tfield4>
        constexpr OptionalStateParameters value_or(
            const std::function<OptionalStateParameters(const typename Tfield2::value_type, const typename Tfield3::value_type, const typename Tfield4::value_type)> f, 
            const Tfield2 a, 
            const Tfield3 b, 
            const Tfield4 c) const
        {
            if(entry.index() > 0) // no substitute needed
            {
                return *this;
            }
            else if(!a.has_value() || !b.has_value() || !c.has_value()) // any monostates
            {
                return std::monostate();
            }
            else // constant
            {
                return f(
                    a(a.parameters()).value(),
                    b(b.parameters()).value(),
                    c(c.parameters()).value()
                );
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a`, `b`, and `c`
        */
        template<typename Tfield2, typename Tfield3, typename Tfield4, typename Tfield5>
        constexpr OptionalStateParameters value_or(
            const std::function<OptionalStateParameters(const typename Tfield2::value_type, const typename Tfield3::value_type, const typename Tfield4::value_type, const typename Tfield5::value_type)> f, 
            const Tfield2 a, 
            const Tfield3 b, 
            const Tfield4 c, 
            const Tfield5 d) const
        {
            if(entry.index() > 0) // no substitute needed
            {
                return *this;
            }
            else if(!a.has_value() || !b.has_value() || !c.has_value() || !d.has_value()) // any monostates
            {
                return std::monostate();
            }
            else 
            {
                return f(
                    a(a.parameters()).value(),
                    b(b.parameters()).value(),
                    c(c.parameters()).value(),
                    d(d.parameters()).value()
                );
            }
        }
        /*
        Return the index of the variant
        */
        constexpr int index() const
        {
            return entry.index();
        }
        /*
        Return whether a entry exists within the field
        */
        constexpr bool has_value() const
        {
            return entry.index() > 0;
        }

    };
    
    /*
    Return a StateParameters representing `a`. Underwrite with `b` if `a` is missing a entry.
    This function exists merely to be analogous with similar functions for OptionalStateField and OptionalSpectralField
    */
    constexpr StateParameters complete(const OptionalStateParameters a, const StateParameters b)
    {
        return a.complete(b);
    }

    constexpr OptionalStateParameters aggregate(const OptionalStateParameters a, const OptionalStateParameters b)
    {
        return !a.has_value()? b : !b.has_value()? a : StateParametersAggregate();
    }

}}



