#pragma once

// in-house libraries
#include "OptionalStateRecord.hpp"
#include "CompletedStateRecord.hpp"

namespace compound { 
namespace record {
    namespace {
        template<typename T1>
        class OptionalStateRecordCompletionVisitor
        {
            CompletedStateRecord<T1> fallback;
        public:
            OptionalStateRecordCompletionVisitor(CompletedStateRecord<T1> fallback) : fallback(fallback)
            {

            }
            CompletedStateRecord<T1> operator()(const std::monostate a               ) const {
                return fallback;
            }
            CompletedStateRecord<T1> operator()(const T1 a                           ) const {
                return CompletedStateRecord<T1>(a);
            }
            CompletedStateRecord<T1> operator()(const StateSampleRecord<T1> a        ) const {
                return CompletedStateRecord<T1>(a);
            }
            CompletedStateRecord<T1> operator()(const AuthoritativeStateRecord<T1> a ) const {
                return CompletedStateRecord<T1>(a);
            }
        };
    }
    /*
    Return a CompletedStateRecord<T1> record representing `a`. Underwrite with `b` if `a` is missing a value.
    */
    template<typename T1>
    constexpr CompletedStateRecord<T1> complete(const OptionalStateRecord<T1> a, const CompletedStateRecord<T1> b)
    {
        return std::visit(OptionalStateRecordCompletionVisitor<T1>(b), a.entry());
    }
}}