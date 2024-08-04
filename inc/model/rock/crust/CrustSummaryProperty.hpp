#pragma once

#include <cmath>

#include <vector>

#include <unit/si.hpp>

#include "CrustSummary.hpp"

/*
*/

namespace rock {

    /*
    `CrustSummaryProperty` allows creation of a generic Unary CrustSummary callable given an arbitrary StratumSummaryProperty.
    It provides a more generic alternative to `CrustSummaryPredicates`
    */
    template <typename ColumnSummaryProperty>
    class CrustSummaryProperty
    {
        const ColumnSummaryProperty property;
    public:
        CrustSummaryProperty(const ColumnSummaryProperty& property):
            property(property)
        {}
        void operator()(const CrustSummary& summary, auto& out) const
        {
            for (auto i = 0*summary.size(); i < summary.size(); ++i)
            {
                out[i] = property(summary[i]);
            }
        }
    };


}

