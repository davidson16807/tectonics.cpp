#pragma once

// in-house libraries
#include <si.hpp>

#include "OptionalStateRecord.hpp"

namespace compound { 
namespace record {
    namespace {
        template<typename T1>
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
        template<typename T1, typename T2>
        class OptionalStateRecordMapVisitor
        {
        public:
            typedef std::function<T2(const T1)> F;
            F f;
            OptionalStateRecordMapVisitor(const F f)
            : f(f)
            {

            }
            OptionalStateRecord<T2> operator()(const std::monostate a               ) const {
                return std::monostate();
            }
            OptionalStateRecord<T2> operator()(const T1 a                           ) const {
                return f(a);
            }
            OptionalStateRecord<T2> operator()(const StateSampleRecord<T1> a        ) const {
                return StateSampleRecord<T1>(f(a.value), a.pressure, a.temperature);
            }
            OptionalStateRecord<T2> operator()(const AuthoritativeStateRecord<T1> a ) const {
                // NOTE: capturing `this` results in a segfault when we call `this->f()`
                // This occurs even when we capture `this` by value.
                // I haven't clarified the reason, but it seems likely related to accessing 
                // the `f` attribute in a object that destructs after we exit out of the `map` function.
                auto fcopy = f; 
                return [fcopy, a](const si::pressure p, const si::temperature T){ return fcopy(a(p, T)); };
            }
        };
        template<typename T1, typename T2>
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
    }
    /*
    Return `a` if available, otherwise return `b` as a substitute.
    */
    template<typename T1>
    constexpr OptionalStateRecord<T1> substitute(const OptionalStateRecord<T1> a, const OptionalStateRecord<T1> b)
    {
        return a.index() > 0? a : b;
    }
    /*
    Return whichever record provides more information, going by the following definition:
        std::monostate < T1 < AuthoritativeStateRecord<T1> < std::pair<T1, AuthoritativeStateRecord<T1>>
    If both provide the same amount of information, return `a` by default.
    */
    template<typename T1>
    constexpr OptionalStateRecord<T1> best(const OptionalStateRecord<T1> a, const OptionalStateRecord<T1> b)
    {
        return a.index() >= b.index()? a : b;
    }
    template<typename T1>
    constexpr bool has_value(const OptionalStateRecord<T1> a)
    {
        return a.index() == 0;
    }
    template<typename T1>
    constexpr std::optional<T1> value(const OptionalStateRecord<T1> a, si::pressure p, si::temperature T)
    {
        return std::visit(OptionalStateRecordValueVisitor<T1>(p, T), a);
    }
    /*
    Return a OptionalStateRecord<T1> record representing `a` after applying the map `f`
    */
    template<typename T1, typename T2>
    constexpr OptionalStateRecord<T2> map(const OptionalStateRecord<T1> a, const std::function<T2(const T1)> f)
    {
        return std::visit(OptionalStateRecordMapVisitor<T1,T2>(f), a);
    }
    /*
    Return a OptionalStateRecord<T1> record representing `a` after applying the map `f`
    */
    template<typename T1, typename T2>
    constexpr std::optional<T1> map_to_constant(
        const OptionalStateRecord<T1> a, 
        const si::pressure default_p, 
        const si::temperature default_T, 
        const std::function<T2(const T1, const si::pressure, const si::temperature)> f
    ){
        return std::visit(OptionalStateRecordMapToConstantVisitor<T1,T2>(default_p, default_T, f), a);
    }
}}