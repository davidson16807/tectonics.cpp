#pragma once

// in house libraries

namespace rock{

    class CrustOps
    {
        const FormationOps formations;
    public:
        CrustOps(const FormationOps& formations):
            formations(formations)
        {}
        void combine (const Crust<M>& crust, Formation<M>& out) const
        {
            formations.combine(crust.bedrock,  crust.sedimentary, out);
            formations.combine(crust.sediment, out,               out);
        }
    };

}

