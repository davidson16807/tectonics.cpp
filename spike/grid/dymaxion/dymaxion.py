
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

'''
GUIDE TO VARIABLE NAMES:
* `is_*` prefix: booleans
* lowercase: scalars
* UPPERCASE: vectors
* `N/S/E/W`: cardinal direction
* `V3`: 3d position
* `V2`: 2d position
* `N`: normal
* `O`: origin
* `i`: subgrid id
'''

class Subprojection:
	def __init__(self):
		pass

	def z(self, i):
		return 0.5-i%2

	def origin_z(self, subgrid_polarity, is_polar):
		return subgrid_polarity*[-0.5,1][is_polar]

	def corner(self, i, z):
		longitude = i*half_subgrid_longitude_arc_length
		return cartesian_from_zlon(z,longitude)

	def is_inverted_subgrid_id(self, i, is_polar):
		return is_polar == i%2

	def is_polar_subgrid_id(self, i, is_inverted):
		return is_inverted == i%2

	def is_inverted_subgrid_position(self, V2):
		return V2.x+V2.y < 1

	def is_polar_sphere_position(self, V3, subgrid_polarity, W, E):
		# V3⋅(W×E)>0 indicates whether V3 occupies a polar triangle
		return glm.dot(V3, subgrid_polarity * glm.cross(W, E)) > 0

	def is_eastern_sphere_position(self, V3, N, S):
		# V3⋅(N×S)>0 indicates that V3 occupies the grid indicated by Eid
		return glm.dot(V3, glm.cross(N,S)) > 0

	def subgrid_polarity(self, i):
		return (-1)**i

	def basis(self, W,E,O, is_inverted):
		return glm.mat3(E-O,W-O,O) if is_inverted else glm.mat3(W-O,E-O,O)

	def sphere_project(self, V3):
		return glm.normalize(V3)

	def plane_project(self, V3, N, origin):
		return V3 * glm.dot(N, origin) / glm.dot(N, V3)

class Projection:
	def __init__(self, sugbrids):
		self.sugbrids = sugbrids

	def subgrid_position(self, sphere_position):
		V3 = sphere_position
		longitude = math.atan2(V3.y,V3.x)
		EWid = longitude/half_subgrid_longitude_arc_length
		Nid = 2*round(EWid/2)       # Nid: northernmost edge vertex id
		Sid = 2*round((EWid-1)/2)+1 # Sid: southernmost edge vertex id
		Wid = min(Nid,Sid)          # Wid: westernmost  edge vertex id
		N = self.sugbrids.corner(Nid,  0.5)
		S = self.sugbrids.corner(Sid, -0.5)
		i = (Wid-1 + self.sugbrids.is_eastern_sphere_position(V3,N,S)) % subgrid_count
		subgrid_polarity = self.sugbrids.subgrid_polarity(i)
		Wid = i     # west   longitude id
		Oid = i + 1 # origin longitude id
		Eid = i + 2 # east   longitude id
		W = self.sugbrids.corner(Wid, self.sugbrids.z(Wid)) # W: westernmost triangle vertex
		E = self.sugbrids.corner(Eid, self.sugbrids.z(Eid)) # E: easternmost triangle vertex
		is_polar = self.sugbrids.is_polar_sphere_position(V3, subgrid_polarity, W,E)
		is_inverted = self.sugbrids.is_inverted_subgrid_id(i, is_polar)
		O = self.sugbrids.corner(Oid, self.sugbrids.origin_z(subgrid_polarity, is_polar))
		basis = self.sugbrids.basis(W,E,O, is_inverted)
		N = glm.cross(basis[0], basis[1])
		triangle_position = glm.inverse(basis) * self.sugbrids.plane_project(V3,N,O)
		V2 = triangle_position.xy if is_inverted else 1-triangle_position.xy
		return i, V2

	def unit_sphere_position(self, subgrid_position):
		i,V2 = subgrid_position
		is_inverted = self.sugbrids.is_inverted_subgrid_position(V2)
		is_polar = self.sugbrids.is_polar_subgrid_id(i, is_inverted)
		triangle_position = V2 if is_inverted else 1-V2
		U2 = triangle_position
		Wid = i     # west   longitude id
		Oid = i + 1 # origin longitude id
		Eid = i + 2 # east   longitude id
		W = self.sugbrids.corner(Wid, self.sugbrids.z(Wid)) # W: westernmost triangle vertex
		E = self.sugbrids.corner(Eid, self.sugbrids.z(Eid)) # E: easternmost triangle vertex
		O = self.sugbrids.corner(Oid, self.sugbrids.origin_z(self.sugbrids.subgrid_polarity(i), is_polar))
		basis = self.sugbrids.basis(W,E,O, is_inverted)
		return self.sugbrids.sphere_project(basis * glm.vec3(U2.x,U2.y,1))

	def standardize(self, subgrid_position):
		i,V2 = subgrid_position
		is_inverted = self.sugbrids.is_inverted_subgrid_position(V2)
		is_polar = self.sugbrids.is_polar_subgrid_id(i, is_inverted)
		clampedV2 = glm.clamp(V2, 0, 1)
		are_local = glm.equal(V2, clampedV2)
		are_nonlocal = glm.notEqual(V2, clampedV2)
		deviation = V2 - clampedV2
		are_flipped = glm.equal(glm.sign(deviation), glm.vec2(self.sugbrids.subgrid_polarity(i)))
		is_nonlocal = are_nonlocal.x or are_nonlocal.y
		is_flipped = are_flipped.x or are_flipped.y
		inverted_nonlocal = (
			glm.vec2(are_local)*V2 + glm.vec2(are_nonlocal)*(1-V2) 
			if is_flipped else V2)
		modded = inverted_nonlocal%1
		flipped = modded.yx if is_flipped else modded.xy
		deviation_sign = glm.dot(glm.vec2(1,-1), glm.sign(deviation))
		di = 2 * deviation_sign * is_flipped + deviation_sign * (is_nonlocal and not is_flipped)
		return ((i+di)%subgrid_count, flipped)


