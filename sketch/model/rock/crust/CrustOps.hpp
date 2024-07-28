#pragma once

// in house libraries
#include <model/rock/formation/FormationOps.hpp>
#include <model/rock/crust/FormationType.hpp>

#include "Crust.hpp"

namespace rock{

    // NOTE: `M` is mineral count, `F` is formation count
    template <int M>
    class CrustOps
    {
        static constexpr int F = 5;

        const FormationOps<M> ops;
    public:
        CrustOps():
            ops()
        {}
        void absorb (const Crust<M,F>& top, const Crust<M,F>& bottom, Crust<M,F>& out) const
        {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                ops.copy(top[i], out[i]);
            }
            ops.combine(bottom[formations::sediment],        out[formations::igneous],     out[formations::igneous]     );
            ops.combine(bottom[formations::sedimentary],     out[formations::igneous],     out[formations::igneous]     );
            ops.combine(bottom[formations::metasedimentary], out[formations::metaigneous], out[formations::metaigneous] );
            ops.combine(bottom[formations::igneous],         out[formations::igneous],     out[formations::igneous]     );
            ops.combine(bottom[formations::metaigneous],     out[formations::metaigneous], out[formations::metaigneous] );
        }
        void flatten (const Crust<M,F>& crust, Formation<M>& out) const
        {
            ops.copy(crust[0], out);
            for (std::size_t i = 1; i < crust.size(); ++i)
            {
                ops.combine(crust[i], out, out);
            }
        }
    };

}
