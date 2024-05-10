#pragma once

// in house libraries

namespace rock{

    class CrustTopSediment
    {
        const FormationSummarization summarize;
        const FormationSummaryOps ops;
    public:
        CrustTopSediment(const FormationSummarization& summarize, const FormationSummaryOps ops):
            summarize(summarize),
            ops(ops)
        {}
        void operator() (
            const int plate_id, 
            const Crust<M>& crust,
            const LithosphereSummary& summary,
            Formation<M>& out
        ) const {
            for (std::size_t i = 0; i < crust.size(); ++i)
            {
                out[i] = summary[i].top.includes(i)? crust.sediment[i] : StratumStore<M>();
            }
        }
    };

}

