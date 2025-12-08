
ecs libraries namely provide fast lookup of entities that possess all the components of a system

component table:
* vector of components
* entity→id map
* id→entity map, allowing search on entities that share components

implementation 1:
	entity:
		represents a node in a scene graph, namely a coordinate system relative to a parent
	components:
		spin
		orbit
		beeline
	+ transform / scene graph logic is very straight forward

implementation 2:
	entity:
		represents a reference position, namely a barycenter or world origin
	components:
		spin
		procession
		orbit
		beeline
	+ naturally imposes constraints seen in reality (e.g. a spin node is never parent to a spin)
	+ entity-component relationships might make it easier to check applicability of ui controls (e.g. spin lock camera)
	- may complicate transform/scene graph logic

implementation 3:
	entity:
		represents a body
	component:
		spin,procession,orbit
		spin,procession,beeline
	+ entity-component relationships might make it easier to check applicability of ui controls (e.g. spin lock camera)
	- may complicate transform/scene graph logic
	- some concepts do not apply to bodies, resulting in nonsense or sentinel values (e.g. spin and barycentrs)
