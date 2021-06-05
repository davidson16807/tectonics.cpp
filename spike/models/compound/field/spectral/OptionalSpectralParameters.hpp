#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include "SpectralParameters.hpp"

namespace compound { 
namespace field {

    class OptionalSpectralParameters
    {
        typedef std::variant<std::monostate, SpectralParameters, SpectralParametersAggregate> OptionalSpectralParametersVariant;

        class OptionalSpectralParametersCompletionVisitor
        {
            SpectralParameters fallback;
        public:
            constexpr OptionalSpectralParametersCompletionVisitor(const SpectralParameters fallback) : fallback(fallback)
            {

            }
            SpectralParameters operator()(const std::monostate a) const {
                return fallback;
            }
            SpectralParameters operator()(const SpectralParameters a) const {
                return a;
            }
            SpectralParameters operator()(const SpectralParametersAggregate a ) const {
                return fallback;
            }
        };

        OptionalSpectralParametersVariant entry;
    public:
        OptionalSpectralParameters(const OptionalSpectralParametersVariant entry)
        : entry(entry)
        {

        }
        constexpr OptionalSpectralParameters(const std::monostate entry)
        : entry(entry)
        {

        }
        constexpr OptionalSpectralParameters(const SpectralParameters entry)
        : entry(entry)
        {

        }
        constexpr OptionalSpectralParameters(const SpectralParametersAggregate entry)
        : entry(entry)
        {

        }
        constexpr OptionalSpectralParameters()
        : entry(std::monostate())
        {

        }
        template<typename T2>
        constexpr OptionalSpectralParameters& operator=(const T2& other)
        {
            entry = OptionalSpectralParametersVariant(other);
            return *this;
        }
        constexpr bool operator==(const OptionalSpectralParameters other) const
        {
            return entry == other.entry;
        }
        constexpr bool operator!=(const OptionalSpectralParameters other) const
        {
            return entry != other.entry;
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        OptionalSpectralParameters value_or(const OptionalSpectralParameters other) const
        {
            return entry.index() > 0? *this : other;
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        constexpr SpectralParameters complete(const SpectralParameters fallback) const 
        {
            return std::visit(OptionalSpectralParametersCompletionVisitor(fallback), entry);
        }


        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a`, `b`, and `c`
        */
        template<typename Tfield2, typename Tfield3, typename Tfield4>
        constexpr OptionalSpectralParameters value_or(
            const std::function<OptionalSpectralParameters(const typename Tfield2::value_type)> f, 
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
        constexpr OptionalSpectralParameters value_or(
            const std::function<OptionalSpectralParameters(const typename Tfield2::value_type, const typename Tfield3::value_type)> f, 
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
        constexpr OptionalSpectralParameters value_or(
            const std::function<OptionalSpectralParameters(const typename Tfield2::value_type, const typename Tfield3::value_type, const typename Tfield4::value_type)> f, 
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
        constexpr OptionalSpectralParameters value_or(
            const std::function<OptionalSpectralParameters(const typename Tfield2::value_type, const typename Tfield3::value_type, const typename Tfield4::value_type, const typename Tfield5::value_type)> f, 
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
            else // constant
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
    Return a SpectralParameters representing `a`. Underwrite with `b` if `a` is missing a entry.
    This function exists merely to be analogous with similar functions for OptionalSpectralField and OptionalSpectralField
    */
    constexpr SpectralParameters complete(const OptionalSpectralParameters a, const SpectralParameters b)
    {
        return a.complete(b);
    }

    constexpr OptionalSpectralParameters aggregate(const OptionalSpectralParameters a, const OptionalSpectralParameters b)
    {
        return !a.has_value()? b : !b.has_value()? a : SpectralParametersAggregate();
    }

}}



