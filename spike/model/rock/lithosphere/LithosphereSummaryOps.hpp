#pragma once

// in house libraries

namespace rock{

    class LithosphereSummaryOps
    {
        const ColumnSummaryOps ops;
    public:
        LithosphereSummaryOps(const ColumnSummaryOps& ops):
            ops(ops)
        {}
        void absorb (const LithosphereSummary& a, const FormationSummary& b, FormationSummary& out) const
        {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = ops.absorb(a[i],b[i]);
            }
        }
    };

}

