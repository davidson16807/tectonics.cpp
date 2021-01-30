#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include "StateSampleRecord.hpp"
#include "AuthoritativeStateRecord.hpp"

namespace compound { 
namespace record {
	template<typename T1>
    using OptionalStateRecord = std::variant<std::monostate, T1, StateSampleRecord<T1>, AuthoritativeStateRecord<T1>>;
}}