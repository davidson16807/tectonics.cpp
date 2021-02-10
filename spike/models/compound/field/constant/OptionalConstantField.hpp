#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include <units/si.hpp>

namespace compound { 
namespace field {

    /*
    Q: Why not use std::optional<T>?
    A: 2 reasons: 
      1.) to maintain consistency with other `Optional*Field` classes, e.g. using std::monostate/field::missing
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
        template<typename T2>
        class OptionalConstantFieldMapVisitor
        {
        public:
            typedef std::function<T2(const T1)> F;
            F f;
            OptionalConstantFieldMapVisitor(const F f)
            : f(f)
            {

            }
            OptionalConstantFieldVariant<T2> operator()(const std::monostate a) const {
                return std::monostate();
            }
            OptionalConstantFieldVariant<T2> operator()(const T1 a            ) const {
                return f(a);
            }
        };

        OptionalConstantFieldVariant<T1> entry;
    public:
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
            return entry.index() == 0;
        }
        /*
        Return a OptionalConstantField<T1> field representing `a` after applying the map `f`
        */
        template<typename T2>
        constexpr OptionalConstantField<T2> map(const std::function<T2(const T1)> f) const
        {
            return OptionalConstantField<T2>(std::visit(OptionalConstantFieldMapVisitor<T2>(f), entry));
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