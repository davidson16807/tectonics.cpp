#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries

namespace rock
{

    struct FormationSummaryProperties
    {
        FormationSummaryProperties()
        {}
        void area_density (const FormationSummary& formation, auto& out) const
        {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = formation[i].area_density();
            }
        }

        // `thickness` returns displacement using an isostatic model
        void thickness(const FormationSummary& summary, std::vector<si::length<float>> out) const
        {
            for (auto i = 0*out.size(); i < out.size(); ++i)
            {
                out[i] = summary[i].thickness();
            }
        }

    };

}

