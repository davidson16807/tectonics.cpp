#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include <units/si.hpp>

#include "StateSample.hpp"
#include "StateFunction.hpp"

#include "OptionalStateParameters.hpp"

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
        class OptionalStateFieldParametersVisitor
        {
        public:
            OptionalStateFieldParametersVisitor()
            {

            }
            OptionalStateParameters operator()( const std::monostate a               ) const {
                return std::monostate();
            }
            OptionalStateParameters operator()( const T1 a                           ) const {
                return std::monostate();
            }
            OptionalStateParameters operator()( const StateSample<T1> a        ) const {
                return StateParameters(a.pressure, a.temperature);
            }
            OptionalStateParameters operator()( const StateFunction<T1> a ) const {
                return std::monostate();
            }
        };
        class OptionalStateFieldCompletionVisitor
        {
            CompletedStateFieldVariant<T1> fallback;
        public:
            OptionalStateFieldCompletionVisitor(const CompletedStateFieldVariant<T1> fallback) : fallback(fallback)
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
        using value_type = T1;
        using parameter_type = StateParameters;
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
        constexpr OptionalStateField(const std::optional<T1> option)
        : entry(option.has_value()? OptionalStateFieldVariant<T1>(option.value()) : OptionalStateFieldVariant<T1>(std::monostate()))
        {

        }
        constexpr OptionalStateField(const std::optional<StateSample<T1>> option)
        : entry(option.has_value()? OptionalStateFieldVariant<T1>(option.value()) : OptionalStateFieldVariant<T1>(std::monostate()))
        {

        }
        constexpr OptionalStateField(const std::optional<StateFunction<T1>> option)
        : entry(option.has_value()? OptionalStateFieldVariant<T1>(option.value()) : OptionalStateFieldVariant<T1>(std::monostate()))
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
        constexpr std::optional<T1> operator()(const StateParameters parameters) const
        {
            return std::visit(OptionalSpectralFieldValueVisitor<T1>(parameters.pressure, parameters.temperature), entry);
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
            if(entry.index() > 0) // no substitute needed
            {
                return *this;
            }
            else if(!a.has_value() || !b.has_value()) // any monostates
            {
                return std::monostate();
            }
            else if((a.index() == 3) || (b.index() == 3)) // any functions
            {
                auto f2 = f;
                auto a2 = a;
                auto b2 = b;
                return StateFunction<T1>(
                    [f2, a2, b2]
                    (const si::pressure p, const si::temperature T)
                    { return f2(a2(p,T).value(), b2(p,T).value()); }
                );
            }
            else // constant
            {
                OptionalStateParameters parameters = aggregate(a.parameters(), b.parameters());
                // NOTE: The values in "dummy" are never read, since a and b are not functions by this point, 
                // and we only record values in a SpectralSample if exactly one of a and b are defined.
                StateParameters dummy(si::standard_pressure, si::standard_temperature);
                StateParameters defaults = parameters.value_or(dummy);
                si::pressure p = defaults.pressure;
                si::temperature T = defaults.temperature;
                T1 result = f(a(p,T).value(), b(p,T).value());
                return parameters.has_value()? OptionalStateField<T1>(StateSample<T1>(result, p,T)) : OptionalStateField<T1>(result);
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename T2, typename T3, typename T4>
        constexpr OptionalStateField<T1> value_or(
            const std::function<T1(const T2, const T3, const T4)> f, 
            const OptionalStateField<T2> a, 
            const OptionalStateField<T3> b, 
            const OptionalStateField<T4> c) const
        {
            // the following are dummy values, since we only invoke them on StateSamples or constants
            const si::pressure p = si::standard_pressure;
            const si::temperature T = si::standard_temperature;
            if(entry.index() > 0)
            {
                return *this;
            }
            else if(!a.has_value() || !b.has_value() || !c.has_value()) // any monostates
            {
                return std::monostate();
            }
            else if((a.index() == 3) || (b.index() == 3) || (c.index() == 3)) // any functions
            {
                auto f2 = f;
                auto a2 = a;
                auto b2 = b;
                auto c2 = c;
                return StateFunction<T1>(
                    [f2, a2, b2, c2]
                    (const si::pressure p, const si::temperature T)
                    { return f2(a2(p,T).value(), b2(p,T).value(), c2(p,T).value()); }
                );
            }
            else // constant
            {
                OptionalStateParameters parameters = aggregate(a.parameters(), aggregate(b.parameters(), c.parameters()));
                // NOTE: The values in "dummy" are never read, since a, b, and c are not functions by this point, 
                // and we only record values in a SpectralSample if exactly one of a, b, and c are defined.
                StateParameters dummy(si::standard_pressure, si::standard_temperature);
                StateParameters defaults = parameters.value_or(dummy);
                si::pressure p = defaults.pressure;
                si::temperature T = defaults.temperature;
                T1 result = f(a(p,T).value(), b(p,T).value(), c(p,T).value());
                return parameters.has_value()? OptionalStateField<T1>(StateSample<T1>(result, p,T)) : OptionalStateField<T1>(result);
            }
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
        constexpr OptionalStateParameters parameters() const
        {
            return std::visit(OptionalStateFieldParametersVisitor(), entry);
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