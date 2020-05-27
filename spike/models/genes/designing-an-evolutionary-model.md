# Designing an Evolutionary Model

## Overview
Most computer simulations for biological evolution work by physically simulating an organism trying to accomplish some objective in an environment. Some measure of an organism is observed and assumed to be an organism's fitness, around which the rest of the algorithm is executed. 

This is ideal in concept because it simulates exactly what is happening in the real world, so it allows greater flexibility in body shapes and solutions, and can produce some interesting, open-minded, and insightful possibilities.

However, there are also few problems with this approach, in practice. It can be difficult to precisely specify the kind of fitness function that applies to real world organisms. More often than not the fitness functions are overly simplistic. Solutions tend to "hack" the system, creating weird body shapes that wouldn't be practical in the real world, even though they do technically optimize the fitness function. 

We can't go that route anyways, since it takes ridiculous amounts of time to run a physics simulation on a population or ecosystem. We could reduce the amount of time if we simulate an organism, or an interaction between different species, then extract some sort of representative quantity from the interaction to be used in some more abstract model, but even that will very likely take too much time. We need something that can simulate entire ecosystems *fast*, incredibly fast, at simulation speeds comparable to 1 million years per second. Even at 60 fps, we're talking timesteps of 16,000 years. That's 500 human generations. 

So we need a more abstract model, something that assumes particular rules about body shapes, reducing body shapes to a series of measurements that apply to all animals with a certain body type, and then states relationships between those measurements and fitness quantities. For instance, if we were trying to simulate flying animals, we could simply state that all flying animals have a wing area, a ratio between wing width and length, and so on. We could then assume that any life will evolve to fill all available niches over enough time, and establish a fitness function that's reflective of that niche. 

For instance, there may be a particular "raptor" niche that requires fast flight speed. If the model for flying animals were simple enough, we could then solve analytically for the maximum, but more likely than not it would be complex enough to justify using optimization algorithms. This is fine, since by this point the model is fast enough and abstract enough to do this at the timesteps we need. 

