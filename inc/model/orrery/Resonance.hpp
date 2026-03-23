
template<typename id, typename duration>
struct Resonance {

    const id node;         // node that participates in this resonance
    const id resonance;    // resonance id
    const id sample_count; // how many samples are in this resonance
    const duration period; // period of the cycle for this node

    Resonance(
        const id node,
        const id resonance,
        const id sample_count,
        const duration period
    ): 
        node(node), 
        resonance(resonance), 
        sample_count(sample_count),
        period(period)
    {}

};
