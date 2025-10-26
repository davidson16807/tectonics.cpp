
import math

import glm
import plotly.express as px

def sign(x):
    return 0 if x==0 else 1 if x>0 else -1

class State:
    def __init__(self, position, velocity):
        self.position = position
        self.velocity = velocity
    def __repr__(self):
        return f"State({list(self.position)}, {list(self.velocity)})"
    def __str__(self):
        return "{"+f"r={list(self.position)}, v={list(self.velocity)}"+"}"

def state_distance(state1, state2):
	return glm.distance(state1.position, state2.position) + glm.distance(state1.velocity, state2.velocity)

class Orbit:
    def __init__(self, gravitational_parameter, time_offset, initial_position, initial_velocity):
        self.gravitational_parameter = gravitational_parameter
        self.time_offset = time_offset
        self.initial_position = initial_position
        self.initial_velocity = initial_velocity
    def __repr__(self):
        return (f"Orbit("
                f"{self.gravitational_parameter}, "
                f"{self.time_offset}, "
                f"{list(self.initial_position)}, "
                f"{list(self.initial_velocity)})")
    def __str__(self):
        return ("{"
                f"μ={self.gravitational_parameter}, "
                f"t₀={self.time_offset}, "
                f"r₀={list(self.initial_position)}, "
                f"v₀={list(self.initial_velocity)}"
                "}")

def C(y):
	abs_y = abs(y)
	return (((1 - math.cos(math.sqrt(abs_y))) if y > 0 else
			 (math.cosh(math.sqrt(abs_y)) - 1))
		/ abs_y)

def S(y):
	sqrt_abs_y = math.sqrt(abs(y))
	sqrt_abs_y3 = sqrt_abs_y*sqrt_abs_y*sqrt_abs_y
	return (((sqrt_abs_y - math.sin(sqrt_abs_y))   if y > 0 else
			 (math.sinh(sqrt_abs_y) - sqrt_abs_y))
		/ sqrt_abs_y3)

def refine(x, a, r0, sigma0, dtsqrtmu, laguerre_method_n = 5):
	x2 = x*x
	x3 = x*x2
	ax2 = a*x2
	Cax2 = C(ax2)
	Sax2 = S(ax2)
	one_r0a = 1-r0*a
	F      = sigma0 * x2 * Cax2 + one_r0a*x3 * Sax2 + r0*x - dtsqrtmu
	dFdx   = sigma0*x*(1-ax2*Sax2) + one_r0a*x2*Cax2 + r0
	n = laguerre_method_n
	n_1 = n-1
	d2Fdx2 = sigma0 - sigma0*ax2*(Cax2 - 3*Sax2) + (one_r0a/x)*(1 - ax2*Sax2 - 2*Cax2)
	return -(n*F / # Laguerre method 
		(dFdx + sign(dFdx) * math.sqrt(abs(n_1*n_1*dFdx*dFdx - n*n_1*F*d2Fdx2))))

def solve(x0, a, r0, sigma0, dtsqrtmu, max_refinement_count = 10, max_precision = 1e-10, laguerre_method_n = 5, debug=False):
	x = x0
	for i in range(max_refinement_count):
		dxdi = refine(x, a, r0, sigma0, dtsqrtmu, laguerre_method_n)
		if dxdi:
			x += dxdi
		if abs(dxdi) < max_precision:
			print(i, dxdi)
			return x
	print(max_refinement_count, dxdi)
	return x

def state(orbit, t, max_refinement_count = 10, laguerre_method_n = 5):
	t0 = orbit.time_offset
	R0 = orbit.initial_position
	V0 = orbit.initial_velocity
	mu = orbit.gravitational_parameter
	sqrt_mu = math.sqrt(mu)
	r0 = glm.length(R0)
	v0 = glm.length(V0)
	a = 2/r0 - v0*v0/mu
	dt = t-t0
	x = solve(
		x0 = (dt*sqrt_mu*a if a>0 # elliptic
			else sign(dt) * math.sqrt(-a) 
				* math.log(-2*mu*dt / 
					(a * (glm.dot(R0,V0) + sign(dt) * math.sqrt(-mu*a) * (1-r0/a)))) # hyperbolic
		),
		a=a, r0=r0, 
		sigma0 = glm.dot(R0, V0) / sqrt_mu, 
		dtsqrtmu = dt*sqrt_mu, 
		max_refinement_count = max_refinement_count, 
		laguerre_method_n = laguerre_method_n
	)
	x2 = x*x
	x3 = x*x2
	ax2 = a*x2
	Cax2 = C(ax2)
	Sax2 = S(ax2)
	R  = ((1 - x2/r0 * Cax2) * R0 + 
		  ((t-t0) - x3/sqrt_mu * Sax2) * V0)
	r  = glm.length(R)
	V  = ((x*sqrt_mu/(r*r0) * (ax2 * Sax2 - 1.0)) * R0 +
		  (1.0 - x2/r * Cax2) * V0)
	return State(R,V)

