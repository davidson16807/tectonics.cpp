The "view" folder stores `*ShaderProgram` classes.

A `*ShaderProgram` is a proper object oriented class 
that seals off access to resources relating to an 
OpenGL shader program within an OpenGL Context, 
allowing view state to be managed statelessly elsewhere. 

It guarantees the following:
* all internal resources are created on initialization to minimize state transitions (RAII)
* all internal resources are strictly encapsulated
* the program can be in only one of two states: "created" and "disposed"
* the disposed state can be entered at any time but never exited
* all methods will continue to produce sensible, well defined behavior in the disposed state
* the output that draw() sends to the currently bound framebuffer is 
  a pure function of its input

Its internal state transitions can be described with the following diagram:

initialized
    ↓        dispose()
 disposed
    ↻        dispose()

Any attempt to relax guarantees made here will severely cripple 
your ability to reason with the code base. 

`*ShaderProgram`s are the lowest level of abstraction in the graphics architecture used here.
As such they are extremely general purpose. `*ShaderProgram`s should never introduce restrictive assumptions 
about the objects they render unless there is a strong performance reason.
Valid assumptions that can be introduced at this layer include:
* how the projection matrix is applied (e.g. whether it is a HUD, billboard, or regular 3d object)
* how the user will supply vertex data (e.g. winding order, backface culling, how they will specify color, whether they can control transparency, etc.)
* whether the `*ShaderProgram` renders points, triangles, volumetrics, etc.
* whether the `*ShaderProgram` renders facsimiles of the real world vs. abstract concepts such as arrows, points, etc.
* how a surface will be depicted stylistically (e.g. cell shader, photorealistic, etc.)

`*ShaderProgram`s have two methods, `canDepict()` and `draw()`.
`draw()` adds a depiction of a scalar field to the framebuffer that is currently 
bound to the program's context using options from a given view state.
It does nothing if it is unable to depict the given view state, as determined by `canDepict()`.
The only state that is allowed to be modified is that of the framebuffer,
so the state of the framebuffer after invocation is a strict function 
of its current state and the arguments passed to `draw()`

`canDepict()` returns whether this instance can fully depict a model using the given
view state upon calling `.draw()`.
If it cannot, the view state should be used to create a new instance, 
and the old instance should be disposed before falling out of scope.
This is not a test for whether the WebGL context or the view state 
is well formatted. It is strictly a test of the program's private state.

This demonstrates our approach to handling WebGL state management.
If changing something is trivial, like uniforms or attributes, 
we simply change it during the draw call without disposing resources.
If changing something requires managing highly intertwined private 
resources like shaders or programs, we simply wipe the slate clean
on the first sign that anything falls out of sync.

# Terminology
When implementing `*ShaderPrograms`, we use the same terminology that's standard for OpenGl.
We attempt definitions for them as follows:

* fragment   the smallest entity considered by a fragment shader
* vertex     the smallest entity considered by a geometry shader
* point      a vertex that is rendered in isolation
* line       a duple:  (vertex, vertex)
* triangle   a triple: (vertex, vertex, vertex)
* primitive  a point, line, triangle, or triangle strip
* element    a duple: (vertex, primitive), it is a specific usage of a vertex in a primitive
* instance   a collection of primitives that are intended for reuse
* uniform    aspects that do not vary over the course of a `draw()` call
* model      the set of all graphics that are uniform
* view       the orientation of a camera that are uniform
* projection aspects of perspective that are uniform
* attribute  aspects that are not uniform, either across elements, vertices, or instances
* static     something that is not intended to change once per frame
* dynamic    something that is expected to change once per frame

We also introduce or coopt the following terms:

* graphic    either a primitive or an instance
* pictorial  a graphic that is only meant to represent an abstract concept, such as an arrow or point
* indicator  a pictorial instance
* cloud      a static set of graphics where primitives do not share vertices, it either features instances or does not distincguish vertices and elements
* swarm      a dynamic set of graphics where primitives do not share vertices, it either features instances or does not distincguish vertices and elements
* surface    a set of graphics where primitives share vertices and vice versa, it features an element buffer object