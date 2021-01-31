#pragma once

// in-house libraries
#include "OptionalStateField.hpp"
#include "CompletedStateField.hpp"

namespace compound { 
namespace field {
    namespace {
        template<typename T1>
        class OptionalStateFieldCompletionVisitor
        {
            CompletedStateField<T1> fallback;
        public:
            OptionalStateFieldCompletionVisitor(CompletedStateField<T1> fallback) : fallback(fallback)
            {

            }
            CompletedStateField<T1> operator()(const std::monostate a               ) const {
                return fallback;
            }
            CompletedStateField<T1> operator()(const T1 a                           ) const {
                return CompletedStateField<T1>(a);
            }
            CompletedStateField<T1> operator()(const StateSample<T1> a        ) const {
                return CompletedStateField<T1>(a);
            }
            CompletedStateField<T1> operator()(const StateFunction<T1> a ) const {
                return CompletedStateField<T1>(a);
            }
        };
    }
    /*
    Return a CompletedStateField<T1> field representing `a`. Underwrite with `b` if `a` is missing a value.
    */
    template<typename T1>
    constexpr CompletedStateField<T1> complete(const OptionalStateField<T1> a, const CompletedStateField<T1> b)
    {
        return std::visit(OptionalStateFieldCompletionVisitor<T1>(b), a.entry());
    }
}}