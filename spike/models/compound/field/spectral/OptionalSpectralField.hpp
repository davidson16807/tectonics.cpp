#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include <si.hpp>

#include "SpectralSample.hpp"
#include "SpectralFunction.hpp"

#include "OptionalSpectralField.hpp"

namespace compound { 
namespace field {

    template<typename T1>
    class OptionalSpectralField
    {
    	template<typename T2>
	    using OptionalSpectralFieldVariant = std::variant<std::monostate, T2, SpectralSample<T2>, SpectralFunction<T2>>;
        template<typename T2>
        using CompletedSpectralFieldVariant = std::variant<T2, SpectralSample<T2>, SpectralFunction<T2>>;

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
            bool operator()(const T1 a                           ) const {
                return other == a;
            }
            bool operator()(const SpectralSample<T1> a        ) const {
                return other == a;
            }
            bool operator()(const SpectralFunction<T1> a ) const {
                return other(0.0 * si::pascal, 0.0* si::kelvin) == a(0.0 * si::pascal, 0.0* si::kelvin);
            }
        };
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
            std::optional<T1> operator()(const std::monostate a               ) const {
                return std::optional<T1>();
            }
            std::optional<T1> operator()(const T1 a                           ) const {
                return a;
            }
            std::optional<T1> operator()(const SpectralSample<T1> a        ) const {
                return a.entry;
            }
            std::optional<T1> operator()(const SpectralFunction<T1> a ) const {
                return a(nlo, nhi,p,T);
            }
        };
        template<typename T2>
        class OptionalSpectralFieldMapVisitor
        {
        public:
            typedef std::function<T2(const T1)> F;
            F f;
            OptionalSpectralFieldMapVisitor(const F f)
            : f(f)
            {

            }
            OptionalSpectralFieldVariant<T2> operator()(const std::monostate a               ) const {
                return std::monostate();
            }
            OptionalSpectralFieldVariant<T2> operator()(const T1 a                           ) const {
                return f(a);
            }
            OptionalSpectralFieldVariant<T2> operator()(const SpectralSample<T1> a        ) const {
                return SpectralSample<T1>(f(a.entry), a.nlo, a.nhi, a.pressure, a.temperature);
            }
            OptionalSpectralFieldVariant<T2> operator()(const SpectralFunction<T1> a ) const {
                // NOTE: capturing `this` results in a segfault when we call `this->f()`
                // This occurs even when we capture `this` by entry.
                // I haven't clarified the reason, but it seems likely related to accessing 
                // the `f` attribute in a object that destructs after we exit out of the `map` function.
                auto fcopy = f; 
                return [fcopy, a](const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ return fcopy(a(nlo, nhi, p, T)); };
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
            std::optional<T2> operator()( const T1 a                           ) const {
                return f(a, default_nlo, default_nhi, default_p, default_T);
            }
            std::optional<T2> operator()( const SpectralSample<T1> a        ) const {
                return f(a.entry, a.nlo, a.nhi, a.pressure, a.temperature);
            }
            std::optional<T2> operator()( const SpectralFunction<T1> a ) const {
                return f(a(default_nlo, default_nhi, default_p, default_T), default_nlo, default_nhi, default_p, default_T);
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
            CompletedSpectralFieldVariant<T1> operator()(const T1 a                           ) const {
                return a;
            }
            CompletedSpectralFieldVariant<T1> operator()(const SpectralSample<T1> a        ) const {
                return a;
            }
            CompletedSpectralFieldVariant<T1> operator()(const SpectralFunction<T1> a ) const {
                return a;
            }
        };

        OptionalSpectralFieldVariant<T1> entry;
    public:
        constexpr OptionalSpectralField(const OptionalSpectralFieldVariant<T1> entry)
        : entry(entry)
        {

        }
        constexpr OptionalSpectralField(const std::monostate entry)
        : entry(entry)
        {

        }
        constexpr OptionalSpectralField(const T1 entry)
        : entry(entry)
        {

        }
        constexpr OptionalSpectralField(const SpectralSample<T1> entry)
        : entry(entry)
        {

        }
        constexpr OptionalSpectralField(const SpectralFunction<T1> entry)
        : entry(entry)
        {

        }
    	template<typename T2>
        constexpr OptionalSpectralField<T1>& operator=(const T2& other)
        {
        	entry = OptionalSpectralFieldVariant<T1>(other);
        	return *this;
        }
        constexpr bool operator==(const OptionalSpectralField<T1> other) const
        {
            // std::function does not and cannot have a comparator,
            // so equality can only be determined pragmatically by sampling at given pressures/temperatures
            if(entry.index() != other.entry.index()){
                return false;
            }
            auto max_llo = 1000.0 * si::nanometer;
            auto max_lhi = 1000.0 * si::nanometer;
            auto max_p = si::standard_pressure;
            auto max_T = si::standard_temperature;
            auto f = std::function<SpectralSample<T1>(const T1, const si::wavenumber, const si::wavenumber, const si::pressure, const si::temperature)>(
                [](const T1 x, const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ 
                    return SpectralSample<T1>(x,nlo,nhi,p,T);
                }
            );
            for(si::pressure p = 0.0*si::pascal; p<=max_p; p+=max_p/2.0)
            {
                for(si::temperature T = 0.0*si::kelvin; T<=max_T; T+=max_T/2.0)
                {
                    for(si::length llo = 100.0*si::nanometer; llo<=max_llo; llo+=200.0*si::nanometer)
                    {
                        for(si::length lhi = 100.0*si::nanometer; lhi<=max_lhi; lhi+=200.0*si::nanometer)
                        {
                            if( map_to_constant(1.0/lhi, 1.0/llo, p, T, f) != 
                                other.map_to_constant(1.0/lhi, 1.0/llo, p, T, f) ){
                                return false;
                            }
                        }
                    }
                }
            }
            return true;
        }
        constexpr std::optional<T1> operator()(const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T) const
        {
            return std::visit(OptionalSpectralFieldValueVisitor(nlo, nhi, p, T), entry);
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        constexpr OptionalSpectralField<T1> substitute(const OptionalSpectralField<T1> other) const
        {
            return entry.index() > 0? *this : other;
        }
        /*
        Return whichever field provides more information, going by the following definition:
            std::monostate < T1 < SpectralFunction<T1> < std::pair<T1, SpectralFunction<T1>>
        If both provide the same amount of information, return `a` by default.
        */
        constexpr OptionalSpectralField<T1> compare(const OptionalSpectralField<T1> other) const
        {
            return entry.index() >= other.entry.index()? *this : other;
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        constexpr OptionalSpectralField<T1> value_or(const OptionalSpectralField<T1> other) const
        {
            return entry.index() > 0? *this : other;
        }
        constexpr CompletedSpectralFieldVariant<T1> value_or(CompletedSpectralFieldVariant<T1> fallback) const 
        {
            return std::visit(OptionalSpectralFieldCompletionVisitor(fallback), entry);
        }
        /*
		Return whether a entry exists within the field
        */
        constexpr bool has_entry() const
        {
            return entry.index() == 0;
        }
        /*
		Return whether a entry exists within the field
        */
        constexpr int index() const
        {
            return entry.index();
        }
        /*
        Return a OptionalSpectralField<T1> field representing `a` after applying the map `f`
        */
        template<typename T2>
        constexpr OptionalSpectralField<T2> map(const std::function<T2(const T1)> f) const
        {
            return OptionalSpectralField<T2>(std::visit(OptionalSpectralFieldMapVisitor<T2>(f), entry));
        }
        /*
        Return a OptionalSpectralField<T1> field representing `a` after applying the map `f`
        */
        template<typename T2>
        constexpr std::optional<T2> map_to_constant(
            const si::wavenumber default_nlo,
            const si::wavenumber default_nhi,
            const si::pressure default_p, 
            const si::temperature default_T, 
            const std::function<T2(const T1, const si::wavenumber, const si::wavenumber, const si::pressure, const si::temperature)> f
        ) const {
            return std::visit(OptionalSpectralFieldMapToConstantVisitor<T2>(default_nlo, default_nhi, default_p, default_T, f), entry);
        }

        template<typename T2>
		friend class OptionalSpectralField;
    };
    
}}