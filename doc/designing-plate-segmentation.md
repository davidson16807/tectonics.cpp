Consider a velocity field represented as a raster mapping vertices on a grid to velocities
For each arrow in the grid, a value is calculated representing the contribution of that arrow towards viscous force, reflecting the sum of div(grad(velocity)) and grad(div(velocity))
If the viscous force of an arrow exceeds a threshold, we set the force to zero and claim that a fracture between the cells has occurred 
We can demonstrate that repeatedly applying this viscous force to the velocity field will result in a fracture growing larger, eventually allowing us to find plate boundaries
This is similar, but not identical, to our present method of defining plate boundaries using an image segmentation algorithm
We may be able to formally make this association if we introduce additional assumptions, but we are presently unsure what these assumptions are, or if its even possible to do so
Anyway, we can demonstrate the ability to define plate boundaries using this method, however we are still left with the problem of redefining boundaries once they are already defined
As with our present system, we wind up with a list of plates, each with a set of rasters indicating the crust that belongs to them
There will generally never be a region where no plates are present, since any region large enough to occupy a cell should be treated as newly created crust near a rift zone
However there will be regions where plates overlap, forming subduction zones or mountain ranges
Any technique we design for continuously recalculating plate boundaries should necessarily change where plate boundaries lie every timestep
We know this because we have seen it in past experiments
This is no problem near the rift zones, but if plates are redefined where crust overlaps, we will necessarily have to condense the two overlapping rock layers into one 
This is fine if redefinition only occurs infrequently, but it becomes a problem if redefinition is performed with every timestep 
It will have implications on the thickness of mountain ranges, which would then become highly dependant on grid resolution and other unpredictable circumstances
We could modify the approach to avoid overlapping altogether, representing all crust as if it were one single contiguous fluid represented using a layered spheroid grid. 
However we have heard from others that treating crust as a fluid is not likely to produce results that are similar to the real world
Plates really do appear to behave as rigid bodies, at least to some extent
And overlapping plates are virtually the only practical way to represent folding crust
So we have to make use of rigid bodies but rigid body boundaries are constantly redefined based on stress fields?
But there is a simple solution, or at the very least a simplification: we limit plate boundary redefinition to where plates do not overlap!
This will however require some way to map plates to their corresponding redefinitions. This is fine however. Our previous implementation worked by repeatedly invoking a flood fill algorithm to group similar cells around a seed. This seed was usually the cell with the singlemost largest vector magnitude among the remaining ungrouped cells. This implementation could be modified to use the cells around mountains (which we know will not need redefinition) as the seed for the next iteration. Unfortunately, without any additional modification this would also mean that we may not necessarily have a good representative value for the vector that characterizes the group. 

Another consideration: we've heard that bubble rafts may serve as a good conceptual model for plate tectonics. Under this scheme, grid cells would exist as separate entities that do not belong to any single plate. Plates would still exist, definitely still having their own orientations, and likely still having their boundaries, but within a plate boundary each external cell would be mapped to the nearest unoccupied internal cell within the plate. This has some advantages. We would no longer dwell on which mountaneous grid cell belongs to what plate. They would simply exist, representing conserved mass that has simply been folded in some way. Mass conservation issues could not possibly occur under this scheme.
External cells that fall within a plate boundary will be subject to that plate's motion. Their positions will be updated to reflect the motion of the plate as a whole.

Whatever scheme we choose, it should have a concrete answer for how to deal with the following:
determining plate boundaries continuously with each timestep
determining which cell belongs to which plate
determining plate motion
determining cell motion



buoyancy
fracture

