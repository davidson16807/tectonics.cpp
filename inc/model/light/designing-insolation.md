
# Information

It is generally safe to assume that, at the scale of a star system, any source of light that is substantial enough to drive climate will be a point source and a black body emitter, such as a star, white dwarf, or brown dwarf. Not all sources of light at this scale match this description - to illustrate:

* **the hot disc of a black hole** It is a black body emitter, but not a point source. It emits light as a function of the surface normal of a disc, so it does not emit uniformly in all directions as a point source.
* **the jet of a black hole** It is a black body emitter, but not a point source. It emits light as a function of the tangent to a ray, so it does not emit uniformly in all directions as a point source.
* **the light of earth's moon** It is neither a black body emitter, nor a point source. It emits light as a function that is the integral of a function over all surface normals. It spectra is that of the sun's spectra when filtered by the albedo of the moon's surface. While the light of the moon affects the visibility of objects, it does not significantly contribute to earth's climate. Reflected sunlight from a body reduces as the inverse square of distance from the sun, the distance to its parent is on the same order as other moons if not closer, and since the earth's moon is the closest of any moon to the sun, it contributes substantially more heat from reflected light than any other moon, so we can safely say there is no body in the solar system where this kind of reflected light appreciably affects a body's temperature. Exotic scenarios may exist around other stars where this is not the case. For instance, an airless tidally-locked super earth that is close to its star may have a moon that contributes the only significant heat source to its night side. However to reduce scope, we will consider this a rare situation that is not yet worth modeling.

While our software architecture will permit light sources of these kinds to eventually be represented, they will remain out of scope until well after tectonics.cpp reaches parity with tectonics.js.

So given these assumptions, what do we need to track? 

Well, consider a body that is heated by the light of an arbitrary number of black-body point sources of light. Assuming we know everything about the body itself, such as its terrain, or albedo, we only need to know the luminosity, temperature, and position of each light source to determine everything there is to know about that body's climate. If we simulate climate over a long period of time, we may additionally want to track the amount of time that the light source is in this configuration, such as for averaging and numerical integration. This is in effect the `light::Exposure` object: luminosity, temperature, offset, and duration.

# Calculation

If users visualize a world's climate while simulating at a fast rate, like a million simulated years for every real second, we will make the assumption that they do not care about that world's climate at a particular time of year, time of month, or time of day. We say this because, if they were concerned, the model allows them to slow the rate of simulation to a point where they could observe climate at these moments in time. We will instead assume the user only cares about the average climate when they run at these rates of simulation. This has consequences for what the code does. It absolutely must calculate some average of climate - that much is known. We further know that a single time step in this case covers many orbital configurations with different amounts of solar heating ("insolation"). The math that would allow us to integrate insolation over arbitrary orbital systems is not known to us and likely too complex to implement if it were available, so we are reduced to using some kind of averaging or numerical integration across several sample configurations. However there are still at least two approaches that this allows. In pseudo-code:

## Approach A

```python
for sample in samples:
	sum += insolation(sample)
average_insolation = sum/sample_count
climate = update_climate(average_insolation)
```

## Approach B

```python
for sample in samples:
	climate = f(insolation(sample))
	sum += climate
average_climate = sum/sample_count
```

