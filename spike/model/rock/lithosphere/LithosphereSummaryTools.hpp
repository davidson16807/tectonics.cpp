#pragma once

// in house libraries

namespace rock{

    class LithosphereSummaryTools
    {
        const ColumnSummaryOps ops;
    public:
        LithosphereSummaryTools(const ColumnSummaryOps& ops):
            ops(ops)
        {}
        void reset (LithosphereSummary& lithosphere& out) const
        {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = ColumnSummary();
            }
        }
        void absorb (const LithosphereSummary& lithosphere, const FormationSummary& formation, FormationSummary& out) const
        {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = ops.absorb(lithosphere[i], formation[i]);
            }
        }
    };

}

