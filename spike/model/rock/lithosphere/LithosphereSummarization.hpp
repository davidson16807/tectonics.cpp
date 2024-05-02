#pragma once

// in house libraries

namespace rock{

    template<typename Resample>
    class LithosphereSummarization
    {
        const CrustSummarization summarize;
        const LithosphereSummaryOps ops;
        const Resample resample;
    public:
        LithosphereSummarization(
            const CrustSummarization& summarize, const LithosphereSummaryOps ops, const Resample& resample
        ):
            summarize(summarize), ops(ops), resample(resample)
        {}
        void operator() (const std::vector<Plate>& plates, LithosphereSummary& out, FormationSummary& local_summary, FormationSummary& global_summary) const
        {
            &scratch_summary = global_summary;
            ops.reset(out);
            for (std::size_t i = 0; i < plates.size(); ++i)
            {
                summarize(i, plates[i].crust, local_summary, scratch_summary);
                resample(local_summary, global_summary);
                ops.absorb(out, global_summary, out);
            }
        }
    };

}

