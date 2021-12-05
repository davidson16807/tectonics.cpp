#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include <units/si.hpp>

#include "SpectralSample.hpp"
#include "SpectralFunction.hpp"

#include "OptionalSpectralParameters.hpp"

namespace compound { 
namespace field {

    template<typename T1>
    class OptionalSpectralField
    {
        typedef std::variant<std::monostate, SpectralParameters, SpectralParametersAggregate> OptionalSpectralParametersVariant;
    	template<typename T2>
	    using OptionalSpectralFieldVariant = std::variant<std::monostate, SpectralFunction<T2>>;
        template<typename T2>
        using CompletedSpectralFieldVariant = std::variant<SpectralFunction<T2>>;

        class OptionalSpectralFieldEqualityVisitor
        {
            OptionalSpectralFieldVariant<T1> other;
        public:
            OptionalSpectralFieldEqualityVisitor(OptionalSpectralFieldVariant<T1> other)
            : other(other)
            {

            }
            bool operator()(const std::monostate a               ) const {
                return other == a;
            }
            bool operator()(const SpectralFunction<T1> a ) const {
                return other(0.0 * si::pascal, 0.0* si::kelvin) == a(0.0 * si::pascal, 0.0* si::kelvin);
            }
        };
        template<typename Tout>
        class OptionalSpectralFieldValueVisitor
        {
            si::wavenumber nlo;
            si::wavenumber nhi;
            si::pressure p;
            si::temperature T;
        public:
            OptionalSpectralFieldValueVisitor(const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T)
            : nlo(nlo), nhi(nhi), p(p), T(T)
            {

            }
            std::optional<Tout> operator()(const std::monostate a               ) const {
                return std::optional<Tout>();
            }
            std::optional<Tout> operator()(const SpectralFunction<Tout> a ) const {
                return a(nlo, nhi,p,T);
            }
        };
        template<typename T2>
        class OptionalSpectralFieldMapToConstantVisitor
        {
            si::wavenumber default_nlo;
            si::wavenumber default_nhi;
            si::pressure default_p;
            si::temperature default_T;
            std::function<T2(T1, si::wavenumber, si::wavenumber, si::pressure, si::temperature)> f;
        public:
            OptionalSpectralFieldMapToConstantVisitor(
                const si::wavenumber default_nlo, 
                const si::wavenumber default_nhi,
                const si::pressure default_p, 
                const si::temperature default_T,
                const std::function<T2(const T1, const si::wavenumber, const si::wavenumber, const si::pressure, const si::temperature)> f
            )
            : default_nlo(default_nlo), 
              default_nhi(default_nhi), 
              default_p(default_p), 
              default_T(default_T),
              f(f)
            {

            }
            std::optional<T2> operator()( const std::monostate a               ) const {
                return std::optional<T2>();
            }
            std::optional<T2> operator()( const SpectralFunction<T1> a ) const {
                return f(a(default_nlo, default_nhi, default_p, default_T), default_nlo, default_nhi, default_p, default_T);
            }
        };
        class OptionalSpectralFieldParametersVisitor
        {
        public:
            OptionalSpectralFieldParametersVisitor()
            {

            }
            OptionalSpectralParametersVariant operator()( const std::monostate a               ) const {
                return std::monostate();
            }
            OptionalSpectralParametersVariant operator()( const SpectralFunction<T1> a ) const {
                return std::monostate();
            }
        };
        class OptionalSpectralFieldCompletionVisitor
        {
            CompletedSpectralFieldVariant<T1> fallback;
        public:
            OptionalSpectralFieldCompletionVisitor(CompletedSpectralFieldVariant<T1> fallback) : fallback(fallback)
            {

            }
            CompletedSpectralFieldVariant<T1> operator()(const std::monostate a               ) const {
                return fallback;
            }
            CompletedSpectralFieldVariant<T1> operator()(const SpectralFunction<T1> a ) const {
                return a;
            }
        };
        class CompletedSpectralFieldOptionalizeVisitor
        {
        public:
            CompletedSpectralFieldOptionalizeVisitor()
            {

            }
            OptionalSpectralFieldVariant<T1> operator()(const SpectralFunction<T1> a ) const {
                return OptionalSpectralFieldVariant<T1>(a);
            }
        };

        template<typename Tout, typename Tin1>
        class OptionalSpectralFieldUnaryMapVisitor
        {
        public:
            typedef std::function<Tout(const Tin1)> F;
            F f;
            OptionalSpectralFieldUnaryMapVisitor(const F f)
            : f(f)
            {

            }
            OptionalSpectralFieldVariant<Tout> operator()(const std::monostate a               ) const {
                return std::monostate();
            }
            OptionalSpectralFieldVariant<Tout> operator()(const SpectralFunction<Tin1> a ) const {
                // NOTE: capturing `this` results in a segfault when we call `this->f()`
                // This occurs even when we capture `this` by entry.
                // I haven't clarified the reason, but it seems likely related to accessing 
                // the `f` attribute in a object that destructs after we exit out of the `map` function.
                auto fcopy = f; 
                return [fcopy, a](const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ return fcopy(a(nlo, nhi, p, T)); };
            }
        };
        OptionalSpectralFieldVariant<T1> entry;
    public:
        using value_type = T1;
        using parameter_type = SpectralParameters;
        constexpr OptionalSpectralField(const OptionalSpectralFieldVariant<T1> entry)
        : entry(entry)
        {

        }
        constexpr OptionalSpectralField(const std::monostate entry)
        : entry(entry)
        {

        }
        constexpr OptionalSpectralField(const T1 value)
        {
            auto value2 = value;
            entry = SpectralFunction<T1>([value2](
                const si::wavenumber nlo,
                const si::wavenumber nhi,
                const si::pressure p, 
                const si::temperature T
              ){
                return value2;
              });
        }
        constexpr OptionalSpectralField(const SpectralFunction<T1> entry)
        : entry(entry)
        {

        }

