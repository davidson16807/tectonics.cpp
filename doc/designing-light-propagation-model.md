#First Pass

We need some way to represent how light of different wavelengths propogates through the atmosphere. 
We need to do this in order to model the greenhouse gas effect, among other things. 

Let's describe the greenhouse gas effect: 
Visible light comes in from the sun, 
it gets radiated back out as infrared, 
then gets absorbed by greenhouse gases. 

Greenhouse gases transmit visible light but block infrared, 
so infrared gets absorbed as heat. 

So it appears we need some way to represent the intensity of light at different wavelengths.
To do this, we create two data structures.
One is composed of scalars (let's call it "SpectrumProfile"), 
and the other is composed of rasters (let's call it "SpectrumRaster"),
much in the same way that RockColumn and Crust are scalar and raster representations for components of earths crust. 

We observe we only need to represent a limited range of the spectrum. 
We don't need to represent microwaves or x-rays, for instance.
As we see from sites like [this](https://www.omnicalculator.com/physics/wiens-law),
If the peak wavelength of a celestial body is longer than infrared, it is likely too dim to account for.
For instance, I have never heard of a GCM accounting for the cosmic microwave background radiation, 
yet the CMB only broadcasts on the threshold of infrared and microwave (3K, 1mm).
Likewise, the hottest stars broadcast at a peak that is well within the ultraviolet range (40000K, 72nm). 

Other effects like stellar flares might complicate things. 
For instance, we do not know whether [Proxima b](https://en.wikipedia.org/wiki/Proxima_Centauri_b) 
has an atmosphere, because it would likely be blasted by ionizing radiation during stellar flares.
However we do not yet have a way to model planetary outgassing anyways,
so we should only consider this wavelength once we can fully model its effects.
If we do need to model x-rays in the future, it should be trivial to add a new attribute to these data structures.

It's clear to see we only need to represent the spectrum between ultraviolet and infrared.
However it's less clear what the resolution should be. 
It's at least clear that we probably do not need to represent individual color channels, 
like red, green, and blue, because I am not aware of any thermodynamic phenomenon 
that concerns itself with the selective propagation of red light vs blue light. 
It is true these color channels will become extremely important when we want to render a world using a graphics shader,
but this is a completely different implementation that's written in a completely different language.

However we do at least need to capture the distinction between visible and infrared light,
since that is required by our motivating example.
We will suffice for now to represent ultraviolet, visible, and infrared light as separate channels. 

So that leaves our implementation as follows:

  SpectrumProfile: 
      double ultraviolet
      double visible
      double infrared
  
  SpectrumRaster:   
      raster ultraviolet
      raster visible
      raster infrared

Now, when we take another look at our greenhouse gas example,
the second thing we notice is that light propagates in different ways for different materials.
It is transmitted by the atmosphere, 
reflected or reemitted by the surface, 
and absorbed by the greenhouse gas.
We must represent the quantities of light that undergo these processes, 
and ideally we should do it for all of the materials that are worth accounting for.

So what quantities should we represent? I can think of the following:
      reception         the amount that interacts with a material
      reflection        the amount that is reflected immediately
      absorption        the amount that is absorbed as heat
      emission          the amount that is emitted when the material gets hot
      transmission      the amount that passes through the 

And what materials should we represent? For a first pass, I suggest we limit ourself to the air and the surface.
Later on, we can represent other materials like clouds, once we have enough code to represent those materials.

In order to represent the greenhouse gas effect, we need to represent both incoming and outgoing light. 
Incoming light interacts with the air and the surface.
Outgoing light only interacts with the air.
That leaves us with the following quantities:

    intensity_of_incoming_light_received_by_air
    intensity_of_incoming_light_reflected_by_air
    intensity_of_incoming_light_absorbed_by_air
    //intensity_of_incoming_light_emitted_by_air
    intensity_of_incoming_light_transmitted_by_air

    intensity_of_incoming_light_received_by_surface
    intensity_of_incoming_light_reflected_by_surface
    intensity_of_incoming_light_absorbed_by_surface
    intensity_of_incoming_light_emitted_by_surface
    //intensity_of_incoming_light_transmitted_by_surface

    intensity_of_outgoing_light_received_by_surface
    intensity_of_outgoing_light_reflected_by_surface
    intensity_of_outgoing_light_absorbed_by_surface
    intensity_of_outgoing_light_emitted_by_surface
    //intensity_of_outgoing_light_transmitted_by_surface

    intensity_of_outgoing_light_received_by_air
    intensity_of_outgoing_light_reflected_by_air
    intensity_of_outgoing_light_absorbed_by_air
    //intensity_of_outgoing_light_emitted_by_air
    intensity_of_outgoing_light_transmitted_by_air
 
We comment out the rasters that are likely not relevant in the general case
(e.g. we will never need to model a planet that is completely transparent,
so we do not have to track intensity_of_incoming_light_transmitted_by_surface)

If we only want to represent the greenhouse gas effect on earth, we only need the following Float32Rasters:

    intensity_of_incoming_visible_light_received_by_surface 
    intensity_of_incoming_visible_light_absorbed_by_surface
    intensity_of_incoming_visible_light_emitted_by_surface
    intensity_of_outgoing_infrared_light_transmitted_by_air

And "intensity_of_incoming_visible_light_received_by_surface" is equivalent to the output from Universe.average_insolation()
This representation assumes that surface air temperature will equilibrate to whatever is the temperature of the surface proper 
This is all we need for our first pass. 

If we model climate using a steady state assumption, 
such as if our timestep is very large,
then we assume intensity_of_incoming_visible_light_absorbed_by_surface equals
intensity_of_outgoing_infrared_light_transmitted_by_air. 

The order of calculation is as follows:

    intensity_of_incoming_visible_light_received_by_surface 
    intensity_of_incoming_visible_light_absorbed_by_surface
    intensity_of_incoming_visible_light_emitted_by_surface

However, if we model climate using numerical integration,
the order is as follows:

    intensity_of_incoming_visible_light_received_by_surface 
    intensity_of_incoming_visible_light_absorbed_by_surface
    intensity_of_incoming_visible_light_emitted_by_surface
    intensity_of_outgoing_infrared_light_transmitted_by_air

This has implications for our architecture.
See more information about this in model-architecture.md

Now let's discuss our second pass.
In our second pass, we want to model 
Let's introduce an additional datastructure,
to represent the way light from a certain source propogates within a material:

  LightInteractionRaster:
      SpectralRaster received
      SpectralRaster reflected
      SpectralRaster absorbed
      SpectralRaster emitted
      SpectralRaster transmitted

If needed in the future, we can add an additional "LightInteractionProfile"
that operates analogously to "SpectrumProfile" or "RockColumn".
But this won't be needed for our discussion here.

So all told, we need the following LightInteractionRasters:
      
      incoming_light_interaction_with_air
      incoming_light_interaction_with_surface
      outgoing_light_interaction_with_surface
      outgoing_light_interaction_with_air

And if we account for clouds, we will expand this list to the following:

      incoming_light_interaction_with_air
      incoming_light_interaction_with_clouds
      incoming_light_interaction_with_surface
      outgoing_light_interaction_with_clouds
      outgoing_light_interaction_with_air

However this will not occur within our first pass, nor second.
For our first pass, we will limit ourselves to 4 LightInteractionRasters, 
each with 5 SpectralRasters containing 3 Float32Rasters. 
60 rasters total.
At 10k cells per raster, that's ~2MB, so our implementation should not be constrained by memory.
I suspect it should not be constrained by runtime, either, 
since these rasters are strictly computed using in-order memory access
(see "performance-considerations.md" for an explanation of this concept). 




#Second Pass

We need to convey the amount of energy absorbed from:

  incoming light
  outgoing light reflected by the surface
  outgoing light emitted by the surface

We will discount the amount that is absorbed due to scattered light, since scattering is assumed to be a secondary effect. This goes for both incoming scattered light and outgoing scattered light. 

Each of the three energy sources above can be represented as a straight path taken by light from a source to a sink, in which the sink is our parcel of air. We can reuse our column density calculations from our ray marching shader to determine the fraction of light that reaches the sink. A certain fraction of that is then absorbed as heat, as determined by the number density of the parcel and its absorption cross section. 

So then we just have to determine our sources. For incoming light, this is fairly straight forward: the source is the sun. For outgoing light emitted by the surface, we assume that emission always occurs in a direction that is normal to a surface microfacet. Therefore, emission is best approximated by diffuse/lambertian surface lighting, and is of greatest intensity in the direction of the macroscopic surface normal. We therefore approximate by saying that the source of light emitted by the surface occurs directly below the parcel of air being considered. That just leaves the source of light reflected by the surface...

Reflected light can in principle emerge from any point along the surface, just as with emitted light, however it would be too computationally expensive to calculate the amount of light absorbed from every point along the surface. And unlike emitted light, we cannot simply state that the majority of reflected light will be received from directly underneath the parcel of air. Surface reflection is greatest where the surface normal (N) is the halfway vector (H) between view (V) and light (L). We could in principle use this to find the surface normal, then treat this surface normal as a position on a unit sphere at which reflection occurs. We could then lookup surface reflectivity at that point, and the column density from it. This would be all we need to find the amount of reflected light received. However, many surface normals will exist that fulfill this criteria, so to lookup column density and surface reflectivity for all these surface normals will still be too expensive. 

If we assume the opacity of air is constant (i.e. no clouds or dust storms) then we need only sample from one of these points to find an approximation for column density. As for surface reflectivity, we note that if N⋅V<1, we are effectively asking for the average surface reflectivity across a small circle of the globe. Since this is probably a wide enough coverage of the globe, we can assume the appropriate value for surface reflection is close enough to some global average of surface reflectivity, perhaps weighted by average insolation. However, if N⋅V=1, we are only interested in the reflectivity of the surface that's immediately below, which is trivial to lookup. Therefore, I propose we should assume that surface reflectivity is an interpolation between these two values. 

We now have gathered a list of all the paths that energy can take and have identified their sources. But we must also consider outgoing light emitted by the air, and this cannot be represented as a path. Contrary to some misleading diagrams that visualize greenhouse gas as an invisible barrier reflecting infrared light, greenhouse gas actually *absorbs* infrared light, and then reemits light equally in all directions. Since it reemits light equally in all directions, approximately half the light goes up, and half the light goes back down. The actual fraction will be less than half, but this is only significant if the planet is small or the gas is very high up. Since the scale height of most planets   If a planet does have an atmosphere that is worth considering, then the boltzmann distribution tells us it will either be very large, or very cold, and in either case it will concentrate exponentially towards the surface, so we find this to be a very suitable approximation, in general. So we simply calculate the amount of energy given off by a black body when it is the same temperature as the parcel of air, then figure that half that amount is actually lost to space. 

