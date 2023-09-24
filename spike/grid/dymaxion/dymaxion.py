
import math

import glm
import plotly.express as px
import plotly.graph_objects as go

turn = 2*math.pi
subgrid_count = 10
half_subgrid_longitude_arc_length = turn / subgrid_count

def cartesian_from_zlon(z,lon):
	r = math.sqrt(1-z**2)
	return glm.vec3(
		r*math.cos(lon),
		r*math.sin(lon),
		z)

class Subgrid:
	def __init__(self):
		pass

	def corner_position(self, longitude_id, z):
		longitude = longitude_id*half_subgrid_longitude_arc_length
		return cartesian_from_zlon(z,longitude)

	def subgrid_id(self, sphere_position):
		V3 = sphere_position
		longitude = math.atan2(V3.y,V3.x)
		longitude_id = longitude/half_subgrid_longitude_arc_length
		Nid = 2*round(longitude_id/2)       # Nid: northernmost edge vertex id
		Sid = 2*round((longitude_id-1)/2)+1 # Sid: southernmost edge vertex id
		Wid = min(Nid,Sid)                  # Wid: westernmost  edge vertex id
		N = self.corner_position(Nid,  0.5)
		S = self.corner_position(Sid, -0.5)
		# V3⋅(N×S)>0 indicates that V3 occupies the grid indicated by Eid
		return (Wid-1 + (glm.dot(V3, glm.cross(N,S)) > 0)) % subgrid_count

	def subgrid_position(self, subgrid_id, sphere_position):
		i = subgrid_id
		subgrid_polarity = (-1)**i
		V3 = sphere_position
		Wid = i     # west   longitude id
		Oid = i + 1 # origin longitude id
		Eid = i + 2 # east   longitude id
		W = self.corner_position(Wid, 0.5-Wid%2) # W: westernmost triangle vertex
		E = self.corner_position(Eid, 0.5-Eid%2) # E: easternmost triangle vertex
		# V3⋅(W×E)>0 indicates whether V3 occupies a polar triangle
		is_polar = glm.dot(V3, subgrid_polarity * glm.cross(W,E)) > 0
		is_inverted = is_polar == i%2
		# O: center triangle vertex, serves as origin of coordinate basis
		O = self.corner_position(Oid, subgrid_polarity*[-0.5,1][is_polar]) 
		# N: surface normal of the triangle
		N = glm.cross(E-O,W-O) if is_inverted else glm.cross(W-O,E-O)
		side_distance = glm.dot(N,O) / glm.dot(N,V3)
		polyhedron_projection = sphere_position * side_distance
		basis = glm.mat3(E-O,W-O,O) if is_inverted else glm.mat3(W-O,E-O,O)
		triangle_position = glm.inverse(basis) * polyhedron_projection
		return 1-triangle_position if is_inverted else triangle_position

	def unit_sphere_position(self, subgrid_id, subgrid_position):
		i = subgrid_id
		is_inverted = subgrid_position.x+subgrid_position.y > 1
		is_polar = is_inverted == i%2
		triangle_position = 1-subgrid_position if is_inverted else subgrid_position
		subgrid_polarity = (-1)**i
		V2 = triangle_position
		Wid = i     # west   longitude id
		Oid = i + 1 # origin longitude id
		Eid = i + 2 # east   longitude id
		W = self.corner_position(Wid, 0.5-Wid%2) # W: westernmost triangle vertex
		E = self.corner_position(Eid, 0.5-Eid%2) # E: easternmost triangle vertex
		O = self.corner_position(Oid, subgrid_polarity*[-0.5,1][is_polar]) 
		basis = glm.mat3(E-O,W-O,O) if is_inverted else glm.mat3(W-O,E-O,O)
		return glm.normalize(basis * glm.vec3(V2.x,V2.y,1))

	def standardize(self, subgrid_id, subgrid_position):
		pass


subgrid = Subgrid()
samplecount = 10000
golden_ratio = 1.618033
golden_angle = turn/golden_ratio

z   = [2*i/samplecount-1  	for i in range(samplecount)]
lon = [(i*golden_angle)%turn for i in range(samplecount)]

unit_sphere_position = [
	cartesian_from_zlon(z,lon)
	for z,lon in zip(z,lon)]

subgrid_id = [
	subgrid.subgrid_id(V3)
	for V3 in unit_sphere_position]

subgrid_position = [
	subgrid.subgrid_position(i, V3)
	for V3,i in zip(unit_sphere_position,subgrid_id)]

reconstructed = [
	subgrid.unit_sphere_position(i, V2)
	for V2,i in zip(subgrid_position,subgrid_id)]

def vector_aoa(vector_aos):
	return [
		[V.x for V in vector_aos],
		[V.y for V in vector_aos],
		[V.z for V in vector_aos]
	]

subgrid_x = [V2.x for V2 in subgrid_position]
subgrid_y = [V2.y for V2 in subgrid_position]

offset     = [V-N for V,N in zip(reconstructed, unit_sphere_position)]
distance   = [glm.length(o) for o in offset]
similarity = [glm.dot(V,N)/(glm.length(V)*glm.length(N)) 
	for V,N in zip(reconstructed, unit_sphere_position)]
offset_normal = [glm.dot(O, N)
	for O,N in zip(offset, unit_sphere_position)]

domain = dict(zip('xyz', vector_aoa(unit_sphere_position)))

px.scatter_3d(**domain,color=subgrid_id).show()
px.scatter_3d(**domain,color=subgrid_x).show()
px.scatter_3d(**domain,color=subgrid_y).show()

# px.scatter_3d(**domain,color=Nx).show()
# px.scatter_3d(**domain,color=roundtrip_x).show()
# px.scatter_3d(**domain,color=Ny).show()
# px.scatter_3d(**domain,color=roundtrip_y).show()
# px.scatter_3d(**domain,color=Nz).show()
# px.scatter_3d(**domain,color=roundtrip_z).show()

# go.Figure(data=go.Cone(**domain, **dict(zip('uvw', vector_aoa(reconstructed))))).show()
# go.Figure(data=go.Cone(**domain, **dict(zip('uvw', vector_aoa(offset))), sizeref=0.03)).show()
# px.scatter_3d(**domain,color=distance).show()
# px.scatter_3d(**domain,color=offset_normal).show()
# px.scatter_3d(**domain,color=similarity).show()
