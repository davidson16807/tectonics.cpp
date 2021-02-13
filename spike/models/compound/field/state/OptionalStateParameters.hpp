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
        constexpr StateParameters value_or(const StateParameters fallback) const 
        {
            return std::visit(OptionalStateParametersCompletionVisitor(fallback), entry);
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
        return a.value_or(b);
    }

    constexpr OptionalStateParameters aggregate(const OptionalStateParameters a, const OptionalStateParameters b)
    {
        return !a.has_value()? b : !b.has_value()? a : StateParametersAggregate();
    }

}}



