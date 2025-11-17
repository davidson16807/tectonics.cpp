
import math

from pyglm import glm

from State import *
from Universals import *
from Properties import Properties
from ElementsAndState import ElementsAndState
from Elements import Elements

def sign(x):
    return 0 if x==0 else 1 if x>0 else -1

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

class UniversalPropagator:

	def __init__(self, 
		standard_gravitational_parameter,
		max_refinement_count = 10, max_precision = 1e-10, laguerre_method_n = 5
	):
		self.standard_gravitational_parameter = standard_gravitational_parameter
		self.max_refinement_count = 10
		self.max_precision = 1e-10
		self.laguerre_method_n = 5

	def refine(self, x, a, r0, sigma0, dtsqrtmu):
		x2 = x*x
		x3 = x*x2
		ax2 = a*x2
		Cax2 = C(ax2)
		Sax2 = S(ax2)
		one_r0a = 1-r0*a
		F      = sigma0 * x2 * Cax2 + one_r0a*x3 * Sax2 + r0*x - dtsqrtmu
		dFdx   = sigma0*x*(1-ax2*Sax2) + one_r0a*x2*Cax2 + r0
		n = self.laguerre_method_n
		n_1 = n-1
		d2Fdx2 = sigma0 - sigma0*ax2*(Cax2 - 3*Sax2) + (one_r0a/x)*(1 - ax2*Sax2 - 2*Cax2)
		return -(n*F / # Laguerre method 
			(dFdx + sign(dFdx) * math.sqrt(abs(n_1*n_1*dFdx*dFdx - n*n_1*F*d2Fdx2))))

	def solve(self, x0, a, r0, sigma0, dtsqrtmu):
		x = x0
		for i in range(self.max_refinement_count):
			dxdi = self.refine(x, a, r0, sigma0, dtsqrtmu)
			if dxdi:
				x += dxdi
			if abs(dxdi) < self.max_precision:
				return x
		return x

	def state(self, orbit, t):
		t0 = orbit.time_offset
		R0 = orbit.initial_position
		V0 = orbit.initial_velocity
		mu = self.standard_gravitational_parameter * orbit.combined_mass
		sqrt_mu = math.sqrt(mu)
		r0 = glm.length(R0)
		v0 = glm.length(V0)
		a = 2/r0 - v0*v0/mu
		dt = t-t0
		x = self.solve(
			x0 = (dt*sqrt_mu*a if a>0 # elliptic
				else sign(dt) * math.sqrt(-a) 
					* math.log(-2*mu*dt / 
						(a * (glm.dot(R0,V0) + sign(dt) * math.sqrt(-mu*a) * (1-r0/a)))) # hyperbolic
			),
			a=a, r0=r0, 
			sigma0 = glm.dot(R0, V0) / sqrt_mu, 
			dtsqrtmu = dt*sqrt_mu, 
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

	def advance(self, orbit, t):
		state_ = self.state(orbit, t)
		return Universals(orbit.combined_mass, 0.0, state_.position, state_.velocity)

	def tare(self, orbit):
		return self.advance(orbit, orbit.time_offset)

