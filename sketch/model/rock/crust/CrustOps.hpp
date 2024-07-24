#pragma once

// in house libraries

namespace rock{

    template <int M, int F>
    class CrustOps
    {
        const FormationOps<M> formations;
    public:
        CrustOps(const FormationOps<M>& formations):
            formations(formations)
        {}
        void absorb (const Crust<M,F>& top, const Crust<M,F>& bottom, Formation<M>& out) const
        {
            for (std::size_t i = 0; i < bottom.size(); ++i)
            {
                formations.combine(crust[i], out, out);
            }
        }
        void flatten (const Crust<M,F>& crust, Formation<M>& out) const
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

