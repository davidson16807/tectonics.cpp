#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include <units/si.hpp>

#include "EmptyParameters.hpp"

namespace compound { 
namespace field {

    /*
    Q: Why not use std::optional<T>?
    A: 2 reasons: 
      1.) to maintain consistency with other `Optional*Field` classes, e.g. using std::monostate/field::missing, value_or(f,a), etc.
      2.) because std::optional<T> is not constexpr, so it can't be used to make constexpr classes
    */
    template<typename T1>
    class OptionalConstantField
    {
    	template<typename T2>
        using OptionalConstantFieldVariant = std::variant<std::monostate, T2>;

        class OptionalConstantFieldCompletionVisitor
        {
            T1 fallback;
        public:
            OptionalConstantFieldCompletionVisitor(T1 fallback) : fallback(fallback)
            {

            }
            T1 operator()(const std::monostate a) const {
                return fallback;
            }
            T1 operator()(const T1 a            ) const {
                return a;
            }
        };
        class OptionalConstantFieldValueVisitor
        {
        public:
            OptionalConstantFieldValueVisitor()
            {

            }
            std::optional<T1> operator()( const std::monostate a) const {
                return std::optional<T1>();
            }
            std::optional<T1> operator()( const T1 a ) const {
                return std::optional<T1>(a);
            }
        };

        OptionalConstantFieldVariant<T1> entry;
    public:
        using value_type = T1;
        using parameter_type = EmptyParameters;

        constexpr OptionalConstantField(const OptionalConstantFieldVariant<T1> entry)
        : entry(entry)
        {

        }
        constexpr OptionalConstantField(const std::monostate entry)
        : entry(entry)
        {

        }
        constexpr OptionalConstantField(const T1 entry)
        : entry(entry)
        {

        }
        constexpr OptionalConstantField()
        : entry(std::monostate())
        {

        }
    	template<typename T2>
        constexpr OptionalConstantField<T1>& operator=(const T2& other)
        {
            entry = OptionalConstantFieldVariant<T1>(other);
            return *this;
        }
        constexpr bool operator==(const OptionalConstantField<T1> other) const
        {
            return entry == other.entry;
        }
        constexpr bool operator!=(const OptionalConstantField<T1> other) const
        {
            return entry != other.entry;
        }
        constexpr std::optional<T1> operator()() const
        {
            return std::visit(OptionalConstantFieldValueVisitor(), entry);
        }
        constexpr std::optional<T1> operator()(const EmptyParameters parameters) const
        {
            return std::visit(OptionalConstantFieldValueVisitor(), entry);
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        constexpr OptionalConstantField<T1> value_or(const OptionalConstantField<T1> other) const
        {
            return entry.index() > 0? *this : other;
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        constexpr T1 value_or(T1 fallback) const 
        {
            return std::visit(OptionalConstantFieldCompletionVisitor(fallback), entry);
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename Tfield2>
        constexpr OptionalConstantField<T1> value_or(
            const std::function<T1(const typename Tfield2::value_type)> f, 
            const typename Tfield2::parameter_type defaults,
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
                typename Tfield2::parameter_type parameters = a.parameters().value_or(defaults);
                return f(a(parameters).value());
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename Tfield2, typename Tfield3>
        constexpr OptionalConstantField<T1> value_or(
            const std::function<T1(const typename Tfield2::value_type, const typename Tfield3::value_type)> f, 
            const typename Tfield2::parameter_type defaults,
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
                typename Tfield2::parameter_type parameters = aggregate(a.parameters(), b.parameters()).value_or(defaults);
                return f(a(parameters).value(), b(parameters).value());
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a`, `b`, and `c`
        */
        template<typename Tfield2, typename Tfield3, typename Tfield4>
        constexpr OptionalConstantField<T1> value_or(
            const std::function<T1(const typename Tfield2::value_type, const typename Tfield3::value_type, const typename Tfield4::value_type)> f, 
            const typename Tfield2::parameter_type defaults,
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
                typename Tfield2::parameter_type parameters = aggregate(a.parameters(), aggregate(b.parameters(), c.parameters())).value_or(defaults);
                return f(a(parameters).value(), b(parameters).value(), c(parameters).value());
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
            return entry.index() != 0;
        }
        constexpr EmptyParameters parameters() const
        {
            return EmptyParameters();
        }

        template<typename T2>
		friend class OptionalConstantField;
    };
    
    /*
    Return a T1 representing `a`. Underwrite with `b` if `a` is missing a entry.
    This function exists merely to be analogous with similar functions for OptionalStateField and OptionalSpectralField
    */
    template<typename T1>
    constexpr T1 complete(const OptionalConstantField<T1> a, const T1 b)
    {
        return a.value_or(b);
    }
}}