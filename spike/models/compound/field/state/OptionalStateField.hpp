#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include <units/si.hpp>

#include "StateFunction.hpp"

#include "OptionalStateParameters.hpp"

namespace compound { 
namespace field {

    template<typename T1>
    class OptionalStateField
    {
    	template<typename T2>
        using OptionalStateFieldVariant = std::variant<std::monostate, StateFunction<T2>>;
        template<typename T2>
	    using CompletedStateFieldVariant = std::variant<StateFunction<T2>>;

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
            std::optional<Tout> operator()(const std::monostate a      ) const {
                return std::optional<Tout>();
            }
            std::optional<Tout> operator()(const StateFunction<Tout> a ) const {
                return a(p,T);
            }
        };
        class OptionalStateFieldParametersVisitor
        {
        public:
            OptionalStateFieldParametersVisitor()
            {

            }
            OptionalStateParameters operator()( const std::monostate a          ) const {
                return std::monostate();
            }
            OptionalStateParameters operator()( const StateFunction<T1> a       ) const {
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
            CompletedStateFieldVariant<T1> operator()(const std::monostate a    ) const {
                return fallback;
            }
            CompletedStateFieldVariant<T1> operator()(const StateFunction<T1> a ) const {
                return a;
            }
        };
        class CompletedStateFieldOptionalizeVisitor
        {
        public:
            CompletedStateFieldOptionalizeVisitor()
            {

            }
            OptionalStateFieldVariant<T1> operator()(const StateFunction<T1> a ) const {
                return OptionalStateFieldVariant<T1>(a);
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
        constexpr OptionalStateField(const T1 value)
        {
            auto value2 = value;
            entry = StateFunction<T1>([value2](
                const si::pressure p, 
                const si::temperature T
              ){
                return value2;
              });
        }
        constexpr OptionalStateField(const StateFunction<T1> entry)
        : entry(entry)
        {

        }
        constexpr OptionalStateField(const std::monostate entry)
        : entry(entry)
        {

        }
        constexpr OptionalStateField(const std::optional<T1> option)
        {
            auto value = option.value();
            entry = option.has_value()? OptionalStateFieldVariant<T1>(
                field::StateFunction<T1>(
                    [value]
                    (const si::pressure p, 
                     const si::temperature T)
                    {
                        return value;
                    })
                ) : OptionalStateFieldVariant<T1>(std::monostate());
        }
        constexpr OptionalStateField(const std::optional<StateFunction<T1>> option)
        {
            auto value = option.value();
            entry = option.has_value()? OptionalStateFieldVariant<T1>(value) : OptionalStateFieldVariant<T1>(std::monostate());
        }

        constexpr OptionalStateField<T1>& operator=(const std::monostate other)
        {
            entry = other;
            return *this;
        }
        constexpr OptionalStateField<T1>& operator=(const T1 other)
        {
            auto value = other;
            entry = StateFunction<T1>([value](
                const si::pressure p, 
                const si::temperature T
              ){
                return value;
              });
            return *this;
        }
        constexpr OptionalStateField<T1>& operator=(const StateFunction<T1> other)
        {
            entry = other;
            return *this;
        }
        constexpr OptionalStateField<T1>& operator=(const CompletedStateFieldVariant<T1> other)
        {
            entry = std::visit(CompletedStateFieldOptionalizeVisitor(), other);
            return *this;
        }

        constexpr std::optional<T1> operator()(const si::pressure p, const si::temperature T) const
        {
            return std::visit(OptionalStateFieldValueVisitor<T1>(p, T), entry);
        }
        constexpr std::optional<T1> operator()(const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T) const
        {
            return std::visit(OptionalStateFieldValueVisitor<T1>(p, T), entry);
        }
        constexpr std::optional<T1> operator()(const StateParameters parameters) const
        {
            return std::visit(OptionalStateFieldValueVisitor<T1>(parameters.pressure, parameters.temperature), entry);
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
        constexpr CompletedStateFieldVariant<T1> complete(const T1 fallback) const 
        {
            auto fallback2 = fallback;
            StateFunction<T1> f = StateFunction<T1>([fallback2](
                const si::pressure p, 
                const si::temperature T
              ){
                return fallback2;
              });
            return std::visit(OptionalStateFieldCompletionVisitor(f), entry);
        }
        constexpr CompletedStateFieldVariant<T1> complete(const CompletedStateFieldVariant<T1> fallback) const 
        {
            return std::visit(OptionalStateFieldCompletionVisitor(fallback), entry);
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        constexpr OptionalStateField<T1> value_or(const OptionalStateField<T1> other) const
        {
            return has_value()? *this : other;
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameter `a`
        */
        template<typename T2>
        constexpr OptionalStateField<T1> value_or(const std::function<T1(const typename T2::value_type)> f, const T2 a) const
        {
            if(has_value()) // no substitute needed
            {
                return *this;
            }
            else if(!a.has_value()) // any monostates
            {
                return std::monostate();
            }
            else
            {
                auto f2 = f;
                auto a2 = a;
                return StateFunction<T1>(
                    [f2, a2]
                    (const si::pressure p, const si::temperature T)
                    { return f2(a2(p,T).value()); }
                );
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename T2, typename T3>
        constexpr OptionalStateField<T1> value_or(
            const std::function<T1(const typename T2::value_type, const typename T3::value_type)> f, 
            const T2 a, 
            const T3 b) const
        {
            if(has_value()) // no substitute needed
            {
                return *this;
            }
            else if(!a.has_value() || !b.has_value()) // any monostates
            {
                return std::monostate();
            }
            else
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
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename T2, typename T3, typename T4>
        constexpr OptionalStateField<T1> value_or(
            const std::function<T1(const typename T2::value_type, const typename T3::value_type, const typename T4::value_type)> f, 
            const T2 a, 
            const T3 b, 
            const T4 c) const
        {
            // the following are dummy values, since we only invoke them on StateSamples or constants
            const si::pressure p = si::standard_pressure;
            const si::temperature T = si::standard_temperature;
            if(has_value())
            {
                return *this;
            }
            else if(!a.has_value() || !b.has_value() || !c.has_value()) // any monostates
            {
                return std::monostate();
            }
            else
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
        }




        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameter `a`
        */
        template<typename T2>
        constexpr OptionalStateField<T1> value_or(
            const std::function<T1(const StateParameters, const typename T2::value_type)> f, 
            const StateParameters defaults,
            const T2 a) const
        {
            if(has_value()) // no substitute needed
            {
                return *this;
            }
            else if(!a.has_value()) // any monostates
            {
                return std::monostate();
            }
            else
            {
                auto f2 = f;
                auto a2 = a;
                return StateFunction<T1>(
                    [f2, a2]
                    (const si::pressure p, const si::temperature T)
                    { return f2(StateParameters(p,T), a2(p,T).value()); }
                );
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename T2, typename T3>
        constexpr OptionalStateField<T1> value_or(
            const std::function<T1(const StateParameters, const typename T2::value_type, const typename T3::value_type)> f, 
            const StateParameters defaults,
            const T2 a, 
            const T3 b) const
        {
            if(has_value()) // no substitute needed
            {
                return *this;
            }
            else if(!a.has_value() || !b.has_value()) // any monostates
            {
                return std::monostate();
            }
            else
            {
                auto f2 = f;
                auto a2 = a;
                auto b2 = b;
                return StateFunction<T1>(
                    [f2, a2, b2]
                    (const si::pressure p, const si::temperature T)
                    { return f2(StateParameters(p,T), a2(p,T).value(), b2(p,T).value()); }
                );
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename T2, typename T3, typename T4>
        constexpr OptionalStateField<T1> value_or(
            const std::function<T1(const StateParameters, const typename T2::value_type, const typename T3::value_type, const typename T4::value_type)> f, 
            const StateParameters defaults,
            const T2 a, 
            const T3 b, 
            const T4 c) const
        {
            if(has_value())
            {
                return *this;
            }
            else if(!a.has_value() || !b.has_value() || !c.has_value()) // any monostates
            {
                return std::monostate();
            }
            else
            {
                auto f2 = f;
                auto a2 = a;
                auto b2 = b;
                auto c2 = c;
                return StateFunction<T1>(
                    [f2, a2, b2, c2]
                    (const si::pressure p, const si::temperature T)
                    { return f2(StateParameters(p,T), a2(p,T).value(), b2(p,T).value(), c2(p,T).value()); }
                );
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename T2, typename T3, typename T4, typename T5>
        constexpr OptionalStateField<T1> value_or(
            const std::function<T1(const StateParameters, const typename T2::value_type, const typename T3::value_type, const typename T4::value_type, const typename T5::value_type)> f, 
            const StateParameters defaults,
            const T2 a, 
            const T3 b, 
            const T4 c, 
            const T5 d) const
        {
            if(has_value())
            {
                return *this;
            }
            else if(!a.has_value() || !b.has_value() || !c.has_value() || !d.has_value()) // any monostates
            {
                return std::monostate();
            }
            else
            {
                auto f2 = f;
                auto a2 = a;
                auto b2 = b;
                auto c2 = c;
                auto d2 = d;
                return StateFunction<T1>(
                    [f2, a2, b2, c2, d2]
                    (const si::pressure p, const si::temperature T)
                    { return f2(StateParameters(p,T), a2(p,T).value(), b2(p,T).value(), c2(p,T).value(), d2(p,T).value()); }
                );
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


        template<typename T2>
		friend class OptionalStateField;
    };
    
  
}}