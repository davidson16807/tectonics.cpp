
ecs libraries namely provide fast lookup of entities that possess all the components of a system

component table:
* vector of components
* entity→id map
* id→entity map, allowing search on entities that share components

"node" implementation:
	entity:
		represents a node in a scene graph, namely a coordinate system relative to a parent
	components:
		spin
		orbit
		beeline
	+ transform / scene graph logic is very straight forward
	+ greatly simplifies logic for resonances that involve spin
	- may be harder to coordinate application of force or changes in mass across components
	- may be harder to customize ui controls

"origin" implementation:
	entity:
		represents a reference position, namely a barycenter or world origin
	components:
		spin
		procession
		orbit
		beeline
		world?
	+ naturally imposes constraints seen in reality (e.g. a spin node is never parent to a spin)
	+ entity-component relationships might make it easier to check applicability of ui controls (e.g. spin lock camera)
	+ easier to synchronize applications of force or changes in mass across components for a single body (e.g. tidal forces, spin and orbit after asteroid impact, supernovae)
	- may complicate transform/scene graph logic
	- harder to write logic for resonances that involve spin

"body" implementation:
	entity:
		represents a body
	component:
		spin,procession,orbit
		spin,procession,beeline
	+ entity-component relationships might make it easier to check applicability of ui controls (e.g. spin lock camera)
	+ easier to synchronize applications of force or changes in mass across components for a single body (e.g. tidal forces, spin and orbit after asteroid impact, supernovae)
	- may complicate transform/scene graph logic
	- some concepts do not apply to bodies, resulting in nonsense or sentinel values (e.g. spin and barycentrs)
