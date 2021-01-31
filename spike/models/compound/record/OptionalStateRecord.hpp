#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include <si.hpp>

#include "StateSampleRecord.hpp"
#include "AuthoritativeStateRecord.hpp"

#include "OptionalStateRecord.hpp"

namespace compound { 
namespace record {

    template<typename T1>
    class OptionalStateRecord
    {
    	template<typename T2>
	    using OptionalStateRecordVariant = std::variant<std::monostate, T2, StateSampleRecord<T2>, AuthoritativeStateRecord<T2>>;

        class OptionalStateRecordValueVisitor
        {
            si::pressure p;
            si::temperature T;
        public:
            OptionalStateRecordValueVisitor(const si::pressure p, const si::temperature T)
            : p(p), T(T)
            {

            }
            std::optional<T1> operator()(const std::monostate a               ) const {
                return std::optional<T1>();
            }
            std::optional<T1> operator()(const T1 a                           ) const {
                return a;
            }
            std::optional<T1> operator()(const StateSampleRecord<T1> a        ) const {
                return a.value;
            }
            std::optional<T1> operator()(const AuthoritativeStateRecord<T1> a ) const {
                return a(p,T);
            }
        };
        template<typename T2>
        class OptionalStateRecordMapVisitor
        {
        public:
            typedef std::function<T2(const T1)> F;
            F f;
            OptionalStateRecordMapVisitor(const F f)
            : f(f)
            {

            }
            OptionalStateRecordVariant<T2> operator()(const std::monostate a               ) const {
                return std::monostate();
            }
            OptionalStateRecordVariant<T2> operator()(const T1 a                           ) const {
                return f(a);
            }
            OptionalStateRecordVariant<T2> operator()(const StateSampleRecord<T1> a        ) const {
                return StateSampleRecord<T1>(f(a.value), a.pressure, a.temperature);
            }
            OptionalStateRecordVariant<T2> operator()(const AuthoritativeStateRecord<T1> a ) const {
                // NOTE: capturing `this` results in a segfault when we call `this->f()`
                // This occurs even when we capture `this` by value.
                // I haven't clarified the reason, but it seems likely related to accessing 
                // the `f` attribute in a object that destructs after we exit out of the `map` function.
                auto fcopy = f; 
                return [fcopy, a](const si::pressure p, const si::temperature T){ return fcopy(a(p, T)); };
            }
        };
        template<typename T2>
        class OptionalStateRecordMapToConstantVisitor
        {
            si::pressure default_p;
            si::temperature default_T;
            std::function<T2(T1, si::pressure, si::temperature)> f;
        public:
            OptionalStateRecordMapToConstantVisitor(
                const si::pressure default_p, 
                const si::temperature default_T,
                const std::function<T2(const T1, const si::pressure, const si::temperature)> f
            )
            : default_p(default_p), 
              default_T(default_T),
              f(f)
            {

            }
            std::optional<T2> operator()( const std::monostate a               ) const {
                return std::optional<T2>();
            }
            std::optional<T2> operator()( const T1 a                           ) const {
                return f(a, default_p, default_T);
            }
            std::optional<T2> operator()( const StateSampleRecord<T1> a        ) const {
                return f(a.value, a.pressure, a.temperature);
            }
            std::optional<T2> operator()( const AuthoritativeStateRecord<T1> a ) const {
                return f(a(default_p, default_T), default_p, default_T);
            }
        };

        OptionalStateRecordVariant<T1> value;
    public:
        constexpr OptionalStateRecord(const OptionalStateRecordVariant<T1> value)
        : value(value)
        {

        }
        constexpr OptionalStateRecord(const std::monostate value)
        : value(value)
        {

        }
        constexpr OptionalStateRecord(const T1 value)
        : value(value)
        {

        }
        constexpr OptionalStateRecord(const StateSampleRecord<T1> value)
        : value(value)
        {

        }
        constexpr OptionalStateRecord(const AuthoritativeStateRecord<T1> value)
        : value(value)
        {

        }
    	template<typename T2>
        constexpr OptionalStateRecord<T1>& operator=(const T2& other)
        {
        	value = OptionalStateRecordVariant<T1>(other);
        	return *this;
        }
        constexpr std::optional<T1> operator()(const si::pressure p, const si::temperature T) const
        {
            return std::visit(OptionalStateRecordValueVisitor(p, T), value);
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        constexpr OptionalStateRecord<T1> substitute(const OptionalStateRecord<T1> other) const
        {
            return index() > 0? *this : other;
        }
        /*
        Return whichever record provides more information, going by the following definition:
            std::monostate < T1 < AuthoritativeStateRecord<T1> < std::pair<T1, AuthoritativeStateRecord<T1>>
        If both provide the same amount of information, return `a` by default.
        */
        constexpr OptionalStateRecord<T1> compare(const OptionalStateRecord<T1> other) const
        {
            return index() >= other.index()? *this : other;
        }
        /*
		Return whether a value exists within the record
        */
        constexpr bool has_value() const
        {
            return index() == 0;
        }
        /*
		Return whether a value exists within the record
        */
        constexpr int index() const
        {
            return value.index();
        }
        /*
        Return a OptionalStateRecord<T1> record representing `a` after applying the map `f`
        */
        template<typename T2>
        constexpr OptionalStateRecord<T2> map(const std::function<T2(const T1)> f) const
        {
            return OptionalStateRecord<T2>(std::visit(OptionalStateRecordMapVisitor<T2>(f), value));
        }
        /*
        Return a OptionalStateRecord<T1> record representing `a` after applying the map `f`
        */
        template<typename T2>
        constexpr std::optional<T1> map_to_constant(
            const si::pressure default_p, 
            const si::temperature default_T, 
            const std::function<T2(const T1, const si::pressure, const si::temperature)> f
        ) const {
            return std::visit(OptionalStateRecordMapToConstantVisitor<T2>(default_p, default_T, f), value);
        }

        template<typename T2>
		friend class OptionalStateRecord;
    };
    
}}