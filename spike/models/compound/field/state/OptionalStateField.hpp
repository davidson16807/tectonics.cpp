#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include <units/si.hpp>

#include "StateSample.hpp"
#include "StateFunction.hpp"

#include "OptionalStateField.hpp"

namespace compound { 
namespace field {

    template<typename T1>
    class OptionalStateField
    {
    	template<typename T2>
        using OptionalStateFieldVariant = std::variant<std::monostate, T2, StateSample<T2>, StateFunction<T2>>;
        template<typename T2>
	    using CompletedStateFieldVariant = std::variant<T2, StateSample<T2>, StateFunction<T2>>;

        template<typename Tout>
        class OptionalStateFieldValueVisitor
        {
            si::pressure p;
            si::temperature T;
        public:
            OptionalStateFieldValueVisitor(const si::pressure p, const si::temperature T)
            : p(p), T(T)
            {

            }
            std::optional<Tout> operator()(const std::monostate a               ) const {
                return std::optional<Tout>();
            }
            std::optional<Tout> operator()(const Tout a                           ) const {
                return a;
            }
            std::optional<Tout> operator()(const StateSample<Tout> a        ) const {
                return a.entry;
            }
            std::optional<Tout> operator()(const StateFunction<Tout> a ) const {
                return a(p,T);
            }
        };
        class OptionalStateFieldEqualityVisitor
        {
            OptionalStateFieldVariant<T1> other;
        public:
            OptionalStateFieldEqualityVisitor(OptionalStateFieldVariant<T1> other)
            : other(other)
            {

            }
            bool operator()(const std::monostate a               ) const {
                return other == a;
            }
            bool operator()(const T1 a                           ) const {
                return other == a;
            }
            bool operator()(const StateSample<T1> a        ) const {
                return other == a;
            }
            bool operator()(const StateFunction<T1> a ) const {
                return other(0.0 * si::pascal, 0.0* si::kelvin) == a(0.0 * si::pascal, 0.0* si::kelvin);
            }
        };
        template<typename Tout, typename Tin1>
        class OptionalStateFieldUnaryMapVisitor
        {
        public:
            typedef std::function<Tout(const Tin1)> F;
            typedef std::function<Tout(const si::pressure, const si::temperature)> FpT;
            F f;
            OptionalStateFieldUnaryMapVisitor(const F f)
            : f(f)
            {

            }
            OptionalStateFieldVariant<Tout> operator()(const std::monostate a               ) const {
                return std::monostate();
            }
            OptionalStateFieldVariant<Tout> operator()(const Tin1 a                           ) const {
                return f(a);
            }
            OptionalStateFieldVariant<Tout> operator()(const StateSample<Tin1> a        ) const {
                return StateSample<Tin1>(f(a.entry), a.pressure, a.temperature);
            }
            OptionalStateFieldVariant<Tout> operator()(const StateFunction<Tin1> a ) const {
                // NOTE: capturing `this` results in a segfault when we call `this->f()`
                // This occurs even when we capture `this` by entry.
                // I haven't clarified the reason, but it seems likely related to accessing 
                // the `f` attribute in a object that destructs after we exit out of the `map` function.
                F fcopy = f; 
                return FpT([fcopy, a](const si::pressure p, const si::temperature T){ return fcopy(a(p, T)); });
            }
        };
        template<typename Tout, typename Tin1, typename Tin2>
        class OptionalStateFieldRightCurriedBinaryMapVisitor
        {
        public:
            typedef std::function<Tout(const Tin1,const Tin2)> F;
            typedef std::function<Tout(const si::pressure, const si::temperature)> FpT;
            F f;
            Tin2 b;
            OptionalStateFieldRightCurriedBinaryMapVisitor(const F f, const Tin2 b)
            : f(f), b(b)
            {

            }
            OptionalStateFieldVariant<Tout> operator()(const std::monostate a) const {
                return std::monostate();
            }
            OptionalStateFieldVariant<Tout> operator()(const Tin1 a) const {
                return f(a,b);
            }
            OptionalStateFieldVariant<Tout> operator()(const StateSample<Tin1> a) const {
                return StateSample<Tout>(f(a.entry,b), a.pressure, a.temperature);
            }
            OptionalStateFieldVariant<Tout> operator()(const StateFunction<Tin1> a ) const {
                // NOTE: capturing `this` results in b segfault when we call `this->f()`
                // This occurs even when we capture `this` by entry.
                // I haven't clarified the reason, but it seems likely related to accessing 
                // the `f` attribute in b object that destructs after we ebit out of the `map` function.
                F fcopy = f; 
                Tin2 bcopy = b; 
                return FpT([fcopy, bcopy, a](const si::pressure p, const si::temperature T){ return fcopy(a(p, T), bcopy); });
            }
        };
        template<typename Tout, typename Tin1, typename Tin2>
        class OptionalStateFieldLeftCurriedBinaryMapVisitor
        {
        public:
            typedef std::function<Tout(const Tin1,const Tin2)> F;
            typedef std::function<Tout(const si::pressure, const si::temperature)> FpT;
            F f;
            OptionalStateFieldVariant<Tin1> a;
            OptionalStateFieldLeftCurriedBinaryMapVisitor(const F f, const OptionalStateFieldVariant<Tin1> a)
            : f(f), a(a)
            {

            }
            OptionalStateFieldVariant<Tout> operator()(const std::monostate b) const {
                return std::monostate();
            }
            OptionalStateFieldVariant<Tout> operator()(const Tin2 b) const {
                return std::visit(OptionalStateFieldRightCurriedBinaryMapVisitor<Tout,Tin1,Tin2>(f, b), a);
            }
            OptionalStateFieldVariant<Tout> operator()(const StateSample<Tin2> b) const {
                /*
                if both a and b are StateSamples, we cannot represent the pressure/temperature metadata from both of them in the result,
                so we simply strip the metadata from the result and store as a constant.
                */
                si::pressure p = b.pressure;
                si::temperature T = b.temperature;
                return 
                    a.index() == 1?  StateSample<Tout>(f(std::visit(OptionalStateFieldValueVisitor<Tin1>(p, T), a).value(), b.entry), p, T) 
                  : a.index() == 2?  f(std::visit(OptionalStateFieldValueVisitor<Tin1>(p, T), a).value(), b.entry)
                  : std::visit(OptionalStateFieldRightCurriedBinaryMapVisitor<Tout,Tin1,Tin2>(f, b.entry), a);
            }
            OptionalStateFieldVariant<Tout> operator()(const StateFunction<Tin2> b ) const {
                // NOTE: capturing `this` results in a segfault when we call `this->f()`
                // This occurs even when we capture `this` by entry.
                // I haven't clarified the reason, but it seems likely related to accessing 
                // the `f` attribute in a object that destructs after we exit out of the `map` function.
                F fcopy = f; 
                OptionalStateFieldVariant<Tin1> acopy = a; 
                return
                    a.index() == 0? OptionalStateFieldVariant<Tout>(std::monostate() )
                  : OptionalStateFieldVariant<Tout>(FpT([fcopy, acopy, b](const si::pressure p, const si::temperature T){ 
                                                            return fcopy(std::visit(OptionalStateFieldValueVisitor<Tin1>(p, T), acopy).value(), b(p, T)); 
                                                        }));
            }
        };
        template<typename T2>
        class OptionalStateFieldMapToConstantVisitor
        {
            si::pressure default_p;
            si::temperature default_T;
            std::function<T2(T1, si::pressure, si::temperature)> f;
        public:
            OptionalStateFieldMapToConstantVisitor(
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
            std::optional<T2> operator()( const StateSample<T1> a        ) const {
                return f(a.entry, a.pressure, a.temperature);
            }
            std::optional<T2> operator()( const StateFunction<T1> a ) const {
                return f(a(default_p, default_T), default_p, default_T);
            }
        };
        class OptionalStateFieldCompletionVisitor
        {
            CompletedStateFieldVariant<T1> fallback;
        public:
            OptionalStateFieldCompletionVisitor(CompletedStateFieldVariant<T1> fallback) : fallback(fallback)
            {

            }
            CompletedStateFieldVariant<T1> operator()(const std::monostate a               ) const {
                return fallback;
            }
            CompletedStateFieldVariant<T1> operator()(const T1 a                           ) const {
                return a;
            }
            CompletedStateFieldVariant<T1> operator()(const StateSample<T1> a        ) const {
                return a;
            }
            CompletedStateFieldVariant<T1> operator()(const StateFunction<T1> a ) const {
                return a;
            }
        };

