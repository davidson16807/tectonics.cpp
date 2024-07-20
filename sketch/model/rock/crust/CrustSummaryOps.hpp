#pragma once

namespace rock{


    class CrustSummaryOps
    {

        static constexpr ColumnSummary empty;

        const ColumnSummaryTools ops;

    public:

        CrustSummaryOps(): 
            ops(ColumnSummaryTools())
        {}

        void absorb (const CrustSummary& crust, const FormationSummary& formation, CrustSummary& out) const
        {
            for (int i = 0; i < out.size(); ++i)
            {
                out[i] = ops.absorb(crust[i], formation[i]);
            }
        }

        void flatten (const CrustSummary& crust, FormationSummary& out) const
        {
            for (int i = 0; i < out.size(); ++i)
            {
                out[i] = ops.flatten(crust[i]);
            }
        }

        void reset (FormationSummary& out) const
        {
            for (int i = 0; i < out.size(); ++i)
            {
                out[i] = empty;
            }
        }

    };

}

