#pragma once

#include <.h>

// in house libraries

namespace rock{

    template<typename Grid>
    class LithosphereSummarization
    {
        const CrustSummarization summarize;
        const LithosphereSummaryOps ops;
        const Grid grid;
    public:
        LithosphereSummarization(
            const CrustSummarization& summarize, const LithosphereSummaryOps ops, const Grid& grid
        ):
            summarize(summarize), ops(ops), grid(grid)
        {}
        void operator() (const std::vector<Plate>& plates, LithosphereSummary& out, FormationSummary& local_summary, FormationSummary& global_summary) const
        {


            *FormationSummary scratch_summary = global_summary;
            ops.reset(out);
            for (std::size_t i = 0; i < plates.size(); ++i)
            {
                auto resample = 
                    series::map(
                        field::compose(
                            dymaxion::NearestVertexId(grid), 
                            field::Transform(plate.local_to_global)),
                        dymaxion::VertexPositions(grid)
                    );
                /*
                 grid1     rotation    grid2
                resample = id ⟶ position ⟶ position ⟶ id
                */
                summarize(i, plates[i].crust, local_summary, scratch_summary);
                each::get(local_summary, resample, global_summary);
                ops.absorb(out, global_summary, out);
            }
        }
    };

}

