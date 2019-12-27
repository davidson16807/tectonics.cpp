Questions I have for the morning: 
How come you need ellipsoids? Seems like all you'd need for a hyperdrive check is a line-sphere intersection, unless you're checking for the intersection with a planet's orbit?
Actually, come to think of it, you could derive a generic ray-orbit intersection with just a ray-plane and point-cone test: just do a ray-plane intersection, then test whether the point of intersection lies inside a cone that's been oriented to form a particular orbit along that plane. Works for hyperbolic orbits, too! Just need to figure out how to orient the cone, which I'd be surprised if someone hadn't already done it. 
Another thing: his ray-cone intersection could be really useful for ray marching algorithms that have to work with a planet's umbra/penumbra
Felt like I was wasting so much time building up a working knowledge of geometric intersections, now I'm feeling a little better thinking for it

You know the area of an ellipsis. If you assume a simple patched conics model and know the sphere of influence for a planet you can figure out the area its sweeping out simply by taking the difference in area between a big ellipsis and a small ellipsis. 

tools I'd like to add to my tool box:
* Is there a generic way to detect intersections between rays and extruded objects, either rotationally extruded or linearly extruded?
* is there a way to perform ellipsis-ellipsis intersections, or a class of ellipsis-object intersections?
* can we combine what we know about ellipsis intersections with integral calculus and the boltzmann distribution to somehow prove useful modeling solar nebula accretion?
* There are many intersection problems that have any number of solutions, from 0 to many. Is there a generic way to solve these, by for instance, starting with a generic solution for an intersection object like a sphere-sphere intersection and then whittling it down to also include other intersections?




Math seems to be a lot more about building up tools and then exploring them rather than setting out to solve a problem and then solving it. Tools should be simple, generic, and easily able to interact with one another. 

For instance, we see when performing intersections that we start out with a ray-object intersection, then whittle it down using a point-object intersection. Might this apply more generically? For instance, start with a sphere-sphere intersection, producing a sphere-sphere object then whittle it down to a (sphere-sphere)-plane? 