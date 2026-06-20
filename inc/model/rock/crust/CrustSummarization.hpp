#pragma once

// C libraries
#include <cstddef> // std::size_t

// in house libraries
#include <model/rock/crust/Crust.hpp>
#include <model/rock/crust/CrustSummary.hpp>
#include <model/rock/formation/FormationSummary.hpp>

namespace rock{

    // NOTE: `M` is mineral count, `F` is formation count
    template<std::size_t M, std::size_t F, typename FormationSummarization, typename CrustSummaryOps>
    class CrustSummarization
    {

        const FormationSummarization summarize;
        const CrustSummaryOps ops;

    public:

        CrustSummarization(const FormationSummarization& summarize, const CrustSummaryOps& ops):
            summarize(summarize),
            ops(ops)
        {}

        template<typename Grid>
        void operator() (
            const Grid& grid, 
            const int plate_id, 
            const Crust<M,F>& crust, 
            CrustSummary& out, 
            FormationSummary& scratch
        ) const {
            ops.empty(out);
            for (std::size_t i = 0; i < crust.size(); ++i)
            {
                // summarize(plate_id, crust[i], scratch);
                summarize(grid, plate_id, crust[i], scratch);
                ops.absorb(out, scratch, out);
            }
        }

    };

    template<std::size_t M, std::size_t F, typename FormationSummarization, typename CrustSummaryOps>
    auto crust_summarization(
        const FormationSummarization& summarize, 
        const CrustSummaryOps& ops
    ){
        return CrustSummarization<M,F,FormationSummarization,CrustSummaryOps>(summarize, ops);
    }

}

