#pragma once

namespace rock{

    template <typename ColumnSummaryProperty>
    class CrustSummaryProperty
    {

        const ColumnSummaryProperty property;

    public:

        CrustSummaryProperty(const ColumnSummaryProperty& property):
            property(property)
        {}

        void operator()(const CrustSummary& summary, auto& out) const
        {
            for (auto i = 0*summary.size(); i < summary.size(); ++i)
            {
                out[i] = property(summary[i]);
            }
        }

    };

}

