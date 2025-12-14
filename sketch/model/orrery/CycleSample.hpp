
template<typename id, typename scalar, typename duration>
struct CycleSample {

    const duration period; // period of the cycle for this node
    const id cycle;        // node or resonance of the cycle
    const scalar phase;       // phase id
    const scalar phase_count; // number of phases

    CycleSample(
        const id cycle,
        const duration period,
        const scalar phase,
        const scalar phase_count
    ): 
        period(period), 
        cycle(cycle), 
        phase(phase), 
        phase_count(phase_count)
    {}

    CycleSample(
        const Cycle& cycle,
        const scalar phase,
        const scalar phase_count
    ): 
        period(cycle.period), 
        cycle(cycle.cycle), 
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

    CycleSample with_time(duration value) const
    {
        return CycleSample<id,scalar,duration>(cycle, period, (value/period) * phase_count, phase_count);
    }

};
