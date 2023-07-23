The "rasters" namespace exclusively represents data structures and functions that define a mathematical category.
The category concerns the ways in which values can be mapped to cells within a grid,
while making sure that objects always maintain internal consistency. 
The category manifests as an implementation of the Entity/Component/System (ECS) pattern.
The "components" are data structures that enable special functionality to be performed on the grid.
The "entities" are the grids themselves. 

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZARgBoAGAXVJADcBDAGwFcYkQBhEAX1PU1z5CKAMwVqdJq3YcABAB15eALbxZAZR58QGbHgJEATOJoMWbRCABqCpVlVxZAMS389QomWISz0y3MUVNQAZVx0BfWFkMW9TKQtOWyDHdST7ELDdQQMUchNJc3YAJTSHWS5eN2yovNiCvxASwPTHALsy0Mrw9xzkABZ83wSm9rUAcVK1TS6syKIBuqHiyccJ5o6VjUyIjxRjRfj2G3W1J03OiRgoAHN4IlAAMwAnCGUkYxAcCCQ8+oSAYwIcBwT2Y-xwYWer3eNC+SDIf3YgLAwNB4JANEY9AARjBGAAFHY5ECMGAPCFdKFvRADT7fRAADjihUsyNRYIp2ip8Nh9IA7MyGmyQRzIS9qb84YgAKyCgFAkXoynipBiOlIABscqRCrRnMeKsQHylH0YEAgaE8DLyDyYcBgEixuIJROEICeWGuAAt9SBuYg1VLfowsGAElAIDgcFcxdDGbykAKSaHw5Ho1BY9TaVKESGw+wI1GY8q47L1TLMebLShiNbSLbGPbHTi8YSem6Pd7ff6k1Kk2aLVabXaHZiWy72+xOz7M0gmeX5wPq8hh43RyTx23qlPPTOS9SteXD0uiKum2PnVu5pZp93DYepWW86mixn90gAJwJo2YlMFtPFlyhpfuWIjvogIFSrSSysrqorcJQ3BAA
)

Or it can be viewed with the following [tikzcd](http://ctan.math.washington.edu/tex-archive/graphics/pgf/contrib/tikz-cd/tikz-cd-doc.pdf) markup:

```
% https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZARgBoAGAXVJADcBDAGwFcYkQBhEAX1PU1z5CKAMwVqdJq3YcABAB15eALbxZAZR58QGbHgJEATOJoMWbRCABqCpVlVxZAMS389QomWISz0y3MUVNQAZVx0BfWFkMW9TKQtOWyDHdST7ELDdQQMUchNJc3YAJTSHWS5eN2yovNiCvxASwPTHALsy0Mrw9xzkABZ83wSm9rUAcVK1TS6syKIBuqHiyccJ5o6VjUyIjxRjRfj2G3W1J03OiRgoAHN4IlAAMwAnCGUkYxAcCCQ8+oSAYwIcBwT2Y-xwYWer3eNC+SDIf3YgLAwNB4JANEY9AARjBGAAFHY5ECMGAPCFdKFvRADT7fRAADjihUsyNRYIp2ip8Nh9IA7MyGmyQRzIS9qb84YgAKyCgFAkXoynipBiOlIABscqRCrRnMeKsQHylH0YEAgaE8DLyDyYcBgEixuIJROEICeWGuAAt9SBuYg1VLfowsGAElAIDgcFcxdDGbykAKSaHw5Ho1BY9TaVKESGw+wI1GY8q47L1TLMebLShiNbSLbGPbHTi8YSem6Pd7ff6k1Kk2aLVabXaHZiWy72+xOz7M0gmeX5wPq8hh43RyTx23qlPPTOS9SteXD0uiKum2PnVu5pZp93DYepWW86mixn90gAJwJo2YlMFtPFlyhpfuWIjvogIFSrSSysrqorcJQ3BAA
\begin{tikzcd}
R \times C \arrow[loop, distance=2em, in=125, out=55]                             & C \arrow[l, "construct"]                            & V \times F \arrow[l, "construct"] \arrow[r, "construct"] \arrow[loop, distance=2em, in=125, out=55] & C \times S \arrow[r, "construct"]                            & R \times G \times S \arrow[loop, distance=2em, in=125, out=55]                             \\
R \times C \times L \arrow[loop, distance=2em, in=305, out=235] \arrow[u, dotted] & C \times L \arrow[l, "construct"] \arrow[u, dotted] & V \times F \times L \arrow[u, dotted] \arrow[l] \arrow[r, "construct"]                              & C \times S \times L \arrow[r, "construct"] \arrow[u, dotted] & R \times G \times L \times S \arrow[u, dotted] \arrow[loop, distance=2em, in=305, out=235]
\end{tikzcd}
```

Trivial product and identity morphisms are omitted for brevity.

The category above uses several abbreviations, which are defined as follows:
* `vertices`        "V"   a list of glm::vec3s indicating position in space
* `faces`           "F"   a list of glm::uvec3s indicating interconnected indices within `vertices`
* `mesh`            "V×F" an unopinionated data structure describing the surface of a 3d object using only vertices and faces
* `MeshCache`       "C"   a large cache of derived mesh attributes that can be used to efficiently operate on grids
* `SpheroidVoronoi` "S"   a lookup that offers O(1) retrieval of nearest vertices on spheroidal meshes
* `LayerScheme`     "L"   a simple datastructure that specifies a way in which the vertices of a mesh could represent several layered cells of a grid
* `Raster`          "R"   a 1-dimensional container that maps a value to each cell within a grid

`MeshCache`, `SpheroidVoronoi`, and `LayerScheme` are all examples of "components" within our ECS implementation.
The `Grid` is the "entity" that can be formed between them.
A `Raster` is a data structure that maps a value to each cell within the grid. It is one possible representation of a mathematical [field](https://en.wikipedia.org/wiki/Scalar_field), and is currently the only one implemented.

It's important to note other functions could exist that map these objects to one another, 
but we restrict ourselves to a minimal set of functions that allows us to place guarantees 
on the consistency of components within a grid.
For example, we guarantee that `SpheroidVoronoi` will only ever contain indices to valid vertices that appear within `MeshCache`,
and we guarantee that each element of a raster will map to a single cell as defined by `MeshCache` and (possibly) `LayerScheme`. 
Any modifications made to this namespace in the future should still continue to enforce such guarantees. 