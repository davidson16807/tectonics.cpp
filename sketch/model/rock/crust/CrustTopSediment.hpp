#pragma once

// in house libraries

namespace rock{

    // NOTE: `M` is mineral count, `F` is formation count
    template<std::size_t M, std::size_t F>
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
            const Crust<M,F>& crust,
            const LithosphereSummary& summary,
            Formation<M>& out
        ) const {
            for (std::size_t i = 0; i < crust[sediment].size(); ++i)
            {
                out[i] = summary[i].top.includes(i)? crust[sediment][i] : StratumStore<M>();
            }
        }
    };

}

