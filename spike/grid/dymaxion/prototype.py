import math

class Subgrid:
	def __init__(self):
		self.subgrid_leg_arc_length = math.pi/3 # congruence modulo for subgrid coordinates

	def vertex_position(self, longitude_id, z):
		longitude = longitude_id*half_subgrid_longitude_arc_length
		r = math.sqrt(1-z**2)
		return glm.vec3(
			r*math.cos(longitude), 
			r*math.sin(longitude),
			z)

	def subgrid_id(self, unit_sphere_position):
		V = unit_sphere_position
		subgrid_count = 10
		longitude = math.atan2(V.y,V.x) + math.pi
		half_subgrid_longitude_arc_length = 2*math.pi / subgrid_count
		longitude_id = longitude/half_subgrid_longitude_arc_length
		Nid = 2*math.round(longitude_id/2)
		Sid = 2*math.round((longitude_id-1)/2)+1
		Wid = math.floor(longitude_id)
		Eid = math.ceil(longitude_id)
		N = vertex_position(Nid, Nid%2-0.5)
		S = vertex_position(Sid, Sid%2-0.5)
		# v⋅(n×s)>0 indicates that v occupies the grid indicated by e
		return Wid-1 + (glm.dot(V, glm.cross(N, S)) > 0)

	def grid_position_v0(self, unit_sphere_position, subgrid_id):
		n = self.subgrid_leg_arc_length
		is_north = subgrid_id%2
		is_south = not is_north
		V = unit_sphere_position
		R = vertex_position(subgrid_id+2*is_south)
		C = vertex_position(subgrid_id+1)
		L = vertex_position(subgrid_id+2*is_north)
		sign_z = (-1)**is_north
		return glm.vec2(
			math.atan2(glm.dot(V,glm.normalize(glm.cross(R,C)*sign_z)), glm.dot(V,glm.normalize(R+C))) % N
			math.atan2(glm.dot(V,glm.normalize(glm.cross(C,L)*sign_z)), glm.dot(V,glm.normalize(C+L))) % N
		)

	def grid_position_v1(self, unit_sphere_position, subgrid_id):
		n = self.subgrid_leg_arc_length
		V = unit_sphere_position
		i = subgrid_id
		z = i%2-0.5
		N = vertex_position(i+1, min(1, z+1))
		W = vertex_position(i,   z)
		S = vertex_position(i+1, max(-1,z-1))
		return glm.vec2(
			math.atan2(glm.dot(V,glm.normalize(glm.cross(W,N))), glm.dot(V,glm.normalize(W+N))) % n
			math.atan2(glm.dot(V,glm.normalize(glm.cross(W,S))), glm.dot(V,glm.normalize(W+S))) % n
		)

	def grid_position_v3(self, unit_sphere_position, subgrid_id):
		n = self.subgrid_leg_arc_length
		is_north = subgrid_id%2
		is_south = not is_north
		V = unit_sphere_position
		i = subgrid_id
		return glm.vec2(
			math.atan2(glm.dot(V,vertex_position(i+0.5+is_south, 0)), glm.dot(V,vertex_position(i+3+is_south, 0.5))) % n
			math.atan2(glm.dot(V,vertex_position(i+0.5+is_north, 0)), glm.dot(V,vertex_position(i-2+is_north, 0.5))) % n
		)

	def unit_sphere_position(self, subgrid_id, grid_position):
		return glm.vec3(
			grid_position
		)

def vertex_position():
