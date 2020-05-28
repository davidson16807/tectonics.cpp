/*
"particle_system" represents a simple collection of noninteracting particles moving about in a 3d space
We're not sure why you'd need this, but include it for completeness.
*/
struct particles
{
	vec3s 		positions;
	vec3s 		velocities;	
};
struct point_masses
{
	vec3s 		positions;
	vec3s 		velocities;	
	floats 		masses;
};
struct 
{
	
};
struct particle_system
{
	vec3s 		positions;
	vec3s 		velocities;

	explicit particle_system(const uint particle_count)
		: 
			positions	(particle_count),
			velocities	(particle_count)
	{

	}
	static void calculate_changes(const particle_system& state, float timestep, particle_system& changes)
	{
		changes.positions = state.velocities * timestep;
	}
	static void apply_changes(const particle_system& original, const particle_system& changes, particle_system& updated)
	{
		updated.positions  += changes.positions;
		updated.velocities += changes.velocities;
	}
};
/*
"tiled_particle_system" represents a collection of particles moving in a 3d space bounded by a cube that's centered around the origin. 
When the position of a particle exceeds the bounds of the 3d space, it loops back to the other side of the cube.
Hence, the space is "tiled" in appearance.
*/
struct tiled_particle_system
{
	vec3s 		positions;
	vec3s 		velocities;
	const float width;

	explicit tiled_particle_system(const uint particle_count, const float width)
		: 
			positions	(particle_count),
			velocities	(particle_count),
			width       (width)
	{

	}
	static void calculate_changes(const tiled_particle_system& state, float timestep, tiled_particle_system& changes)
	{
		changes.positions = state.velocities * timestep;
	}
	static void apply_changes(const tiled_particle_system& original, const tiled_particle_system& changes, tiled_particle_system& updated)
	{
		updated.positions  = (original.positions  + changes.positions  + width/2.) % width - width/2.;
		updated.velocities =  original.velocities + changes.velocities;
	}
};
/*
"spring_system" represents a collection of particles interacting with each other through springs of infinite strength. 
It is assumed the springs cannot deform in any way and will always return to their original shape in the absence of force. 
*/
struct spring_system
{
	vec3s 		positions;
	vec3s 		velocities;
	floats      masses;
	ivec2s      spring_particle_ids;
	floats      spring_resting_lengths;
	floats      spring_stiffness;

	explicit spring_system(const uint particle_count, const uint spring_count)
		: 
			positions				(particle_count),
			velocities				(particle_count),
			masses	    			(particle_count),
			spring_particle_ids		(spring_count),
			spring_resting_lengths	(spring_count),
			spring_stiffness 		(spring_count)
	{

	}
	static void calculate_changes(const spring_system& state, float timestep, spring_system& changes, vec3s& spring_scratch)
	{
		changes.positions = state.velocities * timestep;

		for (int i = 0; i < state.spring_particle_ids.size(); ++i)
		{
			ivec2 particle_ids = state.spring_particle_ids[i];
			vec3  offset = state.positions[particle_ids.y] - state.positions[particle_ids.x];
			float distance = length(offset);
			float force_magnitude = (distance - state.spring_resting_lengths[i]) * state.spring_stiffness[i];
			changes.velocities[particle_ids.y] =  force_magnitude / state.masses[i] * normalize(offset) * timestep;
			changes.velocities[particle_ids.x] = -force_magnitude / state.masses[i] * normalize(offset) * timestep;
		}
	}
	static void apply_changes(const spring_system& original, const spring_system& changes, spring_system& updated)
	{
		updated.positions  += changes.positions;
		updated.velocities += changes.velocities;
	}
}
/*
"deformable_spring_system" represents a collection of particles interacting with each other through deformable springs. 
A representative model is used to deform the springs under enough strain. 
This includes both plastic deformation and fracture. 
*/
struct deformable_spring_system
{
	vec3s 		positions;
	vec3s 		velocities;
	floats      masses;

	ivec2s      spring_particle_ids;
	floats      spring_resting_lengths;

	float       spring_compressive_pre_yield_stiffness;
	float       spring_compressive_yield_strength;
	float       spring_compressive_post_yield_stiffness;
	// float       spring_compressive_ultimate_strength;

