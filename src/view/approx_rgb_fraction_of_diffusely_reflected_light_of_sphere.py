from math import pi
import numpy as np
import plotly.express as px

'''
We assume a similar scenario as the one used 
in the RealisticSphereSwarmShaderProgram fragment shader,
i.e. there is a unit sphere, and V=[0,0,1] faces the viewer.
In this scenario, the light vector L is offset from V 
along the xy plane according to an angle, theta.
The angle theta is the only parameter needed to understand 
the behavior of F, which is the fraction of light striking a sphere
that is reflected onto the viewer according to lambertian reflection.
'''

n=300
x = np.array([
	[i/n-1 for j in range(2*n)]
	for i in range(2*n)
])
y = np.array([
	[j/n-1 for j in range(2*n)]
	for i in range(2*n)
])
z = 1-(x*x+y*y)
I=np.sum(z>=0)

def F(theta):
	return np.sum((z>=0)*np.maximum(0, x*np.sin(theta)+z*np.cos(theta)))/I

thetas = np.linspace(0,pi,100)
Fs = np.array([F(theta) for theta in thetas])

'''
2*F(0)==1 and 2*F(pi)==0, so 2*F(theta) is roughly (cos(theta)+1)/2,
but F for the middle values of theta slightly disagree.
We correct by applying an exponent, `fudge`.
'''

fudge = 0.135 # this value minimizes error as measured by distance()
fudge = 0.149 # this value minimizes error as measured by worst_case()

figure = px.line(x=thetas, y=np.arccos(4*Fs-1)-thetas)
figure.add_scatter(x=thetas, y=fudge*(1-np.cos(2*thetas))/2)
figure.show()

figure = px.line(x=thetas, y=Fs)
figure.add_scatter(x=thetas, y=(np.cos(thetas + fudge*(1-np.cos(2*thetas))/2) + 1)/4)
figure.show()

def distance(fudge):
	F2s = (np.cos(thetas + fudge*(1-np.cos(2*thetas))/2)+1)/4
	return np.sum((F2s-Fs)**2)

def worst_case(fudge):
	F2s = (np.cos(thetas + fudge*(1-np.cos(2*thetas))/2)+1)/4
	return np.max(np.abs(F2s-Fs))
