#pragma once

// in house libraries

namespace rock{

    /*
    `FormationSummaryArchimedianDisplacement` returns displacement from the base of the crust using Archimedes's principle,
    i.e. the crust is buoyed up by a force equal to the weight of the displaced fluid.
    */
    class FormationSummaryArchimedianDisplacement
    {

        using length = si::length<float>;
        using lengths = std::vector<length>;

        const si::density<float> mantle_density;

    public:

        FormationSummaryArchimedianDisplacement(
            const si::density<float> mantle_density
        ):
            mantle_density(mantle_density)
        {}

        template<typename Summary>
        void operator()(
            const Summary& summary,
            lengths& out
        ) const {
            for (std::size_t i = 0; i < summary.size(); ++i)
            {
                out[i] = summary[i].thickness() * (1.0f - summary[i].density()/mantle_density);
            }
        }

    };

}

