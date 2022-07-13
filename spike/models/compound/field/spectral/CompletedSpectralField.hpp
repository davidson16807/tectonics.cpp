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
            si::wavenumber<double> nlo;
            si::wavenumber<double> nhi;
            si::pressure<double> p;
            si::temperature<double> T;
        public:
            CompletedSpectralFieldValueVisitor(const si::wavenumber<double> nlo, const si::wavenumber<double> nhi, const si::pressure<double> p, const si::temperature<double> T)
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
                return [fcopy, a](const si::wavenumber<double> nlo, const si::wavenumber<double> nhi, const si::pressure<double> p, const si::temperature<double> T){ return fcopy(a(nlo, nhi, p, T)); };
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
            si::wavenumber<double> default_nlo;
            si::wavenumber<double> default_nhi;
            si::pressure<double> default_p;
            si::temperature<double> default_T;
            std::function<T2(T1, si::wavenumber<double>, si::wavenumber<double>, const si::pressure<double>, si::temperature<double>)> f;
        public:
            CompletedSpectralFieldMapToConstantVisitor(
                const si::wavenumber<double> default_nlo, 
                const si::wavenumber<double> default_nhi, 
                const si::pressure<double> default_p, 
                const si::temperature<double> default_T,
                const std::function<T2(const T1, const si::wavenumber<double>, const si::wavenumber<double>, const si::pressure<double>, const si::temperature<double>)> f
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
                const si::wavenumber<double> nlo,
                const si::wavenumber<double> nhi,
                const si::pressure<double> p, 
                const si::temperature<double> T
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
                const si::wavenumber<double> nlo,
                const si::wavenumber<double> nhi,
                const si::pressure<double> p, 
                const si::temperature<double> T
              ){
                return value;
              });
        	return *this;
        }
        constexpr T1 operator()(const si::wavenumber<double> nlo, const si::wavenumber<double> nhi, const si::pressure<double> p, const si::temperature<double> T) const
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