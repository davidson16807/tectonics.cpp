#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include <units/si.hpp>

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
            std::optional<Tout> operator()(const Tout a                           ) const {
                return a;
            }
            std::optional<Tout> operator()(const SpectralSample<Tout> a        ) const {
                return a.entry;
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
            OptionalSpectralFieldVariant<Tout> operator()(const Tin1 a                           ) const {
                return f(a);
            }
            OptionalSpectralFieldVariant<Tout> operator()(const SpectralSample<Tin1> a        ) const {
                return SpectralSample<Tin1>(f(a.entry), a.nlo, a.nhi, a.pressure, a.temperature);
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
        constexpr OptionalSpectralField(const std::optional<T1> option)
        : entry(option.has_value()? OptionalSpectralFieldVariant<T1>(option.value()) : OptionalSpectralFieldVariant<T1>(std::monostate()))
        {

        }
        constexpr OptionalSpectralField(const std::optional<SpectralSample<T1>> option)
        : entry(option.has_value()? OptionalSpectralFieldVariant<T1>(option.value()) : OptionalSpectralFieldVariant<T1>(std::monostate()))
        {

        }
        constexpr OptionalSpectralField(const std::optional<SpectralFunction<T1>> option)
        : entry(option.has_value()? OptionalSpectralFieldVariant<T1>(option.value()) : OptionalSpectralFieldVariant<T1>(std::monostate()))
        {

        }
    	template<typename T2>
        constexpr OptionalSpectralField<T1>& operator=(const T2& other)
        {
        	entry = OptionalSpectralFieldVariant<T1>(other);
        	return *this;
        }
        constexpr std::optional<T1> operator()(const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T) const
        {
            return std::visit(OptionalSpectralFieldValueVisitor<T1>(nlo, nhi, p, T), entry);
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
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameter `a`
        */
        template<typename T2>
        constexpr OptionalSpectralField<T1> value_or(
            const std::function<T1(const T2)> f, OptionalSpectralField<T2> a) const
        {
            return entry.index() > 0? *this 
                : OptionalSpectralField<T2>(std::visit(OptionalSpectralFieldUnaryMapVisitor<T1,T2>(f), a.entry));
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename T2, typename T3>
        constexpr OptionalSpectralField<T1> value_or(
            const std::function<T1(const T2, const T3)> f, 
            const OptionalSpectralField<T2> a, 
            const OptionalSpectralField<T3> b) const
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
                return SpectralFunction<T1>(
                    [f2, a2, b2]
                    (const si::wavenumber nlo, const si::wavenumber nhi,  const si::pressure p, const si::temperature T)
                    { return f2(a2(nlo,nhi,p,T).value(), b2(nlo,nhi,p,T).value()); }
                );
            }
            else if((a.index() == 2) + (b.index() == 2) == 1) // one sample
            {
                auto f2 = f;
                auto a2 = a;
                auto b2 = b;
                const si::wavenumber nlo = 14286.0/si::centimeter;
                const si::wavenumber nhi = 25000.0/si::centimeter;
                const si::pressure p = si::standard_pressure;
                const si::temperature T = si::standard_temperature;
                typedef std::function<SpectralSample<T1>(const T2, const si::wavenumber, const si::wavenumber, const si::pressure, const si::temperature)> F2nnpT;
                typedef std::function<SpectralSample<T1>(const T3, const si::wavenumber, const si::wavenumber, const si::pressure, const si::temperature)> F3nnpT;
                if(a.index() == 2)
                {
                    return a.map_to_constant(nlo,nhi,p,T, 
                        F2nnpT([f2,b2]
                            (const T2 apT, const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T)
                            { return SpectralSample<T1>(f2(apT,b2(nlo,nhi,p,T).value()),nlo,nhi,p,T); }
                        )
                    );
                }
                else // if(b.index() == 2)
                {
                    return b.map_to_constant(nlo,nhi,p,T, 
                        F3nnpT([f2,a2]
                            (const T3 bpT, const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T)
                            { return SpectralSample<T1>(f2(a2(nlo,nhi,p,T).value(),bpT),nlo,nhi,p,T); }
                        )
                    );
                }
            }
            else // constant
            {
                const si::wavenumber nlo = 14286.0/si::centimeter;
                const si::wavenumber nhi = 25000.0/si::centimeter;
                const si::pressure p = si::standard_pressure;
                const si::temperature T = si::standard_temperature;
                return f(a(nlo,nhi,p,T).value(), b(nlo,nhi,p,T).value());
            }
        }
        /*
        Return `this` if a value exists, otherwise return the result of function `f` applied to parameters `a` and `b`
        */
        template<typename T2, typename T3, typename T4>
        constexpr OptionalSpectralField<T1> value_or(
            const std::function<T1(const T2, const T3, const T4)> f, 
            const OptionalSpectralField<T2> a, 
            const OptionalSpectralField<T3> b, 
            const OptionalSpectralField<T4> c) const
        {
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
                return SpectralFunction<T1>(
                    [f2, a2, b2, c2]
                    (const si::wavenumber nlo, const si::wavenumber nhi,  const si::pressure p, const si::temperature T)
                    { return f2(a2(nlo,nhi,p,T).value(), b2(nlo,nhi,p,T).value(), c2(nlo,nhi,p,T).value()); }
                );
            }
            else if((a.index() == 2) + (b.index() == 2) + (c.index() == 2) == 1) // one sample
            {
                auto f2 = f;
                auto a2 = a;
                auto b2 = b;
                auto c2 = c;
                const si::wavenumber nlo = 14286.0/si::centimeter;
                const si::wavenumber nhi = 25000.0/si::centimeter;
                const si::pressure p = si::standard_pressure;
                const si::temperature T = si::standard_temperature;
                typedef std::function<SpectralSample<T1>(const T2, const si::wavenumber, const si::wavenumber, const si::pressure, const si::temperature)> F2nnpT;
                typedef std::function<SpectralSample<T1>(const T3, const si::wavenumber, const si::wavenumber, const si::pressure, const si::temperature)> F3nnpT;
                typedef std::function<SpectralSample<T1>(const T4, const si::wavenumber, const si::wavenumber, const si::pressure, const si::temperature)> F4nnpT;
                if(a.index() == 2)
                {
                    return a.map_to_constant(nlo,nhi,p,T, 
                        F2nnpT([f2,b2,c2]
                            (const T2 apT, const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T)
                            { return SpectralSample<T1>(f2(apT,b2(nlo,nhi,p,T).value(),c2(nlo,nhi,p,T).value()),nlo,nhi,p,T); }
                        )
                    );
                }
                else if(b.index() == 2)
                {
                    return b.map_to_constant(nlo,nhi,p,T, 
                        F3nnpT([f2,a2,c2]
                            (const T3 bpT, const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T)
                            { return SpectralSample<T1>(f2(a2(nlo,nhi,p,T).value(),bpT,c2(nlo,nhi,p,T).value()),nlo,nhi,p,T); }
                        )
                    );
                }
                else // if(c.index() == 2)
                {
                    return c.map_to_constant(nlo,nhi,p,T, 
                        F4nnpT([f2,a2,b2]
                            (const T4 cpT, const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T)
                            { return SpectralSample<T1>(f2(a2(nlo,nhi,p,T).value(),b2(nlo,nhi,p,T).value(),cpT),nlo,nhi,p,T); }
                        )
                    );
                }
            }
            else
            {
                const si::wavenumber nlo = 14286.0/si::centimeter;
                const si::wavenumber nhi = 25000.0/si::centimeter;
                const si::pressure p = si::standard_pressure;
                const si::temperature T = si::standard_temperature;
                return f(a(nlo,nhi,p,T).value(), b(nlo,nhi,p,T).value(), c(nlo,nhi,p,T).value());
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
        /*
        Return a OptionalSpectralField<T1> field representing `a` after applying the map `f`
        */
        template<typename T2>
        constexpr OptionalSpectralField<T2> map(const std::function<T2(const T1)> f) const
        {
            return OptionalSpectralField<T2>(std::visit(OptionalSpectralFieldUnaryMapVisitor<T2,T1>(f), entry));
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