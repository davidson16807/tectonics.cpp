
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

class Collignon:
	def __init__(self, triangle_area, longitude0, debug=False):
		self.turn = 2*math.pi
		self.leglength = math.sqrt(2*triangle_area)
		self.longitude0 = longitude0
		self.debug = debug

	def grid_id(self, sphere_position):
		V3 = sphere_position
		longitude = math.atan2(V3.y,V3.x)
		dlongitude = math.remainder(self.turn+longitude-self.longitude0, self.turn)
		legscale = math.sqrt(1-abs(V3.z))
		V2 = glm.vec2(
			legscale*2*dlongitude/self.leglength, 
			glm.sign(V3.z)*(1-legscale)*self.leglength)
		# if self.debug:
		# 	breakpoint()
		return V2

	def position(self, grid_id):
		x,y = grid_id.x, grid_id.y
		legscale = 1-abs(y)/self.leglength
		z = glm.sign(y) * (1-legscale**2)
		dlongitude = x*self.leglength/(2*legscale)
		longitude = self.longitude0 + dlongitude
		r = math.sqrt(1-z**2)
		return glm.vec3(
			r*math.cos(longitude),
			r*math.sin(longitude),
			z)

class Lambert:
	def __init__(self, longitude0, debug=False):
		self.turn = 2*math.pi
		self.longitude0 = longitude0
		self.debug = debug

	def grid_id(self, sphere_position):
		V3 = sphere_position
		longitude = math.atan2(V3.y,V3.x)
		dlongitude = math.remainder(self.turn+longitude-self.longitude0, self.turn)
		return glm.vec2(dlongitude, V3.z)

	def position(self, grid_id):
		dlongitude,z = grid_id.x, grid_id.y
		longitude = self.longitude0 + dlongitude
		r = math.sqrt(1-z**2)
		V3 = glm.vec3(
			r*math.cos(longitude),
			r*math.sin(longitude),
			z)
		# if self.debug:
		# 	breakpoint()
		return V3

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

	def is_polar_sphere_position(self, V3):
		return abs(V3.z) > 0.5

	def is_eastern_sphere_position(self, V3, N, S):
		# V3⋅(N×S)>0 indicates that V3 occupies the grid indicated by Eid
		return glm.dot(V3, glm.cross(N,S)) > 0

	def basis(self, is_inverted, W,E,O):
		return glm.mat2(E-O,W-O) if is_inverted else glm.mat2(W-O,E-O)

	def sphere_project(self,V3):
		return glm.normalize(V3)

	def plane_project(self,V3,N,O):
		return V3 * glm.dot(N, O) / glm.dot(N, V3)

class Projection:
	def __init__(self, square, triangle):
		self.squares = square
		self.triangles = triangle
		self.total_area = 4*math.pi
		self.square_count = 10
		self.triangle_count = 20
		self.area_per_square = self.total_area/self.square_count
		self.area_per_triangle = self.total_area/self.triangle_count

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
		idlength = half_square_longitude_arc_length
		longitude = math.atan2(V3.y,V3.x)
		EWid = longitude/idlength
		Nid = 2*round(EWid/2)       # Nid: northernmost edge vertex id
		Sid = 2*round((EWid-1)/2)+1 # Sid: southernmost edge vertex id
		N = self.squares.westmost(Nid)
		S = self.squares.westmost(Sid)
		i = (min(Nid,Sid)-1 + self.triangles.is_eastern_sphere_position(V3,N,S)) % square_count
		Wid = i     # west   longitude id
		Oid = i + 1 # origin longitude id
		Eid = i + 2 # east   longitude id
		W = self.squares.westmost(Wid) # W: westernmost triangle vertex
		E = self.squares.westmost(Eid) # E: easternmost triangle vertex
		is_polar = self.triangles.is_polar_sphere_position(V3)
		is_inverted = self.triangles.is_inverted_square_id(i, is_polar)
		O = self.triangles.origin(Oid, self.squares.polarity(i), is_polar)
		# debug = (glm.distance(V3,glm.vec3(-0.666,-0.666,-0.333)) < 0.03)
		projection = (
			Collignon(self.area_per_triangle, Oid*idlength) if is_polar else 
			Lambert(Oid*idlength))
		basis = self.triangles.basis(
			is_inverted,
			projection.grid_id(W),
			projection.grid_id(E),
			projection.grid_id(O)
		)
		# W2: position on a triangle in projected coordinates
		W2 = projection.grid_id(V3)-projection.grid_id(O)
		# U2: position on a triangle in stored coordinates
		U2 = glm.inverse(basis) * W2
		# V2: position on a square
		V2 = U2 if is_inverted else 1-U2
		# if debug: breakpoint()
		return self.standardize((i, V2))

	def position(self, grid_id):
		idlength = half_square_longitude_arc_length
		# V2: position on a square
		i,V2 = (grid_id)
		# i,V2 = self.standardize(grid_id)
		is_inverted = self.triangles.is_inverted_grid_id(V2)
		is_polar = self.triangles.is_polar_square_id(i, is_inverted)
		# U2: position on a triangle in stored coordinates
		U2 = V2 if is_inverted else 1-V2
		Wid = i     # west   longitude id
		Oid = i + 1 # origin longitude id
		Eid = i + 2 # east   longitude id
		debug = (glm.distance(V2,glm.vec2(0.708,0.458)) < 0.03 and i==5)
		projection = (
			Collignon(self.area_per_triangle, Oid*idlength) if is_polar else 
			Lambert(Oid*idlength,debug))
		W = self.squares.westmost(Wid) # W: westernmost triangle vertex
		E = self.squares.westmost(Eid) # E: easternmost triangle vertex
		O = self.triangles.origin(Oid, self.squares.polarity(i), is_polar)
		basis = self.triangles.basis(
			is_inverted,
			projection.grid_id(W),
			projection.grid_id(E),
			projection.grid_id(O)
		)
		# W2: position on a triangle in projected coordinates
		W2 = basis * U2
		V3 = projection.position(W2+projection.grid_id(O))
		if debug: breakpoint()
		return V3


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
	projection.standardize((i, V2 + glm.vec2(0,0.1)))
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
print('x', min([i for i in square_x if not math.isnan(i)]), max([i for i in square_x if not math.isnan(i)]))
print('y', min([i for i in square_y if not math.isnan(i)]), max([i for i in square_y if not math.isnan(i)]))

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

# go.Figure(data=go.Cone(**domain, **dict(zip('uvw', vector_aoa(offset))), sizeref=0.03)).show()
px.scatter_3d(**domain,color=distance).show()
# px.scatter_3d(**domain,color=similarity).show()

go.Figure(data=go.Cone(**domain, **dict(zip('uvw', vector_aoa(standardize_offset))), sizeref=2)).show()
