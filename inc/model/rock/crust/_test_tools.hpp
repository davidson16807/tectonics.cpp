#pragma once

// std libraries
#include <random>

// 3rd party libraries
#include <catch/catch.hpp>

// in-house libraries
#include <unit/si.hpp>

#include <model/rock/column/_test_tools.hpp>

#include "CrustSummary.hpp"

namespace rock
{

    struct CrustSummaryAdapter{
        const ColumnSummaryAdapter columns;

        CrustSummaryAdapter()
        {}

        bool equal(const CrustSummary& a, const CrustSummary& b) const {
            if (a.size() != b.size())
            {
                return false;
            }

            for (std::size_t i = 0; i < a.size(); ++i)
            {
                if (!columns.equal(a[i],b[i]))
                {
                    return false;
                }
            }

            return true;
        }

        std::string print(const CrustSummary& a) const {
            std::ostringstream os;
            for (std::size_t i = 0; i < a.size(); ++i)
            {
                os << columns.print(a[i]);
            }
            return os.str();
        }

    };

}