def advance(orbit, t, max_refinement_count = 10, laguerre_method_n = 5):
	state_ = state(orbit, t, max_refinement_count = 10, laguerre_method_n = 5)
	return Orbit(orbit.gravitation_parameter, 0.0, state_.position, state_.velocity)

def tare(orbit):
	return advance(orbit, orbit.time_offset)

def plot(orbit, period, sample_count = 10000):
	# ts = range(-int(period/2), int(period/2), int(period/sample_count))
	ts = range(1, int(period), int(period/sample_count))
	states=[state(orbit, t, max_refinement_count=100, laguerre_method_n = 5) for t in ts]
	rs = [state.position for state in states]
	vs = [state.velocity for state in states]
	'''
	we need the plot to display the same bounds for everything 
	so that orbits don't appear squished
	'''
	m = max(max([i for r in rs for i in [r.x,r.y,r.z]]),
		   -min([i for r in rs for i in [r.x,r.y,r.z]]))
	px.scatter_3d(
		x=[0]+[r.x for r in rs], 
		y=[0]+[r.y for r in rs],
		z=[0]+[r.z for r in rs],
		color=[2,1]+[0 for r in rs[1:]],
		range_x=[-m,m],
		range_y=[-m,m],
		range_z=[-m,m],
	).show()

class OrbitalProperties:
	def __init__(self):
		

def make_orbit(
	semi_major_axis,
	eccentricity,
	inclination,
	longitude_of_ascending_node,
	argument_of_periapsis,
	mean_anomaly
):
	I = properties.vernal_equinox_direction
	# J = glm.cross(properties.north_pole_direction, properties.vernal_equinox_direction)
	K = properties.north_pole_direction

	eccentric_anomaly  = properties.solve_eccentric_anomaly_from_mean_anomaly  (elements.mean_anomaly, elements.eccentricity)
	true_anomaly       = properties.get_true_anomaly_from_eccentric_anomaly    (eccentric_anomaly, elements.eccentricity)
	semi_latus_rectum  = properties.get_semi_latus_rectum_from_semi_major_axis_and_eccentricity (elements.semi_major_axis, elements.eccentricity)
	perifocal_position = properties.get_perifocal_position(semi_latus_rectum, elements.eccentricity, true_anomaly)
	perifocal_velocity = properties.get_perifocal_velocity(semi_latus_rectum, elements.eccentricity, true_anomaly)

	# NOTE: we use mat4x4 since it is the only thing that rotate() works with
    perifocal_to_reference_matrix = glm.mat4(1)
    perifocal_to_reference_matrix = glm.rotate(perifocal_to_reference_matrix, elements.longitude_of_ascending_node, K)
    perifocal_to_reference_matrix = glm.rotate(perifocal_to_reference_matrix, elements.inclination,                 I)
    perifocal_to_reference_matrix = glm.rotate(perifocal_to_reference_matrix, elements.argument_of_periapsis,       K)

    position = perifocal_to_reference_matrix * glm.vec4(perifocal_position, 1)
	velocity = perifocal_to_reference_matrix * glm.vec4(perifocal_velocity, 1)
	# NOTE: we convert to vec4 since it is the only thing glm allows to multiply with mat4
    return State(position, velocity)

mu_sun = 1.32712440018e20
au = 1.5e11

eccentric = Orbit(3.986004418e14, 0.0,
	glm.vec3(384399e3,0.0,0.0),
	glm.vec3(0.0,1.44e3,0.0)
	# glm.vec3(0.0,0.4e3,0.0)
)
plot(eccentric, 60*60*24*30*1000, sample_count = 6000)

