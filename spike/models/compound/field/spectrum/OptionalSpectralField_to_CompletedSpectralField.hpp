#pragma once

// in-house libraries
#include "OptionalSpectralField.hpp"
#include "CompletedSpectralField.hpp"

namespace compound { 
namespace field {
    namespace {
        template<typename T1>
        class OptionalSpectralFieldCompletionVisitor
        {
            CompletedSpectralField<T1> fallback;
        public:
            OptionalSpectralFieldCompletionVisitor(CompletedSpectralField<T1> fallback) : fallback(fallback)
            {

            }
            CompletedSpectralField<T1> operator()(const std::monostate a               ) const {
                return fallback;
            }
            CompletedSpectralField<T1> operator()(const T1 a                           ) const {
                return CompletedSpectralField<T1>(a);
            }
            CompletedSpectralField<T1> operator()(const SpectralSample<T1> a        ) const {
                return CompletedSpectralField<T1>(a);
            }
            CompletedSpectralField<T1> operator()(const SpectralFunction<T1> a ) const {
                return CompletedSpectralField<T1>(a);
            }
        };
    }
    /*
    Return a CompletedSpectralField<T1> field representing `a`. Underwrite with `b` if `a` is missing a value.
    */
    template<typename T1>
    constexpr CompletedSpectralField<T1> complete(const OptionalSpectralField<T1> a, const CompletedSpectralField<T1> b)
    {
        return std::visit(OptionalSpectralFieldCompletionVisitor<T1>(b), a.entry());
    }
}}