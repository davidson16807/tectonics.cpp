#pragma once

// in house libraries

namespace rock{

    class CrustSummarization
    {
        const FormationSummarization summarize;
        const FormationSummaryOps ops;
    public:
        CrustSummarization(const FormationSummarization& summarize, const FormationSummaryOps ops):
            summarize(summarize),
            ops(ops)
        {}
        void operator() (
            const int plate_id, 
            const Crust& crust, FormationSummary& out, 
            FormationSummary& scratch
        ) const {

            summarize(plate_id, crust[0], out);
            for (int i = 1; i < crust.size(); ++i)
            {
                summarize(plate_id, crust[i], scratch); 
                ops.combine(scratch, out, out);
            }
        }
    };

}