satellite = orbit = Orbit(3.986004418e14, 0.0,
	glm.vec3(10e6,0.0,0.0),
	glm.vec3(0.0,9.2e3,0.0)) # from example 2.2 in Conway & Prussing
plot(satellite, 60*60*24*30, sample_count = 3000)

halley = Orbit(1.32712440018e20, 0.0,
	glm.vec3(0.59*au,0.0,0.0), 
	glm.vec3(0.0,70.56e3,0.0))
plot(halley, 60*60*24*365*80, sample_count = 6000)

moon = orbit = Orbit(3.986004418e14, 0.0,
	glm.vec3(384399e3,0.0,0.0),
	glm.vec3(0.0,1.022e3,0.0))
plot(moon, 60*60*24*30)

earth = Orbit(1.32712440018e20, 0.0,
	glm.vec3(au,0.0,0.0), 
	glm.vec3(0.0,29.7e3,0.0))
plot(earth, 60*60*24*365)

eccentric = Orbit(1.32712440018e20, 0.0,
	glm.vec3(0.59*au,0.0,0.0), 
	glm.vec3(0.0,54e3,0.0))
plot(eccentric, 60*60*24*365*80)

mars = Orbit(1.32712440018e20, 0.0,
	glm.vec3(1.3814*au,0.0,0.0), 
	glm.vec3(0.0,24.07e3,0.0))
plot(mars, 60*60*24*365*3)

plot(moon, 60*60*24*30)

hyperbolics = [
	
]

elliptics = [ 
    from_6_element(mu=mu_sun, a=5.790905e10,   e=0.2056), # Mercury
    from_6_element(mu=mu_sun, a=1.082080e11,   e=0.0068), # Venus
    from_6_element(mu=mu_sun, a=1.49598023e11, e=0.0167), # Earth
    from_6_element(mu=mu_sun, a=2.279392e11,   e=0.0934), # Mars
    from_6_element(mu=mu_sun, a=7.78570e11,    e=0.0489), # Jupiter
    from_6_element(mu=mu_sun, a=1.43353e12,    e=0.0565), # Saturn
    from_6_element(mu=mu_sun, a=2.87246e12,    e=0.046), # Uranus
    from_6_element(mu=mu_sun, a=4.49506e12,    e=0.009), # Neptune
]

'''

test cases:

	circular lunar satellite
	circular earth satellite
	eccentric lunar satellite
	eccentric earth satellite
	eccentric solar satellite
	all planets
	the sun around the galactic core

for any orbit:

	group:
		advance(o,0) = o
		advance(advance(o,a),b) = advance(advance(o,b),a) 
		advance(advance(o,a),-a) = o
	commutativity:
		advance(advance(o,a),b) = advance(o,a+b)
	behavioral equality (trivial):
		advance(o,0) = state(o)

	identity:
		state(o,0) = o
	state continuity:
		d(state(o,ε), state(o,0)) < d(state(o,2ε), state(o,0))
	invariance under reference time:
		state(o,t) ≈ state(tare(o),t)

	tare idempotence:
		tare(tare(o)) = tare(o)

	gravitational parameter preservation:
		advance(o,t).gravitational_parameter == orbit.gravitational_parameter
		tare(o).gravitational_parameter == orbit.gravitational_parameter

	energy is conserved:
		energy(advance(o,t)) == energy(o)
	angular momentum is conserved
		angular momentum(advance(o,t)) == angular momentum(o)

for known elliptic orbits:
	congruence:
		state(o,n*T) = o for any n∈ℕ
	involute: 
		advance(advance(o, T/2), T/2) = o
	orbit: 
		advance(advance(advance(o, T/3), T/3), T/3) = o etc.
	state(periapsis,T/2).position = apoapsis
	t=0 minimizes glm.length(o↦state(periapsis,t).position)
	t=0 maximizes glm.length(o↦state(periapsis,t).velocity)
	t=0 maximizes glm.length(o↦state(apoapsis,t).position)
	t=0 minimizes glm.length(o↦state(apoapsis,t).velocity)

for circular orbits:
	glm.length(advance(o,t).position) is constant for any t

given: 
	o 	orbit
	T 	period
	d 	distance()

'''