        constexpr OptionalSpectralField<T1>& operator=(const std::monostate other)
        {
            entry = other;
            return *this;
        }
        constexpr OptionalSpectralField<T1>& operator=(const T1 other)
        {
            auto value = other;
            entry = SpectralFunction<T1>([value](
                const si::wavenumber nlo,
                const si::wavenumber nhi,
                const si::pressure p, 
                const si::temperature T
              ){
                return value;
              });
            return *this;
        }
        constexpr OptionalSpectralField<T1>& operator=(const SpectralFunction<T1> other)
        {
            entry = other;
            return *this;
        }
        constexpr OptionalSpectralField<T1>& operator=(const CompletedSpectralFieldVariant<T1> other)
        {
            entry = std::visit(CompletedSpectralFieldOptionalizeVisitor(), other);
            return *this;
        }

        constexpr std::optional<T1> operator()(const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T) const
        {
            return std::visit(OptionalSpectralFieldValueVisitor<T1>(nlo, nhi, p, T), entry);
        }
        constexpr std::optional<T1> operator()(const SpectralParameters parameters) const
        {
            return std::visit(OptionalSpectralFieldValueVisitor<T1>(parameters.nlo, parameters.nhi, parameters.pressure, parameters.temperature), entry);
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        constexpr OptionalSpectralField<T1> substitute(const OptionalSpectralField<T1> other) const
        {
            return entry.index() > 0? *this : other;
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        constexpr OptionalSpectralField<T1> value_or(const OptionalSpectralField<T1> other) const
        {
            return entry.index() > 0? *this : other;
        }
        constexpr CompletedSpectralFieldVariant<T1> complete(CompletedSpectralFieldVariant<T1> fallback) const 
        {
            return std::visit(OptionalSpectralFieldCompletionVisitor(fallback), entry);
        }



        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameter `a`
        */
        template<typename T2>
        constexpr OptionalSpectralField<T1> value_or(const std::function<T1(const typename T2::value_type)> f, const T2 a) const
        {
            if(entry.index() > 0) // no substitute needed
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
                return SpectralFunction<T1>(
                    [f2, a2]
                    (const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T)
                    { return f2(a2(nlo,nhi,p,T).value()); }
                );
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename T2, typename T3>
        constexpr OptionalSpectralField<T1> value_or(
            const std::function<T1(const typename T2::value_type, const typename T3::value_type)> f, 
            const T2 a, 
            const T3 b) const
        {
            if(entry.index() > 0) // no substitute needed
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
                return SpectralFunction<T1>(
                    [f2, a2, b2]
                    (const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T)
                    { return f2(a2(nlo,nhi,p,T).value(), b2(nlo,nhi,p,T).value()); }
                );
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename T2, typename T3, typename T4>
        constexpr OptionalSpectralField<T1> value_or(
            const std::function<T1(const typename T2::value_type, const typename T3::value_type, const typename T4::value_type)> f, 
            const T2 a, 
            const T3 b, 
            const T4 c) const
        {
            // the following are dummy values, since we only invoke them on SpectralSamples or constants
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
            else
            {
                auto f2 = f;
                auto a2 = a;
                auto b2 = b;
                auto c2 = c;
                return SpectralFunction<T1>(
                    [f2, a2, b2, c2]
                    (const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T)
                    { return f2(a2(nlo,nhi,p,T).value(), b2(nlo,nhi,p,T).value(), c2(nlo,nhi,p,T).value()); }
                );
            }
        }




        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameter `a`
        */
        template<typename T2>
        constexpr OptionalSpectralField<T1> value_or(
            const std::function<T1(const SpectralParameters, const typename T2::value_type)> f, 
            const SpectralParameters defaults,
            const T2 a) const
        {
            if(entry.index() > 0) // no substitute needed
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
                return SpectralFunction<T1>(
                    [f2, a2]
                    (const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T)
                    { return f2(SpectralParameters(nlo,nhi,p,T), a2(nlo,nhi,p,T).value()); }
                );
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename T2, typename T3>
        constexpr OptionalSpectralField<T1> value_or(
            const std::function<T1(const SpectralParameters, const typename T2::value_type, const typename T3::value_type)> f, 
            const SpectralParameters defaults,
            const T2 a, 
            const T3 b) const
        {
            if(entry.index() > 0) // no substitute needed
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
                return SpectralFunction<T1>(
                    [f2, a2, b2]
                    (const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T)
                    { return f2(SpectralParameters(nlo,nhi,p,T), a2(nlo,nhi,p,T).value(), b2(nlo,nhi,p,T).value()); }
                );
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename T2, typename T3, typename T4>
        constexpr OptionalSpectralField<T1> value_or(
            const std::function<T1(const SpectralParameters, const typename T2::value_type, const typename T3::value_type, const typename T4::value_type)> f, 
            const SpectralParameters defaults,
            const T2 a, 
            const T3 b, 
            const T4 c) const
        {
            // the following are dummy values, since we only invoke them on SpectralSamples or constants
            if(entry.index() > 0)
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
                return SpectralFunction<T1>(
                    [f2, a2, b2, c2]
                    (const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T)
                    { return f2(SpectralParameters(nlo,nhi,p,T), a2(nlo,nhi,p,T).value(), b2(nlo,nhi,p,T).value(), c2(nlo,nhi,p,T).value()); }
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
        constexpr OptionalSpectralParametersVariant parameters() const
        {
            return std::visit(OptionalSpectralFieldParametersVisitor(), entry);
        };


        template<typename T2>
		friend class OptionalSpectralField;
    };
    
}}