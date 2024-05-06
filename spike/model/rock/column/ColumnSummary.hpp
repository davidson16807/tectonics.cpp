#pragma once

namespace rock{

    class ColumnSummary
    {
        StratumSummary top; 
        StratumSummary rest;
    public:
        constexpr ColumnSummary():
            top(),
            rest()
        {}
        constexpr ColumnSummary(
            const StratumSummary top, 
            const StratumSummary rest,
        ):
            top(top),
            rest(rest)
        {}
    };

}