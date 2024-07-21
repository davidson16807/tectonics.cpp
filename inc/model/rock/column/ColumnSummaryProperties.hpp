#pragma once

namespace rock{

    template <typename StratumProperty>
    class ColumnSummaryProperty
    {

        const StratumProperty stratum_property;

    public:

        ColumnSummaryProperty(const StratumProperty& stratum_property): 
            stratum_property(stratum_property)
        {}

        // returns displacement using an isostatic model
        si::length<float> operator()(const ColumnSummary& summary) const
        {
            return stratum_property(stratum_property.flatten(column.top));
        }

    };

}