grids = Projection(Subprojection())
samplecount = 10000
golden_ratio = 1.618033
golden_angle = turn/golden_ratio

z   = [2*i/samplecount-1  	for i in range(samplecount)]
lon = [(i*golden_angle)%turn for i in range(samplecount)]

unit_sphere_position = [
	cartesian_from_zlon(z,lon)
	for z,lon in zip(z,lon)]

subgrid_position = [
	grids.subgrid_position(V3)
	for V3 in unit_sphere_position]

reconstructed = [
	grids.unit_sphere_position(iV2)
	for iV2 in subgrid_position]

standardize = [
	# grids.standardize(i, V2 + glm.vec2(-0.1,0))
	grids.standardize((i, V2 + glm.vec2(0,-0.1)))
	for i,V2 in subgrid_position]

standardize_sphere_position = [
	grids.unit_sphere_position(iV2)
	for iV2 in standardize]

def vector_aoa(vector_aos):
	return [
		[V.x for V in vector_aos],
		[V.y for V in vector_aos],
		[V.z for V in vector_aos]
	]

subgrid_id = [i    for i,V2 in subgrid_position]
subgrid_x = [V2.x for i,V2 in subgrid_position]
subgrid_y = [V2.y for i,V2 in subgrid_position]

standardize_i = [i    for i,V2 in standardize]
standardize_x = [V2.x for i,V2 in standardize]
standardize_y = [V2.y for i,V2 in standardize]

offset     = [V-N for V,N in zip(reconstructed, unit_sphere_position)]
distance   = [glm.length(o) for o in offset]
similarity = [glm.dot(V,N)/(glm.length(V)*glm.length(N)) 
	for V,N in zip(reconstructed, unit_sphere_position)]

standardize_offset = [V-N for V,N in zip(standardize_sphere_position, unit_sphere_position)]

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

go.Figure(data=go.Cone(**domain, **dict(zip('uvw', vector_aoa(reconstructed))))).show()
# go.Figure(data=go.Cone(**domain, **dict(zip('uvw', vector_aoa(offset))), sizeref=0.03)).show()
px.scatter_3d(**domain,color=distance).show()
px.scatter_3d(**domain,color=similarity).show()
go.Figure(data=go.Cone(**domain, **dict(zip('uvw', vector_aoa(standardize_offset))), sizeref=2)).show()
px.scatter_3d(**domain,color=standardize_i).show()
px.scatter_3d(**domain,color=standardize_x).show()
px.scatter_3d(**domain,color=standardize_y).show()

# debug_vector_x = [V2.x for V2 in debug_vector]
# debug_vector_y = [V2.y for V2 in debug_vector]
# px.scatter_3d(**domain,color=debug_vector_x).show()
# px.scatter_3d(**domain,color=debug_vector_y).show()

# px.scatter_3d(**domain,color=debug_scalar).show()
