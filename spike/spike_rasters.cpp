

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly
#include <glm/vec3.hpp>               // *vec3

#include <many/many.hpp>  
#include <many/string_cast.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/string_cast.hpp>  
#include <many/glm/convenience.hpp> //  operators, etc.

#include <rasters/mesh.hpp>
#include <rasters/types.hpp>
#include <rasters/string_cast.hpp>  
#include <rasters/glm/glm.hpp>
#include <rasters/glm/string_cast.hpp>  
#include <rasters/glm/vector_calculus.hpp>

// #include "academics/tectonics.hpp"

using namespace glm;
using namespace many;
using namespace rasters;

int main(int argc, char const *argv[])
{
    /* 
    "diamond" is a simple 2d grid for testing raster operations 
     that do not require spatial awareness (e.g. arithmetic on scalar fields)
    It looks like this:
         2  
        /|\ 
       3-0-1
        \|/ 
         4   
    */
    std::shared_ptr<Grid> diamond = 
        std::make_shared<Grid>(
            vec3s({
                    vec3( 0, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 0, 1, 0),
                    vec3(-1, 0, 0),
                    vec3( 0,-1, 0)
                }),
            uvec3s({
                    uvec3(0,1,2),
                    uvec3(0,1,4),
                    uvec3(0,3,2),
                    uvec3(0,3,4)
                })
        );
    /*
    "tetrahedron" is a simple 3d grid for testing basic raster operations
     that require spatial awareness without requiring a particular shape.
     (e.g. gradient, divergence, laplacian)
    */
    std::shared_ptr<Grid> tetrahedron = 
        std::make_shared<Grid>(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);
    /*
    "octahedron" is a simple 3d grid for testing raster operations that require 
    something comparable to a unit sphere (e.g. nearest neighbor lookups using SphereGridVoronoi)
    */
    std::shared_ptr<Grid> octahedron = 
        std::make_shared<Grid>(meshes::octahedron.vertices, meshes::octahedron.faces);

    /*
    "icosahedron" is a simple 3d grid for testing rasters with a large number of vertices
    */
    std::shared_ptr<Grid> icosahedron = 
        std::make_shared<Grid>(meshes::icosahedron.vertices, meshes::icosahedron.faces);

    SphereGridVoronoi voronoi_test(
            vec3s({
                    normalize(vec3( 1, 0, 0)),
                    normalize(vec3( 0, 1, 0)),
                    normalize(vec3( 0, 0, 1)),
                    normalize(vec3(-1, 0, 0)),
                    normalize(vec3( 0,-1, 0)),
                    normalize(vec3( 0, 0,-1)),
                    normalize(vec3(-1,-1,-1)),
                    normalize(vec3( 1,-1,-1)),
                    normalize(vec3(-1, 1,-1)),
                    normalize(vec3( 1, 1,-1)),
                    normalize(vec3(-1,-1, 1)),
                    normalize(vec3( 1,-1, 1)),
                    normalize(vec3(-1, 1, 1)),
                    normalize(vec3( 1, 1, 1)),
                    normalize(vec3( 1, 1, 1))
                }),
            1./100.,
            3./100.
        );

    floats a = floats({1,2,3,4,5});
    floats b = floats({1,1,2,3,5});
    floats c = floats({4,8,3,8,2,4,5,9,8,2,3,5,2,1,3,3,3,1,6,1,
                       4,2,5,INFINITY,9,4,6,2,8,1,5,3,7,5,8,5,6,6,7,6,
                       1,2,4,1,4,9,9,8,1,3,7,2,5,5,1,8,9,4,7,6,
                       4,4,4,5,3,1,3,5,8,3,4,1,3,7,5,6,2,9,7,5,
                       4,8,3,8,2,4,5,9,8,2,3,5,2,1,3,3,3,1,6,1,
                       4,2,5,9,9,9,9,9,9,9,9,9,9,9,8,5,6,6,7,6,
                       1,2,4,1,4,9,9,8,1,3,7,2,5,5,1,8,9,4,7,6,
                       4,4,4,5,3,1,3,5,8,3,4,1,3,7,5,6,2,9,7});

    std::string stra = to_string(a);
    std::string strb = to_string(b);
    std::string strc = to_string(c);
    std::cout << strc << std::endl;

    vec2s v2d = vec2s({
        vec2(-1,-1),
        vec2(-1, 1),
        vec2(-1,-0.1),
        vec2(-1, 0.1),
        vec2(-1, 0.0),
    });
    vec3s v3d = vec3s({
        vec3(-1,-1,-1),
        vec3( 0,-1,-1),
        vec3( 1,-1,-1),
        vec3(-1, 0,-1),
        vec3( 0, 0,-1),
        vec3( 1, 0,-1),
        vec3(-1, 1,-1),
        vec3( 0, 1,-1),
        vec3( 1, 1,-1),

        vec3(-1,-1, 0),
        vec3( 0,-1, 0),
        vec3( 1,-1, 0),
        vec3(-1, 0, 0),
        vec3( 0, 0, 0),
        vec3( 1, 0, 0),
        vec3(-1, 1, 0),
        vec3( 0, 1, 0),
        vec3( 1, 1, 0),

        vec3(-1,-1, 1),
        vec3( 0,-1, 1),
        vec3( 1,-1, 1),
        vec3(-1, 0, 1),
        vec3( 0, 0, 1),
        vec3( 1, 0, 1),
        vec3(-1, 1, 1),
        vec3( 0, 1, 1),
        vec3( 1, 1, 1),
    });
    std::string str_v2d = to_string(v2d);
    std::string str_v3d = to_string(v3d, glm::mat3x2(1,1,0, 
                                                     0,1,1));
    std::cout << str_v2d << std::endl;
    std::cout << str_v3d << std::endl;

    raster raster_a = raster(octahedron, {0,1,2,3,4,5});
    std::string str_raster_a = to_string(raster_a);
    std::cout << str_raster_a << std::endl;

    vec2raster raster_v2 = vec2raster(octahedron, {
        vec2( 0,-1),
        vec2( 0, 1),
        vec2(-1, 0),
        vec2( 1, 0),
        vec2(-1,-1),
        vec2( 1, 1),
    });
    vec3raster raster_v3 = vec3raster(octahedron, {
        vec3( 0, 1, 0),
        vec3( 0, 1, 0),
        vec3( 0, 1, 0),
        vec3( 0, 1, 0),
        vec3( 0, 1, 0),
        vec3( 0, 1, 0),
    });

    std::string str_raster_v2 = rasters::to_string(raster_v2);
    std::cout << str_raster_v2 << std::endl;

    std::string str_raster_v3 = rasters::to_string(raster_v3);
    std::cout << str_raster_v3 << std::endl;
}