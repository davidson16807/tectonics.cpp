#pragma once

#include <model/rock/formation/FormationSummary.hpp>

#include <model/rock/column/ColumnSummary.hpp>
#include <model/rock/column/ColumnSummaryOps.hpp>

namespace rock{


    class CrustSummaryOps
    {

        const ColumnSummaryOps ops;

    public:

        CrustSummaryOps(const ColumnSummaryOps& ops): 
            ops(ops)
        {}

        void absorb (const CrustSummary& crust, const FormationSummary& formation, CrustSummary& out) const
        {
            for (auto i = 0*out.size(); i < out.size(); ++i)
            {
                out[i] = ops.absorb(crust[i], formation[i]);
            }
        }

        void absorb (const CrustSummary& crust, const CrustSummary& formation, CrustSummary& out) const
        {
            for (auto i = 0*out.size(); i < out.size(); ++i)
            {
                out[i] = ops.absorb(crust[i], formation[i]);
            }
        }

        void absorb (const FormationSummary& crust, const FormationSummary& formation, CrustSummary& out) const
        {
            for (auto i = 0*out.size(); i < out.size(); ++i)
            {
                out[i] = ops.absorb(crust[i], formation[i]);
            }
        }

        void flatten (const CrustSummary& crust, FormationSummary& out) const
        {
            for (auto i = 0*out.size(); i < out.size(); ++i)
            {
                out[i] = ops.flatten(crust[i]);
            }
        }

        void empty(CrustSummary& out) const
        {
            const ColumnSummary empty;
            for (auto i = 0*out.size(); i < out.size(); ++i)
            {
                out[i] = empty;
            }
        }

    };

}

