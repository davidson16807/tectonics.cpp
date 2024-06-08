#pragma once

// in house libraries

namespace rock{

    class StratumSummaryOps
    {
    public:
        StratumSummaryOps(){}
        StratumSummary combine (const StratumSummary& a, const StratumSummary& b) const
        {
            auto thickness_a = a.thickness();
            auto thickness_b = b.thickness();
            auto thickness = thickness_a + thickness_b;
            return StratumSummary(
                a.plate_ids_bitset() | b.plate_ids_bitset(),
                (a.density()*thickness_a + b.density()*thickness_b) / thickness,
                thickness
            );
        }

    };

}

