# Overview
The design methodology used by the library adheres to principles in both functional and object oriented programming and can be best described using concepts from [category theory](https://en.wikipedia.org/wiki/Category_theory). Design begins by decomposing behavior into a [commutative diagram](https://en.wikipedia.org/wiki/Commutative_diagram) expressing a category, typically where objects are data structures and arrows can be treated as [pure functions](https://en.wikipedia.org/wiki/Pure_function) mapping between them. If two data structures represent the same concept but are each only able to support distinct functionality, then both data structures are implemented and a function is provided to map between them. If a mapping is needed but none can be found, then mappings are found that come closer to the desired mapping, until the desired mapping becomes apparent through function composition. Data structures are consolidated if a single data structure can be found that accounts for the behavior of both without introducing redundancy or a loss of functionality. 

Categories are implemented either as namespaces or classes. If the category is implemented as a class, then object oriented [polymorphism](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)) provides a natural way to implement [functors](https://en.wikipedia.org/wiki/Functor) between categories. Oftentimes, the implementation of a category rests on the composition of the functions and objects from another underlying category, in which case there is a category of categories that describes dependencies between them. Arrows within this category of categories are then implemented as the constructors of classes that represent categories, which evokes the behavior of [dependency injection](https://en.wikipedia.org/wiki/Dependency_injection) within object oriented programming, but allows this design to be arrived at in a strictly procedural manner. 

In order to create diagrams where commutativity is meaningful, it sometimes helps to remove certain function parameters from consideration and treat them as constants within a given category. In this case a value for the parameter might be fed into the constructor of the class that represents the category, in which case the parameter becomes a private `const` member. This evokes [encapsulation](https://en.wikipedia.org/wiki/Encapsulation_(computer_programming)) within object oriented programming, but since the member is `const`, the approach strictly avoids the problems that arise from that methodology due to state management. Another way to view this is to see the class as a cohesive set of functions that have collectively undergone [currying](https://en.wikipedia.org/wiki/Currying). 

Beyond this description, we incorporate aspects of...

# Design by category theory
Start with the following:

* the list of states an entity will be in (e.g. created, disposed, activated, serialized, compressed)
* the list of data structures, each describing the same entity, but different in that each is best suited to a problem you're trying to solve, or concern you're aware of (e.g. valid for all attribute modifications, minimization of memory footprint, etc.)
* the list of interrim states that make morphing one data structure into another the easiest, if needed
* the list of states for which something is arrived at to best ensure the integrity of the final data structure during construction
* the list of transformations for which each state most easily maps to the rest

A category results from this, typically one per entity, or one for several entities if other entities are needed to describe its construction. The objects and morphisms of categories may themselves be implemented using the objects and morphisms of another category. In this way, a hierarchy of categories emerges.

* decide whether any of the states require privatization, e.g. to prevent illegal values, note the privatization (perhaps draw a box around their object in the category diagram) and express their morphisms using methods rather than static functions 
* run through the list of data structures, for each one write a file containing only that data structure (exclude morphisms unless required by privatization)
* run through the list of homsets, for each one write a file containing on functions of that homset (exclude morphisms unless required by privatization)
* for each homset file, write a set of unit tests in a separate file of the same folder, and give it a similar name. In this way you can keep track of how far along you are to vetting the namespace/category, after which you can move on and build the code that executes the morphisms between its objects.
* The unit tests only test for basic properties that rely on abstract algebra, so they are formulaic and don't require knowledge of the class to write. Add more specialized tests if you feel so compelled, but don't feel obligated. Include the unit test file in the same directory as the data structure and give it a similar name.
* if data structures have any additional considerations (e.g. memory footprint below a certain threshold), write a unit test file for that data structure. Include it in the same directory as the data structure and give it a similar name. 

Categories may be constructed either top-down (starting with high level objectives, describing flow from storage variables to derived attributes and storage deltas) or bottom-up (starting with the smallest categories that can be constructed using only primitives). 

# Design by abstract algebra
* consolidate the number of states or objects in your diagram that are behave similarly to abstract algebraic structures,
  e.g. find degenerate cases where one object is described by another
* use algebraic properties of operations within unit tests, e.g. closure, identity, associativity, invertibility, commutativity, distributivity

# Design by type theory
* an instantiation of an object in a category diagram is a sort of proof that such an entity exists,
  if you provide the data that's needed for construction, then that counts as a sort of demonstration of how that entity can exist,
  and the fact the entity exists allow you to prove other entities exist by feeding that entity to other functions
  functions then behave similarly to if-then statements, and the category diagram reflects a flow chart of how things can be proved 
  when referring to states, to possess a state object in a pure function is evidence that the program has entered such a state,
  objects become something like "tokens of proof"
  though in a procedural paradigm, it only demonstrates such as state existed at one point in time,
  e.g. you can prove that an object has been disposed if that object's dispose() method returns a disposal token, 
  but you can never use that object alone as proof that it is currently nondesposed, only that is once existed in a nondisposed state,
  you can however have it return a token that could be a type union for disposed and nondisposed states, 
  then have those token serve as proof for other things via polymorphism

# Design for Performance ("Data Oriented Design")
* start by considering required behavior
* find the algorithm that accomplishes behavior while optimizing for the following, in highest to lowest priority:
** memory/runtime complexity ("big-O" notation)
** data locality ("Data Oriented Design"), ideally using in-order traversal of memory, 
   but if not then by maximizing reuse of cache operations

# Design for Reasonability ("Functional Programming")

**Computer code exists as a means for humans to understand software behavior. The best code can be understood by humans with ease and precision.** Code must compile for there to be software behavior to describe, but if we didn't care to understand the code, then we simply would have spent the last few decades using write-once languages like assembly. Humans need the ability to understand code because we need the option to intervene when things do not go according to plan. 

Since computer code exists as a means for humans to understand software behavior, the best computer code leverages our strengths and makes up for our weaknesses. 

To illustrate, humans are terrible at sequential reasoning. We have limited working memory, we make mistakes, we simplify repetitive tasks without consciously checking for accuracy, and our focus is easily interrupted by subconscious impulse. When tasked with sequential reasoning, we often either forget where we started, make a mistake, gloss over details, or get distracted. These are problems inherent to all neural networks. We often get around these shortcomings by relying on tools to aid in sequential reasoning. For instance, we may write our thoughts onto paper to overcome limited working memory. Computer code is just another example of such a tool.

Computers are terrific at sequential reasoning, when executing software. The operations they perform have remarkably high fidelity, their memory is vast, and they cannot be distracted.

Therefore, the best computer code avoids situations where a human must reason sequentially in order to correctly understand the behavior of the software. 

[Dijkstra](https://homepages.cwi.nl/~storm/teaching/reader/Dijkstra68.pdf) famously used this last point to argue against the use of `goto` statements. It has more recently been used to argue against [OOP](https://www.youtube.com/watch?v=pH-q2m5sb04&ab_channel=CppCon), as well. I have tried to articulate the line of thought that leads to this principle as best I can, in the hope of clarifying it further.

To prevent other similarly harmful programming fads from emerging to replace goto or OOP, it may be important to hold onto this idea as a general principle. The fact this isn't often articulated seems surprising. It seems like it ought to be one of the most important principles in programming, since it describes very broadly what humans and computers are best suited for, and where our efforts as programmers are best spent.

So where are our efforts best spent exactly? Namely, anywhere besides tasks that require sequential reasoning. It's challenging to articulate how we avoid this, since our job as programmers is to specify behavior that occurs on computers, which are best leveraged if the behavior is sequential. How are we to consider sequential tasks without thinking sequentially?

The best answer I can provide so far is to decompose a sequential task as much as possible into components that are nonsequential, then consider only the nonsequential tasks. For instance, rather than think about how a character's position updates, we might consider how the character's position is expressed as a data structure, how the change in the character's position is expressed as a delta, how the delta is a strict function of variables in the game world, and how the delta and state objects compose giving a new state object. We delay any thought of sequential reasoning as long as possible. If we consider the nonsequential tasks to form a hierarchy, the sequential task ought to form the apex of the hierarchy. 

Some subtasks may require sequential operations, such as if we performed numerical optimization, a sorting algorithm, or a specialized algorithm like flood fill. However, these too are decomposed into nonsequential operations, and just like the example concerning a character's position, we put off sequential operations as long as possible until we get an opportunity to treat the result of the combined sequential operation as if it were the results of a single nonsequential operation, which is then encapsulated safely inside a function, allowing us to forget it was even sequential to begin with. The result produced by that encapsulating function may then holds some properties which are useful to us, such as those that allow us to get [theorems for free](), and those are the only properties that we use to reason about the function from the outside.

Now that I've fully discussed the principle, there is an important corrolary I'd like to mention:

At no point is state modification to be introduced into the code base. State modification introduces sequential reasoning requirements when trying to predict the behavior of code, which severely limits our ability to reason about the code without testing its behavior empirically in a debugger. If we have to step through the code, or if we have to simulate the code in our head, we are in essence becoming the computer. I could mention something about how demeaning it is to put a human in such a position, but there is a more serious practical consideration to discuss: we are in essence tasked with staffing for a job, and we're giving it out to the person who is least qualified to do it.

State modification is often used in an attempt to reduce memory consumption and improve performance. I don't think this is a legitimate concern. Most performance concerns are neutralized by allowing the use of output reference parameters. To summarize: objects may serve as "memory footprints" that store values from the results of functions and methods, but if this is done then the entire state of the object afterwards must be completely determined from the input provided. At no point should an object serve as both input and output. 

Sometimes in order to reduce memory consumption and simplify function signatures it is necessary to reference objects in shared pointers. I often find this is useful when an otherwise simple data structure needs some sort of "metadata" attached to it to make sense of its values. Without shared pointers, the metadata would typically accompany the data structure in a function signature. The metadata however is typically large, so copying it across all instances of the data structure is infeasible. Shared pointers are a necessary way to work around this situation. For instance, `rasters::Raster` behavior relies on shared pointers within a `rasters::Grid` instance, and `matter::Liquid` requires a shared pointer to a `matter:PropertyTable` instance. Shared pointers are acceptable, but require special attention to the above rule concerning state modification. This is important: **the value of a shared pointer must never be modified.** Even the use of shared pointers as "memory footprints" is strictly forbidden, since doing so would modify the behavior of other morphisms in such a way that we could not predict the change in behavior from their invocation. If the shared pointer represents something that changes over the course of simulation, the pointer must be swapped with a new pointer that points to the updated value. For this reason, shared pointers should only ever be used to represent things that update rarely, ideally never. 

# AI policy
Speaking of harmful programming fads...

The last section was written many years before this one, but it still applies remarkably well. To restate what was said earlier: humans need the ability to understand code because humans need the option to intervene when things do not go according to plan. Therefore, **the best code can be understood by humans with ease and precision.** 

Code that is written by humans has a much easier time satisfying this criteria, since we at least know that at one point it was possible for a human to understand it. If code is properly maintained, it remains possible to do so. But code that is written by an LLM cannot say the same. The "thought process" of an LLM writing code is not known to anyone, even the LLM. If you ask the LLM to explain itself, it simply offers a best guess as to why the code was written as it was, and it need not understand the code for it to do so. This doesn't change merely because the model has more weights.

This fact informs our policy. As of August 2026, use of large language models ("LLMs") in code is strictly limited to the following techniques. Each technique is followed by a comprehensive list of files where that technique is used.

* reviewing human code without offering revision
** applied liberally throughout the code base ([Linus's Law](https://en.wikipedia.org/wiki/Linus%27s_law) applies here)
* porting code that was written by a human from a scripting language to C++
** `*UniversalPropagator.hpp`
* writing unit tests for properties that are chosen by a human
** `*UniversalPropagator_test.cpp`
* writing classes that follow the same structure as an existing class that was written by a human
** `*Components.hpp`
** `*Codec.hpp`

All LLM code is thoroughly reviewed by a human.

