#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include <si.hpp>

#include "StateSampleRecord.hpp"
#include "AuthoritativeStateRecord.hpp"

#include "CompletedStateRecord.hpp"

namespace compound { 
namespace record {

    template<typename T1>
    class CompletedStateRecord
    {
    	template<typename T2>
	    using CompletedStateRecordVariant = std::variant<T2, StateSampleRecord<T2>, AuthoritativeStateRecord<T2>>;

        class CompletedStateRecordValueVisitor
        {
            si::pressure p;
            si::temperature T;
        public:
            CompletedStateRecordValueVisitor(const si::pressure p, const si::temperature T)
            : p(p), T(T)
            {

            }
            T1 operator()(const T1 a                           ) const {
                return a;
            }
            T1 operator()(const StateSampleRecord<T1> a        ) const {
                return a.value;
            }
            T1 operator()(const AuthoritativeStateRecord<T1> a ) const {
                return a(p,T);
            }
        };
        template<typename T2>
        class CompletedStateRecordMapVisitor
        {
        public:
            typedef std::function<T2(const T1)> F;
            F f;
            CompletedStateRecordMapVisitor(const F f)
            : f(f)
            {

            }
            CompletedStateRecordVariant<T2> operator()(const T1 a                           ) const {
                return f(a);
            }
            CompletedStateRecordVariant<T2> operator()(const StateSampleRecord<T1> a        ) const {
                return StateSampleRecord<T1>(f(a.value), a.pressure, a.temperature);
            }
            CompletedStateRecordVariant<T2> operator()(const AuthoritativeStateRecord<T1> a ) const {
                // NOTE: capturing `this` results in a segfault when we call `this->f()`
                // This occurs even when we capture `this` by value.
                // I haven't clarified the reason, but it seems likely related to accessing 
                // the `f` attribute in a object that destructs after we exit out of the `map` function.
                auto fcopy = f; 
                return [fcopy, a](const si::pressure p, const si::temperature T){ return fcopy(a(p, T)); };
            }
        };
        template<typename T2>
        class CompletedStateRecordMapToConstantVisitor
        {
            si::pressure default_p;
            si::temperature default_T;
            std::function<T2(T1, si::pressure, si::temperature)> f;
        public:
            CompletedStateRecordMapToConstantVisitor(
                const si::pressure default_p, 
                const si::temperature default_T,
                const std::function<T2(const T1, const si::pressure, const si::temperature)> f
            )
            : default_p(default_p), 
              default_T(default_T),
              f(f)
            {

            }
            T2 operator()( const T1 a                           ) const {
                return f(a, default_p, default_T);
            }
            T2 operator()( const StateSampleRecord<T1> a        ) const {
                return f(a.value, a.pressure, a.temperature);
            }
            T2 operator()( const AuthoritativeStateRecord<T1> a ) const {
                return f(a(default_p, default_T), default_p, default_T);
            }
        };

        CompletedStateRecordVariant<T1> value;
    public:
        constexpr CompletedStateRecord(const CompletedStateRecordVariant<T1> value)
        : value(value)
        {

        }
        constexpr CompletedStateRecord(const T1 value)
        : value(value)
        {

        }
        constexpr CompletedStateRecord(const StateSampleRecord<T1> value)
        : value(value)
        {

        }
        constexpr CompletedStateRecord(const AuthoritativeStateRecord<T1> value)
        : value(value)
        {

        }
    	template<typename T2>
        constexpr CompletedStateRecord<T1>& operator=(const T2& other)
        {
        	value = CompletedStateRecordVariant<T1>(other);
        	return *this;
        }
        constexpr T1 operator()(const si::pressure p, const si::temperature T) const
        {
            return std::visit(CompletedStateRecordValueVisitor(p, T), value);
        }
        /*
        Return whichever record provides more information, going by the following definition:
            std::monostate < T1 < AuthoritativeStateRecord<T1> < std::pair<T1, AuthoritativeStateRecord<T1>>
        If both provide the same amount of information, return `a` by default.
        */
        constexpr CompletedStateRecord<T1> compare(const CompletedStateRecord<T1> other) const
        {
            return value.index() >= other.value.index()? *this : other;
        }
        /*
		Return whether a value exists within the record
        */
        constexpr int index() const
        {
            return value.index();
        }
        /*
		Return whether a value exists within the record
        */
        constexpr bool has_value() const
        {
            return value.index() == 0;
        }
        /*
        Return a CompletedStateRecord<T1> record representing `a` after applying the map `f`
        */
        template<typename T2>
        constexpr CompletedStateRecord<T2> map(const std::function<T2(const T1)> f) const
        {
            return CompletedStateRecord<T2>(std::visit(CompletedStateRecordMapVisitor<T2>(f), value));
        }
        /*
        Return a CompletedStateRecord<T1> record representing `a` after applying the map `f`
        */
        template<typename T2>
        constexpr T1 map_to_constant(
            const si::pressure default_p, 
            const si::temperature default_T, 
            const std::function<T2(const T1, const si::pressure, const si::temperature)> f
        ) const {
            return std::visit(CompletedStateRecordMapToConstantVisitor<T2>(default_p, default_T, f), value);
        }

        template<typename T2>
		friend class CompletedStateRecord;
    };
    
}}