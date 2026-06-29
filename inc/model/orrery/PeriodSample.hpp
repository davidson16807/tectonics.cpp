
namespace orrery {

    template<typename scalar, typename duration>
    struct PeriodSample {

        const duration period; // period of the cycle for this node
        const scalar phase;       // phase id
        const scalar phase_count; // number of phases

        /*
        for numerical integration, set phase_count to the number of iterations
        for random phases drawn from [0,1], set phase_count=1.0,
        for fibonacci sampling, set phase_count = 1/ϕ = 0.618033…
        */

        PeriodSample(
            const duration period,
            const scalar phase,
            const scalar phase_count
        ): 
            period(period), 
            phase(phase), 
            phase_count(phase_count)
        {}

        scalar fraction() const
        {
            return scalar(phase) / scalar(phase_count);
        }

        duration time() const
        {
            return period * fraction();
        }

        PeriodSample with_time(duration value) const
        {
            return PeriodSample<scalar,duration>(period, (value/period) * phase_count, phase_count);
        }

    };

}

