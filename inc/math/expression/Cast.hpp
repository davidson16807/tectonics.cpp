#pragma once

namespace math {

    /* 
    `Cast<T,F>` is a trivial class that represents a morphism from one type to another
    */
    template<typename Tx, typename F>
    struct Cast {
        Tx xunits;
        F f;
        constexpr explicit Cast<Tx,F>(const Tx xunits, const F f):
            xunits(xunits), f(f)
        {}
        constexpr auto operator()(const Tx x) const
        {
            return f(x/xunits);
        }
    };

}
