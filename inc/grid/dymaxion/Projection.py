
import math

import glm
import plotly.express as px
import plotly.graph_objects as go

turn = 2*math.pi
square_count = 10
half_square_longitude_arc_length = turn / square_count

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
* `i`: square id
'''

class Square:
	def __init__(self):
		pass

	def westmost(self, i):
		z = 0.5-i%2
		longitude = i*half_square_longitude_arc_length
		return cartesian_from_zlon(z,longitude)

	def polarity(self, i):
		return (-1)**i

class Triangle:
	def __init__(self):
		pass

	def origin(self, i, square_polarity, is_polar):
		z = square_polarity*[-0.5,1][is_polar]
		longitude = i*half_square_longitude_arc_length
		return cartesian_from_zlon(z, longitude)

	def is_inverted_square_id(self, i, is_polar):
		return is_polar == i%2

	def is_polar_square_id(self, i, is_inverted):
		return is_inverted == i%2

	def is_inverted_grid_id(self, V2):
		return V2.x+V2.y < 1

	def is_polar_sphere_position(self, square_polarity, V3, W, E):
		# V3⋅(W×E)>0 indicates whether V3 occupies a polar triangle
		return glm.dot(V3, square_polarity * glm.cross(W, E)) > 0

	def is_eastern_sphere_position(self, V3, N, S):
		# V3⋅(N×S)>0 indicates that V3 occupies the grid indicated by Eid
		return glm.dot(V3, glm.cross(N,S)) > 0

	def basis(self, is_inverted, W,E,O):
		return glm.mat3(E-O,W-O,O) if is_inverted else glm.mat3(W-O,E-O,O)

	def sphere_project(self,V3):
		return glm.normalize(V3)

	def plane_project(self,V3,N,O):
		return V3 * glm.dot(N, O) / glm.dot(N, V3)

class Projection:
	def __init__(self, square, triangle):
		self.squares = square
		self.triangles = triangle

	def standardize(self, grid_id):
		i,V2 = grid_id
		modded = V2%1
		square_polarity = glm.vec2(self.squares.polarity(i))
		are_nonlocal  = glm.notEqual(V2, modded)
		nonlocal_sign = glm.sign(V2 - 0.5) * glm.vec2(are_nonlocal)
		are_polar     = glm.equal(nonlocal_sign, square_polarity)
		are_nonpolar  = glm.notEqual(nonlocal_sign, square_polarity)
		is_nonlocal   = glm.any(are_nonlocal)
		is_corner     = glm.all(are_nonlocal)
		is_polar      = glm.any(are_polar)
		inverted      = glm.vec2(are_nonpolar)*modded + glm.vec2(are_polar)*(1-modded)
		flipped       = inverted.yx if is_polar else inverted
		nonlocal_id   = glm.dot(glm.vec2(1,-1), nonlocal_sign)
		di = 2 * nonlocal_id * is_polar + nonlocal_id * (is_nonlocal and not is_polar)
		# NOTE: there is more than one possible solution if both x>1 and y>1, 
		# and these solution are not representationally equal.
		# However the case where x=1 and y=1 is still valid and must be supported.
		# Therefore, we declare that standardize() is identity if both x≥1 and y≥1.
		standardized  = grid_id if is_corner else ((i+di)%square_count, flipped)
		return standardized

	def grid_id(self, sphere_position):
		V3 = sphere_position
		longitude = math.atan2(V3.y,V3.x)
		EWid = longitude/half_square_longitude_arc_length
		Nid = 2*round(EWid/2)       # Nid: northernmost edge vertex id
		Sid = 2*round((EWid-1)/2)+1 # Sid: southernmost edge vertex id
		N = self.squares.westmost(Nid)
		S = self.squares.westmost(Sid)
		print(V3,N,S,Nid,Sid)
		i = (min(Nid,Sid)-1 + self.triangles.is_eastern_sphere_position(V3,N,S)) % square_count
		print(self.triangles.is_eastern_sphere_position(V3,N,S), min(Nid,Sid)-1, i)
		Wid = i     # west   longitude id
		Oid = i + 1 # origin longitude id
		Eid = i + 2 # east   longitude id
		W = self.squares.westmost(Wid) # W: westernmost triangle vertex
		E = self.squares.westmost(Eid) # E: easternmost triangle vertex
		square_polarity = self.squares.polarity(i)
		is_polar = self.triangles.is_polar_sphere_position(square_polarity, V3, W,E)
		is_inverted = self.triangles.is_inverted_square_id(i, is_polar)
		O = self.triangles.origin(Oid, square_polarity, is_polar)
		basis = self.triangles.basis(is_inverted,W,E, O)
		Nhat = glm.cross(basis[0], basis[1])
		triangle_position = glm.inverse(basis) * self.triangles.plane_project(V3,Nhat,O)
		V2 = triangle_position.xy if is_inverted else 1-triangle_position.xy
		return i, V2

	def position(self, grid_id):
		i,V2 = (grid_id)
		# i,V2 = self.standardize(grid_id)
		is_inverted = self.triangles.is_inverted_grid_id(V2)
		is_polar = self.triangles.is_polar_square_id(i, is_inverted)
		triangle_position = V2 if is_inverted else 1-V2
		Wid = i     # west   longitude id
		Oid = i + 1 # origin longitude id
		Eid = i + 2 # east   longitude id
		W = self.squares.westmost(Wid) # W: westernmost triangle vertex
		E = self.squares.westmost(Eid) # E: easternmost triangle vertex
		O = self.triangles.origin(Oid, self.squares.polarity(i), is_polar)
		basis = self.triangles.basis(is_inverted,W,E, O)
		U2 = triangle_position
		return self.triangles.sphere_project(basis * glm.vec3(U2.x,U2.y,1))


projection = Projection(Square(), Triangle())

projection.standardize((0,glm.vec2(0,1)))

iV2 = projection.grid_id(glm.vec3(-1,-1,1))


samplecount = 10000
golden_ratio = 1.618033
golden_angle = turn/golden_ratio

z   = [2*i/samplecount-1  	for i in range(samplecount)]
lon = [(i*golden_angle)%turn for i in range(samplecount)]

position = [
	cartesian_from_zlon(z,lon)
	for z,lon in zip(z,lon)]

grid_id = [
	projection.grid_id(V3)
	for V3 in position]

reconstructed = [
	projection.position(iV2)
	for iV2 in grid_id]

standardize = [
	# projection.standardize(i, V2 + glm.vec2(-0.1,0))
	projection.standardize((i, V2 + glm.vec2(0,-0.1)))
	for i,V2 in grid_id]

standardize_sphere_position = [
	projection.position(iV2)
	for iV2 in standardize]

def vector_aoa(vector_aos):
	return [
		[V.x for V in vector_aos],
		[V.y for V in vector_aos],
		[V.z for V in vector_aos]
	]

square_id = [i    for i,V2 in grid_id]
square_x = [V2.x for i,V2 in grid_id]
square_y = [V2.y for i,V2 in grid_id]

standardize_i = [i    for i,V2 in standardize]
standardize_x = [V2.x for i,V2 in standardize]
standardize_y = [V2.y for i,V2 in standardize]

offset     = [V-N for V,N in zip(reconstructed, position)]
distance   = [glm.length(o) for o in offset]
similarity = [glm.dot(V,N)/(glm.length(V)*glm.length(N)) 
	for V,N in zip(reconstructed, position)]

standardize_offset = [V-N for V,N in zip(standardize_sphere_position, position)]

domain = dict(zip('xyz', vector_aoa(position)))

px.scatter_3d(**domain,color=square_id).show()
px.scatter_3d(**domain,color=square_x).show()
px.scatter_3d(**domain,color=square_y).show()

# px.scatter_3d(**domain,color=Nx).show()
# px.scatter_3d(**domain,color=roundtrip_x).show()
# px.scatter_3d(**domain,color=Ny).show()
# px.scatter_3d(**domain,color=roundtrip_y).show()
# px.scatter_3d(**domain,color=Nz).show()
# px.scatter_3d(**domain,color=roundtrip_z).show()

# go.Figure(data=go.Cone(**domain, **dict(zip('uvw', vector_aoa(reconstructed))))).show()
# # go.Figure(data=go.Cone(**domain, **dict(zip('uvw', vector_aoa(offset))), sizeref=0.03)).show()
# px.scatter_3d(**domain,color=distance).show()
# px.scatter_3d(**domain,color=similarity).show()
go.Figure(data=go.Cone(**domain, **dict(zip('uvw', vector_aoa(standardize_offset))), sizeref=2)).show()
# px.scatter_3d(**domain,color=standardize_i).show()
# px.scatter_3d(**domain,color=standardize_x).show()
# px.scatter_3d(**domain,color=standardize_y).show()

# debug_vector_x = [V2.x for V2 in debug_vector]
# debug_vector_y = [V2.y for V2 in debug_vector]
# px.scatter_3d(**domain,color=debug_vector_x).show()
# px.scatter_3d(**domain,color=debug_vector_y).show()

# px.scatter_3d(**domain,color=debug_scalar).show()