        OptionalStateFieldVariant<T1> entry;
    public:
        constexpr OptionalStateField(const OptionalStateFieldVariant<T1> entry)
        : entry(entry)
        {

        }
        constexpr OptionalStateField(const std::monostate entry)
        : entry(entry)
        {

        }
        constexpr OptionalStateField(const T1 entry)
        : entry(entry)
        {

        }
        constexpr OptionalStateField(const StateSample<T1> entry)
        : entry(entry)
        {

        }
        constexpr OptionalStateField(const StateFunction<T1> entry)
        : entry(entry)
        {

        }
    	template<typename T2>
        constexpr OptionalStateField<T1>& operator=(const T2& other)
        {
            entry = OptionalStateFieldVariant<T1>(other);
            return *this;
        }
        constexpr std::optional<T1> operator()(const si::pressure p, const si::temperature T) const
        {
            return std::visit(OptionalStateFieldValueVisitor<T1>(p, T), entry);
        }
        /*
        Return whichever field provides more information, going by the following definition:
            std::monostate < T1 < StateFunction<T1> < std::pair<T1, StateFunction<T1>>
        If both provide the same amount of information, return `a` by default.
        */
        constexpr OptionalStateField<T1> compare(const OptionalStateField<T1> other) const
        {
            return entry.index() >= other.entry.index()? *this : other;
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        constexpr OptionalStateField<T1> value_or(const OptionalStateField<T1> other) const
        {
            return entry.index() > 0? *this : other;
        }
        constexpr CompletedStateFieldVariant<T1> value_or(const CompletedStateFieldVariant<T1> fallback) const 
        {
            return std::visit(OptionalStateFieldCompletionVisitor(fallback), entry);
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameter `a`
        */
        template<typename T2>
        constexpr OptionalStateField<T1> value_or(const std::function<T1(const T2)> f, const OptionalStateField<T2> a) const
        {
            return entry.index() > 0? *this 
                : OptionalStateField<T2>(std::visit(OptionalStateFieldUnaryMapVisitor<T1,T2>(f), a.entry));
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename T2, typename T3>
        constexpr OptionalStateField<T1> value_or(
            const std::function<T1(const T2, const T3)> f, 
            const OptionalStateField<T2> a, 
            const OptionalStateField<T3> b) const
        {
            return entry.index() > 0? *this 
                : std::visit(OptionalStateFieldLeftCurriedBinaryMapVisitor<T1,T2,T3>(f, a.entry), b.entry);
        }
        /*
		Return whether a entry exists within the field
        */
        constexpr bool has_value() const
        {
            return entry.index() > 0;
        }
        /*
		Return whether a entry exists within the field
        */
        constexpr int index() const
        {
            return entry.index();
        }
        /*
        Return a OptionalStateField<T1> field representing `a` after applying the map `f`
        */
        template<typename T2>
        constexpr std::optional<T2> map_to_constant(
            const si::pressure default_p, 
            const si::temperature default_T, 
            const std::function<T2(const T1, const si::pressure, const si::temperature)> f
        ) const {
            return std::visit(OptionalStateFieldMapToConstantVisitor<T2>(default_p, default_T, f), entry);
        }

        template<typename T2>
		friend class OptionalStateField;
    };
    
}}