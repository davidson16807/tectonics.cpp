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
            summarize(plate_id, crust.sediment,    out);      
            summarize(plate_id, crust.sedimentary, scratch); ops.combine(scratch, out, out);
            summarize(plate_id, crust.bedrock,     scratch); ops.combine(scratch, out, out);
        }
    };

}

