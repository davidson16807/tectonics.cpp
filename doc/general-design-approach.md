Design by category theory:
* the list of states an entity will be in (e.g. created, disposed, activated, serialized, compressed)
* the list of data structures describing the same entity for which each problem you're trying to solve can be made the easiest
* the list of data structures describing the same entity for which each concern you're aware of can be best addressed (e.g. valid for all attribute modifications, minimization of memory footprint, etc.)
* the list of interrim states that make morphing one data structure into another the easiest
* the list of states for which something is arrived at to best ensure the integrity of the final data structure during construction
* the list of transformations for which each state most easily maps to the rest

a category results from this, typically one per entity, or one for several entities if other entities are needed to describe its construction

* decide whether any of the states require privitization, e.g. to prevent illegal values, note the privitization (perhaps draw a box around their object in the category diagram) and express their morphisms using methods rather than static functions 
* run through the list of data structures, for each one write a file containing only that data structure (exclude morphisms unless required by privitization)
* run through the list of homsets, for each one write a file containing on functions of that homset (exclude morphisms unless required by privitization)
* for each homset file, write a set of unit tests in a separate file of the same folder, and give it a similar name. In this way you can keep track of how far along you are to vetting the namespace/category, after which you can move on and build the code that executes the morphisms between its objects.
* The unit tests only test for basic properties that rely on abstract algebra, so they are formulaic and don't require knowledge of the class to write. Add more specialized tests if you feel so compelled, but don't feel obligated. Include the unit test file in the same directory as the data structure and give it a similar name.
* if data structures have any additional considerations (e.g. memory footprint below a certain threshold), write a unit test file for that data structure. Include it in the same directory as the data structure and give it a similar name. 

Design by abstract algebra:
* consolidate the number of states or objects in your diagram that are behave similarly to abstract algabraic structures,
  e.g. find degenerate cases where one object is described by another
* use algabraic properties of operations within unit tests, e.g. closure, identity, associativity, invertibility, commutativity, distributivity

Design by type theory:
* an instantiation of an object in a category diagram is a sort of proof that such an entity exists,
  if you provide the data that's needed for construction, then that counts as a sort of demonstration of how that entity can exist,
  and the fact the entity exists allow you to prove other entities exist by feeding that entity to other functions
  functions then behave similarly to if-then statements, and the category diagram reflects a flow chart of how things can be proved 
  when referring to states, to posess a state object in a pure function is evidence that the program has entered such a state,
  objects become something like "tokens of proof"
  though in a procedural paradigm, it only demonstrates such as state existed at one point in time,
  e.g. you can prove that an object has been disposed if that object's dispose() method returns a disposal token, 
  but you can never use that object alone as proof that it is currently nondesposed, only that is once existed in a nondisposed state,
  you can however have it return a token that could be a type union for disposed and nondispoed states, 
  then have those token serve as proof for other things via polymorphism