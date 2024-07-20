#pragma once

// std libraries
#include <random>

// 3rd party libraries
#include <catch/catch.hpp>

// in-house libraries
#include <unit/si.hpp>

#include <model/rock/stratum/_test_tools.hpp>

#include "ColumnSummary.hpp"

namespace rock
{

    struct ColumnSummaryAdapter{
        const StratumSummaryAdapter strata;

        ColumnSummaryAdapter()
        {}

        bool equal(const ColumnSummary& a, const ColumnSummary& b) const {

            if (!strata.equal(a.top, b.top))
            {
                return false;
            }

            if (!strata.equal(a.rest, b.rest))
            {
                return false;
            }

            return true;
        }

        std::string print(const ColumnSummary& a) const {
            std::ostringstream os;
            os << "top: " << std::endl << strata.print(a.top);
            os << "rest:" << std::endl << strata.print(a.rest);
            return os.str();
        }

        std::string print(const StratumSummary& a) const {
            return strata.print(a);
        }

    };

}

