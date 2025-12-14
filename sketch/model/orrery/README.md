observations:

	position trajectories (e.g. orbit, beeline) never convey attitude information, only position
	rotation trajectories (e.g. spin, lock) never convey position information, only rotation
	for any pair of target and origin, rotation trajectories need only be considered for the target and origin
	we only need to track positions from the origin back to the universal common ancestor node in the reverse pass
	for greatest accuracy, we should only start to apply positions for each target starting from the common ancestor 
		node in the forward pass
	both rotation and position trajectories may occur in cycles
	cycles of imperceptibly small periods ("imperceptible cycles") have properties that are measured at multiple sample phases
	resonant cycles are cycles for which one sample phase implies the existance of another, which may be imperceptible or not
	imperceptible nonresonant cycles are considered "ergodic cycles" so properties must be sampled for phases in combination
	spins have up to 3 parameters that can be sampled (rotation, precession, nutation)
	spins can be sampled without modification to the rest of the scene tree
	there is a constant finite number of samples that can be afforded for a calculation to fit in a single frame,
		samples can therefore be stored in an array or finite-sized vector, provided their representation is suitable
	sampling across ergodic cycles can be represented as a vector storing tuples of (sample, phase) indices,
		which can be procedurally iterated.
	if samples are indicated by their position in an a scene graph as represented using an array of trajectory offsets, 
		a function exists that accepts the scene graph array and a (sample, phase) tuple, 
		and either modifies the array in-place or returns a copy to reflect positions for that sample and phase.

	Spins, processions, and nutations are the last sequence of nodes in an array that represents a scene tree
	where the root node is at index 0. Nutations appear before processions. Processions appear before spins.
	If no other positions trajectories are considered aside from orbits, then node id = entity id for orbits,
	and node ids for spins, processions, and nutations start at entities.size().

Therefore, the following concepts exist in the implementation:

	Oⁿ → Nᵐ     	filter noncyclic
	Oⁿ → Nᵐ     	filter perceptible cycles and their periods
	Oⁿ → Nᵐ     	filter imperceptible cycles and their periods
	Oⁿ(NPCT)ᵐ→(Nℝ³)ᵐ	update positions in scene tree for trajectories at phases
	Sⁿ(NPCT)ᵐ→ℝ⁴⁴  	return rotation of a spin for trajectories at phases
	OⁿNᵐt→(Nℝ³)ᵐ 	update positions in scene tree for trajectories at time
	BⁿNᵐt→(Nℝ³)ᵐ 	update positions in scene tree for trajectories at time
	NᵐNⁿ → Nⁿ⁺ᵐ    	combine sample phases
	Nⁿ → (N1CT)ⁿ    	initialize sample phase
	Nⁿ → (N1CT)ⁿ    	initialize sample phase
	(NPCT)ⁿ → (NPCT)ⁿ     	iterate through combinations of sample phases
	(NPCT)ⁿ(NPCT)ⁿ → (NPCT)ⁿ 	overwrite sample phases with another where available
	(NPCT)ⁿ → (NRCT)ᵐ     	identify resonances by indices with sample counts
	(NRCT)ⁿ → (RPCT)ᵐ     	initialize sample phases for resonances
	(NPCT)ⁿ → (NPCT)ⁿ     	iterate through combinations of sample phases for resonances
	(NPCT)ⁿ(RPCT)ᵐ → (NPCT)ⁿ 	update sample phases to represent resonances

Letters in the notation above represent the following:

	notation:
	n an array id
	m another array id
	N node id
	P phase id
	R resonance id
	T period
	B beeline
	O orbit
	S spin

Expressions above are named/implemented as follows in C++:

	Oⁿ 	Components<Orbit>
	Sⁿ 	Components<Spin>
	Bⁿ 	Components<Beeline>
	Nⁿ 	std::vector<Node>
	NPCT 	CycleSample
	RPCT 	CycleSample
	Nℝ³ 	TrajectoryPosition
	NRCT 	Resonance
	ℝ³  	vec3

