#pragma once

// in house libraries

namespace rock{

    class CrustSummarization
    {
        const FormationSummarization summarize;
        const FormationSummaryOps ops;
    public:
        CrustSummarization(const FormationSummarization& summarize, const FormationSummaryOps& ops):
            summarize(summarize),
            ops(ops)
        {}
        void operator() (
            const int plate_id, 
            const Crust& crust, 
            FormationSummary& out, 
            FormationSummary& scratch
        ) const {
            ops.reset(out);
            for (int i = 0; i < crust.size(); ++i)
            {
                summarize(plate_id, crust[i], scratch);
                ops.combine(out, scratch, out);
            }
        }
    };

}

/*
test:
                flatten
             crust ⟶ formation
    summarize  ↓         ↓
              cs   ⟶    fs
                flatten
*/
