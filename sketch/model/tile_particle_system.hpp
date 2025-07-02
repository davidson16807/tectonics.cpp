/*
desired use cases:

tectonics.js parity
x	elliptic orbits
x	multipoint sampling of orbits
nice model
x	single barycenter
x	~1000 gravitational influences
oumuamua
x	hyperbolic trajectory
pluto/charon system
	multiple bodies
	single barycenter
	no parent orrery
interstellar spacecraft
x	linear trajectory
interstellar docking
	linear trajectory
	shared reference frame
*/

body::NestedField<N>                gravity_field; // tracks largest influences individually up to N, then averages the rest
body::components<body::Orbit>       orbits; // this effectively defines the primary gravitational influence
body::components<body::Beeline>     beelines; // all trajectories without gravitational influence
body::components<body::State>       states; // (position, velocity) tuples, for real bodies
body::components<body::Spin>        spins;
body::components<body::Lock>        locks; // for guaranteed tidal locking and articulated parts like solar panels or antennas
body::components<body::Orientation> orientations;
body::components<body::Illuminator> illuminators;
body::NestedField<N>                illumination_field; // tracks largest sources individually up to N, then averages the rest
body::components<mat4>              frames; 
body::components<id>                parents; 
body::components<body::Mass>        masses; // for barycenters and real bodies
body::components<glm::mat4>         local_to_parent; // parent is usually barycentric, but may be e.g. a targeted interstellar ship
body::NestedField<N>                collision_field; // tracks closest and fastest potential colliders individually up to N

/*
approximate update algorithm:

orbits ← orbits
orbits ← beelines
spins  ← spins
illuminators ← iluminators
states[mask] ← orbits
states[mask] ← beelines
orientations[mask] ← spins
local_to_parent[mask] ← states
local_to_parent[mask] ← orientations
local_to_root ← local_to_parent
global_states ← states,local_to_root
global_orientations ← orientations,local_to_root
global_gravity[dirty] ← global_states,masses,gravity_field
global_drag[dirty] ← global_states,aerodynamics
global_tidal_force[dirty] ← global_states,masses,gravity_field
global_states[dirty] ← global_states,masses,global_gravity
global_states[dirty] ← global_states,masses,global_drag
gravity_field ← global_states,masses
illumination_field ← global_states,illuminators
collision_field ← global_states
orbits ← global_states,masses,gravity_field
states[dirty] ← local_to_root,global_states
beelines[dirty] ← states
*/

/*
implementation roadmap:

minimum climate forcing:
	body::Orbit
	body::State
	body::Spin
	body::Orientation
	body::Propagator
	body::Illuminator
minimum nice model:
	body::NestedField
*/