This "niche-centric" model offers us another advantage: it allows us a way to save on memory when storing spatial distributions for genotypes. We see from the [competitive exclusion principle](https://en.wikipedia.org/wiki/Competitive_exclusion_principle) that no two species can share the same niche in equilibrium for a particular region of space. We already rely heavily on assumptions of equilibrium throughout the rest of the application, so we loose nothing by making use of the principle. The principle makes it clear that we would only need a single raster to store the spatial distribution of genotypes for a particular niche. Each cell within one of these "niche rasters" would store the genotype for only one species. Species may transfer to unoccupied grid cells within niche rasters if they meet some basic criteria to qualify for that niche. If there is an established competitor already occupying that grid cell, their fitness function for that particular niche must also exceed that of the established competitor.

We will next establish a memory budget for this model. We will note some constraints that follow from it, and also note some other considerations regarding the amount of development time needed.

## Allocating a Memory Budget
As with our [design for a statigraphic model](designing-stratigraphic-model.md), we will say 100MB is the order of magnitude for the utmost maximum we can allocate towards the evolutionary model (dex 8 bytes). 

The amount of memory consumed by the evolutionary model is a strict product of:
* the number of ecological niches
* the grid cells per niche raster
* the size of the genotype in memory

For most models, the number of grid cells within a raster can be anywhere between dex 3 and dex 4 and provide satisfactory results. This provides us with anywhere between dex 4 and dex 5 bytes to allocate between the number of niches and the size of the genotype. 

The number of niches is itself linear to the amount of effort we put in, since for each niche we need to derive a custom fitness function, and that could require extensive research and hairy debugging. Compare this to the amount of effort needed to expand the size of the genotype, which could amount to adding a new floating point number to a data structure. This means we have a good reason to prefer limiting the number of niches, rather than limiting the size of the genotype. A reasonable number of niches is anywhere between dex 1.0 and dex 1.5 considering the amount of effort involved. This means we should target anywhere between dex 2.5 bytes and dex 4.0 bytes to store a genotype. Even the lower bound sounds easy to work with, so based on this, we choose to continue with our design process.

We will next discuss what the model of a genotype might look like in memory. After that, we'll outline the ecological niches that we might consider and their fitness functions.

## Designing a Model for Genotypes
There are a few lines of inquiry that we can use to inform the design of our model:
* **teleology**: observe the functions that body plans perform, then find a model that expresses those functions
* **comparative morphology**: observe the features of body plans in the real world, then find a model that expresses all those features
* **evolutionary biology**: observe the operations through which body plans evolve, then find a model that would restrict itself to similar operations
* **developmental biology**: observe the operations through which body plans emerge in an embryo, then find a model that would restrict itself to similar operations
* **genetics**: observe how body plans are expressed in real life using DNA, then find a model that distills it

I've ordered these lines of inquiry based on what I think will give better results. Obviously, if our model perfectly reproduced genetic code and its expression, it would be able to express any body plan that we see in the real world with perfect fidelity. However, this approach is limited by our lack of knowledge, not to mention performance and memory considerations. Nevertheless, it's probably best to start with the most promising approach first and see how far it takes us. 

There are several concepts within genetics that I think are important to the problem, however some of these concepts cannot be understood without introducing other concepts within biology, so even though the genetics discussion is our goal we will start with other branches of biology. The very first concept we will discuss is something called **segmentation**, from evolutionary biology, and **reaction/diffusion patterns**, from developmental biology.

### Segmentation
Most animals we're familiar with are [segmented](https://en.wikipedia.org/wiki/Segmentation_(biology\)). This concept is best demonstrated in an earth worm or a millipede: there are compartmentalized regions of the body that repeat again and again with minor variations to express for distinctions like whether to grow legs vs. antenna. For some animals, this is less obvious, but we still see the pattern. In vertebrates we see this sort of repetition internally, in the vertebral column and rib cage. In insects and spiders this is harder to see since they are degenerate cases in the mathematical sense: insects have only 3 major body segments, spiders have only 2. There are of course many minor body segments in insects and spiders, but they are fused together. For instance, insect heads are typically composed of several fused segments that each individually express for different appendages like mandibles vs. antenna. We see a similar thing going on in the heads of vertebrates, as well. Somewhat recursively, the appendages of segments may themselves be segmented. For instance, the tibia and fibula would be considered segments of appendages in vertebrates. 

### Reaction Diffusion Patterns
Now that that we've discussed body segments, I'd next like to discuss a general mechanism through which segmentation could develop.

Before we go on, I highly recommend you go out and read Turing (1952), who discusses the subject. It is a seminal paper written by a well reknown scientist, and if that weren't enough he makes it a point to provide gentle introductions to foundational concepts across a wide range of subjects including physics, chemistry, biology, and mathematics. There is even a brief cameo mention of computer science, hinting at things then to come for that discipline. Some parts may be inaccessible to you, but Turing anticipates this and you can safely ignore parts you're not interested in. Overall, it's a great paper for anyone who likes shoring up their cross discipline knowledge. 

One important insight I took from Turing is that an organism can only assume certain body plans, and the body plans that are available to the organism depend highly on scale. This applies very liberally to any life as we know it, and as we will see it very likely applies to many forms of life as we do *not* know it. 

If an animal is small enough, then any chemical diffusion that occurs will likely outpace growth, so the chemical composition of its body is roughly uniform, and the same reactions occur evenly throughout the body. As Turning mentioned, the body can be simulated using only chemical and physical terms, and in this case chemical terms are less significant spatially, so we're left to consider only the physical. Since cohesive forces are minimized on a sphere, such a body will assume that shape if it's maleable enough to do so. Once the body assumes a spherical shape, all the conditions needed for Turning's example then apply.

>It is well known... that any function on the surface of the sphere, or at least any that is likely to arise in a physical problem, can be 'expanded in spherical surface harmonics'. 

and

>The essential property of a spherical harmonic of degree n is that when the operator ∇² [expressing diffusion] is applied to it, the effect is the same as multiplication by [-n(n-1)/r²]

where r is the radius of the sphere. Pay close attention to that r² when you run through his equations. When r is big, the diffusion terms drop off. The degree 1 spherical harmonics are the first to come into play as the organism increases in size. The first assymetry that emerges will define the axis for the Y₁⁰ spherical harmonic. In an evolutionary sense, Y₁⁰ is a tool within the organism's chemical toolset that is made available once it reaches a certain size. It only takes a simple network of two interacting growth chemical agents (known as "morphogens") for the tool to be used, so we could easily see it arising at first by chance. If that tool proves useful, then evolutionary pressure will steer the organism towards its usage. Certain conditions may lower the size threshold needed to make use of Y₁⁰, namely by lowering the rate of diffusion. This could be accomplished by setting up barriers between the fluid that carries out chemical reaction, such as through cell walls in a multicellular organism. However we know that multicellularity is not essential for spherical harmonics to come into play. We see from the math that reducing diffusion only hastens emergence of spherical harmonics, and any organism that is large enough could make use of Y₁⁰. We also see many real world examples of single celled organisms forming patterns described by spherical harmonics, such as spermatozoa or radiolarians. 

To summarize: assymetries arising by chance cause an axis in the organism to form. Growth agents become distributed along the axis according to Y₁⁰. Growth agents cause one end to grow slower than the rest. The slower growing end becomes enveloped by the faster growing end. Gastrulation commences. The same growth agents that cause gastrulation may also serve as signaling agents for germ layer specialization. The end that grows slower specializes as the endoderm, and the end that grows faster specializes as the ectoderm. The organism is now a diploblast. If a another morphogen is produced from a reaction between morphogens of the endoderm and the ectoderm, then a third germ layer can form: the mesoderm. The organism is now a triploblast. 

At this point, the organism assumes a ring shape, in which case Turing's other examples are more applicable. Turing lists the six patterns that may emerge on a ring shape due to chemical reactions. We assume the reader is familiar with them, so we will refer them as dappled, oscillatory, drifting, standing wave, travelling wave, and oscillating wave. The only two that he knows to occur in the real world are the dappled patterns and standing wave patterns. Dappled patterns play a role in forming things like the color of leopard skin, or the placement of tentacles on hydra. Standing wave patterns play a role in producing an integer number of repeating patterns along an axis, making them a likely mechanism for segmentation. For this reason we will give special treatment to the standing wave pattern.

A single organism could have multiple networks of chemical reactions performed between morphogens. If the morphogens of one network do not react with the morphogens of another network, one network can express a pattern without interfering with the pattern formed by another. This establishes a sense of orthogonality, or noninterference, between patterns. An obvious example of this noninterference can be seen in leopards: the dappled patterns on their skin presumably did not evolve while interfering with the segmentation pattern along their skeletons. Any combination of orthogonal patterns could exist within an organism this way, each expressing their own pattern along an axis of the body. Furthermore, any number of patterns could share the same axis this way. For instance, there could be one standing wave pattern that forms body segments along the head-tail axis, and another standing wave pattern along the same axis that repeats less frequently along the length of the body, controlling the width of body segments so that they taper at the head and tail. 

Run through the reasoning above and you'll notice we only make a few liberal assumptions about the organism:
* the organism starts life small and grows in size
* the organism is malleable at the start of life
* the organism is submitted to cohesive forces
* the organism is submitted to reactions between diffusive chemicals
* the organism is submitted to evolutionary pressure and makes use of simple chemical controls when it is possible to do so

Just because an axis could exist does not necessarily mean it will exist for all organisms where these assumptions hold. We will next outline the axes we are familiar with in the real world and which necessarily hold given the above assumptions.

One could picture a simple creature that uses a coordinate basis to express patterns within its body shape. This is a "topographical" coordinate system, to borrow a term from anatomy:
* x, axial
* r, radial
* θ, tangential

Each axis within the basis may have several independant networks expressing noninterfering standing wave patterns. We will describe these patterns using sets of numbers indicating the number of segments that form a standing wave pattern along a given axis. For instance, nx={2,3} will indicate two patterns along the logitudinal axis, with 2 and 3 segments within each of them. This model alone is sufficient to describe a wide variety of life on Earth. Take for instance the following possible assignments:

`     
             nx    nr  nθ
starfish    {1}   {1} {1,5}
volvox      {}    {}  {}
roundworms  {1}   {}  {}
flatworms   {1}   {1} {}
earth worms {1,a} {}  {}
spiders     {1,b} {c} {1}
mammals     {1,d} {e} {1}
corn        {1}   {1} {3}
`

where a,b,c,d and e are greater than 1. 

How many of these kinds of axes can life choose from, given our previously stated assumptions? It seems intuitive that the radial and tangential axes would emerge as obvious choices for expressing body shape, but since I can't see a way to demonstrate this from the current assumptions, I will simply have to add it as a new assumption whenever considering sufficiently complex organisms.

### Homeobox genes
By this point, we have discussed what body segments are, and we have discussed what chemical tools may be used by the body to help express them. This brings us up to speed with what was known by the early 1950s. We will next try to discuss how segments are represented within the genetic code. 

Modern genetics fortunately gives us the answers, and rest assured they will be revealed in time, but let's first try to see if we can figure it out on our own.

We might speculate from past discussion that there is something within the genetic code that specifies the number of segments within a fixed set of patterns occuring along the three previously mentioned axes of the body, something like the previously mentioned nx, nr, and nθ. 

We might also suspect the segmentation patterns from our previous discussion could map to multiple potentially orthogonal phenotypic traits, each being determined by their own set of genes. For instance, Where you are along a segment dictates whether a leg will form, but there are many traits within a leg that differ between species. Some species share some aspects of their legs while differing in others, and other species vice versa. There may be a whole coordinate system of phenotypic traits that are only made visible by the presence or absence of the chemical agent that causes the segmentation pattern. Morphogens may only serve to activate the [expression](https://en.wikipedia.org/wiki/Gene_expression) of a second layer of genes, which then goes on to determine phenotype. 

In this sense, the concentration of morphogens may serve as a separate coordinate system, distinct from the phenotype coordinates mentioned above, and the topographical coordinates mentioned before that. This morphogen coordinate system uniquely identifies which part of the body that a cell is located within, and indicates to the cell how it is to specialize. So there are a set of functions that map from one coordinate system to the other, each with its own set of genes:

topographic space -> morphogen space -> phenotype space

But what do these functions look like, and how do genes represent their behavior? In the absence of evidence it might be natural to model this with a linear map, since matrix multiplication is fast and easy on computers, but we already know that can't be done for the topographic->morphogen map (the standing wave patterns would require the use of sinusoids). And as we will see, it's not so simple for the morphogen->phenotype map, either. 

In the real world, biologists since the 1950s have discovered what are known as [homeobox genes](https://en.wikipedia.org/wiki/Homeobox). Homeobox genes perform part of the functionality within the morphogen->phenotype map we described earlier, at least for the latitudinal axis. Homeobox genes encode for what are known as **homeodomain proteins** (think "domain" in the sense of a mathematical function's domain). Homeodomain proteins are **transcription factors**: they speed up or slow down the production rate of proteins that are encoded by other genes. They act as master control switches for gene regulation, and their production is itself regulated based on what part of a body a cell grows in. 

Homeobox genes have a curious property: the order in which they occur within the genetic code mirrors the way they are expressed along the latitudinal axis. What's even more interesting: if you were to somehow disable a single homeobox gene, the segment would still occur within the body of an animal, but its appearance would mirror that of the segment that came before it (that is, it would resemble the segment that's closer to the head). 

These are important observations! They tell us that the morphogen->phenotype map behaves nothing at all like the linear map we initally presumed. It's more like a switch statement: 

`
if 
switch(segment_id)
{
  case 0: instructions for head
  case 1: instructions for thorax
  case 2: instructions for abdomen
  ...
}
`

The hox genes are not the only genes we know to regulate specialization. The [ParaHox](https://en.wikipedia.org/wiki/ParaHox) genes are thought to be a paralogue to the Hox genes that perform a similar function in the gut (endoderm). 

BTW, Scientists have also found genes that perform functions similar to the topographic->morphogen mapping we mentioned. [Maternal Effect Genes](https://en.wikipedia.org/wiki/Drosophila_embryogenesis#Maternal_effect_genes) create a pattern of expression within the body that's strikingly similar to the nx={1} pattern axis we mentioned. [Pair-Rule](https://en.wikipedia.org/wiki/Pair-rule_gene) and [Segment Polarity](https://en.wikipedia.org/wiki/Segment_polarity_gene) genes create expression patterns very similar to standing waves, with segment polarity genes creating waves at double the frequency of pair-rule genes. We also see additional genes in regulatory networks that provide fine tuning, which you can read about [here](https://en.wikipedia.org/wiki/Drosophila_embryogenesis#Additional_segmentation_genes). It's a fascinating read, but goes well beyond the specificity we want for our genotype model.

### Synthesis
From what we've learned we can establish the following requirements for our genotype model:
* There must be some way to specify the presence of symmetries and segmentation patterns. This should resemble our nx/nr/nθ model, at least to the extent allowed by other requirements. We shall call these state variables "segmentation genes" after the real life genes of the same name.
* When a segmentation pattern exists along an axis that is known to harbor homeotic genes, there must be some data structure to express phenotypic parameters of segments on a per-segment basis. One instance of this data structure will be stored per segment, up to a predetermined maximum segment count. We shall call the state variables within these data structures "realizator genes" after the real life genes of the same name. Note that we do not call them "homeotic genes" since we hard code the behavior of homeotic genes using conditional logic that switches between realizator data structures. Also note the predetermined maximum on segment count places restrictions on what can be specified within selector genes. 
* In the interest of conserving memory, we have the option to omit a realizator data structure along axes where homeotic genes are not known to occur. In this case, realizator genes must be specified using linear maps betwen morphogen space and phenotype space. 
* Realizator data structures must store some kind of flag to indicate whether the homeotic gene for that segment is disabled. If disabled, the segment reads the phenotype of the last enabled data structure that was read, where data structures are read starting with the head segment and finishing with the tail. 

### Implications for Memory Budget
As mentioned before, we should target anywhere between dex 2.5 bytes and dex 4.0 bytes to store the genotype for a species. 

We anticipate that instances of the realizator data structure for a body segment will consume the most memory, since we expect it to be a large data structure that is repeated frequently within a genotype. The number of times this data structure is repeated depends on the maximum segment count. Taking fruit flies as an example (as most geneticists do), we see up to 14 bands where expression for the Segment Polarity gene alternates, however this could be interpreted as a fine tuning parameter for segments from the Pair-Rule gene, which only has 7 bands. We'll say the number of segments is around dex 1. This leaves anywhere from dex 1.5 to dex 3.0 bytes to store a single body segment. 

The body segment data structure may itself be composed of several appendage segment data structures. We see in spiders as many as 7 appendage segments. We'll say the number of appendage segments is around dex 1. This leaves anywhere from dex 0.5 to dex 2.0 bytes for a single appendage segment. Please keep in mind though that we probably want to avoid using dex 2.0 bytes since it will start to cut into quality in other areas, such as niche count or spatial resolution. We should probably aim for dex 1.0, or 10 bytes. 

There is also the matter of segment specialization along the tangential axis, however we do not see this often in nature and we are starting to become memory limited. If we wanted to, we could introduce another dex 1.0 factor to our memory model. This assumes we could store our smallest data structure within at most 10 bytes, However doing so would very likely require us to work at the lower end of spatial resolution. We may opt instead to either omit tangential segment specialization, allowing tangential coordinates only to have a linear scaling effect on other attributes, or allow the creature to toggle whether to use the axial segment data structure to instead represent tangential segment specialization. 

Let's accept for now that an appendage segment consumes 10 bytes. We said an organism may take up to 1KB of memory in its most compact form. We expect this will require storing attributes using the smallest data types possible. The smallest data type we can use depends on our precision requirements. We don't necessarily need to standardize on a data type, however it is easiest to pick a representative type to estimate the number of attributes we can use. Whatever representative data type we use should probably consume a number of bits that is a multiple of 2, since that allows us to easily reason with memory alignment and caching concerns. That leaves us with a choice between uint2 (4 unique values), uint4 (16 unique values), and uint8 (256 unique values). We expect many attributes will have a spatial nature, and will measure the size of one part relative to the size of another. For instance, the thickness of a muscle may be expressed as a fraction of an appendage's width. We should make sure that whatever representative data type we choose should allow for more than one sensible value to be expressed in this way. It doesn't seem like we could achieve this requirement with 4 unique values, however 16 seems like it may be just enough to get by with. 

This means a 10 byte appendage segment might have 20 attributes, and a 1KB organism could store around 2000 attributes. 

## Designing a Category Diagram of Classes
In order to simulate evolution, we need data structures that will easily allow us to define two basic operations:
* `evaluate(...)` an operation that accepts a data structure that represents an organism and returns a floating point number that indicates its fitness within a particular ecological niche, and...
* `nudge(...)` an operation that accepts a data structure that represents an organism and transforms one of its attributes for use in a generic optimization algorithm.

The details of each function are not important. The fitness function used within `evaluate(...)` has still to be defined. The optimization algorithm that uses `nudge(...)` could be a genetic algorithm, being as that algorithm tries to mimic the very thing we are trying to model, but depending on performance and ease of implementation we could also choose an algorithm such as gradient decent. What's most important to note is this:

* `evaluate(...)` requires a data structure that can be easily navigated by a human in order to specify a fitness function. Attributes should be organized neatly into nested data structures and arrays, and it should be possible to reference attributes by name. 
`nudge(...)` requires a data structure that can be easily traversed and manipulated. It should allow for use by a generic optimization algorithm, one that does not know the specifics of the data it is optimizing. For this reason, we are better suited using a flat data structure of some numeric data type that is of sufficient precision to describe all attributes within an organism. 

Also please remember that we still need a data structure that represents an organism within 1KB of memory.

We do not prefer to specify a single data structure that meets all the above requirements, nor should we. It is generally better as a matter of principle to separate concerns where we see them, and even if we disregarded this principle, there are likely too many concerns here to do so while still being able to reason with code. We shall next explore what it would look like to work with multiple data structures.

Let's start by assuming a single data structure for each concern:
* `dense` for storing genotypes within memory
* `traversible` for use within `nudge(...)`
* `organized` for use by humans when writing `evaluate(...)`

At the start of an iteration, we access an instance of `dense` from a larger data structure that represents a spatial distribution of species for each ecological niche. `dense` may be nothing more than a large array of uint4s. 

Since uint4s are prone to underflow/overflow errors, they don't make good candidates to express a parameter space that can be traversed within `nudge(...)`. This is where `traversible` steps in. 

`traversible` is a data structure, likely an array, whose component data type is large enough to obviate underflow/overflow errors from occuring within a single iteration. As a result, it represents a wider range of values compared to `dense`. An instance of `traversible` may be operated on several times over the course of an iteration in order to determine which direction to travel within parameter space. Once that direction is found, an updated `traversible` is converted back to a `dense`, clamping values if they should fall outside the range that's represented by `dense`. Please keep in mind that converting back to `dense` would result in a loss of precision if `traversible` had a higher precision than `dense`. Since `dense` is an extremely compressed data structure, this could easily occur. Unlike underflow/overflow errors, it is not possible to address loss of precision by making an adjustment to `traversible`. It is probably best instead to consider precision limitations as we perform operations on `traversible`. We could for instance define the step size of our optimization algorithm to match the precision limitations of `dense`. As a result, we decide that `traversible` will still represent the same numbers as `dense`, but will allow for a wider range of values. Defining `traversible` as an array of int8s is sufficient to meet these requirements. Since the memory footprint for the `traversible` can be reused for each organism, the increased memory footprint of a `traversible` shouldn't come with any noticeable implications.

When an operation is performed on `traversible`, we must figure out the fitness of the result. It is easiest to define a fitness function if `traversible` is expressed in a way that makes sense to the developer. This is where `organized` steps in. `organized` has no restrictions on precision or layout, and we have gone over how it might model organisms in great detail during previous sections, so we will discuss it no further.

We will summarize our findings with a very trivial category diagram:

`dense <=> traversible -> organized`

We will next note a few complications, which we will then use to refine the implementation.

It is important to note that uint4s are most conveniently implemented in C++ using [bitfields](https://en.cppreference.com/w/cpp/language/bit_field). C++ places few restrictions on how compilers represent bitfields, and they may optimize performance in different ways on different hardware. A bitfield's representation within memory cannot be relied on when creating save files. So there must be another `canonical` data structure that represents an attribute on the hard drive.

`canonical <=> dense <=> traversible -> organized`

`organized` is also not as easy to use as it could be. This is because it shares two concerns: it strives for ease of use when writing a fitness function, yet it must also express parameters in such a way that there is no illegal state space (Stepanov calls this condition "properly partial": a proper subset of values within an abstract entity). This latter requirement introduces a lot of weird stipulations within the definitions of attributes. For instance, there are lots of "log_*" attributes where a real world value must not assume a negative value, and some body proportions are expressed in terms of other body measurements instead of directly stating the measurement in meters. To separate these concerns we introduce an additional `understandable` data structure, which expresses all attributes directly in linear units of measure by relaxing guarantees of proper partiality.

`canonical <=> dense <=> traversible -> organized -> understandable` 

Since `understandable` has the potential to store invalid values, we wish to prevent its modification, and since we can't modify `understandable`, there is no reason to add logic that converts back to `organized`. 

At this point the category digram describes an acceptable implementation. `understandable` may be further refined depending on whether we find any coherent stages in transformation along the way, but this is a secondary issue. In the future, we may also consider taking advantage of the existing code and exposing an interface to the user to allow them to play around with organisms of their own making. This could be a game in-and-of-itself, as we see with spore, and the properties of the genotype model we described previously could provide for some unique and interesting challenges that have real world implications. Setting up this interface takes us well past minimum viable product, however if we wanted to do this we would only need one more morphism in the diagram, converting `organized` back to `traversible`:

`canonical <=> dense <=> traversible <=> organized -> understandable`

Precision concerns need not apply when converting back to `traversible`. We could offer them a sliding scale for each attribute within `organized`, and precision issues would be indistinguishable from a user interface where sliders had a particular step size. 

## Designing Genotype Attributes

This section relates to how we will design attributes within the genotype model to allow for the most efficient representation within memory while still allowing evolution to occur at a realistic rate. 

This section originated as a code comment that grew too large, so it may read a little differently than other sections. It will serve as a case study, where we attempt to address a concrete example of the issue. I expect there should be a lot of commonality between issues of this kind, so it is hoped the solution here may be adapted to other genotype attributes.

Our case study addresses this issue: 

*what is the most efficient way to represent an animal's body length within memory?*

We start with some observations.

[Pelagibacter ubique](https://en.wikipedia.org/wiki/Pelagibacter_ubique) 
is among the smallest known free-living bacteria, on the order of dex -6.5 m at smallest.
The blue whale is on the order of dex 1.5 m long.
The tallest known tree is [Hyperion](https://en.wikipedia.org/wiki/Hyperion_(tree)),
a redwood on the order of dex 2 m tall.
If we want to represent realistic speculative creatures that could occur in alien circumstances,
we could consider Wayne Douglas Barlowe's Emperor Sea Strider, which is dex 2.5 m tall. 
We will discount larger organisms such as [Pando](https://en.wikipedia.org/wiki/Pando_(tree))
since we're unable to represent their body shape in our model.
This means there are around 9 orders of magnitude to account for. 

Changes in body size would be very noticeable if we were to implement some visualization for it over time, 
so we want to pay special attention to its precision.

We should like to establish an utmost lowest precision needed to represent a change over a timestep.
Not every speed setting in our simulation needs to represent evolution, this we already know,
but there must be at least some valid timestep at which evolution occurs.
Our maximum timestep is 1My, and the highest frame rate our model is allowed to cap to is 60fps, 
so a timestep is at most 16,000 years. 

We should like the minimum precision of body size to allow representing changes that occur over that time.
Matilla (2008) shows in an analysis that body mass could evolve quickly 
if selective pressure is provided by the environment:
body mass may scale up as much as 26% per million years assuming only gradual evolution,
but brief speciation events may account for all but 5% of that change. 
We will note that body mass scales as the cube of body length,
letting us approximate an 8%/My change in length, of which 1.6%/My is gradual. 
Because so little change is accounted for by gradual evolution,
and since we only seek a estimate for minimum precision needed,
we will discount the precision requirements that are needed to represent gradual evolution,
and assume all change comes from speciation.
That means speciation events should account for an average 8%/My change in length.

But how fast does change occur per timestep during a speciation event? 
In order to continue we need to estimate speciation frequency and duration.
Rosenblum (2012) estimates speciation varies from 0.01 to 10 events per lineage per million years
with a "canonical" estimate of 0.3/lineage/My provided by Sepkoski (1998).
Etienne (2014) estimates an average 0.6 My to complete a successful speciation event,
and states that number is roughly invariant to speciation initiation rate and speciation failure rate. 
This is a refinement on Curnoe's estimate (2006) for primates, around 1 My. 
If we accept the canonical speciation event, 
then every 3 million years there will be a speciation event lasting 0.6 million years
that accounts for a factor of `1.08^3 = 1.26` change in body length. 
That speciation event takes 40 frames at the fastest speed setting and frame rate.
If we store body length on a log scale, 
it needs to account for changes at a factor of `1.26 ^ (1/40) = 1.006`, at *minimum*.
This means each order of magnitude in size needs `1/log10(1.006) = 384` unique values.
For the 9 orders of magnitude we need to represent, we must store at minimum `9*384 = 3456` unique values.

Side note: I admit I'm grasping at straws here: synthesizing results from so many papers like this
is not something I'd have the bravery to do in academia,
but I'm trying to put together a representative model here that works:
I just need numbers to feed the model, and this is what it took to get them.

3456 unique values is too much for a typical uint4, 
but it can be accomplished by using the storage space of several uint4s.
We must now pick a generic way to account for variables that span multiple uint4 slots
within the array that's used by our optimization algorithm. 
In other words, we must find a way to support variable precision. 
The optimization algorithm works by incrementing or decrementing values within an array of integers.
This is a design that works satisfatorily in the general case,
and we do not wish to modify it in any way to acount for variable precision.

One trivial way to allow variable precision is to have 
multiple uint4s within the array represent a single phenotype. 
If, say, the first uint4 represented fine grain control 
and the second represented coarse control,
it would only take ceil(log16(3456)) = 3 uint4 slots to represent body length.
This effectively mimics a uint12 using 3 uint4s.
However the coarse control would allow changes to body length very quickly,
causing unrealistic jumps in evolution.

We could alternatively have several uint4s that are added together, 
This would have implications for the max rate of evolution, 
though we could introduce ways to limit the rate generically using a generic mutation control system.
It would also be an inefficient form of storage.
We would need 3456/16 = 216 such uint4 slots to store body length,

We mentioned previously the genetic algorithm operated on arrays of integers,
however that array is not the same as the one that is stored across timesteps,
so we can set the size of the integers in that array 
to whatever is most appropriate to capture precision requirements across all attributes.
However this means the function populating the array needs to be informed somehow 
about the precision requirements of individual attributes.
Up to the present point, the `dense` and `traversible` arrays 
only ever needed to know the sizes of the genotype they were representing,
The `organized` data structure was the only place where attributes had individuality,
It can be considered the canonical store for the properties of individual attributes, in that way.
This is an awfully desireable property, so to keep it,
we must provide a way for `organized` to communicate the storage size of attributes 
into a format that is interpretable to the function that converts `dense` into `traversible`.
We shall implement this in the form of several static methods
each called `getAttributeSizes()`, one for each class within `organized`.

## Designing the Optimization Algorithm

We mentioned previously that a single organism could require up to 2000 attributes. This has implications for performance optimization. 

We could think of our optimization algorithm as a traversal through a parameter space whose dimensionality is equal to the number of attributes. A naïve implementation of gradient descent might attempt `2*2000=4000` samples per iteration, one for each direction along each axis. We must next consider whether this approach is performant, whether it can reliably achieve sensible body shapes in an appropriate amount of time, and what steps can be taken if either of these is not the case. 

Computers nowadays typically allow 256KB of L1 cache. My own laptop has 384KiB, so I'm inclined to trust this number. A single animal stores about 1KB of data, so I don't think caching speeds are worth that much consideration, at least not during the calulation of fitness. Rather as we'll see, I think we're most tightly constrained by raw CPU performance.

We start with the observation that our optimization algorithm must run at least a single iteration within a single frame. We require the application be able to run at 60fps at slowest - that is widely considered the highest framerate at which the user notices performance, and to aim any lower invites implementation details from slowing an approach past the point where it is useable. Since we only need order of magnitude estimates, we will round up to dex 2 fps. Each frame takes dex -2 seconds.

Consider a naïve implementation where we simulate all 4000 samples. This is around dex 3.5 per niche, per grid cell, per iteration. As a reminder, there are dex 3-4 grid cells, and dex 1-1.5 ecological niches. The time required to estimate fitness for a single sample must then fall between dex -9.5 and -11.0 seconds per sample. 

I don't have good estimates for how many flops a normal computer has, but I heard the PlayStation 4 had roughly 8 teraflops, dex -13 seconds per flop. I'll assume this is representative to an order of magnitude. If such a CPU were fully dedicated to the task, we could allow anywhere from dex 3.5 to dex 2 floating point operations per sample. 

It's important to note that a fitness function may require looking up performances for multiple niches. This may be the case if there's a predator/prey relationship between species. For instance, if a cheetah regularly preys upon a gazelle, we'd need to know the top speed of both animals, and maybe some other performance characteristics like the degree of camoflage for either animal and the extent of peripherial vision for the gazelle. If we choose a worst case, generic model where each niche is defined by its ability to feed on another (allowing only one species of plant), and where each species tests once per iteration whether it is able to occupy a niche, we have anywhere from dex 2 to dex 3 interactions. Since we can't hope to generate one of these performance characteristics in only 3 floating point operations, we'll consider ourselves to have reached the edge of what's possible on reasonable hardware. We must now find ways to scale back while still giving acceptable results.

An acceleration structure could be introduced to store performance characteristics for species based on the genotype that's currently stored in memory. Our approach to updating fitness mirrors the approach for other state variables in a physics simulation: calculate the change over one timestep for one attribute, given the current value for every other state within the timestep. 

`canonical <=> dense <=> traversible <=> organized -> understandable -> cached -> fitness`

This greatly reduces the number of computations for a single traversal, but still requires us to run a single fitness function from scratch, and still requires us to both do this and calculate the gradient in only 3 floating point operations. 

## Designing the Memory Layout

We expect data from different niches will interact with one another within the same grid cell, in order to represent things like predation. We can also expect data from adjacent grid cells to interact with one another for the same niche. So we must choose which memory layout we prefer: grid-cell-sequential or niche-sequential. It would be difficult to accomplish grid-cell-sequential because grids are arranged on a 2d unstructured mesh. However, it would be easy to do niche-sequential. We therefore choose niche sequential. Our full memory layout looks something like this:

species distribution
* grid cell
** niche
*** segmentation genes
*** endoderm axial segment genes
*** mesoderm axial segment genes
*** ectoderm axial segment genes
**** radial segment genes
**** radial segment genes
**** ...
**** radial segment genes
*** axial segment genes
*** axial segment genes
*** ...
*** axial segment genes
** niche
** niche
** ...
** niche
* grid cell
* grid cell
* ...
* grid cell


Idea: "timer" genes
We have a bunch of genes that are sliding scales from 0 to 1, and they only trigger activation of some other genes once they reach a certain threshold
This is done to mimic how some genes rely on the emergence of others, such as to model the origin of segmentation in animals
This causes a problem though, since we want the emergence of those genes to occur at the same rate that we see in the real world, a vast parameterization problem
Basically, the idea is not to fret around with parameterizing genes so that they can evolve in the same amount of time we see in the real world.
Instead, just set the scale so that the threshold occurs at a value equal to the number of years or generations it took to accomplish. 
Then we can use a default parameter value for the rate of our gradient descent. 



## Outlining Niches
There are several approaches to categorizing niches:

Habitat:
* aquatic
* terrestrial
* aerial

Energy source:
* photosynthesis
* omnivores
* carnivores
* burrowing insects
* insects
* grazers
* seeds
* fish
* fruit
* nectar
* detritus
* scavengers

Cases of convergent evolution:
* armored
* burrower
* climbers
* gliders
* long distance fliers
* aerial predators

Strategies:
* pack hunters
* ambush predators
* aggressive mimicry
* megafauna

Considerations:
* Habitat is already largely expressed by spatial variables (e.g. elevation, precipitation, temperature), so can be excluded from consideration
* We should avoid niches that require the existance of particular earth-based taxons (e.g. insectivores) unless we can somehow express their role more generally (e.g. predators of small motile organisms)
* We should select niches that emphasize the importance of the competitive exclusion principle, since we rely on that principle in order to efficiently represent spatial distributions
* Resource utilization seems to be a particularly good way to define niches, since it is a frequent cause for competition. We see many examples where invasive species take over niches that were previously occupied by native species. Food sources are the main concern. Other necessities like water would allow for only one niche if they were particularly scarce, so we should probably leave them out. 
* Food source is also a good way to distinguish niches because it is relatively easy to estimate mass uptake and map it to biomass. This simplies the problem of defining a fitness function. We must make sure the model does not allow species to strip a land of its resources though, since that would produce unstable behavior. We should rely on a steady state assumption, calculating the maximum amount of biomass that a species could sustainably consume within a timestep.
* If food is the only consideration, we may also be able to dynamically generate niches, at least to within our resource allotment. 

## "Performance characteristics": characteristics for determining fitness
* heat dissipation
* heat production, metabolism
* energy procurement
* maximum running speed
* maximum jumping distance
* minimum gliding slope
* maximum flight speed
* minimum takeoff speed
* maximum swimming speed
* maximum soil excavation speed
* camoflage
* adaptive camoflage
* warning coloration
* batesian mimicry (resemblance to warning coloration in another species)
* advertising
* sexual selection coloration
* startle coloration (e.g. moth eyes)
* motion dazzle
* hunting lure (angler fish)
* sun burn protection



the phenotype model is itself a part of a larger set of functions meant to determine the fitness of an animal when it tries to occupy a particular niche:
`
float get_fitness_of_organism_in_niche( vecA body_plan_genes, realizator_genes ) { ... }
float get_performance_characteristic_for_organism( vecA body_plan_genes, realizator_genes ) { ... }
vecM get_phenotype_for_body_part( vecA body_part_index, vecA body_plan_genes, matNxM realizator_genes ) { ... }
`

given the following components:
	`vecN get_phenotype_for_body_part( vecA body_part_index ) {...}`
		A function that returns a collection of phenotypic attributes that can then be easily interpreted by another function to derive performance characteristics for a particular organism
	`float get_performance_characteristic_for_organism( vecA body_plan_genes, realizator_genes ) { ... }`
		A function that estimates a performance characteristic for an organism, for instance maximum running speed. It may make several calls to functions resembling `get_phenotype_for_body_part(...)` in doing so.
	`float get_fitness_of_organism_in_niche(vecA body_plan_genes, realizator_genes ) { ... }`
		A function that returns a metric expressing the fitness of an organism within a particular niche. 


