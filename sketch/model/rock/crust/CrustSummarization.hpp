#pragma once

// in house libraries

#include <model/rock/crust/Crust.hpp>
#include <model/rock/crust/CrustSummary.hpp>
#include <model/rock/formation/FormationSummary.hpp>

namespace rock{

    // NOTE: `M` is mineral count, `F` is formation count
    template<int M, int F, typename FormationSummarization, typename FormationSummaryOps>
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
            const Crust<M,F>& crust, 
            CrustSummary& out, 
            FormationSummary& scratch
        ) const {
            ops.empty(out);
            for (int i = 0; i < crust.size(); ++i)
            {
                summarize(plate_id, crust[i], scratch);
                ops.combine(out, scratch, out);
            }
        }
    };

    template<int M, int F, typename FormationSummarization, typename FormationSummaryOps>
    auto crust_summarization(const FormationSummarization& summarize, const FormationSummaryOps& ops){
        return CrustSummarization<M,F,FormationSummarization,FormationSummaryOps>(summarize, ops);
    }

}
