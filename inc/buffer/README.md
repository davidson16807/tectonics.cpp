`buffer/` consists of classes that store representations of 3d objects into data structures in such a way that they can be immediately passed to a function in a graphics library for rendering, namely OpenGL. In code, we label these data structures as "buffers", but they are typically stored as `std::vector`s.

Method signatures are always of the same form: 

* a series of `const` parameters, followed by...
* the output "buffer" data structure, followed by...
* the `const` index (let's call it "a") where the method is supposed to start writing to the buffer

The method returns the index (let's call it "b") that immediately follows the last index that's written to the buffer. This design is meant to allow reuse of buffer memory and reduce the number of calls to `glDrawElements()` while still allowing method calls to be easily chained to create larger functions, like this example from PrimitiveBuffers.hpp:

```c++
constexpr id storeTriangle(const T CW, const T O, const T CCW, Buffer& output, const id buffer_start_id=0) const
{
	id buffer_id = buffer_start_id;
	buffer_id = storePoint(CW, output, buffer_id);
	buffer_id = storePoint(O, output, buffer_id);
	buffer_id = storePoint(CCW, output, buffer_id);
	return buffer_id;
}
```

The contents of the buffer at indices [a, b) are a strict function of of the `const` parameters at the start of the method signature.

Several types of methods exist so that a `*Buffer()` can represent objects dependant on the kind of mode passed to `glDrawElements()`:

* `storePoint()` for use with `GL_POINTS`
* `storeTriangle()` for use with `GL_TRIANGLES`
* `storeTriangleStrips()` for use with `GL_TRIANGLE_STRIPS`

and so on. For each of these `store*()` methods, there is a corresponding `_size()` method that indicates how much space should be allocated to a buffer so that the object from the `store*()` method can be rendered, e.g. `point_size()`, `triangle_size()`, `triangle_strips_size()`, etc. The output of these methods is always ≥b-a. Note that the use of `snake_case` indicates that a method is conceptually pure, whereas `camelCase` indicates that a method is stateful. 

