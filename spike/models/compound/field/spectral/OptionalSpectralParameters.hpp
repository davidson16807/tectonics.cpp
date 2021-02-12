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
        constexpr SpectralParameters value_or(const SpectralParameters fallback) const 
        {
            return std::visit(OptionalSpectralParametersCompletionVisitor(fallback), entry);
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
        return a.value_or(b);
    }

    constexpr OptionalSpectralParameters aggregate(const OptionalSpectralParameters a, const OptionalSpectralParameters b)
    {
        return !a.has_value()? b : !b.has_value()? a : SpectralParametersAggregate();
    }

}}