	float       spring_tensile_pre_yield_stiffness;
	float       spring_tensile_yield_strength;
	float       spring_tensile_post_yield_stiffness;
	float       spring_tensile_ultimate_strength;

	explicit spring_system(const uint particle_count, const uint spring_count)
		: 
			positions				(particle_count),
			velocities				(particle_count),
			masses	    			(particle_count),
			spring_particle_ids		(spring_count),
			spring_resting_lengths	(spring_count),
			spring_stiffness 		(spring_count)
	{

	}
	float stress(tensile_strain)
	{
		float x  = tensile_strain;

		float kc0 = spring_compressive_pre_yield_stiffness;
		float kc1 = spring_compressive_post_yield_stiffness;
		float Fc0 = spring_compressive_yield_strength;
		float Fc1 = spring_compressive_ultimate_strength;

		float kt0 = spring_tensile_pre_yield_stiffness;
		float kt1 = spring_tensile_post_yield_stiffness;
		float Ft0 = spring_tensile_yield_strength;
		float Ft1 = spring_tensile_ultimate_strength;

		float xc  = -tensile_strain;
		float xc0 =  Fc0/kc0;
		float xc1 =  xc0 + (Fc1-Fc0)/kc1;
		float xt  =  tensile_strain;
		float xt0 =  Ft0/kt0;
		float xt1 =  xt0 + (Ft1-Ft0)/kt1;

		float F = 0.f; 
		F = mix(-Fc0,0.f, smoothstep(xc0,0.f, xc));
		F = mix( F,  Ft0, smoothstep(0.f,xt0, xt));
		F = mix( F,  Ft1, smoothstep(xc0,xc1, xt));
		return F;
	}
	static void calculate_changes(const spring_system& state, float timestep, spring_system& changes, vec3s& spring_scratch)
	{
		changes.positions = state.velocities * timestep;

		for (int i = 0; i < state.spring_particle_ids.size(); ++i)
		{
			ivec2 particle_ids = state.spring_particle_ids[i];
			vec3  offset = state.positions[particle_ids.y] - state.positions[particle_ids.x];
			float strain   = (length(offset) - state.spring_resting_lengths[i]) / state.spring_resting_lengths[i];
			float force_magnitude = strain * state.spring_stiffness[i];
			changes.velocities[particle_ids.y] =  force_magnitude / state.masses[i] * normalize(offset) * timestep;
			changes.velocities[particle_ids.x] = -force_magnitude / state.masses[i] * normalize(offset) * timestep;
		}
	}
	static void apply_changes(const spring_system& original, const spring_system& changes, spring_system& updated)
	{
		updated.positions  += changes.positions;
		updated.velocities += changes.velocities;
	}
};
/*
"creeping_spring_system" represents a collection of particles interacting with each other through deformable springs. 
Their motion is "creeping", meaning that inertia is inconsequential next to viscous forces. 
Another way to put it is the spring system behaves through Stokes flow, or as a fluid with a low Reynolds number.
Because inertia is inconsequential, there is no need to track velocity as a state variable. 
The change in position that occurs across timesteps ("displacement") 
Also, the springs are deformable: a representative model is used to deform the springs under enough strain. 
This includes both plastic deformation and fracture. 
*/
struct creeping_deformable_spring_system
{
	vec3s 		positions;
	floats      masses;

	ivec2s      spring_particle_ids;
	floats      spring_resting_lengths;

	float       spring_compressive_pre_yield_stiffness;
	float       spring_compressive_yield_strength;
	float       spring_compressive_post_yield_stiffness;
	float       spring_compressive_ultimate_strength;

	float       spring_tensile_pre_yield_stiffness;
	float       spring_tensile_yield_strength;
	float       spring_tensile_post_yield_stiffness;
	float       spring_tensile_ultimate_strength;

	static void calculate_changes(const creeping_deformable_spring_system& state, float timestep, creeping_deformable_spring_system& changes)
	{
		changes.positions = state.velocities * timestep;
	}
	static void apply_changes(const creeping_deformable_spring_system& original, const creeping_deformable_spring_system& changes, creeping_deformable_spring_system& updated)
	{
		updated.positions  += changes.positions;
		updated.velocities += changes.velocities;
	}
};