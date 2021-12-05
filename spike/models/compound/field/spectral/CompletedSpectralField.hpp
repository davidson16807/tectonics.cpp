#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include <units/si.hpp>

#include "SpectralSample.hpp"
#include "SpectralFunction.hpp"
#include "SpectralParameters.hpp"

#include "CompletedSpectralField.hpp"

namespace compound { 
namespace field {

    template<typename T1>
    class CompletedSpectralField
    {
    	template<typename T2>
	    using CompletedSpectralFieldVariant = std::variant<SpectralFunction<T2>>;

        class CompletedSpectralFieldValueVisitor
        {
            si::wavenumber nlo;
            si::wavenumber nhi;
            si::pressure p;
            si::temperature T;
        public:
            CompletedSpectralFieldValueVisitor(const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T)
            : nlo(nlo), nhi(nhi), p(p), T(T)
            {

            }
            T1 operator()(const SpectralFunction<T1> a ) const {
                return a(nlo, nhi,p,T);
            }
        };
        template<typename T2>
        class CompletedSpectralFieldMapVisitor
        {
        public:
            typedef std::function<T2(const T1)> F;
            F f;
            CompletedSpectralFieldMapVisitor(const F f)
            : f(f)
            {

            }
            CompletedSpectralFieldVariant<T2> operator()(const SpectralFunction<T1> a ) const {
                // NOTE: capturing `this` results in a segfault when we call `this->f()`
                // This occurs even when we capture `this` by entry.
                // I haven't clarified the reason, but it seems likely related to accessing 
                // the `f` attribute in a object that destructs after we exit out of the `map` function.
                auto fcopy = f; 
                return [fcopy, a](const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ return fcopy(a(nlo, nhi, p, T)); };
            }
        };
        class CompletedSpectralFieldFunctionVisitor
        {
        public:
            CompletedSpectralFieldFunctionVisitor()
            {

            }
            SpectralFunction<T1> operator()(const SpectralFunction<T1> a) const {
                // NOTE: capturing `this` results in a segfault when we call `this->f()`
                // This occurs even when we capture `this` by entry.
                // I haven't clarified the reason, but it seems likely related to accessing 
                // the `f` attribute in a object that destructs after we exit out of the `map` function.
                return a;
            }
        };
        template<typename T2>
        class CompletedSpectralFieldMapToConstantVisitor
        {
            si::wavenumber default_nlo;
            si::wavenumber default_nhi;
            si::pressure default_p;
            si::temperature default_T;
            std::function<T2(T1, si::wavenumber, si::wavenumber, const si::pressure, si::temperature)> f;
        public:
            CompletedSpectralFieldMapToConstantVisitor(
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
            T2 operator()( const SpectralFunction<T1> a ) const {
                return f(a(default_nlo, default_nhi, default_p, default_T), default_nlo, default_nhi, default_p, default_T);
            }
        };

        CompletedSpectralFieldVariant<T1> entry;
    public:
        constexpr CompletedSpectralField(const CompletedSpectralFieldVariant<T1> entry)
        : entry(entry)
        {

        }
        constexpr CompletedSpectralField(const T1 value)
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
        constexpr CompletedSpectralField(const SpectralFunction<T1> entry)
        : entry(entry)
        {

        }
    	template<typename T2>
        constexpr CompletedSpectralField<T1>& operator=(const T2& other)
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
        constexpr T1 operator()(const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T) const
        {
            return std::visit(CompletedSpectralFieldValueVisitor(nlo, nhi, p, T), entry);
        }
        constexpr T1 operator()(const SpectralParameters parameters) const
        {
            return std::visit(CompletedSpectralFieldValueVisitor(parameters.nlo, parameters.nhi, parameters.pressure, parameters.temperature), entry);
        }
        /*
        Return whichever field provides more information, going by the following definition:
            std::monostate < T1 < SpectralFunction<T1> < std::pair<T1, SpectralFunction<T1>>
        If both provide the same amount of information, return `a` by default.
        */
        constexpr CompletedSpectralField<T1> compare(const CompletedSpectralField<T1> other) const
        {
            return entry.index() >= other.entry.index()? *this : other;
        }
        constexpr CompletedSpectralFieldVariant<T1> value() const
        {
            return entry;
        }
        /*
		Return whether a entry exists within the field
        */
        constexpr int index() const
        {
            return entry.index();
        }

        template<typename T2>
		friend class CompletedSpectralField;
    };
    
}}