#pragma once

// in house libraries

namespace rock{

    class StratumSummaryOps
    {
    public:
        StratumSummaryOps(){}
        StratumSummary combine() (const StratumSummary& a, const StratumSummary& b) const
        {
            auto thickness = a.thickness + b.thickness;
            return StratumSummary(
                a.plate_id_bitfield & b.plate_id_bitfield,
                (a.density*a.thickness + b.density*b.thickness) / thickness,
                thickness
            );
        }
    };

}

