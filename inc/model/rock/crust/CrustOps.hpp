#pragma once

// in house libraries

namespace rock{

    template <int M>
    class CrustOps
    {
        const FormationOps formations;
    public:
        CrustOps(const FormationOps& formations):
            formations(formations)
        {}
        void flatten (const Crust<M>& crust, Formation& out) const
        {
            formations.combine(crust[0], crust[1], out);
            for (std::size_t i = 2; i < crust.size(); ++i)
            {
                formations.combine(crust[i], out, out);
            }
        }
    };

}

/*
test:
                flatten
             crust ⟶ formation
    summarize  ↓         ↓  summarize
              cs   ⟶    fs
                flatten
*/

