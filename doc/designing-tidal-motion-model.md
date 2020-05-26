We should like to design a way to model the effects of tidal forces between worlds. 

We are mainly interested in reproducing the following observations:

1.) When the moon orbits around the earth, the parts of the earth that are nearest
to the moon are pulled the hardest, causing the earth to get stretched. 
This stretching causes tides.

2.) In the same scenario, the parts of the moon that are nearest to the earth 
are also stretched. If the moon rotated faster than we see it today 
(that is, out of sync with its revolution) the parts of the moon 
that were nearest would get dragged backwards and start to rub up against 
parts that were further away. The rubbing causes heating, and it causes
motion to transfer to the parts that are further away. When all the forces
are considered together, there's a net effect that causes the moon's rotation to slow down.

3.) When the moon orbits around the earth, the moon sees the earth stretched 
out as from observation 1.). There are two parts of the earth that protrude:
one that's closer, and one that's further away. The part that's closer 
previously pointed to where the moon once was, 
but now it's been dragged along by the earth's rotation, 
so it's slightly ahead of the moon. Both parts of the earth are about the same mass,
but the part that's closer pulls the moon harder. 
The net effect drags the moon inward and slightly forwards, 
which over a long time causes the moon's revolution to speed up and it's orbit to move outward.

We should like to estimate the following from this scenario:
* how much does the earth stretch?
* how much does the moon heat up when nearer parts get dragged around?
* how much does the moon's rotation slow down?
* how much does the moon speed up when the protruding part of the earth that's closer is dragged forward?

What are our state variables?
* there is a continuum of mass for the earth and the moon
* there is a state of rotation for the earth and the moon
* there is an orbit for the earth and the moon

#Orbit
An orbit can be expressed either in terms of orbital elements (such as semi major axis and eccentricity),
or by state vectors (strictly including position and velocity). 
Orbits can be converted effortlessly from either state with the following category diagram:

 O     P
 ⇣     ⇣
OxM ⇆ PxV
 ⇡     ⇡
 M     V

given the following elements:
* **O** orbital elements
* **M** mean anomaly
* **P** position
* **V** velocity

where M and V can each be usefully updated in isolation 
to represent advancements in time (M) and applications of force (V)

#Mass Continuums
We need to find some way to represent mass continuum. 
Previously, we've used raster fields to describe how 