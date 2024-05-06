#pragma once

// in house libraries

namespace rock{

    class StratumSummaryTools
    {
    public:
        StratumSummaryTools(){}
        StratumSummary combine() (const StratumSummary& a, const StratumSummary& b) const
        {
            auto thickness = a.thickness + b.thickness;
            return StratumSummary(
                a.plate_ids & b.plate_ids,
                (a.density*a.thickness + b.density*b.thickness) / thickness,
                thickness
            );
        }

    };

}

