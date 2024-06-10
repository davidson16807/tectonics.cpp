#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries

namespace rock
{

    template<typename StratumSummarization>
    class FormationSummarization
    {
        const StratumSummarization summarization;
    public:
        FormationSummarization(const StratumSummarization& summarization)
        {}
        void operator() (const int plate_id, const Formation<M>& formation, std::vector<StratumSummary>& out) const
        {
            for (std::size_t i = 0; i < formation.size(); ++i)
            {
                out[i] = summarization(formation[i]);
            }
        }
    };

}

